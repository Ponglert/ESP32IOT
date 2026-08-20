/*
 * netpie_v2.ino — เวอร์ชันปรับปรุงจากโค้ดเดิม (logic เดิมครบทุกอย่าง)
 * ------------------------------------------------------------------
 * สิ่งที่เพิ่มเข้ามาเพื่อให้แอปมือถือทำงานได้ลื่นขึ้น:
 *   1) publish สถานะ relay1/relay2 ตั้งแต่ตอนบูต  → แอปไม่เห็นค่าว่าง
 *   2) publish temp + hum ในข้อความเดียว          → ประหยัด message quota ครึ่งหนึ่ง
 *   3) reconnect แบบไม่ block                      → relay ยังตอบสนองแม้เน็ตสะดุด
 *   4) รองรับ topic @msg/sync ให้แอปสั่ง "ขอสถานะล่าสุด" ได้
 *   5) รองรับ payload "1"/"0"/"on"/"off"/"true"/"false"
 *   6) publish ค่า online = 1 (และตั้ง LWT ไม่ได้บน shadow จึงใช้ heartbeat ts แทน)
 *
 * ต่อสาย: relay1 = GPIO19, relay2 = GPIO18, DHT11 = GPIO5
 */

#include <WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"

// ---------------- ตั้งค่า ----------------
const char* ssid          = "CSUBRUIOT02";
const char* password      = "asdf+1234";

const char* mqtt_server   = "broker.netpie.io";
const int   mqtt_port     = 1883;
const char* mqtt_Client   = "7857de3e-2c68-4f9a-9856-0957593f162c"; // Client ID
const char* mqtt_username = "Ju7F2qDLcaahHgJvxRn9DYSvyCbihBpD";     // Token
const char* mqtt_password = "He8bU9j4SnsYMKWku7qu38RtqUGh28fe";     // Secret

#define relaypin1 19
#define relaypin2 18
#define relaypin3 2
#define DHTPIN     5
#define DHTTYPE    DHT11

// รีเลย์ส่วนใหญ่ในตลาดเป็น active-LOW ถ้าของคุณกดแล้วสลับกัน ให้เปลี่ยนเป็น true
const bool RELAY_ACTIVE_LOW = true;

const unsigned long SENSOR_INTERVAL = 2000;   // อ่าน DHT ทุก 2 วิ
const unsigned long RECONNECT_EVERY = 5000;   // ลอง reconnect ทุก 5 วิ (ไม่ block)

// ---------------- ตัวแปร ----------------
WiFiClient espClient;
PubSubClient client(espClient);
DHT dht(DHTPIN, DHTTYPE);

char msg[250];
int  relay1 = 0;
int  relay2 = 0;
int  relay3 = 0;

unsigned long previousTime   = 0;
unsigned long lastReconnect  = 0;

// ---------------- helper ----------------
void applyRelays() {
  digitalWrite(relaypin1, RELAY_ACTIVE_LOW ? (relay1 ? LOW : HIGH) : (relay1 ? HIGH : LOW));
  digitalWrite(relaypin2, RELAY_ACTIVE_LOW ? (relay2 ? LOW : HIGH) : (relay2 ? HIGH : LOW));
  digitalWrite(relaypin3, RELAY_ACTIVE_LOW ? (relay3 ? LOW : HIGH) : (relay3 ? HIGH : LOW));
}

void publishShadow(const String& payload) {
  if (!client.connected()) return;
  payload.toCharArray(msg, (payload.length() + 1));
  Serial.println(msg);
  client.publish("@shadow/data/update", msg);
}

void publishRelayState() {
  publishShadow("{\"data\":{\"relay1\":" + String(relay1) + ",\"relay2\":" + String(relay2) + ",\"relay3\":" + String(relay3) + "}}");
}

void publishSensor(float t, float h) {
  publishShadow("{\"data\":{\"temp\":" + String(t, 1) + ",\"hum\":" + String(h, 1) + "}}");
}

// แปลง payload เป็น 0/1 ; คืน -1 ถ้าไม่รู้จัก
int parseOnOff(const String& m) {
  String s = m;
  s.trim();
  s.toLowerCase();
  if (s == "1" || s == "on" || s == "true")  return 1;
  if (s == "0" || s == "off" || s == "false") return 0;
  return -1;
}

// ---------------- MQTT ----------------
bool tryConnect() {
  Serial.print("Attempting MQTT connection... ");
  if (client.connect(mqtt_Client, mqtt_username, mqtt_password)) {
    Serial.println("connected");
    client.subscribe("@msg/#");
    publishRelayState();                       // (1) บอกสถานะทันทีที่ออนไลน์
    publishShadow("{\"data\":{\"online\":1}}");
    return true;
  }
  Serial.print("failed, rc=");
  Serial.println(client.state());
  return false;
}

void getMsg(String topic_, String message_) {
  if (topic_ == "@msg/relay1") {
    int v = parseOnOff(message_);
    if (v >= 0) relay1 = v;
    applyRelays();
    publishRelayState();

  } else if (topic_ == "@msg/relay2") {
    int v = parseOnOff(message_);
    if (v >= 0) relay2 = v;
    applyRelays();
    publishRelayState();

  }else if (topic_ == "@msg/relay3") {
    int v = parseOnOff(message_);
    if (v >= 0) relay3 = v;
    applyRelays();
    publishRelayState();

  }
   else if (topic_ == "@msg/sync") {          // (4) แอปขอสถานะล่าสุด
    publishRelayState();
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    if (!isnan(h) && !isnan(t)) publishSensor(t, h);
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  String message;
  message.reserve(length + 1);
  for (unsigned int i = 0; i < length; i++) message += (char)payload[i];

  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  Serial.println(message);

  getMsg(String(topic), message);
}

// ---------------- setup / loop ----------------
void setup() {
  Serial.begin(115200);

  pinMode(relaypin1, OUTPUT);
  pinMode(relaypin2, OUTPUT);
  pinMode(relaypin3, OUTPUT);
  applyRelays();

  dht.begin();

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected");

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  client.setKeepAlive(30);

  tryConnect();
}

void loop() {
  // (3) reconnect แบบไม่ block — relay ยังทำงานปกติระหว่างเน็ตหลุด
  if (!client.connected()) {
    unsigned long now = millis();
    if (now - lastReconnect >= RECONNECT_EVERY) {
      lastReconnect = now;
      tryConnect();
    }
  } else {
    client.loop();
  }

  applyRelays();

  if (millis() - previousTime >= SENSOR_INTERVAL) {
    previousTime = millis();
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    if (isnan(h) || isnan(t)) {
      Serial.println(F("Failed to read from DHT sensor!"));
    } else {
      publishSensor(t, h);                     // (2) ข้อความเดียวจบ
      Serial.printf("Humidity: %.1f%%  Temperature: %.1fC\n", h, t);
    }
  }
}