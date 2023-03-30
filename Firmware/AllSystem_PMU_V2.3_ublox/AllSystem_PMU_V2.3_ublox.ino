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
const char Port_LASS[] PROGMEM = "1883";
const char MSG_INIT[] PROGMEM = "|device=Linkit7697|ver_app=1.1.0|tick=0|FAKE_GPS=1";

#define MSG_SIZE 300  // MQTT 訊息的上限
uint16_t _sUartBufferSize;
static char TOPIC[40] = { '\0' };
static char MSG[MSG_SIZE] = { '\0' };
static char ID[20] = { '\0' };
static char radomID[6] = { '_', (char)random(65, 90), (char)random(65, 90),
                           (char)random(65, 90), (char)random(65, 90), '\0' };

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
  PMU.setWakeUpVolate(3.6);
  PMU.PowerSaveMode(PMU.ON);  // PMU.ON進入睡眠, Delay不睡覺 PMU.OFF
  PMU.setINA219(0x40);

  Serial.println(F("[System] Setup Done"));
  Serial.println(F("-----------------------------------"));
  Serial.println(F("\r\n"));


  PMU.Sleep();                // 先睡一次，讓MCU 取得資料並且紀錄
  Serial.println(F("\r\n"));  // 讓顯示比較清楚一些
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

    // packaged the publish payload
    memset(MSG, '\0', MSG_SIZE);
    strcat_PF(MSG, MSG_INIT);
    // System Datetime
    if (strlen(PMU.Field_1) > 0) {
      strcat_PF(MSG, PSTR("|date="));
      strcat(MSG, PMU.Field_1);
    }
    if (strlen(PMU.Field_2) > 0) {
      strcat_PF(MSG, PSTR("|time="));
      strcat(MSG, PMU.Field_2);
    }
    // Device ID
    if (strlen(PMU.Field_3) > 0) {
      strcat_PF(MSG, PSTR("|device_id="));
      strcat(MSG, PMU.Field_3);
    }
    // Longitude, Latitude
    if (strlen(PMU.Field_4) > 0) {
      strcat_PF(MSG, PSTR("|gps_lon="));
      strcat(MSG, PMU.Field_4);
    }
    if (strlen(PMU.Field_5) > 0) {
      strcat_PF(MSG, PSTR("|gps_lat="));
      strcat(MSG, PMU.Field_5);
    }
    // Sensoring items
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
    // Battery status
    if (strlen(PMU.Field_10) > 0) {
      strcat_PF(MSG, PSTR("|bat_v="));
      strcat(MSG, PMU.Field_10);
    }
    if (strlen(PMU.Field_11) > 0) {
      strcat_PF(MSG, PSTR("|bat_a="));
      strcat(MSG, PMU.Field_11);
    }
    // 沒有用到的欄位
    if (strlen(PMU.Field_12) > 0) {
      strcat_PF(MSG, PSTR("|F12="));
      strcat(MSG, PMU.Field_12);
    }
    // Reserve
    strcat_PF(MSG, PSTR("|"));

    // 把要傳送封包(LASS 格式), 打包成String
    // msg_str = "|device=Linkit7697|device_id=9C65F920C020|ver_app=1.1.0|date=2019-03-21|time=06:53:55|tick=0|FAKE_GPS=1|gps_lon=121.787|gps_lat=25.1933|s_ec=200000.00|s_ph=14.00|s_t0=100.00|s_Tb=10000.00|bat_v=3.70|bat_a=400.00|";
    // 確認打包後的封包內容
    Serial.println(F("==================================="));
    Serial.print(F("[MQTT]\tClientID -> "));
    Serial.println(ID);
    Serial.print(F("[MQTT]\t   Topic -> "));
    Serial.println(TOPIC);
    Serial.print(F("[MQTT]\tSize:"));
    Serial.print(strlen(MSG));
    Serial.print(F(" >> "));
    Serial.println(MSG);
    Serial.println(F("==================================="));
    Serial.println(F("\r\n"));

    delay(1000);  // 停一秒做7697 及 NBIOT模組供電切換

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
      isOnline = NBIOT.isOnline();  // 用 "AT+CGATT?"測試跟是否加入電信商的網路

      if (!isOnline) {
        Serial.print(_i);
        Serial.println(F("[SYS-ERROR]\tNBIOT was offline"));
        delay(2000);
      } else {
        Serial.println(F("[SYS-OK]\tNBIOT is online"));
        NBIOT.AT_CMD("AT+UMQTTC=0", true, 2000);  // 先強制關閉連線(2sec，怕之前的ClientID還在Broker上
        NBIOT.MQTT_pub(TOPIC, MSG, 1, 0);         // _topic, _msg, _QoS=0, _retain=0
        _i = -1;                                  // 強制跳出 for
      }
    }  // Eno of for 45

    NBIOT.OFF();

    PMU.Sleep();
  }
}
