#include <Arduino.h>

#ifndef _UBLOX_H_
#define _UBLOX_H_

#define BUFFER_SIZE 256

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
    uint16_t AT_CMD(char *_cmd, uint8_t _pop = true, uint8_t _check = false);
    uint16_t ATReceive(); // 取得回傳值

private:
    Stream *_sSerial;
    Stream *_ref;

    uint8_t _isDebug = false;

    uint16_t _count_uart; // 計算uart 溝通用的字串長度

    void _Debuger(char *_msg, UART _header, UART _eol = NONE);
    AT_STATUS _ATState();
};

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

uint16_t ublox::AT_CMD(char *_cmd, uint8_t _pop, uint8_t _check)
{
    _sSerial->println(_cmd);
    _sSerial->flush();
    delay(800);

    uint16_t _l_cmd = ATReceive();              // 收到的回應長度
    uint16_t _l_Re = _l_cmd - strlen(_cmd) - 2; // 實際回應的指令長度

    if (_pop)
    {
        // 移除指令: Buffer起始位置, Buffer中回應字串的起始位置, 轉移的長度(剩下的char)
        // _cmd: 不包含\r\n，計算指令長度時需要另外+2+1(下一個才開始)
        memmove(BUFFER, BUFFER + strlen(_cmd) + 3, _l_Re);
    }
    _Debuger(BUFFER, H_NBIOT, NONE);

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
    }

    // while (_sSerial->available())
    // {
    //     _c = (char)_sSerial->read();
    //     _ref->write(_c);
    // }

    return _count_uart;
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