#include <Arduino.h>

#ifndef _UBLOX_H_
#define _UBLOX_H_

#define BUFFER_SIZE 256
#define defPowerPin 3

class ublox
{
public:
    enum UART
    {
        NONE,
        EOL,
        H_NBIOT,
        H_CMD
    };

    enum AT_STATUS
    {
        OK,
        ERROR
    };

    struct NBIOT_APN
    {
        const char *cht_NB = "nbiot";
        const char *fet_NB = "internet.iot";
        const char *twm_NB = "twm.nbiot";
        const char *fet_NB_30M = "iot4ga2";
    };

    NBIOT_APN APN;

    char BUFFER[BUFFER_SIZE]; // 裝模組回傳資料
    char INFO[BUFFER_SIZE];   // 裝系統提示訊息

    ublox();
    ~ublox();

    void setDebuger(Stream &refSer);   // 設定Debug的輸出UART
    void setUART(Stream &refSerNBIOT); // 設定NBIOT的UART

    void init(uint8_t _pin);
    void ON(float _sec = 1.0);
    void OFF();

    void MQTT_init(char *_id, char *_broker, uint16_t _port);
    void MQTT_pub(char *_topic, char *_msg, uint8_t _QoS = 0, uint8_t _retain = 0);

    uint16_t AT_CMD(char *_cmd, uint8_t _show = false, uint16_t _delay = 800);
    uint16_t ATReceive(); // 取得回傳值
    uint8_t isOnline();

private:
    static Stream *_sSerial;
    static Stream *_ref;

    uint8_t _isDebug = false;

    uint16_t _count_uart;             // 計算uart 溝通用的字串長度
    uint16_t _PowerPin = defPowerPin; // 控制電源的pin

    void _Debuger(char *_msg, UART _header, UART _eol = NONE);
    AT_STATUS _ATState();
    uint8_t _uint[5]; // sscanf 比對時，用來放uint8_t 的位置, 預留5個
};

//
// Statics, 必須在class.h 之外在宣告一次
//
Stream *ublox::_sSerial;
Stream *ublox::_ref;

ublox::ublox(){};

ublox::~ublox(){};

void ublox::setDebuger(Stream &refSer)
{ // 設定Debug的輸出UART
    _ref = &refSer;
    _isDebug = true;
}

void ublox::setUART(Stream &refSerNBIOT)
{ // 設定NBIOT的UART
    _sSerial = &refSerNBIOT;
}

void ublox::init(uint8_t _pin)
{
    _PowerPin = _pin;
    pinMode(_PowerPin, OUTPUT);
}

void ublox::ON(float _sec)
{
    digitalWrite(_PowerPin, HIGH);
    delay(_sec * 1000);
}

void ublox::OFF()
{
    digitalWrite(_PowerPin, LOW);
}

uint16_t ublox::AT_CMD(char *_cmd, uint8_t _show, uint16_t _delay)
{
    _sSerial->print(_cmd);
    _sSerial->print("\r");
    _sSerial->flush();
    delay(_delay);

    uint16_t _l_cmd = ATReceive();              // 收到的回應長度
    uint16_t _l_Re = _l_cmd - strlen(_cmd) - 2; // 實際回應的指令長度

    if (_show)
        _Debuger(BUFFER, H_NBIOT, EOL);

    return strlen(BUFFER);
}

uint16_t ublox::ATReceive()
{
    char _c;
    memset(BUFFER, '\0', BUFFER_SIZE); // 清空BUFFER
    _count_uart = _sSerial->available();

    for (uint16_t _i = 0; _i < _count_uart; _i++)
    {
        BUFFER[_i] = (char)_sSerial->read();
        delay(5);
    }

    return _count_uart;
}

void ublox::MQTT_init(char *_id, char *_broker, uint16_t _port)
{
    sprintf(BUFFER, "AT+UMQTT=0,\"%s\"", _id);
    AT_CMD(BUFFER, true); // _Debuger(BUFFER, H_CMD, EOL);
    sprintf(BUFFER, "AT+UMQTT=1,1883");
    AT_CMD(BUFFER, true); // _Debuger(BUFFER, H_CMD, EOL);
    sprintf(BUFFER, "AT+UMQTT=2,\"%s\",%d", _broker, _port);
    AT_CMD(BUFFER, true); // _Debuger(BUFFER, H_CMD, EOL);
    sprintf(BUFFER, "AT+UMQTT=10,30");
    AT_CMD(BUFFER, true); // _Debuger(BUFFER, H_CMD, EOL);
}

void ublox::MQTT_pub(char *_topic, char *_msg, uint8_t _QoS, uint8_t _retain)
{
    // 開啟MQTT Broker連線(登入broker)
    Serial.println("[MQTT] Concenting MQTT Broker");
    sprintf(BUFFER, "AT+UMQTTC=1");
    AT_CMD(BUFFER, true, 10000); // 登入10秒等待

    // 傳松資料（預留）5秒空檔
    sprintf(BUFFER, "AT+UMQTTC=2,%d,%d,\"%s\",\"%s\"", _QoS, _retain, _topic, _msg);
    _Debuger(BUFFER, H_CMD, EOL);
    Serial.println(strlen(BUFFER)); // 顯示封包長度字串長度
    AT_CMD(BUFFER, true, 5000);

    // 關閉MQTT Broker連線
    Serial.println("[MQTT] Disconnect MQTT Broker");
    sprintf(BUFFER, "AT+UMQTTC=0");
    AT_CMD(BUFFER, true);
}

uint8_t ublox::isOnline()
{
    AT_CMD("AT+CGATT?"); // 輸入查詢指令
    uint8_t _result = sscanf(BUFFER, "%s\r+CGATT: %d", INFO, &_uint[0]);
    return _uint[0];
}

void ublox::_Debuger(char *_msg, UART _header, UART _eol)
{
    memset(INFO, '\0', BUFFER_SIZE); // 清空BUFFER

    // add message Header
    switch (_header)
    {
    case H_NBIOT:
        strcat(INFO, "[ NB] ");
        break;

    case H_CMD:
        strcat(INFO, "[CMD] ");
        break;
    default:
        break;
    }

    // add message content
    strcat(INFO, _msg);

    // add message EOL
    switch (_eol)
    {
    case EOL:
        strcat(INFO, "\r\n");
        break;
    default:
        break;
    }
    _ref->print(INFO);
}
#endif