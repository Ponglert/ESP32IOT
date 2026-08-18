# ESP32 + Firebase Firestore


- ส่งขึ้น: `humidity` (h), `temperature` (t), `relay1State`, `relay2State`, `updatedAt`
- ดึงกลับ: `relay1`, `relay2` (คำสั่งเปิด/ปิดจากแอปหรือหน้าเว็บ)

---

## 1. เตรียมโปรเจกต์ Firebase

1. เข้า https://console.firebase.google.com → **Add project**
2. เมนู **Build → Firestore Database** → *Create database* → เลือก **Production mode** → เลือก location (แนะนำ `asia-southeast1`)
3. เมนู **Build → Authentication** → *Get started* → แท็บ **Sign-in method** → เปิด **Email/Password**
4. แท็บ **Users** → *Add user* → ใส่อีเมล/รหัสผ่านสำหรับบอร์ด เช่น
   `device01@example.com` / `SuperSecret123`
   > ใช้อีเมลปลอมได้ ไม่ต้องยืนยันอีเมล
5. **Project settings (⚙️) → General** → คัดลอก
   - **Project ID** → ใส่ใน `FIREBASE_PROJECT_ID`
   - **Web API Key** → ใส่ใน `API_KEY`

---

## 2. ค่าที่ต้องแก้ในสเก็ตช์

```cpp
const char* WIFI_SSID     = "CSUBRUIOT";
const char* WIFI_PASSWORD = "asdf+1234";

#define API_KEY             "AIza..."               // Web API Key
#define FIREBASE_PROJECT_ID "my-esp32-project"      // Project ID
#define USER_EMAIL          "device01@example.com"
#define USER_PASSWORD       "SuperSecret123"
#define DEVICE_ID           "esp32-01"
```

ถ้าโมดูลรีเลย์เป็นแบบ **Active LOW** (สั่ง LOW แล้วติด — พบบ่อยในโมดูลจีน 5V opto)
ให้เปลี่ยน `#define RELAY_ACTIVE_LOW 0` เป็น `1`

---

## 3. ไลบรารีที่ต้องติดตั้ง

Arduino IDE → **Tools → Manage Libraries…**

| ไลบรารี | ผู้พัฒนา | เวอร์ชัน |
|---|---|---|
| Firebase Arduino Client Library for ESP8266 and ESP32 | Mobizt | **4.4.x** |
| DHT sensor library | Adafruit | ล่าสุด |
| Adafruit Unified Sensor | Adafruit | ล่าสุด |

> ⚠️ อย่าใช้เวอร์ชัน 5.x หรือไลบรารีตัวใหม่ชื่อ `FirebaseClient` — API คนละแบบ
> โค้ดนี้จะคอมไพล์ไม่ผ่าน

Board: **ESP32 Dev Module** (ESP32 Arduino core 2.0.x ขึ้นไป)
Partition Scheme: ถ้าคอมไพล์แล้วบอกว่าโปรแกรมใหญ่เกิน ให้เลือก
`Huge APP (3MB No OTA/1MB SPIFFS)`

---

## 4. โครงสร้างข้อมูลใน Firestore

เอกสารเดียว: `devices/esp32-01`

| ฟิลด์ | ชนิด | ใครเขียน | ความหมาย |
|---|---|---|---|
| `relay1` | number | **แอป/เว็บ** | คำสั่ง 0 = ปิด, 1 = เปิด |
| `relay2` | number | **แอป/เว็บ** | คำสั่ง 0 = ปิด, 1 = เปิด |
| `relay1State` | number | ESP32 | สถานะจริงบนบอร์ด (ยืนยันว่าสั่งแล้ว) |
| `relay2State` | number | ESP32 | สถานะจริงบนบอร์ด |
| `humidity` | number | ESP32 | ค่า h (%RH) |
| `temperature` | number | ESP32 | ค่า t (°C) |
| `updatedAt` | timestamp | ESP32 | เวลาอัปเดตล่าสุด (UTC) |

**ทำไมต้องแยก `relay1` กับ `relay1State`?**
ถ้า ESP32 เขียนทับฟิลด์ `relay1` เอง จะมีจังหวะที่แอปเพิ่งกดเปิด แล้ว ESP32
เขียนค่าเก่า (0) ทับพอดี → รีเลย์เด้งกลับ (race condition)
โค้ดนี้ ESP32 จึง **อ่านอย่างเดียว** จาก `relay1/relay2` และรายงานผลจริงลง `*State`
ฝั่งหน้าจอเอาไว้เช็คได้ว่า “สั่งไปแล้วบอร์ดทำตามจริงไหม”

