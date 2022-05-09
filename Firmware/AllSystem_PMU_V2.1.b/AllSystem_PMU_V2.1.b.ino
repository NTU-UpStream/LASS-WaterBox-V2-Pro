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

#define MSG_INIT "|device=Linkit7697|ver_app=1.1.0|tick=0|FAKE_GPS=1"
#define SIMCOM_MS 10
String AT_Buffer;


#define MSG_SIZE 300 // MQTT 訊息的上限

// String 轉換成 Char array 緩衝用
char STR_BUFFER[20];

// String 轉換成 Char array 緩衝用
uint32_t TOTAL_PL = 0; // 計算RL之前 用來統計整個訊息長度用
byte MQTT_RL[4];

String radom_str;  // 給client id 用

uint16_t _loop_count = 0; // 記數用


/***********<< 封包格式 >>**********
  CONNECT
  PUBLISH
*********************************/
struct PUBLISH {
  uint8_t Fix_Header = 0x30 | 0b0010; // 0x30(publish) | 0b0010 (QoS:1)| 0b0001 (Retain)
  byte RL[4];
  uint16_t RL_size;

  byte PL_topic[2];
  char TOPIC[40] = {'\0'};

  byte PL_msg[2];
  char MSG[MSG_SIZE] = {'\0'};
};

struct CONNECT {
  uint8_t Fix_Header = 0x10;
  byte RL[4];
  uint16_t RL_size;
  byte Flags = 0x02;                                     // Connect flags: 0b00000010 (Clean Session)
  byte Protocol[7] = {
    0x00, 0x04,                                          // Protocol Length: 4
    (byte)'M', (byte)'Q',  (byte)'T',  (byte)'T',        // Protocol Name:MQTT
    0x04                                                 // Protocol level (mqtt3.1.1 是0x04)
  };
  byte KeepAlive[2] = {0x00, 0x3C};                       // Keep Alive: 60 sec

  byte PL_id[2];
  char ID[20] = {'\0'};
};

CONNECT CONNECT_KPG;
PUBLISH PUBLISH_KPG;


/***********<< 資料整理 >>**********
  ZeroPadding
*********************************/
String ZeroHexPadding(byte _value) {
  String _buffer = "";

  if (_value < 16)  _buffer = "0" + String(_value, HEX);
  else              _buffer = String(_value, HEX);
  _buffer.toUpperCase();
  return _buffer;
}

void showByteBuffer(byte* _buffer, uint16_t _size, uint8_t _isChar = false) {
  for (uint16_t _i = 0; _i < _size; _i++) {
    if (_isChar) Serial.print((char)_buffer[_i]);
    else {
      Serial.print(F("0x"));
      Serial.print(ZeroHexPadding(_buffer[_i]));
      Serial.print(F(" "));
    }

    if (_i % 10 == 9) Serial.print(F("\r\n"));
    delay(10);
  }
  Serial.println(F("\r\n"));
}

String CharToHexStr(byte _char, uint8_t _show = false)
{
  byte _buffer = (byte) _char;
  if (_show) Serial.print(ZeroHexPadding(_buffer));
  return ZeroHexPadding(_buffer);
}

String ByteToHexStr(byte _byte, uint8_t _show = false)
{
  if (_show) Serial.print(ZeroHexPadding(_byte));
  return ZeroHexPadding(_byte);
}


