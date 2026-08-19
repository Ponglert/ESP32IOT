#include <WiFi.h>
#include <PubSubClient.h>
const char* ssid = "CSUBRUIOT";  //ชื่อ Wifi 
const char* password = "asdf+1234";  //รหัส Wifi
const char* mqtt_server = "broker.netpie.io";
const int mqtt_port = 1883;
const char* mqtt_Client = "03182886-d7eb-4222-ae06-962057ac9de1"; //ข้อมูล Client ID 
const char* mqtt_username = "kFN77CFz4gNNxmYmjB4EjnoMxtbpj1x2"; //ข้อมูล Token 
const char* mqtt_password = "BR7JsjHR61ghFs7vgBjWJZgKD1Enunrg"; //ข้อมูล Secret 
WiFiClient espClient;
PubSubClient client(espClient);
char msg[250];
#define relaypin1 19
#define relaypin2 18
int relay1=0;
int relay2=0;

#include "DHT.h"
#define DHTPIN 5
#define DHTTYPE DHT11    
DHT dht(DHTPIN, DHTTYPE);
unsigned long previousTime = 0;


void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection…");
    if (client.connect(mqtt_Client, mqtt_username, mqtt_password)) {
      Serial.println("connected");
      client.subscribe("@msg/#");  
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      delay(5000);
    }
  }
}

void getMsg(String topic_, String message_) {     
  if (topic_ == "@msg/relay1") {
    if (message_ == "1") {                  
      relay1 = 1;
    } else if (message_ == "0") {          
      relay1 = 0;
    }
    String data = "{\"data\":{\"relay1\": " + String(relay1) + "}}";
    Serial.println(data);
    data.toCharArray(msg , (data.length() + 1));
    client.publish("@shadow/data/update", msg);
  }else if(topic_ == "@msg/relay2"){ 
     if (message_ == "1") {                  
      relay2 = 1;
    } else if (message_ == "0") {          
      relay2 = 0;
    }
    String data = "{\"data\":{\"relay2\": " + String(relay2) + "}}";
    Serial.println(data);
    data.toCharArray(msg , (data.length() + 1));
    client.publish("@shadow/data/update", msg);
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String message;
  for (int i = 0; i < length; i++) {
    message = message + (char)payload[i];
  }
  Serial.println(message);
  getMsg(topic, message);
}

void setMsgTemp(String temp, String hum){
  String data = "{\"data\":{\"temp\": " + temp + "}}";
  Serial.println(data);
  data.toCharArray(msg , (data.length() + 1));
  client.publish("@shadow/data/update", msg);

  data = "{\"data\":{\"hum\": " + hum + "}}";
  Serial.println(data);
  data.toCharArray(msg , (data.length() + 1));
  client.publish("@shadow/data/update", msg);
}


void setup() {
  pinMode(relaypin1, OUTPUT);
  pinMode(relaypin2, OUTPUT);

  dht.begin();

  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  delay(1000);  
 if(!client.connected()) {
    reconnect();
 }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  if(relay1 == 1){
    if(!digitalRead(relaypin1)){
      digitalWrite(relaypin1, HIGH);
    }      
  }else{
    if(digitalRead(relaypin1)){
      digitalWrite(relaypin1, LOW);
    }  
  }
  if(relay2 == 1){
    if(!digitalRead(relaypin2)){
      digitalWrite(relaypin2, HIGH);
    }      
  }else{
    if(digitalRead(relaypin2)){
      digitalWrite(relaypin2, LOW);
    }  
  }

  if((millis() - previousTime) >= 2000){
    previousTime = millis();
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    if (isnan(h) || isnan(t)) {
      Serial.println(F("Failed to read from DHT sensor!"));
    }else{
      setMsgTemp(String(t),String(h));
      Serial.print(F("Humidity: "));
      Serial.print(h);
      Serial.print(F("%  Temperature: "));
      Serial.print(t);
      Serial.println(F("°C "));
    }
  }

  client.loop();
}