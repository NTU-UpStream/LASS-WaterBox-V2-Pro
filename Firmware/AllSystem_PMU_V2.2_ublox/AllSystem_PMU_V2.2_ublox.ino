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

char TOPIC[40] = {'\0'};
char MSG[MSG_SIZE] = {'\0'};
char ID[20] = {'\0'};

String radom_str; // 給client id 用

uint16_t _loop_count = 0; // 記數用
bool isOnline;

/***********<< 更新 BUFFER >>**********
  Flash_STR_BUFFER 更新 STR_BUFFER, 輸入String
*********************************/
bool Flash_STR_BUFFER(String _str)
{
  if (_str.length() > 20)
  {
    Serial.print(F("[STR BUFFER ERROR]"));
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
    memset(TOPIC, '\0', 40); // 清空 Topic Buffer
    strcat(TOPIC, MQTT_LASS_Topic);

    if (Flash_STR_BUFFER(PMU.Field_3))
      strcat(TOPIC, STR_BUFFER); // 把(String)ID 放到STR_BUFFER中

    // 建立 從Field 建立ID 的String
    memset(ID, '\0', 20);            // 清空 ID Buffer
    strcat(ID, STR_BUFFER);          // 沿用 STR_BUFFER裡面的 MAC
    if (Flash_STR_BUFFER(radom_str)) // 把亂數轉存在STR_BUFFER後面
      strcat(ID, STR_BUFFER);

    // 建立Publish封包
    memset(MSG, '\0', MSG_SIZE);
    strcat(MSG, MSG_INIT);

    if (Flash_STR_BUFFER(PMU.Field_1))
    {
      strcat(MSG, "|date=");
      strcat(MSG, STR_BUFFER);
    }
    if (Flash_STR_BUFFER(PMU.Field_2))
    {
      strcat(MSG, "|time=");
      strcat(MSG, STR_BUFFER);
    }

    if (Flash_STR_BUFFER(PMU.Field_3))
    {
      strcat(MSG, "|device_id=");
      strcat(MSG, STR_BUFFER);
    }

    if (Flash_STR_BUFFER(PMU.Field_4))
    {
      strcat(MSG, "|gps_lon=");
      strcat(MSG, STR_BUFFER);
    }
    if (Flash_STR_BUFFER(PMU.Field_5))
    {
      strcat(MSG, "|gps_lat=");
      strcat(MSG, STR_BUFFER);
    }

    if (Flash_STR_BUFFER(PMU.Field_6))
    {
      strcat(MSG, "|s_ec2=");
      strcat(MSG, STR_BUFFER);
    }
    if (Flash_STR_BUFFER(PMU.Field_7))
    {
      strcat(MSG, "|s_ph2=");
      strcat(MSG, STR_BUFFER);
    }
    if (Flash_STR_BUFFER(PMU.Field_8))
    {
      strcat(MSG, "|s_t7=");
      strcat(MSG, STR_BUFFER);
    }
    if (Flash_STR_BUFFER(PMU.Field_9))
    {
      strcat(MSG, "|s_Tb=");
      strcat(MSG, STR_BUFFER);
    }

    if (Flash_STR_BUFFER(PMU.Field_10))
    {
      strcat(MSG, "|bat_v=");
      strcat(MSG, STR_BUFFER);
    }
    if (Flash_STR_BUFFER(PMU.Field_11))
    {
      strcat(MSG, "|bat_a=");
      strcat(MSG, STR_BUFFER);
    }

    // if (Flash_STR_BUFFER(PMU.Field_12))
    // {
    //   strcat(MSG, "|F12=");
    //   strcat(MSG, STR_BUFFER);
    // }
    strcat(MSG, "|");

    // 把要傳送封包(LASS 格式), 打包成String
    // msg_str = "|device=Linkit7697|device_id=9C65F920C020|ver_app=1.1.0|date=2019-03-21|time=06:53:55|tick=0|FAKE_GPS=1|gps_lon=121.787|gps_lat=25.1933|s_ec=200000.00|s_ph=14.00|s_t0=100.00|s_Tb=10000.00|bat_v=3.70|bat_a=400.00|";

    /*********** 封包內容印出來確認(不要刪) ***********/
    //     Serial.println("[ID-"+String(_loop_count)+"]");
    //     Serial.print("\t");
    //     Serial.print(strlen(ID));
    //     Serial.print(" -> ");
    //     Serial.println(ID);

    /*********** 檢查 PUBLISH_KPG.TOPIC 內容(不要刪) ***********/
    //     Serial.println("[TOPIC-"+String(_loop_count)+"]");
    //     Serial.print("\t");
    //     Serial.print(strlen(TOPIC));
    //     Serial.print(" -> ");
    //     Serial.println(TOPIC);

    /*********** 檢查 PUBLISH_KPG.MSG 內容(不要刪) ***********/
    Serial.println("[MSG-" + String(_loop_count) + "]");
    Serial.print(F("\t"));
    Serial.print(strlen(MSG));
    Serial.print(F(" -> "));
    Serial.print(MSG);
    Serial.println(F("\r\n"));

    PMU.ControlPower(PMU.OFF);

    NBIOT.ON(45);                                // 開啟動模組45秒
    NBIOT.MQTT_init(ID, Server_LASS, Port_LASS); // 初始化MQTT參數(花時0.8*4 = 3.2sec)

    // 嘗試連線45次, 一次2秒，最多90秒
    for (int8_t _i = 45; _i > 0; _i--)
    {
      isOnline = NBIOT.isOnline();
      if (!isOnline)
      {
        Serial.print(_i);
        Serial.println(F("[SYS-ERROR] NBIOT was offline"));
        delay(2000);
      }
      else
      {
        Serial.println(F("[SYS-OK] NBIOT is online"));
        NBIOT.AT_CMD("AT+UMQTTC=0", true, 2000); // 先強制關閉連線(2sec)
        NBIOT.MQTT_pub(TOPIC, MSG, 1);           // 發布訊息(10+5+0.8 sec)
        _i = -1;                                 // 強制跳出 for
      }
    }

    NBIOT.OFF();
    PMU.Sleep();
  }
  //  else{
  //    PMU.ControlPower(PMU.ON); // 一直開啟INA219的電源
  //  }
}
