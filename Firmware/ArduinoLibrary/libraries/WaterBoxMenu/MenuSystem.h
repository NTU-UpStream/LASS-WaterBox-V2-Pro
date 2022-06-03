#ifndef MENUSYSTEM_H
#define MENUSYSTEM_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <Ezo_i2c.h>
#include <WaterBox_util.h>
#include <dfrobot_turbidity.h>
#include <U8g2lib.h>
#include <DS3231M.h>
#include <Adafruit_ADS1015.h>
#include <SdFat.h>
#include <WaterBox_util.h>
#include <Adafruit_INA219.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

#include<cstring>
#include<iostream>

#define PRESS_TIME_1 10
#define PRESS_TIME_2 500
#define KNOB_MAX 1650

/*Update level of all menu function in MenySystem.
0: update level when the text is not selected.
1: update level when the text is chosen.
2: update level when the button is pressed and the text is chosen.*/
#define PASSIVE_update 0
#define CHOSEN_update 1
#define ACTIVATE_update 2


class MenuSystem;
class MenuPage;

typedef void (MenuSystem::*MenuFunc)(int);

class MenuPage{
    public:
        MenuFunc Rootfunc;
        char title[20];
        char text[10][20];
        MenuFunc options[10];
        int func_num;
        MenuPage(MenuFunc Rootfunc_in, char title_in[20], char text_in[10][20], MenuFunc options_in[10]);
};

class MenuSystem{
    public:
        MenuSystem(MenuPage* page, Ezo_board* pH, Ezo_board* EC, Ezo_board* TEMP, Dfrobot_Turbidity* TURB, U8G2* oled,
                   DS3231M_Class* Clock, Adafruit_ADS1015* voltage_chip, Adafruit_INA219* current_chip, SdFat* sdcard){
                        Current = page;
                        pH_sensor = pH;
                        ec_sensor = EC;
                        temp_sensor = TEMP;
                        turb_sensor = TURB;
                        waterbox_oled = oled;
                        //Set font style
                        waterbox_oled->setFont(u8g2_font_profont12_mf);
                        clock = Clock;
                        ads1015 = voltage_chip;
                        ina219 = current_chip;
                        SDcard = sdcard;
                   }

        MenuPage* Current;
        Ezo_board* pH_sensor;
        Ezo_board* ec_sensor;
        Ezo_board* temp_sensor;
        Dfrobot_Turbidity* turb_sensor;
        U8G2* waterbox_oled;
        DS3231M_Class* clock;
        Adafruit_ADS1015* ads1015;
        Adafruit_INA219* ina219;
        SdFat* SDcard;
        File dir;
        File CurrentFile;
        StaticJsonDocument<1024> in_export_buffer;
        char json_char_buffer[1024];
        char file_path_buffer[40];
 

        int menu_window_index;

        int button; //Status of button   0:notpressed   1:pressed   2:pressed for 2+ second
        int knob_L; //status of knob_L 0~99
        int knob_R; //status of knob_R 0~99
        int sen_L;
        int sen_R;
        char ezo_buffer[32];
        const static uint8_t ezo_buffer_len = 32;

        float longitude;
        float latitude;
        char APN[32];
        int interval;

        float temp_cache;

        void load_setting();

        void Main_Setting(int);
        void Main_Sensor(int);
        void Main_SDcard(int);
        void Main_Info(int);
        void Main_root(int);

        void Setting_TimeSetting(int);
        void Setting_root(int);

        void TimeSetting_Show(int);
        void TimeSetting_Years(int);
        void TimeSetting_Months(int);
        void TimeSetting_Days(int);
        void TimeSetting_Hours(int);
        void TimeSetting_Minutes(int);
        void TimeSetting_Seconds(int);
        void TimeSetting_root(int);
        
        void Sensor_pH(int);
        void Sensor_EC(int);
        void Sensor_Temp(int);
        void Sensor_Turbidity(int);
        void Sensor_Nitrate(int);
        void Sensor_Ammonium(int);
        void Sensor_root(int);

