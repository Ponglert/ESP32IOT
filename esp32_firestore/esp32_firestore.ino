/*
 * ============================================================================
 *  ESP32 + Firebase Firestore  
 * ----------------------------------------------------------------------------
 *  - อ่านค่า Humidity (h) และ Temperature (t) จาก DHT11  ->  เขียนขึ้น Firestore
 *  - เก็บสถานะการเปิด/ปิดของ relay1 และ relay2         ->  เขียนขึ้น Firestore
 *  - อ่านคำสั่งเปิด/ปิด relay จาก Firestore กลับมาสั่งงานขา GPIO (polling)
 *
 *  ไลบรารีที่ต้องติดตั้ง (Library Manager):
 *      1) "Firebase Arduino Client Library for ESP8266 and ESP32"
 *         โดย Mobizt  ***เวอร์ชัน 4.4.x***  (อย่าใช้ 5.x / FirebaseClient ตัวใหม่
 *         เพราะ API คนละแบบ)
 *      2) "DHT sensor library" โดย Adafruit
 *      3) "Adafruit Unified Sensor" โดย Adafruit
 *
 *  Board: ESP32 Dev Module (ESP32 Arduino core 2.0.x ขึ้นไป)
 * ============================================================================
 */

#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>   // ช่วยพิมพ์สถานะการขอ token ทาง Serial
#include "DHT.h"
#include <time.h>

/* ==========================================================================
 *  1) ค่าคอนฟิก  -- แก้ตรงนี้ให้ตรงกับของคุณ
 * ========================================================================== */

// ---------- WiFi ----------
const char* WIFI_SSID     = "CSUBRUIOT";
const char* WIFI_PASSWORD = "asdf+1234";

// ---------- Firebase ----------
// Web API Key : Firebase Console > Project settings > General > Web API Key
#define API_KEY            "AIzaSyCbzeeFMeKpE2B04Mk-gMxZpt0PF3yyFKY"
// Project ID  : Firebase Console > Project settings > General > Project ID
#define FIREBASE_PROJECT_ID "esp32test-80d12"
// บัญชีที่สร้างไว้ใน Authentication > Users (Email/Password provider)
#define USER_EMAIL         "device01@example.com"
#define USER_PASSWORD      "SuperSecret123"

// ---------- Firestore ----------
// เอกสารสถานะ 1 ตัว เก็บทั้งคำสั่ง / สถานะจริง / ค่าเซ็นเซอร์
#define DEVICE_ID          "esp32-01"
String documentPath = String("devices/") + DEVICE_ID;

// ---------- ขา GPIO ----------
#define relaypin1 19
#define relaypin2 18
#define DHTPIN     5
#define DHTTYPE    DHT11

// ถ้าโมดูลรีเลย์ของคุณเป็นแบบ Active LOW (สั่ง LOW = ติด) ให้เปลี่ยนเป็น 1
#define RELAY_ACTIVE_LOW  0

// ---------- จังหวะเวลา (มิลลิวินาที) ----------
const unsigned long DHT_READ_INTERVAL   = 2000;   // อ่านเซ็นเซอร์ทุก 2 วิ
const unsigned long UPLOAD_INTERVAL     = 15000;  // ส่งค่าขึ้น Firestore ทุก 15 วิ
const unsigned long COMMAND_INTERVAL    = 5000;   // ดึงคำสั่ง relay ทุก 5 วิ
// หมายเหตุโควตา Free (Spark): write 20,000/วัน, read 50,000/วัน
//   15 วิ -> 5,760 writes/วัน , 5 วิ -> 17,280 reads/วัน  (อยู่ในโควตา)

/* ==========================================================================
 *  2) ตัวแปรส่วนกลาง
 * ========================================================================== */

FirebaseData   fbdo;      // สำหรับเขียน (patch)
FirebaseData   fbdoRead;  // สำหรับอ่าน (get) แยกอ็อบเจกต์กันชนกัน
FirebaseAuth   auth;
FirebaseConfig config;

DHT dht(DHTPIN, DHTTYPE);

int   relay1 = 0;          // คำสั่งล่าสุดจาก Firestore
int   relay2 = 0;
int   relay1Applied = -1;  // สถานะที่ "สั่งลงขาจริง" แล้ว (-1 = ยังไม่เคยสั่ง)
int   relay2Applied = -1;

