/**********************************************************
 *  \||||||||||||||||||||||||||||||||||||||||||||||||||/
    -Reference : https://github.com/DFRobot/DFRobot_PH -
    /||||||||||||||||||||||||||||||||||||||||||||||||||\
***********************************************************/

/***** << OLED library: u8g2 >> *****/
#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

//U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE);

/**** << WaterBox_V2.1:pin state test with ADS1115 >> *****
   ADC 數據及 switch切換
   使用Library Adafruit_ADS1X15 (Ver. 1.0.1)
***********************************************************/
#include <Wire.h>
#include <Adafruit_ADS1015.h>

Adafruit_ADS1115 ads; /* Use this for the 16-bit version */
//Adafruit_ADS1015 ads;     /* Use thi for the 12-bit version */
int16_t adc0, adc1, adc2, adc3;

#define pH_pin 0
#define EC_pin 1
#define Rotary_Pin_1 2 // VR1
#define Rotary_Pin_2 3 // VR2

/***** << GPIO設定 >> *****/
#define sensorSwitch 6
#define modeSwitch 5
#define pinLED 7
#define modulePower 14

float neutral_voltage = 1500;
float acid_voltage = 2032.44;
float pH_voltage = 0;
float pH_value = 0;
int pH_calibration_steps = 0;

void initADC(void)
{
  ads.begin();
  //                                                                ADS1015  ADS1115
  //                                                                -------  -------
  // ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
  ads.setGain(GAIN_ONE); // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
  // ads.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
  // ads.setGain(GAIN_FOUR);       // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV
  // ads.setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
  // ads.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV
}

float _getVoltage(int _ch)
{
  delay(100);
  int adc = ads.readADC_SingleEnded(_ch);
  int mV = adc * 0.125;
  return mV;
}

float getPH()
{
  // 因為板子的電路會進行分壓，所以ADS1115量出來的電壓要再乘上2才會是電錶的電壓
  float _ads_Volate = 2.0 * _getVoltage(pH_pin);
  // 計算兩點之間的斜率 (neutral_voltage, 7.0) 以及 (acid_voltage, 4.0)
  float slope = (7.0 - 4.0) / (neutral_voltage - acid_voltage);
  float pH_Value = 7 + slope * (_ads_Volate - neutral_voltage);
  return pH_Value;
}

void calibration()
{
  pinMode(sensorSwitch, INPUT);
  int sw2 = digitalRead(sensorSwitch);
  switch (pH_calibration_steps)
  {
    case 0:
      Serial.println("請將pH sensor置於 ph = 7.0 標準液");
      Serial.println("完成時請按下Switch 2");
      pH_calibration_steps = 1;
      break;

    case 1:
      //檢測是否按下按鈕
      if (sw2 == 1)
        pH_calibration_steps = 2;
      break;

    case 2:
      //檢測是否鬆開按鈕
      if (sw2 == 0)
        pH_calibration_steps = 3;
      break;

    case 3:
      //開始量測標準液電壓
      delay(50);
      pinMode(sensorSwitch, OUTPUT);
      digitalWrite(sensorSwitch, HIGH); // 切換到 pH
      pH_voltage = 2 * _getVoltage(pH_pin);
      Serial.print("ph = 7.0 目前電壓 : ");
      Serial.print(pH_voltage);
      Serial.println("      穩定後按下Switch 2");
      pinMode(sensorSwitch, INPUT);
      //檢測是否按下按鈕
      if (sw2 == 1)
      {
        pH_calibration_steps = 4;
      }
      break;

    case 4:
      //檢測是否鬆開按鈕
      if (sw2 == 0)
        pH_calibration_steps = 5;
      break;

    case 5:
      pH_voltage = 0;
      pinMode(sensorSwitch, OUTPUT);
      digitalWrite(sensorSwitch, HIGH); // 切換到 pH
      for (int i = 0; i < 10; i++)
      { 
        delay(500);
        pH_voltage += 2 * _getVoltage(pH_pin);
        Serial.println(pH_voltage);
      }
      pinMode(sensorSwitch, INPUT);
      neutral_voltage = pH_voltage / 10;
      Serial.print("Calculated pH Voltage : ");
      Serial.print(neutral_voltage);
      Serial.println("    (Standard : 1500");
      Serial.println("請將pH sensor置於 ph = 4.0 標準液");
      Serial.println("完成時請按下Switch 2");
      pH_calibration_steps = 6;

    case 6:
      if (sw2 == 1)
        pH_calibration_steps = 7;
      break;
    
    case 7:
      if (sw2 == 0)
        pH_calibration_steps = 8;
      break;

    case 8:
      //開始量測標準液電壓
      delay(50);
      pinMode(sensorSwitch, OUTPUT);
      digitalWrite(sensorSwitch, HIGH); // 切換到 pH
      pH_voltage = 2 * _getVoltage(pH_pin);
      Serial.print("ph = 4.0 目前pH : ");
      Serial.print(pH_voltage);
      Serial.println("      穩定後按下Switch 2");
      pinMode(sensorSwitch, INPUT);
      //檢測是否按下按鈕
      if (sw2 == 1)
      {
        pH_calibration_steps = 9;
      }
      break;

    case 9:
      if (sw2 == 0)
        pH_calibration_steps = 10;
      break;
      
    case 10:
      pH_voltage = 0;
      pinMode(sensorSwitch, OUTPUT);
      digitalWrite(sensorSwitch, HIGH); // 切換到 pH
      //若按下按鈕，紀錄10次電壓並取平均
      for (int i = 0; i < 10; i++)
      {
        delay(200);
        pH_voltage += 2 * _getVoltage(pH_pin);
      }
      pinMode(sensorSwitch, INPUT);
      acid_voltage = pH_voltage / 10;
      Serial.print("Calculated pH Voltage : ");
      Serial.print(acid_voltage);
      Serial.println("    (Standard : 2032.44");
      pH_calibration_steps = 11;

    case 11:
      Serial.println("");
      Serial.println("---Calibration finished---");
      Serial.print("Neutral ( ph = 7.0 ) Solution Voltage : ");
      Serial.println(neutral_voltage);
      Serial.print("Asid ( ph = 4.0 ) Solution Voltage : ");
      Serial.println(acid_voltage);
      Serial.print("Toggle Switch 1 to start analyze");
      pH_calibration_steps = 0;
      break;

    default:
      break;
  }
}

void setup()
{
  //power_pin 初始化
  pinMode(modulePower, OUTPUT);    //
  digitalWrite(modulePower, HIGH); // 開啟模組電源

  pinMode(modeSwitch, INPUT); // 切換分析模式以及設定模式

  Serial.begin(9600);
  initADC();
  Serial.println("[System] ADS1115 初始化完成");

  pinMode(sensorSwitch, OUTPUT);    // 用USR pin 控制 pH & EC 模組電源切換
  delay(500);                       // 先等 0.5秒
  digitalWrite(sensorSwitch, HIGH); // 切換到 pH
  Serial.println("Change to pH sensor");
}

void loop()
{
  digitalWrite(modulePower, HIGH); // 開啟模組電源
  bool _mode = digitalRead(modeSwitch);

  if (_mode)
  {
    pinMode(sensorSwitch, OUTPUT);
    digitalWrite(sensorSwitch, HIGH); // 切換到 pH
    pH_value = getPH();
    Serial.print("pH : ");
    Serial.println(pH_value);
  }
  else
  {
    calibration();
  }
}