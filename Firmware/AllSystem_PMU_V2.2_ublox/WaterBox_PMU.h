#include <Arduino.h>
#ifndef _WATERBOX_PMU_
#define _WATERBOX_PMU_

#define _I2C_BUFFER_SIZE 30

class WaterBox_PMU
{
public:
  enum POWER
  {
    OFF,
    ON
  };

  enum STATE
  {
    SLEEP,
    MASTER,
    SLAVER
  };

  enum UART
  {
    NONE,
    EOL,
    H_PMU,
    H_I2C,
    H_CMD
  };

  // I2C 收到指令後，定義指令狀態後，用於後續執行相關動作
  enum IC2Request
  {
    Req_NONE,        // 無Request
    Req_ERROR,       // 錯誤指令
    Req_TIME,        // 設定鬧鐘(秒), 回傳OK -> timer ? sec
    Req_SLEEP,       // 進入睡眠, 不回傳
    Req_LOWPOWER,    // 省電模式,  回傳OK->LowPower ON/OFF
    Req_BAT_VOLATE,  // 電池電壓,  回傳OK->volate
    Req_BAT_CURRENT, // 電池電壓,  回傳OK->current
    Req_AT,          // AT Cmd( 給外面的NBIOT用)
    Req_APN,
    Req_MQTT_CONNECT,
    Req_MQTT_MSG,
    Req_Field,
    Req_RESTFUL_BUFFER
  };

  WaterBox_PMU();              // 建構函式
  WaterBox_PMU(uint8_t _addr); // 建構函式(指定 Slaver時的I2C Address)
  ~WaterBox_PMU();             // 解構函式

  void init(uint16_t _pinMain = 2, uint16_t _pinNBIOT = 3, uint16_t _pinINA219 = 4);
  void setINA219(uint8_t _addr);

  static void setDebuger(Stream &refSer); // 設定debug用的輸出
  static void setWakeUpVolate(float _v);  // 設定喚醒電壓
  static void setSleepSec(uint32_t _sec);

  static uint8_t Sleep(STATE _type = SLAVER); // 開始睡眠：切換到Master，關閉供電後開始進入睡眠循環，循環結束時更新電池狀態，低於喚醒電源後再開始供電返回1 (可以用while(Sleep())一值睡
  static void PowerSaveMode(POWER _state);
  static void ControlPower(POWER _state);

  static void getBetteryState(); // 取得電池狀態：切換到Master，呼叫INA219拿資料後更新變數
  static void LED(uint16_t _times, uint16_t _interval);

  static float Volate;  // 電池上次的電壓
  static float Current; // 電池上次的電流量
  static enum STATE state;

  static char Field_1[16];
  static char Field_2[16];
  static char Field_3[16];
  static char Field_4[16];
  static char Field_5[16];
  static char Field_6[16];
  static char Field_7[16];
  static char Field_8[16];
  static char Field_9[16];
  static char Field_10[16];
  static char Field_11[16];
  static char Field_12[16];

  static char APN[16];

  static char ATCMD[32]; //  給NBIOT的Command
  static void ATClear();

private:
  static uint8_t _Debug;
  static uint16_t _CrtlPinMAIN;
  static uint16_t _CrtlPinNBIOT;
  static uint16_t _CrtlPinINA219;

  static Stream &refSerial;

  static uint8_t _Addr;
  static uint8_t _INA219Addr;

  // uint8_t _SleepMode;                       // 未使用
  static uint8_t _PowerSave; // 定義是不是要進入省電模式
  static uint16_t _SleepSec; // 睡眠秒數

  static uint16_t _i_for;
  static float _WakeUpVoltage;

  static void _SwitchToSlaver(uint8_t _addr);
  static void _SwitchToMaster(void);

  static void _receiveEvent(int howMany);
  static void _requestEvent();
  static void _cmd(char *_str);

  static char _ComStr[32];
  static char _CmdStr[16];
  static char _CmdType[16];
  static char _recBuffer[32];
  static IC2Request _REQUEST;

  static void _Deguber(String _msg, UART _header, UART _uart = NONE);
  static void _Deguber(char *_msg, UART _header, UART _uart = NONE);
};
#endif
