#include <LWiFi.h>
#include <Wire.h>

byte mac[6];

#define SW_1   5
#define SW_2   6
#define LED 7
#define MainPower   14

#define MPU_I2C     0x70

#define buffer_size 32

#define cmd_setAPN       "APN,iot4ga2"
#define cmd_setSleepTime "TIME,10"
#define cmd_setLowPower  "LOWPOWER,1" // 1 為啟動睡眠, 0 為不睡覺
#define cmd_getCurrent   "CURRENT"
#define cmd_setVolate    "VOLATE"
#define cmd_enterSleep   "SLEEP"

#define cmd_F1     "F1,2022-03-10"
#define cmd_F2     "F2,23:00:00"
#define cmd_F3     "F3," // DeviceID:MAC
#define cmd_F4     "F4,-123.222222"      //經度 gps_lon
#define cmd_F5     "F5,-25.193323"       // gps_lat
#define cmd_F6     "F6,200000.00"        // EC
#define cmd_F7     "F7,14.00"            // pH
#define cmd_F8     "F8,100.00"           // Temp
#define cmd_F9     "F9,10000.00"         // s_Tb
#define cmd_F10    "F10,3.71"            // bat_v
#define cmd_F11    "F11,-400.00"         // bat_mA


char CMD_BUFFER[buffer_size]; // CMD_BUFFER

uint8_t loop_x = 0;

void setPMU(char* _cmd) {
  Wire.beginTransmission(MPU_I2C); // transmit to device #8
  Wire.write(_cmd);
  Wire.endTransmission();    // stop transmitting

  reqPMU();
}

void reqPMU() {
  Serial.print("[REQ]\t");
  Wire.requestFrom(MPU_I2C, 30);    // request 6 bytes from slave device #8
  while (Wire.available()) { // slave may send less than requested
    char c = Wire.read(); // receive a byte as character
    Serial.print(c);         // print the character
  }
  Serial.println();
  delay(500);
}

void initBuffer(String _str) {
  memset(CMD_BUFFER, '\0', buffer_size); // 清空buffer
  strcat(CMD_BUFFER, _str.c_str());       // 把CMD.c_str() 填入 CMD_BUFFER
}

String ZeroPadding(byte _value) {
  String _result;
  if (_value < 16)  _result = "0" + String(_value, HEX);
  else              _result = String(_value, HEX);
  return _result;
}

String MacAddress(void) {
  String _result;
  WiFi.macAddress(mac);
  _result.concat(ZeroPadding(mac[5]));
  _result.concat(ZeroPadding(mac[4]));
  _result.concat(ZeroPadding(mac[3]));
  _result.concat(ZeroPadding(mac[2]));
  _result.concat(ZeroPadding(mac[1]));
  _result.concat(ZeroPadding(mac[0]));
  return _result;
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
  Serial.begin(115200);
  Serial.println("序列埠初始化：115200");

  pinMode(LED, OUTPUT);
  pinMode(MainPower, OUTPUT);
  digitalWrite(MainPower, HIGH);

  pinMode(SW_1, INPUT);
  pinMode(SW_2, INPUT);

  Wire.setClock(400000);
  Wire.begin();                // join i2c bus with address #8

  setPMU(cmd_setAPN);
  setPMU(cmd_setSleepTime);
  setPMU(cmd_setLowPower);
  // F3 的設定
  String CMD = cmd_F3 + MacAddress();
  initBuffer(CMD);

  setPMU(cmd_F1);
  setPMU(cmd_F2);
  setPMU(CMD_BUFFER);
  setPMU(cmd_F4);
  setPMU(cmd_F5);
  setPMU(cmd_F6);
  setPMU(cmd_F7);
  setPMU(cmd_F8);
  setPMU(cmd_F9);
  setPMU(cmd_F10);
  setPMU(cmd_F11);

  Serial.println("Setup Done");   // print the integer
  setPMU(cmd_enterSleep);
}


void loop() {
  String CMD = getSerial();
  if (CMD.length() > 0) {

    if  (CMD.equalsIgnoreCase("MAC")) {
      CMD = cmd_F3;
      CMD += MacAddress();
    }
    initBuffer(CMD);

    Serial.print("[CMD]\t");
    Serial.print(CMD_BUFFER);
    Serial.print(",size: ");
    Serial.println(String(CMD_BUFFER).length());

    setPMU(CMD_BUFFER);
    reqPMU();

    if (loop_x % 2)  digitalWrite(LED, HIGH);
    else             digitalWrite(LED, LOW);

    delay(500);
  }
  loop_x++;
}
