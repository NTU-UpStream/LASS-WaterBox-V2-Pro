#include "SoftwareSerial.h"
SoftwareSerial sNBIOT(9, 8); //(rxPin, txPin)

#include "ublox.h"
#define NB_POWER 3
ublox NBIOT;

static char TestBuffer[64];

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

    NBIOT.AT_CMD("AT", true);
    NBIOT.AT_CMD("AT+CSQ", true);

    NBIOT.AT_CMD("AT+CGATT?", true);
    NBIOT.AT_CMD("AT+UMQTTC=0", true);

    NBIOT.MQTT_init("GAIA1234567", "gpssensor.ddns.net", 1883);

    Serial.println("Test Done");
}

void loop()
{
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

    bool isOnline = NBIOT.isOnline();
    if (!isOnline)
        Serial.println("ERROR, NBIOT offline");
    else
    {
        NBIOT.AT_CMD("AT+UMQTTC=0", true);
        NBIOT.MQTT_pub("LASS/Test/WaterBox_TW/Test", "Hi");
    }
    delay(5000);
}