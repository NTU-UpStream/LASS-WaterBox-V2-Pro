#include "avr/pgmspace.h"
#include "WaterBox_PMU.h"
#include <LowPower.h>

#include <Adafruit_INA219.h>

#ifndef TwoWire_h
#include <Wire.h>
#endif // _WIRE_H

#define SLEEPTIME SLEEP_1S
uint8_t i2c_BUFFER[30];

const char f_s1[] PROGMEM = "%s";
const char f_s2[] PROGMEM = "%s %s";
const char f_s1d1[] PROGMEM = "%s %d";
const char f_s1d1s1[] PROGMEM = "%s %d %s";

uint8_t WaterBox_PMU::_Debug;
Stream &WaterBox_PMU::refSerial = Serial;
uint16_t WaterBox_PMU::_i_for;
uint8_t WaterBox_PMU::_PowerSave = false;
uint16_t WaterBox_PMU::_SleepSec;
WaterBox_PMU::IC2Request WaterBox_PMU::_REQUEST;
char WaterBox_PMU::_ComStr[32];
char WaterBox_PMU::_CmdStr[16];
char WaterBox_PMU::_CmdType[16];
char WaterBox_PMU::_recBuffer[32];

float WaterBox_PMU::_WakeUpVoltage = 3.6;
float WaterBox_PMU::Volate;
float WaterBox_PMU::Current;
enum WaterBox_PMU::STATE WaterBox_PMU::state = MASTER;
uint16_t WaterBox_PMU::_CrtlPinMAIN = 2;
uint16_t WaterBox_PMU::_CrtlPinNBIOT = 3;
uint16_t WaterBox_PMU::_CrtlPinINA219 = 4;
uint8_t WaterBox_PMU::_Addr = 0x70;
uint8_t WaterBox_PMU::_INA219Addr = 0x40;

// SIM7000               WaterBox_PMU::_NB;
char WaterBox_PMU::ATCMD[32];
char WaterBox_PMU::Field_1[16];
char WaterBox_PMU::Field_2[16];
char WaterBox_PMU::Field_3[16];
char WaterBox_PMU::Field_4[16];
char WaterBox_PMU::Field_5[16];
char WaterBox_PMU::Field_6[16];
char WaterBox_PMU::Field_7[16];
char WaterBox_PMU::Field_8[16];
char WaterBox_PMU::Field_9[16];
char WaterBox_PMU::Field_10[16];
char WaterBox_PMU::Field_11[16];
char WaterBox_PMU::Field_12[16];

char WaterBox_PMU::APN[16];

WaterBox_PMU::WaterBox_PMU()
{
}

WaterBox_PMU::WaterBox_PMU(uint8_t _addr)
{
  _Addr = _addr;
}

WaterBox_PMU::~WaterBox_PMU()
{
}

void WaterBox_PMU::init(uint16_t _pinMain, uint16_t _pinNBIOT, uint16_t _pinINA219)
{
  _CrtlPinMAIN = _pinMain;
  _CrtlPinINA219 = _pinINA219;
  _CrtlPinNBIOT = _pinNBIOT;
  pinMode(_CrtlPinMAIN, OUTPUT);
  pinMode(_CrtlPinINA219, OUTPUT);
  //_NB.begin(9600);
  //_NB.init(_CrtlPinNBIOT);
}

void WaterBox_PMU::setINA219(uint8_t _addr)
{
  _INA219Addr = _addr;

  _Deguber(F("Setting INA219 I2C address: 0x"), H_PMU, NONE);
  _Deguber(String(_INA219Addr, HEX), NONE, EOL);
}

void WaterBox_PMU::setDebuger(Stream &refSer)
{
  _Debug = true;
  refSerial = refSer;
}

void WaterBox_PMU::setWakeUpVolate(float _v)
{
  _WakeUpVoltage = _v;
  _Deguber(F("WAKE UP Volate: "), H_PMU);
  _Deguber(String(_WakeUpVoltage), NONE);
  _Deguber(F(" V."), NONE, EOL);
}

