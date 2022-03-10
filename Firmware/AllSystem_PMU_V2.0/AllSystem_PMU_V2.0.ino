/*
  Name:    WaterBox_2.ino
  Created: 2020/3/10 下午 07:28:01
  Author:  Liu
*/
#include <Adafruit_INA219.h> // 1.0.6
#include "WaterBox_PMU.h"
WaterBox_PMU PMU;

#include "SIM7000.h"
#define SIMCOM 3
SIM7000 NBIOT;


// LASS MQTT Config
#define Server_LASS "gpssensor.ddns.net"
#define Port_LASS   1883
#define MQTT_LASS_Topic "LASS/Test/WaterBox_TW/"

String _Data;
String id_str, topic_str, msg_str;
String _buffer_str;

byte MQTT_MSG[300] = {0};
byte MQTT_CONNECT[64] = {0};

uint32_t MQTT_PL = 0;
byte MQTT_RL[4];
uint16_t BUFFER_SIZE;
String radom_str;


/***********<< 封包格式 >>**********
  CONNECT
  PUBLISH
*********************************/
struct PUBLISH {
  uint8_t Fix_Header = 0x30 | 0b0010; // 0x30(publish) | 0b0010 (QoS:1)| 0b0001 (Retain)
  byte RL[4];
  uint8_t RL_size;

  byte PL_topic[2];
  byte TOPIC[30];

  byte PL_msg[2];
  byte MSG[300];
};

struct CONNECT {
  uint8_t Fix_Header = 0x10;
  byte RL[4];
  uint8_t RL_size;
  byte Flags = 0x02;                                     // Connect flags: 0b00000010 (Clean Session)
  byte Protocol[7] = {
    0x00, 0x04,                                          // Protocol Length: 4
    (byte)'M', (byte)'Q',  (byte)'T',  (byte)'T',        // Protocol Name:MQTT
    0x04                                                 // Protocol level (mqtt3.1.1 是0x04)
  };
  byte KeepAlive[2] = {0x00, 0x3C};                       // Keep Alive: 60 sec

  byte PL_id[2];
  byte ID[16];
};

CONNECT CONNECT_KPG;
PUBLISH PUBLISH_KPG;


/***********<< 資料整理 >>**********
  ZeroPadding
*********************************/
String ZeroPadding(byte _value) {
  if (_value < 16)  return "0" + String(_value, HEX);
  else              return String(_value, HEX);
}

void showByteBuffer(byte* _buffer, uint16_t _size) {
  for (uint16_t _i = 0; _i < _size; _i++) {
    Serial.print(F("0x"));
    Serial.print(ZeroPadding(_buffer[_i]));
    Serial.print(F(" "));
    if (_i % 10 == 9) Serial.print(F("\r\n"));
    delay(10);
  }
  Serial.println(F("\r\n"));
}

/***********<< 封包格式 >>**********
  CalculatePL 把String填入PAYLOAD 結構內並計算PL(2 bytes)
  CalculateRL 計算RL, 填入MQTT_RL Buffer內，回傳用掉幾個bytes
*********************************/
void CalculatePL(byte* _pl, String _msg) {
  uint16_t _length = _msg.length();
  _pl[0] = (byte)_length / 256;
  _pl[1] = (byte)_length % 256;
}

uint8_t CalculateRL(uint32_t _size) {
  uint8_t _index = 0;
  do
  {
    uint8_t _d = _size % 128;    // 取餘數 _size & 0b01111111
    _size /= 128;                // 取除以128後的整數：_size = _size >> 7;
    if (_size > 0)  _d |= 0x80;  // 除128的整數 > 0 時，對_d + 128 (bit[7]為1)
    MQTT_RL[_index] = _d;        // 計算好的 _d 放到 buffer內
    if (_index < 3)    _index++; // 避免寫入buffer爆掉，限制_index不超過3
  } while (_size > 0);
  return _index;
}


