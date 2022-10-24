/*
  Name:    WaterBox_2.ino
  Created: 2020/3/10 下午 07:28:01
  Author:  Liu
*/
#include "WaterBox_PMU.h"
WaterBox_PMU PMU;

#include "SoftwareSerial.h"
SoftwareSerial sNBIOT(9, 8); //(rxPin, txPin)

#include "ublox.h"
#define NB_POWER 3
ublox NBIOT;

// LASS MQTT Config
#define Server_LASS "gpssensor.ddns.net"
#define Port_LASS 1883
#define MQTT_LASS_Topic "LASS/Test/WaterBox_TW/"

#define MSG_INIT "|device=Linkit7697|ver_app=1.1.0|tick=0|FAKE_GPS=1"
#define SIMCOM_MS 10
String AT_Buffer;

#define MSG_SIZE 300 // MQTT 訊息的上限

// String 轉換成 Char array 緩衝用
char STR_BUFFER[20];

// String 轉換成 Char array 緩衝用
uint32_t TOTAL_PL = 0; // 計算RL之前 用來統計整個訊息長度用
byte MQTT_RL[4];

String radom_str; // 給client id 用

uint16_t _loop_count = 0; // 記數用

/***********<< 封包格式 >>**********
  CONNECT
  PUBLISH
*********************************/
struct PUBLISH
{
  uint8_t Fix_Header = 0x30 | 0b0010; // 0x30(publish) | 0b0010 (QoS:1)| 0b0001 (Retain)
  byte RL[4];
  uint16_t RL_size;

  byte PL_topic[2];
  char TOPIC[40] = {'\0'};

  byte PL_msg[2];
  char MSG[MSG_SIZE] = {'\0'};
};

struct CONNECT
{
  uint8_t Fix_Header = 0x10;
  byte RL[4];
  uint16_t RL_size;
  byte Flags = 0x02; // Connect flags: 0b00000010 (Clean Session)
  byte Protocol[7] = {
      0x00, 0x04,                                 // Protocol Length: 4
      (byte)'M', (byte)'Q', (byte)'T', (byte)'T', // Protocol Name:MQTT
      0x04                                        // Protocol level (mqtt3.1.1 是0x04)
  };
  byte KeepAlive[2] = {0x00, 0x3C}; // Keep Alive: 60 sec

  byte PL_id[2];
  char ID[20] = {'\0'};
};

CONNECT CONNECT_KPG;
PUBLISH PUBLISH_KPG;

/***********<< 資料整理 >>**********
  ZeroPadding
*********************************/
String ZeroHexPadding(byte _value)
{
  String _buffer = "";

  if (_value < 16)
    _buffer = "0" + String(_value, HEX);
  else
    _buffer = String(_value, HEX);
  _buffer.toUpperCase();
  return _buffer;
}

void showByteBuffer(byte *_buffer, uint16_t _size, uint8_t _isChar = false)
{
  for (uint16_t _i = 0; _i < _size; _i++)
  {
    if (_isChar)
      Serial.print((char)_buffer[_i]);
    else
    {
      Serial.print(F("0x"));
      Serial.print(ZeroHexPadding(_buffer[_i]));
      Serial.print(F(" "));
    }

    if (_i % 10 == 9)
      Serial.print(F("\r\n"));
    delay(10);
  }
  Serial.println(F("\r\n"));
}

String CharToHexStr(byte _char, uint8_t _show = false)
{
  byte _buffer = (byte)_char;
  if (_show)
    Serial.print(ZeroHexPadding(_buffer));
  return ZeroHexPadding(_buffer);
}

String ByteToHexStr(byte _byte, uint8_t _show = false)
{
  if (_show)
    Serial.print(ZeroHexPadding(_byte));
  return ZeroHexPadding(_byte);
}

