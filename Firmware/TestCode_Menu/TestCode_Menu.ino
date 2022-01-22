#include <Arduino.h>
#include <U8g2lib.h>


/***** << OLED library: u8g2 >> *****/
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

//U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

#define sensorSwitch 6
#define modulePower 14
#define modeSwitch 5

String _fw = "Ver 2.1.a";

//表單用變數
int menu_oled_interval_ms = 500;            //OLED更新間隔
int menu_oled_current_ms;                   //紀錄上次OLED更新時間，若超過menu_oled_interval_ms便更新
int change_option_num_ms = 100;             //按鈕觸發增加選項數字的時間
int next_option_ms = 1000;                  //按鈕觸發選擇下一個選項數字的時間
int confirm_option_ms = 3000;               //按鈕觸發確認當前選項數字的時間
uint8_t menu_option_num[] = {0, 0};         //儲存選項數字的陣列
bool menu_index = 0;                        //處存目前正在更改哪個選項數字
String menu_oled_str;                       //儲存OLED顯示字串

//使OLED顯示目前的選項00~99
void OLED_menu(bool force = false)
{
  if( (menu_oled_current_ms < millis() - menu_oled_interval_ms) || force)
  {
    menu_oled_current_ms = millis();
    u8g2.setPowerSave(0);
    menu_oled_str = String( menu_option_num[0] ) + "  " + String( menu_option_num[1]);    //將選項數字轉為字串並相連

    //顯示幕前選項數字以及正在更改哪個數字
    u8g2.firstPage();
    do {
      u8g2.setFont(u8g2_font_fub17_tn);
      u8g2.drawStr(40, 40, menu_oled_str.c_str());
      if(menu_index == 0) u8g2.drawLine(40, 44, 52, 44); 
      else u8g2.drawLine(67, 44, 79, 44);
    } while ( u8g2.nextPage() );    
  }
}

//計算按鈕按下時間
float pull_time(uint8_t pin, int _limit)
{
  bool _virutal_button = false;             // 紀錄按鈕狀態用
  int _while_i = 0;                         // 中斷while用
  unsigned long _push_time = 0;             // 按下時的時間(暫存)
  float _duration_time = 0;                 // 總經歷秒數
  bool _btn_state = digitalRead(pin);       // 確認按鈕有沒有被按下

  for (int _i = 0 ; _i < 2; _i++) {         //  跑兩遍，第一遍用來確認按鈕狀態，第二遍用來結算時間
    if (_btn_state) {                       // 按鈕被按下的話，確認虛擬按鈕的狀態
      _virutal_button = true;               // 按下虛擬按鈕
      _push_time = millis();                // 紀錄按下開始時間(millisecond)

      while (_btn_state) {                                                 // 當按鈕被按下時進入while迴圈，超過_limit秒數(*10ms)後自動中斷
        if (_while_i < _limit * 100)  _btn_state = digitalRead(pin);        // 用while停住程式，並持續更新按鈕狀態
        else {
          _btn_state = false;                                              // 跳出前先把按鈕狀態關掉，避免再次進入while
        }
        delay(10);
        _while_i++;
      } // end of while (按鈕確認)

    }
    else {                           // 按鈕彈起時，結算按住時間
      if (_virutal_button) {         // 如果是按鈕彈起後還沒更新虛擬按鈕，結算時間
        _duration_time = (millis() - _push_time);
      }
    }
  }
  return _duration_time;
}

//測試函式一
void print1(){
  Serial.println("1");
}

//測試函式二
void print2(){
  Serial.println("2");
}

//測試函式三
void print3(){
  Serial.println("3");
}

//無用函式
void def(){}


/*函式指標陣列，我認為這個不錯，因為可以非常方便的增加要用的函式進去
 *不用更改其他東西，直接把函式名稱放在希望對應的數字位置就可以
 *然而這種方法不好傳遞變數*/

void (*menu[10][10])() = 
{
  {print1, print2, print3,    def,    def,    def,    def,    def,    def,    def},
  {print3, print2, print1,    def,    def,    def,    def,    def,    def,    def},
  {print2, print1, print3,    def,    def,    def,    def,    def,    def,    def}, 
  {   def,    def,    def,    def,    def,    def,    def,    def,    def,    def},
  {   def,    def,    def,    def,    def,    def, print1, print2,    def,    def},
  {   def,    def,    def,    def,    def,    def,    def,    def,    def,    def},
  {   def,    def,    def,    def,    def,    def,    def,    def,    def,    def},
  {   def,    def, print2, print3,    def,    def,    def,    def,    def,    def},
  {   def,    def,    def,    def,    def,    def,    def, print1, print3,    def},
  {   def,    def,    def,    def,    def,    def,    def,    def,    def,    def}, 
};

//按鈕按下時間，學長寫的
void button_processer(float _button_time)
{
   // 100 < _button_time < 1000 的時候，menu[i+1][j]
  if( _button_time > change_option_num_ms && _button_time < next_option_ms )
  {
    menu_option_num[ menu_index ] += 1;
    menu_option_num[ menu_index ] %= 10;
  }
  
  // 1000 < _button_time < 3000 的時候，menu[i][j+1]
  if( _button_time > next_option_ms && _button_time < confirm_option_ms)
  {
    // 切換 menu_index
    menu_index ^= 1;
  }

  // 1000 < _button_time > 3000 的時候，執行menu[i][j]
  if( _button_time > confirm_option_ms)
  {
    u8g2.setPowerSave(1);
    menu[menu_option_num[0]][ menu_option_num[1]]();
    delay(2000);
  }
}

void setup(void)
{
  Serial.begin(9600);
  digitalWrite(modulePower, HIGH);
  pinMode(sensorSwitch, INPUT_PULLUP);
  pinMode(modulePower, OUTPUT);
  pinMode(modeSwitch, INPUT);
  u8g2.begin();
  u8g2.setFlipMode(0);
}

void loop(void)
{
  
  //Check mode. 1: setting mode, 2: analysis mode
  if(digitalRead( modeSwitch ))
  {
    float button_time = pull_time(sensorSwitch, 4);   //計算按下時間
    button_processer( button_time );                  //決定動作
    OLED_menu();                                      //更改螢幕
  } else {
    //監測模式，若modeSwitch關閉則開始監測
  }
}
