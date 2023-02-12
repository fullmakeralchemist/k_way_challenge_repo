#include <WiFiNINA.h>  
#include <PubSubClient.h>
#include "secrets.h"

WiFiClient wifi;
int status = WL_IDLE_STATUS;

IPAddress server(192, 168, 0, 0); //Ip raspberry
PubSubClient client(wifi);

void setup() {
//  while (!Serial){
//    ;  
//  }
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);
  Serial.begin(9600);
  Serial.println("OK");
  Serial.print("Connessione...");
  while (status != WL_CONNECTED) {
    status = WiFi.begin(WIFI_SSID, WIFI_PASS);
    Serial.print(".");
    delay(1000);
  }
  Serial.println("Connected to WiFi!\n");

  client.setServer(server, 1883);
  client.setCallback(callback);

  if (client.connect("arduinosub")) {
    Serial.println("mqtt connected");
    client.subscribe("dance/lights"); //change depending on the topic you want to use or add new ones in case you're using more
  } else {
    Serial.println("mqtt not connected");
    Serial.print("failed, rc=");
    Serial.println(client.state());
  }
}

void loop() {
  client.loop();
}

void callback(char* topic, byte* payload, unsigned int length){
  String msg;
  for (int i = 0; i < length; i++) {
    msg += (char)payload[i];  
  }
  Serial.println(msg);
  if (strcmp(topic, "dance/lights") == 0) {
    if (msg == "on") {//change this in case you're receiveing different messages
      digitalWrite(LEDR, HIGH);//change all this part depending on what components you're using
      delay(500);
      digitalWrite(LEDR, LOW);
      delay(500);
      digitalWrite(LEDR, HIGH);
      delay(500);
      digitalWrite(LEDR, LOW);
      delay(500);
      digitalWrite(LEDR, HIGH);
      delay(500);
      digitalWrite(LEDR, LOW);
    }else if(msg == "on2"){
      digitalWrite(LEDG, HIGH);
      delay(400);
      digitalWrite(LEDG, LOW);
      delay(400);
      digitalWrite(LEDG, HIGH);
      delay(400);
      digitalWrite(LEDG, LOW);
      delay(400);
      digitalWrite(LEDG, HIGH);
      delay(400);
      digitalWrite(LEDG, LOW);
      Serial.print("el mensaje fue:");
      Serial.print(msg);
    }else if(msg == "on3"){
      digitalWrite(LEDB, HIGH);
      delay(1500);
      digitalWrite(LEDB, LOW);
      Serial.print("el mensaje fue:");
      Serial.print(msg);
    }
    else{
      //lightsOff();
      Serial.print("el mensaje fue:");
      Serial.print(msg);
         } 
  }
}
