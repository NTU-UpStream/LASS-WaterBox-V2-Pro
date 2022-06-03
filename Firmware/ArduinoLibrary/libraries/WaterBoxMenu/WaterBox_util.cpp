
#include <WaterBox_util.h>
#include <Ezo_i2c_util.h>
#include <Ezo_i2c.h>
#include <Wire.h>
#include <EEPROM.h>

void ezo_read(Ezo_board &sensor){
    sensor.send_read_cmd();
    delay(1000);
    Serial.print(sensor.get_name()); Serial.print(": ");
    sensor.receive_read_cmd();
    print_success_or_error(sensor, String(sensor.get_last_received_reading(), 2).c_str());
    Serial.println("");
}

void ezo_clear_cal(Ezo_board &sensor){
    sensor.send_cmd("Cal,clear");
    delay(1000);
    Serial.print(sensor.get_name()); Serial.print(" calibration clear: ");
    ezo_print_status(sensor);
};

void ezo_factory(Ezo_board &sensor){
    sensor.send_cmd("Factory");
}

void ezo_cal_H(Ezo_board &sensor){
    if(sensor.get_address() == 99){
        sensor.send_cmd("Cal,high,10.00");
        Serial.print(sensor.get_name()); Serial.print(" pH 10.00 high calibration: ");
        ezo_print_status(sensor);
    }
    if(sensor.get_address() == 100){
        sensor.send_cmd("Cal,high,80000");
        Serial.print(sensor.get_name()); Serial.print(" 80000 uS high calibration: ");
        ezo_print_status(sensor);
    }
};

void ezo_cal_H(Ezo_board &sensor, float correct){
    if(sensor.get_address() == 99){
        sensor.send_cmd_with_num("Cal,high,", correct, 2);
        Serial.print(sensor.get_name()); Serial.print(" custom high calibration using pH ");
        Serial.print(correct); Serial.print(" Solution: ");
        ezo_print_status(sensor);
    }
    if(sensor.get_address() == 100){
        sensor.send_cmd_with_num("Cal,high,", correct, 2);
        Serial.print(sensor.get_name()); Serial.print(" custom high calibration using ");
        Serial.print(correct); Serial.print(" uS Solution: ");
        ezo_print_status(sensor);
    }
}

void ezo_cal_M(Ezo_board &sensor){
    if(sensor.get_address() == 99){
        sensor.send_cmd("Cal,mid,7.00");
        Serial.print(sensor.get_name()); Serial.print(" pH 7.00 mid calibration: ");
        ezo_print_status(sensor);
    }
    if(sensor.get_address() == 100){
        sensor.send_cmd("Cal,dry");
        Serial.print(sensor.get_name()); Serial.print(" dry calibration: ");
        ezo_print_status(sensor);
    }
};

void ezo_cal_M(Ezo_board &sensor, float correct){
    if(sensor.get_address() == 99){
        sensor.send_cmd_with_num("Cal,mid,", correct, 2);
        Serial.print(sensor.get_name()); Serial.print(" custom mid calibration using pH ");
        Serial.print(correct); Serial.print(" Solution");
        ezo_print_status(sensor);
    }
}

void ezo_cal_L(Ezo_board &sensor){
    if(sensor.get_address() == 99){
        sensor.send_cmd("Cal,low,4.00");
        Serial.print(sensor.get_name()); Serial.print(" pH 4.00 low calibration: ");
        ezo_print_status(sensor);
    }
    if(sensor.get_address() == 100){
        sensor.send_cmd("Cal,low,12880");
        Serial.print(sensor.get_name()); Serial.print(" 12880 uS low calibration: ");
        ezo_print_status(sensor);
    }
};

void ezo_cal_L(Ezo_board &sensor, float correct){
    if(sensor.get_address() == 99){
        sensor.send_cmd_with_num("Cal,low,", correct, 2);
        Serial.print(sensor.get_name()); Serial.print(" custom low calibration using pH ");
        Serial.print(correct); Serial.print(" Solution: ");
        ezo_print_status(sensor);
    }
    if(sensor.get_address() == 100){
        sensor.send_cmd_with_num("Cal,low,", correct, 2);
        Serial.print(sensor.get_name()); Serial.print(" custom low calibration using ");
        Serial.print(correct); Serial.print(" uS Solution: ");
        ezo_print_status(sensor);
    }
};

void ezo_cal_custom(Ezo_board &sensor, const char* command){
    sensor.send_cmd(command);
    Serial.print(sensor.get_name()); Serial.print(" custom command ");
    Serial.print(command); Serial.print(": ");
    ezo_print_status(sensor);
};

void ezo_print_status(Ezo_board &sensor){
    delay(1000);
    Wire.requestFrom(sensor.get_address(), 2);
    switch(Wire.read()){
        case 1:
            Serial.println("Success");
            break;
        case 2:
            Serial.println("Fail");
            break;
        case 254:
            Serial.println("NOT_READY");
            break;
        case 255:
            Serial.println("NO_DATA");
            break;
    }
};