---

## 5. Security Rules

Firestore → **Rules** → วางแล้วกด *Publish*

```
rules_version = '2';
service cloud.firestore {
  match /databases/{database}/documents {
    match /devices/{deviceId} {
      // ต้องล็อกอินก่อนถึงจะอ่าน/เขียนได้
      allow read, write: if request.auth != null;
    }
  }
}
```

**เข้มขึ้นอีกขั้น** — ล็อกให้เฉพาะ UID ของบอร์ดเขียน `*State` และผู้ใช้คนอื่นเขียนได้แค่คำสั่ง:

```
match /devices/{deviceId} {
  allow read: if request.auth != null;
  allow write: if request.auth != null
               && request.resource.data.diff(resource.data)
                    .affectedKeys()
                    .hasOnly(['relay1','relay2','relay1State',
                              'relay2State','humidity',
                              'temperature','updatedAt']);
}
```

> อย่าปล่อย rules เป็น `allow read, write: if true;` เพราะ API Key ฝังอยู่ในบอร์ด
> ใครก็เขียนข้อมูลเข้ามาได้

---

## 6. สั่งเปิด/ปิดรีเลย์จากฝั่งแอป

### JavaScript (Web / React)

```js
import { doc, updateDoc } from "firebase/firestore";

// เปิด relay1
await updateDoc(doc(db, "devices", "esp32-01"), { relay1: 1 });

// ปิด relay2
await updateDoc(doc(db, "devices", "esp32-01"), { relay2: 0 });
```

### อ่านค่าแบบเรียลไทม์

```js
import { doc, onSnapshot } from "firebase/firestore";

onSnapshot(doc(db, "devices", "esp32-01"), (snap) => {
  const d = snap.data();
  console.log(d.temperature, d.humidity, d.relay1State, d.relay2State);
});
```

### หรือทดสอบเร็ว ๆ
เข้า Firestore Console แล้วแก้ค่า `relay1` เป็น `1` ด้วยมือ
ภายใน ~5 วินาที รีเลย์จะติด และ `relay1State` จะเปลี่ยนตาม

---

## 7. เรื่องโควตา (แผน Spark / ฟรี)

| | โควตา/วัน | โค้ดนี้ใช้ |
|---|---|---|
| Writes | 20,000 | ทุก 15 วิ → ~5,760 |
| Reads | 50,000 | ทุก 5 วิ → ~17,280 |

ปรับได้ที่:

```cpp
const unsigned long UPLOAD_INTERVAL  = 15000;  // ส่งค่าขึ้น
const unsigned long COMMAND_INTERVAL = 5000;   // ดึงคำสั่ง
```

> Firestore **ไม่มี** realtime stream บน Arduino จึงต้อง poll เอา
> ถ้าอยากได้ตอบสนองทันที (<1 วิ) และประหยัดโควตากว่า ควรย้ายฝั่งคำสั่งไปใช้
> **Realtime Database** ซึ่งไลบรารีตัวนี้รองรับ `Firebase.RTDB.beginStream()` — บอกมาได้ ผมเขียนเวอร์ชันนั้นให้

---

## 8. เช็คลิสต์เวลามีปัญหา

| อาการบน Serial Monitor (115200) | สาเหตุ |
|---|---|
| `token error: INVALID_EMAIL / EMAIL_NOT_FOUND` | ยังไม่ได้สร้าง user ใน Authentication |
| `INVALID_PASSWORD` | รหัสผ่านผิด |
| `API key not valid` | `API_KEY` ผิด หรือใช้ค่าจาก google-services.json ผิดตัว |
| `PERMISSION_DENIED` | Security Rules ยังเป็น production mode ปิดตาย |
| `NOT_FOUND` ตอน get | เอกสาร `devices/esp32-01` ยังไม่ถูกสร้าง (โค้ดจะ seed ให้เอง) |
| ค้างที่ `Syncing time...` | เราเตอร์บล็อก NTP (พอร์ต 123) — ลองเปลี่ยนเป็นเน็ตมือถือ |
| `connection refused` / SSL error | ESP32 core เก่าเกินไป หรือ RAM ไม่พอ ลองลด `setBSSLBufferSize` |
| `Failed to read from DHT sensor!` | ต่อสายผิด หรือลืมตัวต้านทาน pull-up 10kΩ ระหว่าง VCC กับ DATA |

