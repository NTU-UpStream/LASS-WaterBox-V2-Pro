/*
   Copyright (C) 2018 Andreas Motzek andreas-motzek@t-online.de
   License: CC-By
   Reference:
    1. https://bitbucket.org/amotzek/arduino/src/fab21e1e7785fe9473d83107048d4431c8fd25a9/src/main/cpp/MQTTClient/MQTTClient.h?at=master
    2. https://github.com/monstrenyatko/ArduinoMqtt/tree/master/src/MQTTPacket
   功能：
    1. 傳入一個 byte 或 uint8_t 的array
    2. 設定連接用的參數:
      username, password, ClientID
    3. 設定Retain:
      Retain Topic, Retain Payload,
    4. 設定要發佈的資料:
      Topic, Payload, Qos
    5. 計算打包空間並確認array是否足夠 -> 不夠要跳出訊息
    6. 把array的內容拆解還原成參數

*/

#include <Arduino.h>
#define BufferSize 256

#ifndef _MQTT_PACKER_H_
#define _MQTT_PACKER_H_


/***********<< 封包格式 >>**********
  PAYLOAD
  CONNECT
  PUBLISH
  DISCONNECT
*********************************/

struct PAYLOAD {
  uint8_t Length[2];
  char Content[BufferSize];
};

struct PUBLISH {
  uint8_t Fix_Header = 0x30 | 0b0010; // 0x30(publish) | 0b0010 (QoS:1)| 0b0001 (Retain)
  byte RL[4];
  uint8_t RL_size;
  PAYLOAD* Topic;
  PAYLOAD* Msg;
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
  PAYLOAD* ID;
  PAYLOAD* User;
  PAYLOAD* Password;
};


class MQTT_Packer
{
  public:

    MQTT_Packer(char* _package);           // 建構函式
    ~MQTT_Packer();                            // 解構函式
    void setDebuger(Stream &_ref = Serial);

    void showPackage(uint8_t _isACIIS = 0);

    void CalculatePL(PAYLOAD* _payload, String _msg);
    uint8_t CalculateRL(uint32_t _size);
    uint16_t PackPublishBuffer(PUBLISH* _pack);
    uint16_t PackConnectBuffer(CONNECT* _pack);
    
    PAYLOAD client_id;
    PAYLOAD client_user;
    PAYLOAD client_password;
    PAYLOAD publish_topic;
    PAYLOAD publish_msg;
    PAYLOAD subscribe_topic;
    PAYLOAD subscribe_msg;

    PUBLISH PUBLISH_KPG;
    CONNECT CONNECT_KPG;

  private:
    Stream& _refSerial=Serial;
    uint8_t _DEBUG = 0;
    uint16_t _package_size;
    char* _p_package;
};

#endif // !_MQTT_PACKER_H_