float h = NAN;             // Humidity
float t = NAN;             // Temperature

unsigned long lastDhtRead   = 0;
unsigned long lastUpload    = 0;
unsigned long lastCommand   = 0;
bool          forceUpload   = false;   // บังคับส่งทันทีเมื่อ relay เปลี่ยนสถานะ

/* ==========================================================================
 *  3) ฟังก์ชันช่วยเหลือ
 * ========================================================================== */

// คืนค่าเวลาปัจจุบันในรูปแบบ ISO-8601 UTC เช่น 2026-08-18T07:30:00Z
// (Firestore ต้องการรูปแบบนี้สำหรับฟิลด์ชนิด timestampValue)
String isoTimeUTC() {
  time_t now = time(nullptr);
  if (now < 1700000000) {            // ยังไม่ได้ sync NTP
    return String("1970-01-01T00:00:00Z");
  }
  struct tm tmUTC;
  gmtime_r(&now, &tmUTC);
  char buf[32];
  strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%SZ", &tmUTC);
  return String(buf);
}

// เขียนสถานะลงขา GPIO โดยเคารพ RELAY_ACTIVE_LOW
void writeRelay(uint8_t pin, int on) {
#if RELAY_ACTIVE_LOW
  digitalWrite(pin, on ? LOW : HIGH);
#else
  digitalWrite(pin, on ? HIGH : LOW);
#endif
}

/* --------------------------------------------------------------------------
 *  เขียนข้อมูลขึ้น Firestore  (patchDocument = สร้างเอกสารให้ถ้ายังไม่มี)
 *
 *  โครงสร้าง  devices/esp32-01
 *  ┌─────────────────┬───────────┬────────────────────────────────────────┐
 *  │ relay1          │ integer   │ คำสั่ง 0/1 (แอป/เว็บ เป็นคนเขียน)      │
 *  │ relay2          │ integer   │ คำสั่ง 0/1                             │
 *  │ relay1State     │ integer   │ สถานะจริงบนบอร์ด (ESP32 เป็นคนเขียน)   │
 *  │ relay2State     │ integer   │ สถานะจริงบนบอร์ด                       │
 *  │ humidity        │ double    │ ค่า h  (%RH)                           │
 *  │ temperature     │ double    │ ค่า t  (°C)                            │
 *  │ updatedAt       │ timestamp │ เวลาอัปเดตล่าสุด                       │
 *  └─────────────────┴───────────┴────────────────────────────────────────┘
 *
 *  สังเกตว่า ESP32 จะ "ไม่แตะ" ฟิลด์ relay1 / relay2 เลย เขียนแค่ *State
 *  เพื่อไม่ให้ไปทับคำสั่งใหม่ที่แอปเพิ่งเขียนเข้ามา (กัน race condition)
 * ------------------------------------------------------------------------ */
bool uploadToFirestore() {
  if (!Firebase.ready()) return false;

  FirebaseJson content;
  content.setFloatDigits(2);   // ทศนิยม 2 ตำแหน่งพอ

  // ค่าเซ็นเซอร์ (ส่งเฉพาะเมื่ออ่านได้จริง)
  bool hasSensor = (!isnan(h) && !isnan(t));
  if (hasSensor) {
    content.set("fields/humidity/doubleValue",    h);
    content.set("fields/temperature/doubleValue", t);
  }

  // สถานะรีเลย์จริงบนบอร์ด  (integerValue ของ Firestore REST ต้องส่งเป็น string)
  content.set("fields/relay1State/integerValue", String(relay1Applied));
  content.set("fields/relay2State/integerValue", String(relay2Applied));
  content.set("fields/updatedAt/timestampValue", isoTimeUTC());

  // updateMask = ระบุเฉพาะฟิลด์ที่จะแก้ ฟิลด์อื่นในเอกสารจะไม่ถูกลบ
  String mask = "relay1State,relay2State,updatedAt";
  if (hasSensor) mask += ",humidity,temperature";

  if (Firebase.Firestore.patchDocument(&fbdo,
                                       FIREBASE_PROJECT_ID,
                                       "" /* databaseId: "" = (default) */,
                                       documentPath.c_str(),
                                       content.raw(),
                                       mask.c_str())) {
    Serial.println(F("[Firestore] upload OK"));
    return true;
  } else {
    Serial.print(F("[Firestore] upload FAILED: "));
    Serial.println(fbdo.errorReason());
    return false;
  }
}

