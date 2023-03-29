/*
  Name:    WaterBox_2.ino
  Created: 2020/3/10 下午 07:28:01
  Author:  Liu
*/
#include "WaterBox_PMU.h"
WaterBox_PMU PMU;

#include "SoftwareSerial.h"
SoftwareSerial sNBIOT(9, 8);  //(rxPin, txPin)

#include "ublox.h"
#define NB_POWER 3
ublox NBIOT;


// LASS MQTT Config
const char Server_LASS[] PROGMEM = "gpssensor.ddns.net";
const char MQTT_LASS_Topic[] PROGMEM = "LASS/Test/WaterBox_TW/";
#define Port_LASS 1883
const char MSG_INIT[] PROGMEM = "|device=Linkit7697|ver_app=1.1.0|tick=0|FAKE_GPS=1";

#define MSG_SIZE 300  // MQTT 訊息的上限
static char TOPIC[40] = { '\0' };
static char MSG[MSG_SIZE] = { '\0' };
static char ID[20] = { '\0' };
static char radomID[4] = { '_', (char)random(65, 90), (char)random(65, 90), '\0' };

// String 轉換成 Char array 緩衝用
static char STR_BUFFER[64];

uint16_t _loop_count = 0;  // 記數用
bool isOnline;

/***********<< 系統主程式 >>**********
*********************************/
void setup(void) {
  Serial.begin(9600);

  sNBIOT.begin(9600);
  NBIOT.setDebuger(Serial);
  NBIOT.setUART(sNBIOT);
  NBIOT.init(NB_POWER);

  PMU.init();
  PMU.setDebuger(Serial);
  PMU.setSleepSec(2);
  PMU.setWakeUpVolate(3.2);
  PMU.PowerSaveMode(PMU.ON);  // PMU.ON進入睡眠, Delay不睡覺 PMU.OFF
  PMU.setINA219(0x40);

  Serial.println(F("[System] Setup Done"));
  Serial.println(F("-----------------------------------"));
  Serial.println(F("\r\n"));

  // 先睡一次，讓MCU 取得資料並且紀錄
  PMU.Sleep();
}

