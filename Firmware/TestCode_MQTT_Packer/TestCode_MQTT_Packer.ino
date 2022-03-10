#include "MQTT_Packer.h"
uint8_t _COUNT=0;
char ClientID[] = "UpStream_12345";
char BUFFER[256]={0};

MQTT_Packer MSG(BUFFER);


void setup() {
  delay(1000);
  Serial.begin(9600);
  
  pinMode(13,OUTPUT);
  MSG.setDebuger(Serial);

}

void loop() {
  _COUNT++;
  digitalWrite(13,_COUNT%2);
  delay(200);
}