/* --------------------------------------------------------------------------
 *  ดึงคำสั่ง relay1 / relay2 จาก Firestore
 *  รองรับทั้งกรณีที่ฝั่งแอปเขียนเป็น integer (0/1) และ boolean (true/false)
 * ------------------------------------------------------------------------ */
void fetchCommands() {
  if (!Firebase.ready()) return;

  if (!Firebase.Firestore.getDocument(&fbdoRead,
                                      FIREBASE_PROJECT_ID,
                                      "",
                                      documentPath.c_str(),
                                      "relay1,relay2" /* mask: ดึงแค่ 2 ฟิลด์ */)) {
    Serial.print(F("[Firestore] get FAILED: "));
    Serial.println(fbdoRead.errorReason());
    return;
  }

  FirebaseJson payload;
  FirebaseJsonData jd;
  payload.setJsonData(fbdoRead.payload().c_str());

  // ---- relay1 ----
  if (payload.get(jd, "fields/relay1/integerValue")) {
    relay1 = jd.to<String>().toInt() ? 1 : 0;
  } else if (payload.get(jd, "fields/relay1/booleanValue")) {
    relay1 = jd.to<bool>() ? 1 : 0;
  }

  // ---- relay2 ----
  if (payload.get(jd, "fields/relay2/integerValue")) {
    relay2 = jd.to<String>().toInt() ? 1 : 0;
  } else if (payload.get(jd, "fields/relay2/booleanValue")) {
    relay2 = jd.to<bool>() ? 1 : 0;
  }

  payload.clear();
  fbdoRead.clear();   // คืนหน่วยความจำ payload
}

/* --------------------------------------------------------------------------
 *  สร้างเอกสารเริ่มต้น (ครั้งแรกที่บอร์ดออนไลน์) ให้มีฟิลด์ครบ
 * ------------------------------------------------------------------------ */
void seedDocument() {
  if (!Firebase.ready()) return;

  // ถ้าเอกสารมีอยู่แล้วก็ไม่ต้องทำอะไร
  if (Firebase.Firestore.getDocument(&fbdoRead, FIREBASE_PROJECT_ID, "",
                                     documentPath.c_str(), "relay1")) {
    FirebaseJson payload;
    FirebaseJsonData jd;
    payload.setJsonData(fbdoRead.payload().c_str());
    bool exists = payload.get(jd, "fields/relay1/integerValue") ||
                  payload.get(jd, "fields/relay1/booleanValue");
    payload.clear();
    fbdoRead.clear();
    if (exists) {
      Serial.println(F("[Firestore] document already exists"));
      return;
    }
  } else if (fbdoRead.httpCode() != 404) {
    // get พังเพราะเน็ต/สิทธิ์ ไม่ใช่เพราะเอกสารไม่มี -> อย่าเพิ่งเขียนทับ
    Serial.print(F("[Firestore] seed skipped, get error: "));
    Serial.println(fbdoRead.errorReason());
    return;
  }

  Serial.println(F("[Firestore] seeding document..."));
  FirebaseJson content;
  content.setFloatDigits(2);
  content.set("fields/relay1/integerValue",      "0");
  content.set("fields/relay2/integerValue",      "0");
  content.set("fields/relay1State/integerValue", "0");
  content.set("fields/relay2State/integerValue", "0");
  content.set("fields/humidity/doubleValue",     (float)0);
  content.set("fields/temperature/doubleValue",  (float)0);
  content.set("fields/updatedAt/timestampValue", isoTimeUTC());

  const char* mask = "relay1,relay2,relay1State,relay2State,"
                     "humidity,temperature,updatedAt";

  if (Firebase.Firestore.patchDocument(&fbdo, FIREBASE_PROJECT_ID, "",
                                       documentPath.c_str(),
                                       content.raw(), mask)) {
    Serial.println(F("[Firestore] seed OK"));
  } else {
    Serial.print(F("[Firestore] seed FAILED: "));
    Serial.println(fbdo.errorReason());
  }
}

/* ==========================================================================
 *  4) setup()
 * ========================================================================== */
