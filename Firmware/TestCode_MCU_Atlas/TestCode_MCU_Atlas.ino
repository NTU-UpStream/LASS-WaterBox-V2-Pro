/***** << WaterBox_V2.0:pin state test with ADS1115 >> *****
   ADC 數據及 switch切換
   使用Library Adafruit_ADS1X15 (Ver. 1.1.0~Ver. 1.1.2)
***********************************************************/
#include <Wire.h>
#include <Adafruit_ADS1015.h>

Adafruit_ADS1115 ads;  /* Use this for the 16-bit version */
//Adafruit_ADS1015 ads;     /* Use thi for the 12-bit version */

/****<< 這是舊的pin設定 >>*****************
  #define VR_1  A0
  #define VR_2  A1
  #define EC_pin        A2
  #define pH_pin        A3
*****<< 這是舊的pin設定 >>*****************/

#define power_pin   14      // Linklt 7969 以外的所有模組電源開關
#define switch_pin  5       // 模式開關 SW1
#define USR_pin     6       // 功能鍵 SW2  下拉時會關閉EC/pH的電源
#define pinLED      7       // Linklt 7969 上的USR LED || DS18B20 用腳位

/****
   Atlas
*/
#define addr_pH 0x63
#define addr_EC 0x64
#define addr_Temp 0x66

/****** << 打開系統電源時要帶入的函數 >> *****/
void systemPower(bool _state = false) {
  digitalWrite(power_pin, _state);
  if (_state) {
    ads.begin();
  }
}

/***** << Atlas Scientific Module function >> *****/
String atlas_cmd(String _cmd, uint16_t _delayMS, uint8_t _addr, bool _info = false)
{

  byte _char = 0;                //used as a 1 byte buffer to store inbound bytes from the D.O. Circuit.
  String _responce = "";

  Wire.beginTransmission(_addr);
  Wire.write(_cmd.c_str());
  Wire.endTransmission();

  delay(_delayMS);

  Wire.requestFrom(_addr, 20, 1); //call the circuit and request 20 bytes (this is more than we need)
  byte code = Wire.read();

  switch (code) {                           //switch case based on what the response code is.
    //    case 1:                                 //decimal 1.
    //      Serial.println("[Atlas] command Success");            //means the command was successful.
    //      break;                                //exits the switch case.
    case 2:                                 //decimal 2.
      Serial.println("[Atlas] command Failed");             //means the command has failed.
      break;                                //exits the switch case.
    case 254:                               //decimal 254.
      Serial.println("[Atlas] command Pending");            //means the command has not yet been finished calculating.
      break;                                //exits the switch case.
    case 255:                               //decimal 255.
      Serial.println("[Atlas] command No Data");            //means there is no further data to send.
      break;                                //exits the switch case.
  }

  while (Wire.available()) {                 //are there bytes to receive.
    _char = Wire.read();                   //receive a byte.
    _responce += (char) _char;                    //load this byte into our array.
  }

  if (_info) {
    Serial.print("[Atlas] ");
    Serial.println(_responce);
  }

  return _responce;
}

void setup() {
  Serial.begin(115200);
  Wire.setClock(400000);
  Wire.begin();
  
  pinMode(switch_pin, INPUT);
  pinMode(USR_pin, INPUT);
  pinMode(power_pin, OUTPUT);
  systemPower(true);
  ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV

}

uint16_t _loop_times = 0;

void loop() {
  Serial.print("LOOP-");
  Serial.println(_loop_times);

  Serial.print("\t"); atlas_cmd("i", 600, addr_pH, true);   delay(100);
  Serial.print("\t"); atlas_cmd("i", 600, addr_EC, true);   delay(100);
  Serial.print("\t"); atlas_cmd("i", 600, addr_Temp, true); delay(100);

  delay(900);
  _loop_times++;


}
