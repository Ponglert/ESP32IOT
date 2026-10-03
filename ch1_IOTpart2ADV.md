**ตอนที่ 6**

**การเขียนโปรแกรมเพื่อแสดงค่าบน Blynk Platform**

Blynk Platform คือ Application สำเร็จรูป ที่ถูกออกแบบมาเพื่อใช้ในการควบคุมอุปกรณ์ IOT ซึ่งมีคุณสมบัติในการควบคุมจากระยะไกลผ่านเครือข่ายอินเตอร์เน็ต และยังสามารถแสดงผลค่าจาก เซนเซอร์ต่าง ๆ ได้ แบบ Real time สามารถเชื่อมต่อ Device ต่าง ๆ รองรับทั้งระบบ Android และIOS อีกด้วย

![รูปที่ 1](ch1_IOTpart2ADV_images/fig01.jpg)

1\) เข้าหน้าเว็บ https://www.blynk.io/ และสมัครสมาชิก

![รูปที่ 2](ch1_IOTpart2ADV_images/fig02.jpg)

![รูปที่ 3](ch1_IOTpart2ADV_images/fig03.jpg)

![รูปที่ 4](ch1_IOTpart2ADV_images/fig04.jpg)

2\) สร้างโปรเจ็คบน Blynk

![รูปที่ 5](ch1_IOTpart2ADV_images/fig05.jpg)

![รูปที่ 6](ch1_IOTpart2ADV_images/fig06.jpg)

![รูปที่ 7](ch1_IOTpart2ADV_images/fig07.jpg)

![รูปที่ 8](ch1_IOTpart2ADV_images/fig08.jpg)

![รูปที่ 9](ch1_IOTpart2ADV_images/fig09.jpg)

![รูปที่ 10](ch1_IOTpart2ADV_images/fig10.jpg)

![รูปที่ 11](ch1_IOTpart2ADV_images/fig11.jpg)

![รูปที่ 12](ch1_IOTpart2ADV_images/fig12.jpg)

![รูปที่ 13](ch1_IOTpart2ADV_images/fig13.jpg)

![รูปที่ 14](ch1_IOTpart2ADV_images/fig14.jpg)

![รูปที่ 15](ch1_IOTpart2ADV_images/fig15.jpg)

![รูปที่ 16](ch1_IOTpart2ADV_images/fig16.jpg)

![รูปที่ 17](ch1_IOTpart2ADV_images/fig17.jpg)

![รูปที่ 18](ch1_IOTpart2ADV_images/fig18.jpg)

3\) ติดตั้ง Library ของ Blynk Platform

![รูปที่ 19](ch1_IOTpart2ADV_images/fig19.jpg)

![รูปที่ 20](ch1_IOTpart2ADV_images/fig20.jpg)

![รูปที่ 21](ch1_IOTpart2ADV_images/fig21.jpg)

![รูปที่ 22](ch1_IOTpart2ADV_images/fig22.jpg)

4\) ทำการเชื่อมต่อวงจรโมดูลควบคุมโมดูลรีเลย์ ดังต่อไปนี้

![รูปที่ 23](ch1_IOTpart2ADV_images/fig23.jpg)

5\) การเขียนโปรแกรมเพื่อส่งข้อมูลไปแสดงผลบน Blynk Platform

![รูปที่ 24](ch1_IOTpart2ADV_images/fig24.jpg)

![รูปที่ 25](ch1_IOTpart2ADV_images/fig25.jpg)

![รูปที่ 26](ch1_IOTpart2ADV_images/fig26.jpg)

![รูปที่ 27](ch1_IOTpart2ADV_images/fig27.jpg)

```cpp
#define BLYNK_TEMPLATE_ID "TMPL6za9X8hxg"
#define BLYNK_TEMPLATE_NAME "testesp32"
#define BLYNK_AUTH_TOKEN "HJZ-kfnn9ELeG7771f3vEbPK7Qb2hHch"
#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
char ssid[] = "CSUBRUIOT02";
char pass[] = "asdf+1234";
byte led1 = 19;
byte led2 = 18;
byte val1, val2;
void setup() {
Serial.begin(9600);
Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
pinMode(led1, OUTPUT);
pinMode(led2, OUTPUT);
}
void loop() {
Blynk.run();
digitalWrite(led1,val1);
digitalWrite(led2,val2);
}
BLYNK_CONNECTED(){
Blynk.syncVirtual(V1,V2);
}
BLYNK_WRITE(V1){
val1 = param.asInt();
}
BLYNK_WRITE(V2){
val2 = param.asInt();
}
```