/***********<< 封包格式 >>**********
  CalculateRL 計算RL, 填入MQTT_RL Buffer內，回傳用掉幾個bytes
*********************************/
uint8_t CalculateRL(uint32_t _size) {
  uint8_t _index = 0;
  memset(MQTT_RL, '\0', 4);  // 清空 Topic Buffer
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

/***********<< 更新 BUFFER >>**********
  Flash_STR_BUFFER 更新 STR_BUFFER, 輸入String
*********************************/
bool Flash_STR_BUFFER(String _str) {
  if (_str.length() > 20) {
    Serial.print("[STR BUFFER ERROR]");
    Serial.print(_str.length());
    Serial.println("->" + _str);
    return false;
  }
  else {
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

void loop(void)
{
  if (PMU.state == PMU.MASTER)
  {

    _loop_count++;
    //    showConfig();

    // 建立Topic
    memset(PUBLISH_KPG.TOPIC, '\0', 40);  // 清空 Topic Buffer
    strcat(PUBLISH_KPG.TOPIC, MQTT_LASS_Topic);
    if (Flash_STR_BUFFER(PMU.Field_3))   strcat(PUBLISH_KPG.TOPIC, STR_BUFFER); // 把(String)ID 放到STR_BUFFER中

    // 建立 從Field 建立ID 的String
    memset(CONNECT_KPG.ID, '\0', 20);       // 清空 ID Buffer
    strcat(CONNECT_KPG.ID, STR_BUFFER);     // 沿用 STR_BUFFER裡面的 MAC
    if (Flash_STR_BUFFER(radom_str)) strcat(CONNECT_KPG.ID, STR_BUFFER);
    // radom_str.toCharArray(STR_BUFFER, radom_str.length() + 1);



    memset(PUBLISH_KPG.MSG, '\0', MSG_SIZE);
    strcat(PUBLISH_KPG.MSG, MSG_INIT);
    strcat(PUBLISH_KPG.MSG, "|date=");
    if (Flash_STR_BUFFER(PMU.Field_1))   strcat(PUBLISH_KPG.MSG, STR_BUFFER);
    strcat(PUBLISH_KPG.MSG, "|time=");
    if (Flash_STR_BUFFER(PMU.Field_2))   strcat(PUBLISH_KPG.MSG, STR_BUFFER);
    strcat(PUBLISH_KPG.MSG, "|device_id=");
    if (Flash_STR_BUFFER(PMU.Field_3))   strcat(PUBLISH_KPG.MSG, STR_BUFFER);
    strcat(PUBLISH_KPG.MSG, "|gps_lon=");
    if (Flash_STR_BUFFER(PMU.Field_4))   strcat(PUBLISH_KPG.MSG, STR_BUFFER);
    strcat(PUBLISH_KPG.MSG, "|gps_lat=");
    if (Flash_STR_BUFFER(PMU.Field_5))   strcat(PUBLISH_KPG.MSG, STR_BUFFER);
    strcat(PUBLISH_KPG.MSG, "|s_ec=");
    if (Flash_STR_BUFFER(PMU.Field_6))   strcat(PUBLISH_KPG.MSG, STR_BUFFER);
    strcat(PUBLISH_KPG.MSG, "|s_ph=");
    if (Flash_STR_BUFFER(PMU.Field_7))   strcat(PUBLISH_KPG.MSG, STR_BUFFER);
    strcat(PUBLISH_KPG.MSG, "|s_t0=");
    if (Flash_STR_BUFFER(PMU.Field_8))   strcat(PUBLISH_KPG.MSG, STR_BUFFER);
    strcat(PUBLISH_KPG.MSG, "|s_Tb=");
    if (Flash_STR_BUFFER(PMU.Field_9))   strcat(PUBLISH_KPG.MSG, STR_BUFFER);
    strcat(PUBLISH_KPG.MSG, "|bat_v=");
    if (Flash_STR_BUFFER(PMU.Field_10))  strcat(PUBLISH_KPG.MSG, STR_BUFFER);
    strcat(PUBLISH_KPG.MSG, "|bat_a=");
    if (Flash_STR_BUFFER(PMU.Field_11))  strcat(PUBLISH_KPG.MSG, STR_BUFFER);
    if (Flash_STR_BUFFER("|"))           strcat(PUBLISH_KPG.MSG, STR_BUFFER);


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
     Serial.println("[MSG-"+String(_loop_count)+"]");
     Serial.print("\t");
     Serial.print(strlen(PUBLISH_KPG.MSG));
     Serial.print(" -> ");
     Serial.print(PUBLISH_KPG.MSG);
     Serial.println("\r\n");

    //     for(byte _i=0;_i<strlen(PUBLISH_KPG.MSG);_i++){
    //       CharToHexStr(PUBLISH_KPG.MSG[_i],true);
    //       if(_i%10==1) Serial.print("\r\n");
    //       else         Serial.print(" ");
    //     }
    //     Serial.println();


    /********** < 打包 CONNECT_KPG > *********/
    CONNECT_KPG.PL_id[0] = strlen(CONNECT_KPG.ID) / 256;
    CONNECT_KPG.PL_id[1] = strlen(CONNECT_KPG.ID) % 256;

    TOTAL_PL = 10 + 2 + strlen(CONNECT_KPG.ID);                  // 計算RL長度 = 2+PL(id)
    CONNECT_KPG.RL_size =  CalculateRL(TOTAL_PL);                // 計算RL 用掉幾個bytes，並更新MQTT_RL 這個Buffer
    memcpy(CONNECT_KPG.RL, MQTT_RL, 4);                          // 把MQTT_RL 這個Buffer 複製到PUBLIC_KPG裡面


    /*********** 檢查 CONNECT_KPG RL及PL(不要刪) ***********/
    // Serial.println(">> CONNECT_KPG >>");
    // Serial.println("\tPL client id");
    // showByteBuffer(CONNECT_KPG.PL_id,2);
    // Serial.println("RL array size > "+String(CONNECT_KPG.RL_size));
    // showByteBuffer(CONNECT_KPG.RL,4);


    /********** < 打包 PUBLISH_KPG > *********/
    // 計算 TOPIC 的 PL
    PUBLISH_KPG.PL_topic[0] = strlen(PUBLISH_KPG.TOPIC) / 256;
    PUBLISH_KPG.PL_topic[1] = strlen(PUBLISH_KPG.TOPIC) % 256;

    // 計算 MSG 的 PL
    PUBLISH_KPG.PL_msg[0] = strlen(PUBLISH_KPG.MSG) / 256;
    PUBLISH_KPG.PL_msg[1] = strlen(PUBLISH_KPG.MSG) % 256;

    // 計算整個RL
    TOTAL_PL = 2 + strlen(PUBLISH_KPG.TOPIC) + 2 + strlen(PUBLISH_KPG.MSG);  // 計算RL長度 = 2+PL(Topic)+2+PL(Payload)
    PUBLISH_KPG.RL_size =  CalculateRL(TOTAL_PL);                            // 計算RL 用掉幾個bytes，並更新MQTT_RL 這個Buffer
    memcpy(PUBLISH_KPG.RL, MQTT_RL, 4);                                      // 把MQTT_RL 這個Buffver 複製到PUBLIC_KPG裡面

    /*********** 檢查 PUBLISH_KPG RL及PL(不要刪) ***********/
    // Serial.println(">> PUBLISH_KPG >>");
    // Serial.println("\tPL topic");
    // showByteBuffer(PUBLISH_KPG.PL_topic,2);

    // Serial.println("\tPL msg");
    // showByteBuffer(PUBLISH_KPG.PL_msg,2);

    // Serial.println("\tRL array > "+String(PUBLISH_KPG.RL_size));
    // showByteBuffer(PUBLISH_KPG.RL,4);

    PMU.ControlPower(PMU.OFF);

    NBIOT.ON(true);            // 開啟動模組30秒
    NBIOT.AT_Test();
    delay(1000);

    NBIOT.AT_CMD(F("AT+CIPSENDHEX=1"), true);               // 設定 TCP/IP的傳輸格式為HEX
    delay(500);
    AT_Buffer = (String)"AT+CSTT=\"" + PMU.APN + "\"";
    NBIOT.AT_CMD(AT_Buffer, true);                         // 設定 APN
    delay(1500);

    NBIOT.AT_CMD(F("AT+CIICR"), true);                     // 啟動網路功能
    delay(1500);

    NBIOT.AT_CMD(F("AT+CIFSR"), true);                     // 取得IP: 測試用
    delay(1500);

    NBIOT.openNetwork(Server_LASS, Port_LASS);             // 開啟 TCP 連線
    delay(10000);


    NBIOT.AT_CMD("AT+CIPSEND", true);     // 開始傳輸內容, Concnet 及 publish 一起送
    Serial.println(F("[ >> ] Sending Data"));

    /********** < 逐一輸出 CONNECT_KPG 內容 > *********/
    // 順序很重要：
    Serial.println(F("\r\n[MQTT Concnet Package]"));
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

    // 1.FixHeader
    NBIOT.AT_print(ByteToHexStr(CONNECT_KPG.Fix_Header));
    NBIOT.AT_print(F(" ")); delay(SIMCOM_MS);

    // 2.RL
    for (uint8_t _i = 0; _i < CONNECT_KPG.RL_size; _i++) {
      NBIOT.AT_print(ByteToHexStr(CONNECT_KPG.RL[_i]));
      NBIOT.AT_print(F(" ")); delay(SIMCOM_MS);
    }
    // 3.Protocol
    for (uint8_t _i = 0; _i < 7; _i++) {
      NBIOT.AT_print(ByteToHexStr(CONNECT_KPG.Protocol[_i]));
      NBIOT.AT_print(F(" ")); delay(SIMCOM_MS);
    }
    // 4.Connect flags
    NBIOT.AT_print(ByteToHexStr(CONNECT_KPG.Flags));
    NBIOT.AT_print(F(" ")); delay(SIMCOM_MS);
    // 5.KeepAlive
    for (uint8_t _i = 0; _i < 2; _i++) {
      NBIOT.AT_print(ByteToHexStr(CONNECT_KPG.KeepAlive[_i]));
      NBIOT.AT_print(F(" ")); delay(SIMCOM_MS);
    }
    // 6.Client ID
    for (uint8_t _i = 0; _i < 2; _i++) {
      NBIOT.AT_print(ByteToHexStr(CONNECT_KPG.PL_id[_i]));
      NBIOT.AT_print(F(" ")); delay(SIMCOM_MS);
    }
    for (uint8_t _i = 0; _i < strlen(CONNECT_KPG.ID); _i++) {
      NBIOT.AT_print(CharToHexStr(CONNECT_KPG.ID[_i]));
      NBIOT.AT_print(F(" ")); delay(SIMCOM_MS);
    }


    /********** < 逐一輸出 PUBLISH_KPG 內容> *********/
    Serial.println(F("\r\n[MQTT Data Package]"));

    NBIOT.AT_print(ByteToHexStr(PUBLISH_KPG.Fix_Header));
    NBIOT.AT_print(F(" ")); delay(SIMCOM_MS);

    for (uint8_t _i = 0; _i < PUBLISH_KPG.RL_size; _i++) {
      NBIOT.AT_print(ByteToHexStr(PUBLISH_KPG.RL[_i]));
      NBIOT.AT_print(F(" ")); delay(SIMCOM_MS);
    }

    for (uint8_t _i = 0; _i < 2; _i++) {
      NBIOT.AT_print(ByteToHexStr(PUBLISH_KPG.PL_topic[_i]));
      NBIOT.AT_print(F(" ")); delay(SIMCOM_MS);
    }

    for (uint8_t _i = 0; _i < strlen(PUBLISH_KPG.TOPIC); _i++) {
      NBIOT.AT_print(CharToHexStr(PUBLISH_KPG.TOPIC[_i]));
      NBIOT.AT_print(F(" ")); delay(SIMCOM_MS);
    }

    for (uint8_t _i = 0; _i < 2; _i++) {
      NBIOT.AT_print(ByteToHexStr(PUBLISH_KPG.PL_msg[_i]));
      NBIOT.AT_print(F(" ")); delay(SIMCOM_MS);
    }

    for (uint8_t _i = 0; _i < strlen(PUBLISH_KPG.MSG); _i++) {
      NBIOT.AT_print(CharToHexStr(PUBLISH_KPG.MSG[_i]));
      NBIOT.AT_print(F(" ")); delay(SIMCOM_MS);
    }
    Serial.println();
    delay(100);

    NBIOT.AT_end(); // 發送封包
    delay(1000);

    NBIOT.closeNetwork(); // 關閉網路

    NBIOT.OFF();

    PMU.Sleep();
  }
}