/***********<< 製造封包 >>**********
  PackConnectBuffer 連線用封包
  PackPublishBuffer 發布用封包
*********************************/
uint16_t PackPublishBuffer() {
  uint16_t _pl = 0;
  uint16_t _size = 0;

  // FixHeader
  MQTT_MSG[0] = (byte) PUBLISH_KPG.Fix_Header;
  _size += 1; // 轉移下一個index

  // RL
  for (uint8_t _i = 0; _i < PUBLISH_KPG.RL_size; _i++) {
    MQTT_MSG[_size + _i] = (byte) PUBLISH_KPG.RL[_i];
  }
  _size += PUBLISH_KPG.RL_size;


  // Topic, PL
  for (uint8_t _i = 0; _i < 2; _i++) {
    MQTT_MSG[_size + _i] = (byte) PUBLISH_KPG.PL_topic[_i];
  }
  _size += 2;

  // Topic
  _pl = (uint16_t)PUBLISH_KPG.PL_topic[0] * 256 + (uint16_t)PUBLISH_KPG.PL_topic[1];

  for (uint8_t _i = 0; _i < _pl; _i++) {
    MQTT_MSG[_size + _i] = (byte) PUBLISH_KPG.TOPIC[_i];
  }
  _size += _pl; // 從0 -> _pl 共 _pl+1個


  // MSG, PL
  for (uint8_t _i = 0; _i < 2; _i++) {
    MQTT_MSG[_size + _i] = (byte) PUBLISH_KPG.PL_msg[_i];
  }
  _size += 2;

  // MSG
  _pl = (uint16_t)PUBLISH_KPG.PL_msg[0] * 256 + (uint16_t)PUBLISH_KPG.PL_msg[1];

  for (uint16_t _i = 0; _i < _pl; _i++) {
    MQTT_MSG[_size + _i] = (byte) PUBLISH_KPG.MSG[_i];
  }
  _size += _pl; // 從0 -> _pl 共 _pl+1個

  return _size;
}

uint16_t PackConnectBuffer(String _id) {
  uint16_t _pl = 0;
  uint16_t _size = 0;
  /*
     順序很重要：
     1.FixHeader
     2.RL
     3.Protocol
     4.Connect flags
     5.KeepAlive
     6.Client ID
     7.User
     8.Password
  */

  CalculatePL(CONNECT_KPG.PL_id, _id);

  Serial.print(F("[CONTENT] ID: ")); Serial.println(_id);

  MQTT_PL = 10 + 2 + _id.length();                //計算RL長度 = 10+2+PL(ID)
  CONNECT_KPG.RL_size =  CalculateRL(MQTT_PL);    // 計算RL 用掉幾個bytes，並更新MQTT_RL 這個Buffer
  memcpy(CONNECT_KPG.RL, MQTT_RL, 4);             // 把MQTT_RL 這個Buffer 複製到PUBLIC_KPG裡面

  _id.toCharArray(CONNECT_KPG.ID, _id.length() + 1); // 把 ID轉為char array

  // FixHeader
  MQTT_CONNECT[0] = (byte) CONNECT_KPG.Fix_Header;
  _size += 1; // 轉移下一個index

  // RL
  for (uint8_t _i = 0; _i < CONNECT_KPG.RL_size; _i++) {
    MQTT_CONNECT[_size + _i] = (byte) CONNECT_KPG.RL[_i];
  }
  _size += CONNECT_KPG.RL_size;

  // Protocol
  for (uint8_t _i = 0; _i < 7; _i++) {
    MQTT_CONNECT[_size + _i] = (byte) CONNECT_KPG.Protocol[_i];
  }
  _size += 7;

  // Connect flags
  MQTT_CONNECT[_size] = (byte) CONNECT_KPG.Flags;
  _size += 1; // 轉移下一個index

  // KeepAlive
  for (uint8_t _i = 0; _i < 2; _i++) {
    MQTT_CONNECT[_size + _i] = (byte) CONNECT_KPG.KeepAlive[_i];
  }
  _size += 2;

  // ID, PL
  for (uint8_t _i = 0; _i < 2; _i++) {
    MQTT_CONNECT[_size + _i] = (byte) CONNECT_KPG.PL_id[_i];
  }
  _size += 2;

  // ID
  for (uint8_t _i = 0; _i < _id.length() + 1; _i++) {
    MQTT_CONNECT[_size + _i] = (byte) CONNECT_KPG.ID[_i];
  }
  _size += _id.length();

  return _size;
}


