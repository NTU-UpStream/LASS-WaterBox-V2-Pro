/*
 * Copyright (C) 2018 Andreas Motzek andreas-motzek@t-online.de
 * License: CC-By
 * Reference:
 *  1. https://bitbucket.org/amotzek/arduino/src/fab21e1e7785fe9473d83107048d4431c8fd25a9/src/main/cpp/MQTTClient/MQTTClient.h?at=master
 *  2. https://github.com/monstrenyatko/ArduinoMqtt/tree/master/src/MQTTPacket
 * 功能：
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

#ifndef _MQTT_PACKER_H_
#define _MQTT_PACKER_H_

class MQTT_Packer
{
  public:
    struct Packet {
        bool retain;        // 是否為retain
        char* topicname;    // topic
        char* payload;      // playload
        uint8_t Qos;        // QOS
        uint16_t RL;        // 訊息長度
      };

    MQTT_Packer(char _package[]);           // 建構函式
    ~MQTT_Packer();                            // 解構函式
    void setDebuger(Stream &_ref=Serial);
    void initConnect(char* _id, char* _user="", char* _pw="", uint16_t _keep=30);
    void showPackage(uint8_t _isACIIS=0);

    char* clientid;
    char* username;
    char* password;
    uint16_t keepalive;
    Packet package;

  private:
    Stream& _refSerial;
    uint8_t _DEBUG=0;
    uint16_t _package_size;
    char* _p_package;

};
#endif // !_MQTT_PACKER_H_