void loop(void) {
  if (PMU.state == PMU.MASTER) {
    PMU.ControlPower(PMU.OFF);

    _loop_count++;


    // 建立Topic
    memset(TOPIC, '\0', 40);  // 清空 Topic Buffer
    strcat_PF(TOPIC, MQTT_LASS_Topic);
    strcat(TOPIC, PMU.Field_3);

    // 建立 從Field 建立ID 的String
    memset(ID, '\0', 20);  // 清空 ID Buffer
    strcat(ID, PMU.Field_3);
    strcat(ID, radomID);  // 增加亂數在裡面

    Serial.print(F("[MQTT]\tClient ID -> "));
    Serial.println(ID);
    Serial.print(F("[MQTT]\t Topic -> "));
    Serial.println(TOPIC);

    // 建立Publish封包
    memset(MSG, '\0', MSG_SIZE);
    strcat_PF(MSG, MSG_INIT);

    if (strlen(PMU.Field_1) > 0) {
      strcat_PF(MSG, PSTR("|date="));
      strcat(MSG, PMU.Field_1);
    }
    if (strlen(PMU.Field_2) > 0) {
      strcat_PF(MSG, PSTR("|time="));
      strcat(MSG, PMU.Field_2);
    }

    if (strlen(PMU.Field_3) > 0) {
      strcat_PF(MSG, PSTR("|device_id="));
      strcat(MSG, PMU.Field_3);
    }

    if (strlen(PMU.Field_4) > 0) {
      strcat_PF(MSG, PSTR("|gps_lon="));
      strcat(MSG, PMU.Field_4);
    }
    if (strlen(PMU.Field_5) > 0) {
      strcat_PF(MSG, PSTR("|gps_lat="));
      strcat(MSG, PMU.Field_5);
    }

    if (strlen(PMU.Field_6) > 0) {
      strcat_PF(MSG, PSTR("|s_ec2="));
      strcat(MSG, PMU.Field_6);
    }
    if (strlen(PMU.Field_7) > 0) {
      strcat_PF(MSG, PSTR("|s_ph2="));
      strcat(MSG, PMU.Field_7);
    }
    if (strlen(PMU.Field_8) > 0) {
      strcat_PF(MSG, PSTR("|s_t7="));
      strcat(MSG, PMU.Field_8);
    }
    if (strlen(PMU.Field_9) > 0) {
      strcat_PF(MSG, PSTR("|s_Tb="));
      strcat(MSG, PMU.Field_9);
    }
    if (strlen(PMU.Field_10) > 0) {
      strcat_PF(MSG, PSTR("|bat_v="));
      strcat(MSG, PMU.Field_10);
    }
    if (strlen(PMU.Field_11) > 0) {
      strcat_PF(MSG, PSTR("|bat_a="));
      strcat(MSG, PMU.Field_11);
    }

    if (strlen(PMU.Field_12) > 0) {
      strcat_PF(MSG, PSTR("|F12="));
      strcat(MSG, PMU.Field_12);
    }

    strcat_PF(MSG, PSTR("|"));

    // 把要傳送封包(LASS 格式), 打包成String
    // msg_str = "|device=Linkit7697|device_id=9C65F920C020|ver_app=1.1.0|date=2019-03-21|time=06:53:55|tick=0|FAKE_GPS=1|gps_lon=121.787|gps_lat=25.1933|s_ec=200000.00|s_ph=14.00|s_t0=100.00|s_Tb=10000.00|bat_v=3.70|bat_a=400.00|";
    // 確認打包後的封包內容
    Serial.print(F("[MQTT]\tSize:"));
    Serial.print(strlen(MSG));
    Serial.print(F(" >> "));
    Serial.println(MSG);
    Serial.println(F("==================================="));
    Serial.println(F("\r\n"));


    NBIOT.ON(45);  // 開啟動模組45秒

    // 初始化MQTT參數(花時0.8*4 = 3.2sec)
    sprintf_P(STR_BUFFER, PSTR("AT+UMQTT=0,\"%s\""), ID);
    NBIOT.AT_CMD(STR_BUFFER, true);

    sprintf_P(STR_BUFFER, PSTR("AT+UMQTT=1,1883"));
    NBIOT.AT_CMD(STR_BUFFER, true);

    sprintf_P(STR_BUFFER, PSTR("AT+UMQTT=2,\"gpssensor.ddns.net\",1883"));
    NBIOT.AT_CMD(STR_BUFFER, true);

    sprintf_P(STR_BUFFER, PSTR("AT+UMQTT=10,30"));
    NBIOT.AT_CMD(STR_BUFFER, true);

    // 嘗試連線45次, 一次2秒，最多90秒
    for (int8_t _i = 45; _i > 0; _i--) {
      isOnline = NBIOT.isOnline();
      if (!isOnline) {
        Serial.print(_i);
        Serial.println(F("[SYS-ERROR] NBIOT was offline"));
        delay(2000);
      } else {

        Serial.println(F("[SYS-OK] NBIOT is online"));
        NBIOT.AT_CMD("AT+UMQTTC=0", true, 2000);  // 先強制關閉連線(2sec)

        // 發布訊息(10+5+0.8 sec)
        // 開啟MQTT Broker連線(登入broker)
        Serial.println(F("[MQTT] Concenting MQTT Broker"));
        sprintf_P(STR_BUFFER, PSTR("AT+UMQTTC=1"));
        NBIOT.AT_CMD(STR_BUFFER, true, 10000);  // 登入10秒等待

        // 傳松資料（預留）5秒空檔
        sNBIOT.print(F("AT+UMQTTC=2,2,1,\""));
        sNBIOT.print(TOPIC);
        sNBIOT.print(F("\",\""));
        sNBIOT.print(MSG);
        sNBIOT.println(F("\""));
        sNBIOT.flush();
        delay(5000);
        uint16_t size = sNBIOT.available();
        for (uint16_t _i = 0; _i < size; _i++) {
          Serial.print((char)sNBIOT.read());
        }
        Serial.println();

        // 關閉MQTT Broker連線
        Serial.println(F("[MQTT] Disconnect MQTT Broker"));
        sprintf_P(STR_BUFFER, PSTR("AT+UMQTTC=0"));
        NBIOT.AT_CMD(STR_BUFFER, true);
        _i = -1;  // 強制跳出 for
      }
    }

    NBIOT.OFF();

    PMU.Sleep();
  }
}
