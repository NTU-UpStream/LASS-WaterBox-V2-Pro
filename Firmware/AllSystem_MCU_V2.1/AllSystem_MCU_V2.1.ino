#include <Arduino.h>
#include <ArduinoJson.h>
#include <U8g2lib.h>
#include <dfrobot_turbidity.h>
#include <WaterBox_util.h>
#include <MenuSystem.h>
#include <DS3231M.h>
#include <Ezo_i2c.h>
#include <Adafruit_ADS1015.h>
#include <Adafruit_INA219.h>
#include <SdFat.h>
#include <SPI.h>
#include <Wire.h>
#include <LWiFi.h>

byte mac[6];

#define MPU_I2C 0x70
#define modeSwitch 5
const uint8_t SD_CHIP_SELECT = 4;

#define cmd_setAPN       "APN,iot4ga2"
#define cmd_setSleepTime "TIME,180"
#define cmd_setLowPower  "LOWPOWER,1" // 1 為啟動睡眠, 0 為不睡覺

/***** << OLED library: u8g2 >> *****/

//U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
U8G2_SSD1306_128X64_NONAME_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ OLED_CS, /* dc=*/ 16, /* reset=*/ 17);

//Important Object initialize
extern MenuPage Main_page;

DS3231M_Class Clock;
Ezo_board pH = Ezo_board(99, "PH");
Ezo_board EC = Ezo_board(100, "EC");
Ezo_board TEMP = Ezo_board(102, "TEMP");
Dfrobot_Turbidity TURB = Dfrobot_Turbidity();
Adafruit_ADS1015 ads1115;
Adafruit_INA219 ina219;
SdFat SDcard;

MenuSystem Menu {
  &Main_page,
  &pH, &EC, &TEMP, &TURB,
  &u8g2,
  &Clock,
  &ads1115,
  &ina219,
  &SDcard
};

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

void setPMU(String _cmd) {
  Wire.beginTransmission(MPU_I2C); // transmit to device #8
  Wire.write(_cmd.c_str());
  Wire.endTransmission();    // stop transmitting

  reqPMU();
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

void setup(void)
{
  Wire.setClock(50000);
  Serial.begin(9600);
  
  digitalWrite(modulePower, HIGH);        //Open all module
  pinMode(SDA, INPUT_PULLUP);
  pinMode(sensorSwitch, INPUT_PULLUP);
  pinMode(modulePower, OUTPUT);
  pinMode(modeSwitch, INPUT);
  pinMode(LORA_CS, OUTPUT);
  pinMode(SD_CS, OUTPUT);
  pinMode(OLED_CS, OUTPUT);

  digitalWrite(LORA_CS, HIGH);
  digitalWrite(SD_CS, HIGH);
  digitalWrite(OLED_CS, LOW);
  
  u8g2.begin();
  ads1115.begin();
  ads1115.setGain(GAIN_ONE);
  ina219.begin();
  Wire.begin();
  Menu.load_setting();
}

void loop(void)
{
  if (digitalRead(modeSwitch)) {
    setPMU(Menu.pmu_date());
    setPMU(Menu.pmu_time());
    setPMU(Menu.pmu_lon());
    setPMU(Menu.pmu_lat());
    setPMU(Menu.pmu_temp());    //Request Temperature first can fill up the temp_cache variable that allow ec and ph to compensate directly without extra measurement.
    setPMU(Menu.pmu_ec());
    setPMU(Menu.pmu_ph());
    setPMU(Menu.pmu_turb());
    setPMU(Menu.pmu_volt());
    setPMU(Menu.pmu_current());
    setPMU("F3,"+MacAddress());
    setPMU(Menu.pmu_APN());
    setPMU(Menu.pmu_sleeptime());
    setPMU(cmd_setLowPower);
    setPMU("SLEEP");
  }
  else {
    Menu.runMenu();
  }
}