/***********<< 上傳 >>**********
  updateThingSpeak 上傳到Thingspeak 平台
*********************************/
//void updateThingSpeak(void) {
//  _Data = F("AT+HTTPPARA=\"URL\",\"http://api.thingspeak.com/update?api_key=JM921M08U3DHZ3JC");
//  _Data += F("&field1=");
//  _Data += String(PMU.Volate);
//  _Data += F("&field2=");
//  _Data += String(PMU.Current);
//  _Data += F("&field3=");
//  _Data += String(PMU.Field_3);
//  _Data += F("&field4=");
//  _Data += String(PMU.Field_4);
//  _Data += F("&field5=");
//  _Data += String(PMU.Field_5);
//  _Data += F("&field6=");
//  _Data += String(PMU.Field_6);
//  _Data += F("&field7=");
//  _Data += String(PMU.Field_7);
//  _Data += F("&field8=");
//  _Data += String(PMU.Field_8);
//  _Data += F("\"");
//
//  NBIOT.ON();
//  NBIOT.AT_Test();
//  NBIOT.AT_CMD(F("AT+CSTT=\"iot4ga2\""), true);
//  NBIOT.AT_CMD(F("AT+CIICR"), true);
//  NBIOT.AT_CMD(F("AT+SAPBR=3,1,\"Contype\", \"GPRS\""), true);
//  NBIOT.AT_CMD(F("AT+SAPBR=3,1,\"APN\",\"iot4ga2\""), true);
//  NBIOT.AT_CMD(F("AT+SAPBR=1,1"), true);
//
//  NBIOT.AT_CMD(F("AT+HTTPINIT"), true);
//
//  NBIOT.AT_CMD(F("AT+HTTPPARA=\"CID\",1"), true);
//
//  NBIOT.AT_CMD(_Data, false);
//  delay(500);
//
//  NBIOT.AT_CMD(F("AT+HTTPACTION=0"), true);
//  delay(20000);
//  NBIOT.AT_CMD(F("AT+HTTPREAD"), true);
//  NBIOT.AT_CMD(F("AT+HTTPTERM"), true);
//
//  NBIOT.AT_CMD(F("AT+SAPBR=0,1"), true);
//  NBIOT.AT_CMD(F("AT+CIPSHUT"), true);
//  NBIOT.OFF();
//}

void showConfig(void) {
  Serial.print(F("APN:\t")); Serial.println(PMU.APN);
  Serial.print(F("Date:\t")); Serial.println(PMU.Field_1);
  Serial.print(F("Time:\t")); Serial.println(PMU.Field_2);
  Serial.print(F("ID:\t")); Serial.println(PMU.Field_3);
  Serial.print(F("lon:\t")); Serial.println(PMU.Field_4);
  Serial.print(F("lat:\t")); Serial.println(PMU.Field_5);
  Serial.print(F("EC:\t")); Serial.println(PMU.Field_6);
  Serial.print(F("pH:\t")); Serial.println(PMU.Field_7);
  Serial.print(F("Temp:\t")); Serial.println(PMU.Field_8);
  Serial.print(F("Turbidity:\t")); Serial.println(PMU.Field_9);
  Serial.print(F("BAT_V:\t")); Serial.println(PMU.Field_10);
  Serial.print(F("BAT_mA:\t")); Serial.println(PMU.Field_11);
  Serial.print(F("F12:\t")); Serial.println(PMU.Field_12);
}