void WaterBox_PMU::setSleepSec(uint32_t _sec)
{
  _SleepSec = _sec;

  _Deguber(F("Setting Sleep Time: "), H_PMU);
  _Deguber(String(_SleepSec), NONE);
  _Deguber(F(" sec."), NONE, EOL);
}

// 開始睡眠：切換到Master，關閉供電後開始進入睡眠循環，循環結束時更新電池狀態，低於喚醒電源後再開始供電返回1 (可以用while(Sleep())一值睡
uint8_t WaterBox_PMU::Sleep(STATE _type)
{

  LED(1, 700);
  _Deguber(F("Prepare SLEEP Mode"), H_PMU, EOL);
  _Deguber(F("Off the LED & Control POWER"), H_PMU, EOL);

  digitalWrite(LED_BUILTIN, LOW);
  ControlPower(OFF);                 // 關閉MCU電源
  digitalWrite(_CrtlPinINA219, LOW); // 關閉INA219電源

  _Deguber(F("Enter SLEEP Mode"), H_PMU, EOL);
  delay(100);

  for (_i_for = 0; _i_for < _SleepSec; _i_for++)
  {
    if (_PowerSave)
      LowPower.powerDown(SLEEPTIME, ADC_OFF, BOD_OFF);
    else
      delay(1000);
  }

  state = MASTER;
  _Deguber(F("Waker up, init I2C with Master and get bettery state..."), H_PMU, EOL);

  //  ControlPower(ON);  // 開啟主電源，測量系統供電狀態
  getBetteryState();
  //  ControlPower(OFF);

  // 確認電池電壓是否足夠，低於限值則回傳True
  if (Volate < _WakeUpVoltage)
  {
    _Deguber(F("ERROR: Volate < Wake up voltage"), H_PMU);
    _Deguber(String(_WakeUpVoltage), NONE, EOL);
    return 1;
  }
  else
  {
    // 喚醒WaterBox，並把I2C控制權給WaterBox(I2C設定為slaver)
    if (_type == SLAVER)
    {
      _Deguber(F(" I2C --> Slaver"), H_PMU, EOL);
      _SwitchToSlaver(_Addr);
      digitalWrite(_CrtlPinINA219, HIGH); // 等待 MCU 取得資料並設定INA219 -> MCU 會跟INA219拿資料，所以開啟電源
      ControlPower(ON);
    }
    return 0;
  }
}

// 用來決定是否啟用省電狀態 or 用Delay 1秒替代
void WaterBox_PMU::PowerSaveMode(POWER _state)
{
  if (_state == ON)
    _Deguber(F("Power Save Mode: ON"), H_PMU, EOL);
  else if (_state == OFF)
    _Deguber(F("Power Save Mode: OFF"), H_PMU, EOL);
  _PowerSave = _state;
}

// 控制主電源
void WaterBox_PMU::ControlPower(POWER _state)
{
  if (_state == ON)
  {
    digitalWrite(_CrtlPinMAIN, HIGH);
    _Deguber(F("Control: ON"), H_PMU, EOL);
  }
  else if (_state == OFF)
  {
    digitalWrite(_CrtlPinMAIN, LOW);
    _Deguber(F("Control: OFF"), H_PMU, EOL);
  }
}

// 透過INA219取得電池狀態
void WaterBox_PMU::getBetteryState()
{

  digitalWrite(_CrtlPinINA219, HIGH);

  Wire.begin();

  uint32_t currentFrequency;
  Adafruit_INA219 bettery(_INA219Addr);

  bettery.begin();
  // To use a slightly lower 32V, 1A range (higher precision on amps):
  // bettery.setCalibration_32V_1A();
  // Or to use a lower 16V, 400mA range (higher precision on volts and amps):
  bettery.setCalibration_16V_400mA();
  Volate = bettery.getBusVoltage_V();
  Current = bettery.getCurrent_mA();

  Wire.end();
  digitalWrite(_CrtlPinINA219, LOW);

  _Deguber(F("Bettery Volate: "), H_PMU);
  _Deguber(String(Volate), NONE);
  _Deguber(F(", Current: "), NONE);
  _Deguber(String(Current), NONE, EOL);
}

