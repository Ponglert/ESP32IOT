# ตอนที่ 6: การเขียนโปรแกรมเพื่อแสดงค่าบน Blynk Platform

Blynk Platform คือ Application สำเร็จรูป ที่ถูกออกแบบมาเพื่อใช้ในการควบคุมอุปกรณ์ IOT ซึ่งมีคุณสมบัติในการควบคุมจากระยะไกลผ่านเครือข่ายอินเตอร์เน็ต และยังสามารถแสดงผลค่าจาก เซนเซอร์ต่าง ๆ ได้ แบบ Real time สามารถเชื่อมต่อ Device ต่าง ๆ รองรับทั้งระบบ Android และIOS อีกด้วย

![](media/image1.webp)

1\) เข้าหน้าเว็บ https://www.blynk.io/ และสมัครสมาชิก

![](media/image2.webp)

![](media/image3.webp)

![](media/image4.webp)

2\) สร้างโปรเจ็คบน Blynk

![](media/image5.webp)

![](media/image6.webp)

![](media/image7.webp)

![](media/image8.webp)

![](media/image9.webp)

![](media/image10.webp)

![](media/image11.webp)

![](media/image12.webp)

![](media/image13.webp)

![](media/image14.webp)

![](media/image15.webp)

![](media/image16.webp)

![](media/image17.webp)

![](media/image18.webp)

3\) ติดตั้ง Library ของ Blynk Platform

![](media/image19.webp)

![](media/image20.webp)

![](media/image21.webp)

![](media/image22.webp)

4\) ทำการเชื่อมต่อวงจรโมดูลควบคุมโมดูลรีเลย์ ดังต่อไปนี้

![](media/image23.webp)

5\) การเขียนโปรแกรมเพื่อส่งข้อมูลไปแสดงผลบน Blynk Platform

![](media/image24.webp)

![](media/image25.webp)

![](media/image26.webp)

![](media/image27.webp)

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

![](media/image28.webp)

8\) เข้าสู่ระบบและตั้งค่าใช้งาน Blynk Application ควบการทำงานของอุปกรณ์

![](media/image29.webp)

![](media/image30.webp)

![](media/image31.webp)

![](media/image32.webp)

9\) ทดสอบการทำงาน

## การส่งค่าเซนเซอร์ข้อมูลไปแสดงผลบน Blynk

1\) เพิ่ม Virtual Pin ลงใน Datastreams

![](media/image33.webp)

![](media/image34.webp)

![](media/image35.webp)

![](media/image36.webp)

![](media/image37.webp)

![](media/image38.webp)

![](media/image39.webp)

![](media/image40.webp)

2\) ทำการเชื่อมต่อวงจรโมดูลเซนเซอร์ DHT11 เพิ่มเข้าไป ดังต่อไปนี้

![](media/image41.webp)

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
      Serial.print(F("%  Temperature: "));
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

![](media/image42.webp)

![](media/image43.webp)

![](media/image44.webp)

## แบบฝึกหัด

ทำการแสดงค่า**ความชื้นในอากาศจากเซนเซอร์**บนแอพพลิเคชั่น blynk และเว็บ Dashboard blynk โดยให้ทำการเขียนโปรแกรมเพิ่มจากโค้ดก่อนหน้านี้ และตั้งค่าต่าง ๆ เพิ่มในแพลตฟอร์ม blynk ให้ได้ผลลัพธ์ดังรูป

![](media/image45.webp)

<img src="media/image46.webp" width="274" />