//計算按鈕按下時間
float pull_time(int _limit){
  bool _virutal_button = false;             // 紀錄按鈕狀態用
  int _while_i = 0;                         // 中斷while用
  unsigned long _push_time = 0;             // 按下時的時間(暫存)
  float _duration_time = 0;                 // 總經歷秒數
  bool _btn_state = digitalRead(sensorSwitch);       // 確認按鈕有沒有被按下

  for (int _i = 0 ; _i < 2; _i++) {         // 跑兩遍，第一遍用來確認按鈕狀態，第二遍用來結算時間
    if (_btn_state) {                       // 按鈕被按下的話，確認虛擬按鈕的狀態
      _virutal_button = true;               // 按下虛擬按鈕
      _push_time = millis();                // 紀錄按下開始時間(millisecond)

      while (_btn_state) {                                                 // 當按鈕被按下時進入while迴圈，超過_limit秒數(*10ms)後自動中斷
        if (_while_i < _limit * 100)  _btn_state = digitalRead(sensorSwitch);        // 用while停住程式，並持續更新按鈕狀態
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
};

void read_adc(Adafruit_ADS1015* volt_chip, int* knob_L, int* knob_R, int* sen_L, int* sen_R){
    *knob_R = volt_chip->readADC_SingleEnded(knob_R_Ch);
    delay(volt_measure_delay_ms);
    *sen_R = volt_chip->readADC_SingleEnded(sen_R_Ch); //1bit = 2mV, divided the voltage so x2 again.
    delay(volt_measure_delay_ms);
    *sen_L = volt_chip->readADC_SingleEnded(sen_L_Ch); //1bit = 2mV, divided the voltage so x2 again.;
    delay(volt_measure_delay_ms);
    *knob_L = volt_chip->readADC_SingleEnded(knob_L_Ch);
    delay(volt_measure_delay_ms);

    if(*knob_R > 2000){
        *knob_R = 0;
    };
    if(*sen_R > 2000){
        *sen_R = 0;
    };
    if(*sen_L > 2000){
        *sen_L = 0;
    };
    if(*knob_L > 2000){
        *knob_L = 0;
    };
};

int knob_portion(int knob_value, int num_of_portion, int full_value){
    int result = (int)(full_value - knob_value)/(full_value/(num_of_portion + 1));
    if(result < 0){
        return 0;
    }
    if(result > num_of_portion){
        return num_of_portion;
    }
    return result; 
};

void set_clock(DS3231M_Class* clock, int in_year, int in_month, int in_day, int in_hour, int in_minute, int in_second){
    DateTime TimeNow = clock->now();
    int set_year = TimeNow.year(),
        set_month = TimeNow.month(), 
        set_day = TimeNow.day(), 
        set_hour = TimeNow.hour(), 
        set_minute = TimeNow.minute(), 
        set_second = TimeNow.second();

    if(in_year != -1){set_year = in_year;};
    if(in_month != -1){set_month = in_month;};
    if(in_day != -1){set_day = in_day;};
    if(in_hour != -1){set_hour = in_hour;};
    if(in_minute != -1){set_minute = in_minute;};
    if(in_second != -1){set_second = in_second;};

    if(checkdate(set_year, set_month, set_day)){
        clock->adjust(DateTime(set_year, set_month, set_day, set_hour, set_minute, set_second));
    } else {
        Serial.println("The date is invalid. Clock was not updated. Please set a new date and try again");  
    }
};

    

bool checkdate(int y, int m, int d)
{
  //gregorian dates started in 1582
  if (! (1582<= y )  )//comment these 2 lines out if it bothers you
     return false;
  if (! (1<= m && m<=12) )
     return false;
  if (! (1<= d && d<=31) )
     return false;
  if ( (d==31) && (m==2 || m==4 || m==6 || m==9 || m==11) )
     return false;
  if ( (d==30) && (m==2) )
     return false;
  if ( (m==2) && (d==29) && (y%4!=0) )
     return false;
  if ( (m==2) && (d==29) && (y%400==0) )
     return true;
  if ( (m==2) && (d==29) && (y%100==0) )
     return false;
  if ( (m==2) && (d==29) && (y%4==0)  )
     return true;
 
  return true;
}

void EEPROM_read(char* buffer, int _page = 0, int _length = 31) // 讀取資料，1頁 30 bytes
{
  int _address = _page * 32;
  char _str;

  if (_length > 31) {                         // 超出頁面
    Serial.println("Out Of Pages");
  }
  else {
    for ( int _i = 0; _i < _length; _i++ ) {
      _str = EEPROM.read(_address + _i);
//      Serial.print(_i);
//      Serial.print(":\t");
//      Serial.println(_str);
      buffer[_i] = _str;
    }
  }
} // end of EEPROM_read()

bool EEPROM_write(char* _str, int _page, int _length) // 寫入資料，1頁 32 bytes
{
  int _address = _page * 32;
  if (_length > 31) {                // 超出頁面
    Serial.println("Out Of Pages");
    return false;
  }
  else {
    Serial.print("Writing data：");
    for ( int _i = 0; _i < _length; _i++ ) {
      EEPROM.update(_i, _str[_i]);
      Serial.print(_str[_i]);
    }
    Serial.println();
    return true;
  } // end if
} // end of EEPROM_write()



//float ph
//float EC
//float Temp
//float Turb
//float Ammo
//float Nitrate
//String Date
//String Time
//String longitude
//String latitude