// 板子上的LED燈控制
void WaterBox_PMU::LED(uint16_t _times, uint16_t _ratio)
{
  pinMode(LED_BUILTIN, OUTPUT);
  for (_i_for = 0; _i_for < _times; _i_for++)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(_ratio);
    digitalWrite(LED_BUILTIN, LOW);
    delay(300);
  }
}

// 清除指令
void WaterBox_PMU::ATClear()
{
  memset(ATCMD, '\0', 32);
}

void WaterBox_PMU::_Deguber(String _msg, UART _header, UART _uart = NONE)
{
  if (_Debug)
  {
    switch (_header)
    {
    case H_PMU:
      refSerial.print(F("[PMU]\t"));
      break;
    case H_I2C:
      refSerial.print(F("[I2C]\t"));
      break;
    case H_CMD:
      refSerial.print(F("[CMD]\t"));
      break;
    default:
      break;
    }

    switch (_uart)
    {
    case NONE:
      refSerial.print(_msg);
      break;
    case EOL:
      refSerial.println(_msg);
      break;
    default:
      break;
    }
  }
}

void WaterBox_PMU::_Deguber(char *_msg, UART _header, UART _uart = NONE)
{
  if (_Debug)
  {
    switch (_header)
    {
    case H_PMU:
      refSerial.print(F("[PMU]\t"));
      break;
    case H_I2C:
      refSerial.print(F("[I2C]\t"));
      break;
    case H_CMD:
      refSerial.print(F("[CMD]\t"));
      break;
    default:
      break;
    }

    switch (_uart)
    {
    case NONE:
      refSerial.print(_msg);
      break;
    case EOL:
      refSerial.println(_msg);
      break;
    default:
      break;
    }
  }
}

// I2C 收到指令後要回傳的反應
void WaterBox_PMU::_requestEvent()
{
  memset(_recBuffer, '\0', 32);
  // 按照_cmd解析後的指令狀態(_REQUEST) 回傳30 char長度的資料，多餘的長度用" "填滿
  switch (_REQUEST)
  {
  case Req_NONE:
    sprintf_P(_recBuffer, f_s1, F("NoString"));
    break;
  case Req_TIME:
    sprintf_P(_recBuffer, f_s1d1s1, F("OK"), _SleepSec, F("sec"));
    break;
  case Req_LOWPOWER:
    sprintf_P(_recBuffer, f_s1d1, F("OK "), _PowerSave);
    break;
  case Req_BAT_VOLATE:
    sprintf_P(_recBuffer, f_s1d1s1, F("OK"), Volate, F("V"));
    break;
  case Req_BAT_CURRENT:
    sprintf_P(_recBuffer, f_s1d1s1, F("OK"), Current, F("mA"));
    break;
  case Req_AT:
    sprintf_P(_recBuffer, f_s1, F("OK"));
    break;
  case Req_APN:
    sprintf_P(_recBuffer, f_s2, F("OK"), APN);
    break;
  case Req_Field:
    sprintf_P(_recBuffer, f_s1, F("OK"));
    break;
  default:
    sprintf_P(_recBuffer, f_s1, F("Command ERROR"));
    break;
  }

  _Deguber(_recBuffer, H_I2C, EOL);
  Wire.write(_recBuffer);
}

// 改寫完成
void WaterBox_PMU::_receiveEvent(int howMany)
{
  // 收到WaterBox I2C的指令後，丟給_cmd()處理
  memset(_ComStr, '\0', 32);
  for (_i_for = 0; _i_for < howMany; _i_for++)
  {
    _ComStr[_i_for] = Wire.read();
    delay(10);
  }
  _i_for = 0; // 使用完畢後歸零

  LED(1, 300);
  _Deguber(_ComStr, H_CMD, EOL);
  _cmd(_ComStr);
}

