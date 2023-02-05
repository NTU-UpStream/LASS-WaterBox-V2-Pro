#include "SoftwareSerial.h"
SoftwareSerial sNBIOT(9, 8); //(rxPin, txPin)

#include "ublox.h"
#define NB_POWER 3
ublox NBIOT;

static char TestBuffer[64];

uint8_t _count = 0;

/* 測試用 */
int16_t getSerial(Stream &_ref)
{
    memset(TestBuffer, '\0', 64); // 清空BUFFER
    int16_t _msgSize = _ref.available();
    for (uint16_t _i = 0; _i < _msgSize; _i++)
    {
        TestBuffer[_i] = _ref.read();
        delay(10);
    }
    return strlen(TestBuffer);
}

void setup()
{
    Serial.begin(9600);
    sNBIOT.begin(9600);

    NBIOT.setDebuger(Serial);
    NBIOT.setUART(sNBIOT);
    NBIOT.init(NB_POWER);

    Serial.println("Setup Done");

    NBIOT.ON(20); // 模組供電後須預留啟動時間 20sec

    NBIOT.AT_CMD("AT", true);
    NBIOT.AT_CMD("AT+CSQ", true);
    // NBIOT.AT_CMD("AT+CGATT?", true);
    // NBIOT.AT_CMD("AT+UMQTTC=0", true);
    // NBIOT.MQTT_init("GAIA123", "gpssensor.ddns.net", 1883);

    NBIOT.OFF();
    delay(3000);
    Serial.println("Test Done");
}

// 第一次連線至少要60秒
void loop()
{
    bool isOnline;
    _count++;
    NBIOT.ON(90); // 開機後等待90秒

    NBIOT.AT_CMD("AT+CSQ", true);

    sprintf(TestBuffer, "%s%d", "GAIA12", _count);
    NBIOT.MQTT_init(TestBuffer, "gpssensor.ddns.net", 1883); // 初始化MQTT參數
    delay(1000);

    /* 測試用 */
    int16_t _msg_size = getSerial(Serial);
    if (_msg_size > 0)
    {
        Serial.print("UART -> ");
        Serial.print(_msg_size);
        Serial.print("/");
        Serial.println(TestBuffer);
        NBIOT.AT_CMD(TestBuffer);
    }

    // 嘗試連線45次, 一次2秒共90秒
    for (int8_t _i = 45; _i > 0; _i--)
    {
        isOnline = NBIOT.isOnline();
        if (!isOnline)
        {
            Serial.print(_i);
            Serial.println("[SYS-ERROR] NBIOT was offline");
            delay(2000);
        }
        else
        {
            Serial.println("[SYS-OK] NBIOT is online");

            NBIOT.AT_CMD("AT+UMQTTC=0", true); // 先強制關閉連線
            delay(1000);                       // 斷線後等1秒再重新連線

            sprintf(TestBuffer, "%s %d-%d", "Hi~", _count, _i);
            NBIOT.MQTT_pub("LASS/Test/WaterBox_TW/Test", TestBuffer, 1); // 發布訊息

            _i = -1; // 強制跳出
        }
    }

    NBIOT.OFF();
    delay(5000);
}