void setup()
{
  Serial.begin(9600);

  NBIOT.begin(9600);
  NBIOT.setDebuger(Serial);
  NBIOT.init(SIMCOM);

  PMU.init();
  //  PMU.setDebuger(Serial);
  PMU.setSleepSec(2);
  PMU.setWakeUpVolate(3.2);
  PMU.PowerSaveMode(PMU.ON); // PMU.ON進入睡眠, Delay不睡覺 PMU.OFF
  PMU.setINA219(0x40);

  Serial.println(F("[System] Setup Done"));
  // 先睡一次，讓MCU 取得資料並且紀錄
  PMU.Sleep();

  radom_str = (String)"_" + (char)random(65, 90) + (char)random(65, 90) + (char)random(97, 122) + (char)random(97, 122);
}

void loop()
{
  if (PMU.state == PMU.MASTER)
    //  if (true)
  {
    showConfig();

    id_str = PMU.Field_3 + radom_str;
    topic_str = MQTT_LASS_Topic + PMU.Field_3;
    BUFFER_SIZE = PackConnectBuffer(id_str);
    //    showByteBuffer(MQTT_CONNECT, BUFFER_SIZE);

    //    msg_str = "|device=Linkit7697|device_id=9C65F920C020|ver_app=1.1.0|date=2019-03-21|time=06:53:55|tick=0|FAKE_GPS=1|gps_lon=121.787|gps_lat=25.1933|s_ec=200000.00|s_ph=14.00|s_t0=100.00|s_Tb=10000.00|bat_v=3.70|bat_a=400.00|";
    msg_str = "|device=Linkit7697|ver_app=1.1.0|tick=0|FAKE_GPS=1";
    msg_str.concat(F("|date=")); msg_str.concat(PMU.Field_1);
    msg_str.concat(F("|time=")); msg_str.concat(PMU.Field_2);
    msg_str.concat(F("|device_id=")); msg_str.concat(PMU.Field_3);
    msg_str.concat(F("|gps_lon=")); msg_str.concat(PMU.Field_4);
    msg_str.concat(F("|gps_lat=")); msg_str.concat(PMU.Field_5);
    msg_str.concat(F("|s_ec=")); msg_str.concat(PMU.Field_6);
    msg_str.concat(F("|s_ph=")); msg_str.concat(PMU.Field_7);
    msg_str.concat(F("|s_t0=")); msg_str.concat(PMU.Field_8);
    msg_str.concat(F("|s_Tb=")); msg_str.concat(PMU.Field_9);
    msg_str.concat(F("|bat_v=")); msg_str.concat(PMU.Field_10);
    msg_str.concat(F("|bat_a=")); msg_str.concat(PMU.Field_11);
    msg_str.concat(F("|"));

    Serial.println(msg_str);

    // 先計算PL並打包對應的payload 內容

    //    Serial.print(F("[CONTENT] Publish Topic: ")); Serial.println(topic_str);
    //    Serial.print(F("[CONTENT] Publish Payload: ")); Serial.println(msg_str);

    CalculatePL(PUBLISH_KPG.PL_topic, topic_str);
    //    CalculatePL(PUBLISH_KPG.PL_msg, msg_str);


    MQTT_PL = 2 + topic_str.length() + 2 + msg_str.length();  // 計算RL長度 = 2+PL(Topic)+2+PL(Payload)
    PUBLISH_KPG.RL_size =  CalculateRL(MQTT_PL);        // 計算RL 用掉幾個bytes，並更新MQTT_RL 這個Buffer
    memcpy(PUBLISH_KPG.RL, MQTT_RL, 4);                 // 把MQTT_RL 這個Buffer 複製到PUBLIC_KPG裡面

    topic_str.toCharArray(PUBLISH_KPG.TOPIC, topic_str.length() + 1); // 把 TOPIC 轉為char array
    msg_str.toCharArray(PUBLISH_KPG.MSG, msg_str.length() + 1);       // 把 MSG 轉為char array

    PUBLISH_KPG.PL_msg[0] = (byte)msg_str.length() / 256;
    PUBLISH_KPG.PL_msg[1] = (byte)msg_str.length() % 256;

    BUFFER_SIZE = PackPublishBuffer();
    Serial.println(BUFFER_SIZE);
    Serial.flush();
    
    showByteBuffer(MQTT_MSG, 262);


    PMU.Sleep();
  }
}
