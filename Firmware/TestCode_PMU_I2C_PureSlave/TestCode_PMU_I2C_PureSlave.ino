#define LinkIt7697  2
#define SIMCOM7000E 3
#define INA219      4
#define LED      13

#define MPU_I2C     0x70
#include <Wire.h>

void setup() {

  Serial.println("序列埠初始化：9600");

  pinMode(INA219, OUTPUT);
  pinMode(SIMCOM7000E, OUTPUT);
  pinMode(LinkIt7697, OUTPUT);
  pinMode(LED, OUTPUT);


  digitalWrite(LinkIt7697, HIGH);
  digitalWrite(INA219, HIGH);
  digitalWrite(SIMCOM7000E, HIGH);
  Wire.setClock(400000);

  Wire.begin(MPU_I2C);                // join I2C bus with address #8
  Wire.onReceive(receiveEvent); // register event
}

void loop() {
  delay(500);
  digitalWrite(LED, HIGH);
  delay(500);
  digitalWrite(LED, LOW);
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) {
  while (1 < Wire.available()) { // loop through all but the last
    char c = Wire.read(); // receive byte as a character
    Serial.print(c);         // print the character
  }
  int x = Wire.read();    // receive byte as an integer
  Serial.println(x);         // print the integer
}