6\) จากนั้นให้ทำ Save และรันอัพโหลดโปรแกรมลงใน ESP32

7\) ติดตั้ง Blynk Application ในสมาร์ทโฟน

![รูปที่ 28](ch1_IOTpart2ADV_images/fig28.jpg)

8\) เข้าสู่ระบบและตั้งค่าใช้งาน Blynk Application ควบการทำงานของอุปกรณ์

![รูปที่ 29](ch1_IOTpart2ADV_images/fig29.jpg)

![รูปที่ 30](ch1_IOTpart2ADV_images/fig30.jpg)

![รูปที่ 31](ch1_IOTpart2ADV_images/fig31.jpg)

![รูปที่ 32](ch1_IOTpart2ADV_images/fig32.jpg)

9\) ทดสอบการทำงาน

**การส่งค่าเซนเซอร์ข้อมูลไปแสดงผลบน Blynk**

1\) เพิ่ม Virtual Pin ลงใน Datastreams

![รูปที่ 33](ch1_IOTpart2ADV_images/fig33.jpg)

![รูปที่ 34](ch1_IOTpart2ADV_images/fig34.jpg)

![รูปที่ 35](ch1_IOTpart2ADV_images/fig35.jpg)

![รูปที่ 36](ch1_IOTpart2ADV_images/fig36.jpg)

![รูปที่ 37](ch1_IOTpart2ADV_images/fig37.jpg)

![รูปที่ 38](ch1_IOTpart2ADV_images/fig38.jpg)

![รูปที่ 39](ch1_IOTpart2ADV_images/fig39.jpg)

![รูปที่ 40](ch1_IOTpart2ADV_images/fig40.jpg)

2\) ทำการเชื่อมต่อวงจรโมดูลเซนเซอร์ DHT11 เพิ่มเข้าไป ดังต่อไปนี้

![รูปที่ 41](ch1_IOTpart2ADV_images/fig41.jpg)

3\) การเขียนโปรแกรมเพิ่มเพื่อส่งข้อมูลไปแสดงผลบน Blynk Platform

```cpp
#define BLYNK_TEMPLATE_ID "TMPL6za9X8hxg"
#define BLYNK_TEMPLATE_NAME "testesp32"
#define BLYNK_AUTH_TOKEN "HJZ-kfnn9ELeG7771f3vEbPK7Qb2hHch"
#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
char ssid[] = "CSUBRUIOT02";
char pass[] = "asdf+1234";
#include "DHT.h"
#define DHTPIN 5
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
unsigned long previousTime = 0;
byte led1 = 19;
byte led2 = 18;
byte val1, val2;
void setup() {
Serial.begin(9600);
Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
pinMode(led1, OUTPUT);
pinMode(led2, OUTPUT);
dht.begin();
}
void loop() {
if((millis() - previousTime) >= 2000){
previousTime = millis();
float h = dht.readHumidity();
float t = dht.readTemperature();
if (isnan(h) || isnan(t)) {
Serial.println(F("Failed to read from DHT sensor!"));
}
else{
Blynk.virtualWrite(V3, t);
Serial.print(F("Humidity: "));
Serial.print(h);
Serial.print(F("% Temperature: "));
Serial.print(t);
Serial.println(F("°C "));
}
}
Blynk.run();
digitalWrite(led1,val1);
digitalWrite(led2,val2);
}
BLYNK_CONNECTED(){
Blynk.syncVirtual(V1,V2);
}
BLYNK_WRITE(V1){
val1 = param.asInt();
}
BLYNK_WRITE(V2){
val2 = param.asInt();
}
```

7\) เข้าสู่ระบบและตั้งค่าใช้งานเพิ่มเติม Blynk Application

![รูปที่ 42](ch1_IOTpart2ADV_images/fig42.jpg)

![รูปที่ 43](ch1_IOTpart2ADV_images/fig43.jpg)

![รูปที่ 44](ch1_IOTpart2ADV_images/fig44.jpg)

**แบบฝึกหัด**

ทำการแสดงค่า**ความชื้นในอากาศจากเซนเซอร์**บนแอพพลิเคชั่น blynk และเว็บ Dashboard blynk โดยให้ทำการเขียนโปรแกรมเพิ่มจากโค้ดก่อนหน้านี้ และตั้งค่าต่าง ๆ เพิ่มในแพลตฟอร์ม blynk ให้ได้ผลลัพธ์ดังรูป

![รูปที่ 45](ch1_IOTpart2ADV_images/fig45.jpg)

![รูปที่ 46](ch1_IOTpart2ADV_images/fig46.jpg)