/***********<< 封包格式 >>**********
  CalculateRL 計算RL, 填入MQTT_RL Buffer內，回傳用掉幾個bytes
*********************************/
uint8_t CalculateRL(uint32_t _size)
{
  uint8_t _index = 0;
  memset(MQTT_RL, '\0', 4); // 清空 Topic Buffer
  do
  {
    uint8_t _d = _size % 128; // 取餘數 _size & 0b01111111
    _size /= 128;             // 取除以128後的整數：_size = _size >> 7;
    if (_size > 0)
      _d |= 0x80;         // 除128的整數 > 0 時，對_d + 128 (bit[7]為1)
    MQTT_RL[_index] = _d; // 計算好的 _d 放到 buffer內
    if (_index < 3)
      _index++; // 避免寫入buffer爆掉，限制_index不超過3
  } while (_size > 0);
  return _index;
}

/***********<< 更新 BUFFER >>**********
  Flash_STR_BUFFER 更新 STR_BUFFER, 輸入String
*********************************/
bool Flash_STR_BUFFER(String _str)
{
  if (_str.length() > 20)
  {
    Serial.print("[STR BUFFER ERROR]");
    Serial.print(_str.length());
    Serial.println("->" + _str);
    return false;
  }
  else
  {
    memset(STR_BUFFER, '\0', 20); // 清空 Buffer
    _str.toCharArray(STR_BUFFER, _str.length() + 1);
    return true;
  }
}

/***********<< 系統主程式 >>**********
*********************************/
void setup(void)
{
  Serial.begin(9600);

  Serial.begin(9600);
  sNBIOT.begin(9600);
  NBIOT.setDebuger(Serial);
  NBIOT.setUART(sNBIOT);
  NBIOT.init(NB_POWER);

  PMU.init();
  PMU.setDebuger(Serial);
  PMU.setSleepSec(2);
  PMU.setWakeUpVolate(3.2);
  PMU.PowerSaveMode(PMU.ON); // PMU.ON進入睡眠, Delay不睡覺 PMU.OFF
  PMU.setINA219(0x40);

  Serial.println(F("[System] Setup Done"));
  // 先睡一次，讓MCU 取得資料並且紀錄

  PMU.Sleep();

  Serial.begin(9600);
  Serial.println(PMU.Volate);
  Serial.println(PMU.Current);

  radom_str = (String) "_" + (char)random(65, 90) + (char)random(65, 90) + (char)random(97, 122) + (char)random(97, 122);
}