void WaterBox_PMU::_cmd(char *_str)
{
  /*
    TIME,sec         // 設定鬧鐘(秒)
    SLEEP             // 進入睡眠
    LOWPOWER,1  // 省電模式
    VOLATE           // 回傳 volate
    CURRENT        // 回傳 current
    ATCMD,***** //  輸入AT Commend 資料
  */

  // 先重新利用_ComStr, 取得","後面的字串
  for (_i_for = 0; _i_for < strlen(_str); _i_for++)
  {
    // ',' 換成 ' '
    if (_str[_i_for] == ',')
      _str[_i_for] = ' ';

    // 改成大寫
    _str[_i_for] = toupper(_str[_i_for]);
  }

  if (sscanf_P(_str, PSTR("TIME %d"), &_SleepSec) > 0)
  { // TIME,30 設定睡眠時間30Sec
    setSleepSec(_SleepSec);
    _REQUEST = Req_TIME;
  }

  if (strstr_P(_str, PSTR("SLEEP")) != NULL)
  {
    _SwitchToMaster(); // 直接切換成MASTER狀態
  }

  if (sscanf_P(_str, PSTR("LOWPOWER %d"), &_PowerSave) > 0)
  { // LOWPOWER,1 -> 開啟省電狀態
    PowerSaveMode(POWER(_PowerSave));
    _REQUEST = Req_LOWPOWER;
  }

  if (strstr_P(_str, PSTR("VOLATE")) != NULL)
  { // VOLATE -> 回傳目前電壓
    _REQUEST = Req_BAT_VOLATE;
  }

  if (strstr_P(_str, PSTR("CURRENT")) != NULL)
  { // CURRENT -> 回傳目前電流mA
    _REQUEST = Req_BAT_CURRENT;
  }

  if (sscanf_P(_str, PSTR("%s %s"), _CmdType, _recBuffer) > 0)
  {
    if (strcmp_PF(_CmdType, PSTR("F1")) > 0)
      strcpy_P(Field_1, _recBuffer);

    if (strcmp_PF(_CmdType, PSTR("F2")) > 0)
      strcpy_P(Field_2, _recBuffer);

    if (strcmp_PF(_CmdType, PSTR("F3")) > 0)
      strcpy_P(Field_3, _recBuffer);

    if (strcmp_PF(_CmdType, PSTR("F4")) > 0)
      strcpy_P(Field_4, _recBuffer);

    if (strcmp_PF(_CmdType, PSTR("F5")) > 0)
      strcpy_P(Field_5, _recBuffer);

    if (strcmp_PF(_CmdType, PSTR("F6")) > 0)
      strcpy_P(Field_6, _recBuffer);

    if (strcmp_PF(_CmdType, PSTR("F7")) > 0)
      strcpy_P(Field_7, _recBuffer);

    if (strcmp_PF(_CmdType, PSTR("F8")) > 0)
      strcpy_P(Field_8, _recBuffer);

    if (strcmp_PF(_CmdType, PSTR("F9")) > 0)
      strcpy_P(Field_9, _recBuffer);

    if (strcmp_PF(_CmdType, PSTR("F10")) > 0)
      strcpy_P(Field_10, _recBuffer);

    if (strcmp_PF(_CmdType, PSTR("F11")) > 0)
      strcpy_P(Field_11, _recBuffer);

    if (strcmp_PF(_CmdType, PSTR("F12")) > 0)
      strcpy_P(Field_12, _recBuffer);
    _REQUEST = Req_Field;
  }
}

void WaterBox_PMU::_SwitchToSlaver(uint8_t _addr)
{
  Wire.begin(_addr);
  Wire.onRequest(&WaterBox_PMU::_requestEvent); // I2C Master的指令：回傳
  Wire.onReceive(&WaterBox_PMU::_receiveEvent); // I2C Master的指令：不回傳
  state = SLAVER;
}

void WaterBox_PMU::_SwitchToMaster(void)
{
  Wire.end();
  Wire.begin();
  state = MASTER;
}
