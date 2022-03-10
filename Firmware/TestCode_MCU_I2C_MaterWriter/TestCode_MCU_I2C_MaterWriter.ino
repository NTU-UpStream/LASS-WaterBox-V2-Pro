#include <Wire.h>

#define SW_1   5
#define SW_2   6
#define LED 7
#define MainPower   14

#define MPU_I2C     0x70

#define buffer_size 30

String cmd_setAPN = "APN,iot4ga2";
char CMD_BUFFER[buffer_size]; // CMD_BUFFER

uint8_t loop_x = 0;

void setPMU(char* _cmd) {
  Wire.beginTransmission(MPU_I2C); // transmit to device #8
  Wire.write(_cmd);        // sends five bytes
  Wire.endTransmission();    // stop transmitting
}

void initBuffer(String _str) {
  memset(CMD_BUFFER, '\0', buffer_size); // 清空buffer
  strcat(CMD_BUFFER, _str.c_str());       // 把CMD.c_str() 填入 CMD_BUFFER
}

String getSerial() {
  String _str;
  while (Serial.available()) {
    char _c = Serial.read();
    _str += _c;
  }
  return _str;
}

void setup() {
  Serial.begin(9600);
  Serial.println("序列埠初始化：9600");

  pinMode(LED, OUTPUT);
  pinMode(MainPower, OUTPUT);
  digitalWrite(MainPower, HIGH);

  pinMode(SW_1, INPUT);
  pinMode(SW_2, INPUT);

  Wire.setClock(400000);
  Wire.begin();                // join i2c bus with address #8
  Serial.println("Setup Done");   // print the integer
}


void loop() {
  String CMD = getSerial();
  if (CMD.length() > 0) {
    initBuffer(CMD);
    setPMU(CMD_BUFFER);

    Serial.print(CMD_BUFFER);
    Serial.print(",size:");
    Serial.println(String(CMD_BUFFER).length());

    if (loop_x % 2)  digitalWrite(LED, HIGH);
    else              digitalWrite(LED, LOW);

    delay(500);
  }
  loop_x++; 
}