        void pH_value(int);
        void pH_toggle_LED(int);
        void pH_cal_mid(int);
        void pH_cal_low(int);
        void pH_cal_high(int);
        void pH_cal_number(int);
        void pH_cal_clear(int);
        void pH_cal_slope_acid(int);
        void pH_cal_slope_base(int);
        void pH_cal_slope_Voff(int);
        void pH_pH2(int);
        void pH2_pH(int);
        void pH2_export(int);
        void pH2_pHextend(int);
        void pH2_import(int);
        void pH2_temp_comp_set(int);
        void pH2_temp_comp_show(int);
        void pH2_status_Vcc(int);
        void pH2_status_restart_code(int);
        void pH2_WB_default_setting(int);
        void pH2_factory_setting(int);
        void pH_root(int);

        void EC_value(int);
        void EC_toggle_LED(int);
        void EC_cal_dry(int);
        void EC_cal_num_rough(int);
        void EC_cal_num_medium(int);
        void EC_cal_num_fine(int);
        void EC_cal_any(int);
        void EC_cal_low(int);
        void EC_cal_high(int);
        void EC_EC2(int);
        void EC2_EC(int);
        void EC2_cal_number(int);
        void EC2_cal_clear(int);
        void EC2_TDS_conv_set(int);
        void EC2_TDS_conv_show(int);
        void EC2_K_set(int);
        void EC2_K_show(int);
        void EC2_temp_comp_set(int);
        void EC2_temp_comp_show(int);
        void EC2_EC3(int);
        void EC3_EC2(int);
        void EC3_Vcc(int);
        void EC3_status_restart_code(int);
        void EC3_import(int);
        void EC3_export(int);
        void EC3_WB_default_setting(int);
        void EC3_factory_setting(int);
        void EC3_enable_all(int);
        void EC_root(int);

        void Temp_value(int);
        void Temp_toggle_LED(int);
        void Temp_scale(int);
        void Temp_cal_num_int(int);
        void Temp_cal_num_float(int);
        void Temp_cal_any(int);
        void Temp_cal_clear(int);
        void Temp_Vcc(int);
        void Temp_status_restart_code(int);
        void Temp_import(int);
        void Temp_export(int);
        void Temp_WB_default_setting(int);
        void Temp_factory_setting(int);
        void Temp_Temp2(int);
        void Temp2_Temp(int);
        void Temp_root(int);

        void Turbidity_value(int);
        void Turbidity_Cal_num_rough(int);
        void Turbidity_Cal_num_fine(int);
        void Turbidity_Cal_num_float(int);
        void Turbidity_Cal_low(int);
        void Turbidity_Cal_high(int);
        void Turbidity_Cal_status(int);
        void Turbidity_Cal_reset(int);
        void Turbidity_import(int);
        void Turbidity_Turbidity2(int);
        void Turbidity2_Turbidity(int);
        void Turbidity_export(int);
        void Turbidity_WB_default_setting(int);
        void Turbidity_root(int);

        void Nitrate_value(int);
        void Nitrate_root(int);

        void Ammonium_value(int);
        void Ammonium_root(int);

        void SDcard_allfile(int);
        void SDcard_choose_folder(int);
        void SDcard_choose_page(int);
        void SDcard_file1(int);
        void SDcard_file2(int);
        void SDcard_file3(int);
        void SDcard_file4(int);
        void SDcard_file5(int);
        void SDcard_file6(int);
        void SDcard_file7(int);
        void SDcard_file8(int);
        void SDcard_root(int);

        void Info_version(int);
        void Info_institution(int);
        void Info_battery(int);
        void Info_root(int);

        //------------------------Menu operating functions-------------------------
        void update_button_knob();
        /*
        Since the ADS1015 in WaterBox is a littlt bit weird.
        So all voltage reading of knob, Ammonium and Nitrate (PH and EC on the pcb board) should use this function.
        And then read the member variable of MenySystem.
        */ 

        void update_page();
        /*
        This function run all the function of current page and with different update value.
        Update value passed to page functinos are based on wether the button is pressed and wether the text is chosen.
        After this function. All text of current page will be updated.
        */

        void update_oled();
        /*
        This function display all text of current page on oled.
        */

        void runMenu();
        /*
        Main operating functino. Put this in loop() in your .ino file.
        */

        //Pure functional functions

        int SPI_select(SPI_device);
        void export_json_to_sd(bool);
        void import_sd_to_json();

        /*
            Functino followed is for monitoring mode
        */
        String pmu_date();
        String pmu_time();
        String pmu_lon();
        String pmu_lat();
        String pmu_ec();
        String pmu_ph();
        String pmu_temp();
        String pmu_turb();
        String pmu_volt();
        String pmu_current();
        String pmu_APN();
        String pmu_sleeptime();
};

#endif