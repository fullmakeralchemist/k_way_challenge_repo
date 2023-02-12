#include <WiFiNINA.h>  
#include <PubSubClient.h>
#include <Arduino_MKRIoTCarrier.h>
#include "pitches.h"
#include "secrets.h"

MKRIoTCarrier carrier;

uint32_t colorGreen = carrier.leds.Color(0, 200, 0);
uint32_t colorRed = carrier.leds.Color(200, 0, 0);
uint32_t colorBlue = carrier.leds.Color(0, 0, 200);

int celebrationMelody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

int finalMelody[] = {
  NOTE_E6, NOTE_G6, NOTE_E7, NOTE_C7, NOTE_D7, NOTE_G7, NOTE_E7, NOTE_G6
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};

WiFiClient wifi;
int status = WL_IDLE_STATUS;

IPAddress server(192, 168, 0, 0);//change this value
PubSubClient client(wifi);

bool touchCheckCompleted = false;

void setup() {
  //pinMode(6, OUTPUT);
  CARRIER_CASE = false;
  delay(1500);

  if (!carrier.begin())
  {
    Serial.println("Carrier not connected, check connections");
    while (1);
  }
  //while (!Serial){
  //  ;  
  //}
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
    client.subscribe("dance/lights");
  } else {
    Serial.println("mqtt not connected");
    Serial.print("failed, rc=");
    Serial.println(client.state());
  }

  //carrier.begin();
  //carrier.display.setRotation(0);

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
    if (msg == "on") {
      touchCheckCompleted = true;
      melody1();
      for (int ring = 0; ring <= 4; ring++) {
        carrier.leds.setPixelColor(ring, colorGreen);
        carrier.leds.show();
        delay(100);
        carrier.leds.clear();
        Serial.print("Hola desde if: ");
        Serial.println(msg);
        lightsOff();
      }
      for (int ring = 0; ring <= 4; ring++) {
        carrier.leds.setPixelColor(ring, colorGreen);
        carrier.leds.show();
        delay(100);
        carrier.leds.clear();
        Serial.print("Hola desde if: ");
        Serial.println(msg);
        lightsOff();
      }
    } else if(msg == "on2") {
      touchCheckCompleted = true;
      melody1();
      carrier.leds.setPixelColor(0, 200, 0, 255);
      carrier.leds.setPixelColor(1, 200, 0, 255);
      carrier.leds.setPixelColor(2, 200, 0, 255);
      carrier.leds.setPixelColor(3, 200, 0, 255);
      carrier.leds.setPixelColor(4, 200, 0, 255);
      carrier.leds.show();
      delay(500);
      lightsOff();
      delay(1000); 
      carrier.leds.setPixelColor(0, 200, 0, 255);
      carrier.leds.setPixelColor(1, 200, 0, 255);
      carrier.leds.setPixelColor(2, 200, 0, 255);
      carrier.leds.setPixelColor(3, 200, 0, 255);
      carrier.leds.setPixelColor(4, 200, 0, 255);
      carrier.leds.show();
      delay(500);
      lightsOff();
      delay(1000);
      carrier.leds.setPixelColor(0, 200, 0, 255);
      carrier.leds.setPixelColor(1, 200, 0, 255);
      carrier.leds.setPixelColor(2, 200, 0, 255);
      carrier.leds.setPixelColor(3, 200, 0, 255);
      carrier.leds.setPixelColor(4, 200, 0, 255);
      carrier.leds.show();
      delay(1000);
      lightsOff();
      } else if(msg == "on3") {
      touchCheckCompleted = true;
      melody1();
      for (int ring = 0; ring <= 4; ring++) {
        carrier.leds.setPixelColor(ring, colorBlue);
        carrier.leds.show();
        delay(500);
        carrier.leds.setPixelColor(ring, colorRed);
        carrier.leds.show();
        delay(500);
        carrier.leds.clear();
        Serial.print("Hola desde if: ");
        Serial.println(msg);
        lightsOff();
    }
    } else {  
        //lightsOff();
        Serial.print("el mensaje fue:");
        Serial.print(msg);
      }
         } 
  }

void lightsOff() {
  carrier.leds.setPixelColor(0, 0, 0, 0);
  carrier.leds.setPixelColor(1, 0, 0, 0);
  carrier.leds.setPixelColor(2, 0, 0, 0);
  carrier.leds.setPixelColor(3, 0, 0, 0);
  carrier.leds.setPixelColor(4, 0, 0, 0);
  carrier.leds.show();
}

void melody1() {
  if (touchCheckCompleted) {
    for (int thisNote = 0; thisNote < 8; thisNote++) {

    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    carrier.Buzzer.sound(finalMelody[thisNote]);
    delay(noteDuration);
    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.0;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    carrier.Buzzer.noSound();
    }
  }
}