void loop(void)
{
  if (PMU.state == PMU.MASTER)
  {

    _loop_count++;

    // 建立Topic
    memset(PUBLISH_KPG.TOPIC, '\0', 40); // 清空 Topic Buffer
    strcat(PUBLISH_KPG.TOPIC, MQTT_LASS_Topic);

    if (Flash_STR_BUFFER(PMU.Field_3))
      strcat(PUBLISH_KPG.TOPIC, STR_BUFFER); // 把(String)ID 放到STR_BUFFER中

    // 建立 從Field 建立ID 的String
    memset(CONNECT_KPG.ID, '\0', 20);   // 清空 ID Buffer
    strcat(CONNECT_KPG.ID, STR_BUFFER); // 沿用 STR_BUFFER裡面的 MAC
    if (Flash_STR_BUFFER(radom_str))    // 把亂數轉存在STR_BUFFER後面
      strcat(CONNECT_KPG.ID, STR_BUFFER);
    // radom_str.toCharArray(STR_BUFFER, radom_str.length() + 1);

    // 建立Publish封包
    memset(PUBLISH_KPG.MSG, '\0', MSG_SIZE);
    strcat(PUBLISH_KPG.MSG, MSG_INIT);

    if (Flash_STR_BUFFER(PMU.Field_1))
    {
      strcat(PUBLISH_KPG.MSG, "|date=");
      strcat(PUBLISH_KPG.MSG, STR_BUFFER);
    }
    if (Flash_STR_BUFFER(PMU.Field_2))
    {
      strcat(PUBLISH_KPG.MSG, "|time=");
      strcat(PUBLISH_KPG.MSG, STR_BUFFER);
    }

    if (Flash_STR_BUFFER(PMU.Field_3))
    {
      strcat(PUBLISH_KPG.MSG, "|device_id=");
      strcat(PUBLISH_KPG.MSG, STR_BUFFER);
    }

    if (Flash_STR_BUFFER(PMU.Field_4))
    {
      strcat(PUBLISH_KPG.MSG, "|gps_lon=");
      strcat(PUBLISH_KPG.MSG, STR_BUFFER);
    }
    if (Flash_STR_BUFFER(PMU.Field_5))
    {
      strcat(PUBLISH_KPG.MSG, "|gps_lat=");
      strcat(PUBLISH_KPG.MSG, STR_BUFFER);
    }

    if (Flash_STR_BUFFER(PMU.Field_6))
    {
      strcat(PUBLISH_KPG.MSG, "|s_ec2=");
      strcat(PUBLISH_KPG.MSG, STR_BUFFER);
    }
    if (Flash_STR_BUFFER(PMU.Field_7))
    {
      strcat(PUBLISH_KPG.MSG, "|s_ph2=");
      strcat(PUBLISH_KPG.MSG, STR_BUFFER);
    }
    if (Flash_STR_BUFFER(PMU.Field_8))
    {
      strcat(PUBLISH_KPG.MSG, "|s_t7=");
      strcat(PUBLISH_KPG.MSG, STR_BUFFER);
    }
    if (Flash_STR_BUFFER(PMU.Field_9))
    {
      strcat(PUBLISH_KPG.MSG, "|s_Tb=");
      strcat(PUBLISH_KPG.MSG, STR_BUFFER);
    }

    if (Flash_STR_BUFFER(PMU.Field_10))
    {
      strcat(PUBLISH_KPG.MSG, "|bat_v=");
      strcat(PUBLISH_KPG.MSG, STR_BUFFER);
    }
    if (Flash_STR_BUFFER(PMU.Field_11))
    {
      strcat(PUBLISH_KPG.MSG, "|bat_a=");
      strcat(PUBLISH_KPG.MSG, STR_BUFFER);
    }

    if (Flash_STR_BUFFER(PMU.Field_12))
    {
      strcat(PUBLISH_KPG.MSG, "|F12=");
      strcat(PUBLISH_KPG.MSG, STR_BUFFER);
    }
    strcat(PUBLISH_KPG.MSG, "|");

    // 把要傳送封包(LASS 格式), 打包成String
    // msg_str = "|device=Linkit7697|device_id=9C65F920C020|ver_app=1.1.0|date=2019-03-21|time=06:53:55|tick=0|FAKE_GPS=1|gps_lon=121.787|gps_lat=25.1933|s_ec=200000.00|s_ph=14.00|s_t0=100.00|s_Tb=10000.00|bat_v=3.70|bat_a=400.00|";

    /*********** 封包內容印出來確認(不要刪) ***********/
    //     Serial.println("[ID-"+String(_loop_count)+"]");
    //     Serial.print("\t");
    //     Serial.print(strlen(CONNECT_KPG.ID));
    //     Serial.print(" -> ");
    //     Serial.println(CONNECT_KPG.ID);

    /*********** 檢查 PUBLISH_KPG.TOPIC 內容(不要刪) ***********/
    //     Serial.println("[TOPIC-"+String(_loop_count)+"]");
    //     Serial.print("\t");
    //     Serial.print(strlen(PUBLISH_KPG.TOPIC));
    //     Serial.print(" -> ");
    //     Serial.println(PUBLISH_KPG.TOPIC);

    // for(byte _i=0;_i<strlen(PUBLISH_KPG.TOPIC);_i++){
    //   CharToHexStr(PUBLISH_KPG.TOPIC[_i],true);
    //   if(_i%10==1) Serial.print("\r\n");
    //   else         Serial.print(" ");
    // }
    // Serial.println();

    /*********** 檢查 PUBLISH_KPG.MSG 內容(不要刪) ***********/
    Serial.println("[MSG-" + String(_loop_count) + "]");
    Serial.print("\t");
    Serial.print(strlen(PUBLISH_KPG.MSG));
    Serial.print(" -> ");
    Serial.print(PUBLISH_KPG.MSG);
    Serial.println("\r\n");

    PMU.ControlPower(PMU.OFF);

    NBIOT.ON(30); // 開啟動模組30秒

    NBIOT.OFF();

    PMU.Sleep();
  }
  //  else{
  //    PMU.ControlPower(PMU.ON); // 一直開啟INA219的電源
  //  }
}
