#include <SoftwareSerial.h>
SoftwareSerial sNBIOT(9, 8); //(rxPin, txPin)

#include "ublox.h"
ublox NBIOT;

/* 測試用 */
String getSerial(Stream &_ref)
{

    String _buffer;
    while (_ref.available())
    {
        char _c = (char)_ref.read();
        _buffer += _c;
        delay(10);
    }
    return _buffer;
}

void setup()
{
    Serial.begin(9600);
    sNBIOT.begin(9600);

    NBIOT.setDebuger(Serial);
    NBIOT.setUART(sNBIOT);

    Serial.println("Setup Done");

    NBIOT.AT_CMD("AT＋CSQ", false);

    Serial.println("Test Done");
}

void loop()
{
    /* 測試用 */
    String _UART = getSerial(Serial);

    if (_UART.length() > 0)
    {
        Serial.print("UART -> " + _UART);
        Serial.flush();

        _UART.replace("\n", "");
        _UART.replace("\r", "");
        sNBIOT.println(_UART);
    }

    /* 測試用 */
    String _NBIOT = getSerial(sNBIOT);
    if (_NBIOT.length() > 0)
    {
        Serial.println("NBIOT -> " + _NBIOT);
        Serial.flush();
    }
}