void setup() {
  Serial.begin(115200);
  delay(200);

  pinMode(relaypin1, OUTPUT);
  pinMode(relaypin2, OUTPUT);
  writeRelay(relaypin1, 0);    // เริ่มต้นปิดทั้งคู่
  writeRelay(relaypin2, 0);
  relay1Applied = 0;
  relay2Applied = 0;

  dht.begin();

  // ---------- WiFi ----------
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print(F("Connecting to WiFi"));
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }
  Serial.println();
  Serial.print(F("WiFi connected, IP: "));
  Serial.println(WiFi.localIP());

  // ---------- NTP (จำเป็นสำหรับ SSL/TLS และฟิลด์ timestamp) ----------
  configTime(0, 0, "pool.ntp.org", "time.google.com");
  Serial.print(F("Syncing time"));
  time_t now = time(nullptr);
  unsigned long tStart = millis();
  while (now < 1700000000 && millis() - tStart < 20000) {
    delay(300);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println();
  Serial.print(F("UTC time: "));
  Serial.println(isoTimeUTC());

  // ---------- Firebase ----------
  Serial.printf("Firebase Client v%s\n", FIREBASE_CLIENT_VERSION);

  config.api_key       = API_KEY;
  auth.user.email      = USER_EMAIL;
  auth.user.password   = USER_PASSWORD;
  config.token_status_callback = tokenStatusCallback;  // จาก TokenHelper.h
  config.max_token_generation_retry = 5;

  fbdo.setBSSLBufferSize(4096, 1024);
  fbdoRead.setBSSLBufferSize(4096, 1024);
  fbdo.setResponseSize(2048);
  fbdoRead.setResponseSize(2048);

  Firebase.reconnectWiFi(true);
  Firebase.begin(&config, &auth);

  // รอจนกว่าจะได้ ID token
  Serial.print(F("Waiting for Firebase auth"));
  tStart = millis();
  while (!Firebase.ready() && millis() - tStart < 30000) {
    delay(300);
    Serial.print(".");
  }
  Serial.println();

  if (Firebase.ready()) {
    Serial.println(F("Firebase ready"));
    seedDocument();
    fetchCommands();     // ดึงสถานะล่าสุดทันทีที่บูต
  } else {
    Serial.println(F("Firebase NOT ready -- ตรวจ API_KEY / email / password"));
  }

  lastUpload  = millis();
  lastCommand = millis();
  lastDhtRead = millis();
}

/* ==========================================================================
 *  5) loop()
 * ========================================================================== */
void loop() {
  unsigned long nowMs = millis();

  // ---------- ควบคุมรีเลย์ตามคำสั่งล่าสุด ----------
  if (relay1 != relay1Applied) {
    writeRelay(relaypin1, relay1);
    relay1Applied = relay1;
    forceUpload = true;
    Serial.printf("[RELAY] relay1 -> %d\n", relay1);
  }
  if (relay2 != relay2Applied) {
    writeRelay(relaypin2, relay2);
    relay2Applied = relay2;
    forceUpload = true;
    Serial.printf("[RELAY] relay2 -> %d\n", relay2);
  }

  // ---------- อ่าน DHT11 ----------
  if (nowMs - lastDhtRead >= DHT_READ_INTERVAL) {
    lastDhtRead = nowMs;
    float hNew = dht.readHumidity();
    float tNew = dht.readTemperature();
    if (isnan(hNew) || isnan(tNew)) {
      Serial.println(F("Failed to read from DHT sensor!"));
    } else {
      h = hNew;
      t = tNew;
      Serial.print(F("Humidity: "));
      Serial.print(h);
      Serial.print(F("%  Temperature: "));
      Serial.print(t);
      Serial.println(F(" C"));
    }
  }

  // ---------- ดึงคำสั่ง relay จาก Firestore ----------
  if (nowMs - lastCommand >= COMMAND_INTERVAL) {
    lastCommand = nowMs;
    fetchCommands();
  }

  // ---------- ส่งค่าขึ้น Firestore ----------
  if (forceUpload || (nowMs - lastUpload >= UPLOAD_INTERVAL)) {
    lastUpload  = millis();
    forceUpload = false;
    uploadToFirestore();
  }

  // ---------- WiFi หลุด -> ต่อใหม่ ----------
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println(F("WiFi lost, reconnecting..."));
    WiFi.reconnect();
    delay(1000);
  }
}
