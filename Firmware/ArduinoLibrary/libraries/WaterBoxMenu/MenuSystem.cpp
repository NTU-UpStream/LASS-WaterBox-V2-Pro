#include <MenuSystem.h>

const uint8_t SD_CHIP_SELECT = 4;

//Initialize Main menu
char Main_title[20] = "Main";
char Main_text[10][20] = {"Setting", "Sensor", "SD card", "Info"};
MenuFunc Main_funcs[10] = {&MenuSystem::Main_Setting, &MenuSystem::Main_Sensor, &MenuSystem::Main_SDcard, &MenuSystem::Main_Info};
MenuPage Main_page {
    &MenuSystem::Main_root,
    Main_title,
    Main_text,
    Main_funcs
};

//Initialize Setting menu
char Setting_title[20] = "Setting";
char Setting_text[10][20] = {"TimeSetting"};
MenuFunc Setting_funcs[10] = {&MenuSystem::Setting_TimeSetting};
MenuPage Setting_page {
    &MenuSystem::Setting_root,
    Setting_title,
    Setting_text,
    Setting_funcs
};

//Initialize Time setting menu
char TimeSetting_title[20] = "TimeSetting";
char TimeSetting_text[10][20] = {"2022-01-01 00:00:00", "Years", "Months", "Days", "Hours", "Minutes", "Second"};
MenuFunc TimeSetting_funcs[10] = {&MenuSystem::TimeSetting_Show, &MenuSystem::TimeSetting_Years, &MenuSystem::TimeSetting_Months, 
                                  &MenuSystem::TimeSetting_Days, &MenuSystem::TimeSetting_Hours, &MenuSystem::TimeSetting_Minutes,
                                  &MenuSystem::TimeSetting_Seconds};
MenuPage TimeSetting_page {
    &MenuSystem::TimeSetting_root,
    TimeSetting_title,
    TimeSetting_text,
    TimeSetting_funcs
};

//Initialize Sensor menu
char Sensor_title[20] = "Sensor";
char Sensor_text[10][20] = {"pH", "EC", "Temperature", "Turbidity"};
MenuFunc Sensor_funcs[10] = {&MenuSystem::Sensor_pH, &MenuSystem::Sensor_EC, &MenuSystem::Sensor_Temp,
                             &MenuSystem::Sensor_Turbidity};
MenuPage Sensor_page {
    &MenuSystem::Sensor_root,
    Sensor_title,
    Sensor_text,
    Sensor_funcs
};

//Initialize pH sensor menu
char pH_title[20] = "pH Sensor";
char pH_text[10][20] = {"Reading         OFF", "LED             OFF", "Cal mid", "Cal low", "Cal high", "Cal number", 
                        "Slope Acid:", "      Base:", "      V-off:", "Next Page"};
MenuFunc pH_funcs[10] = {&MenuSystem::pH_value, &MenuSystem::pH_toggle_LED, &MenuSystem::pH_cal_mid, &MenuSystem::pH_cal_low,
                         &MenuSystem::pH_cal_high, &MenuSystem::pH_cal_number, &MenuSystem::pH_cal_slope_acid,
                         &MenuSystem::pH_cal_slope_base, &MenuSystem::pH_cal_slope_Voff, &MenuSystem::pH_pH2};
MenuPage pH_page {
    &MenuSystem::pH_root,
    pH_title,
    pH_text,
    pH_funcs
};

char pH2_title[20] = "pH Sensor";
char pH2_text[10][20] = {"Last Page", "Cal !!!RESET!!!", "Temp set", "Temp show", "Status   Vcc:", "Restart code:", "Import setting", "Export setting",
                         "Factory Setting"};
MenuFunc pH2_funcs[10] = {&MenuSystem::pH2_pH, &MenuSystem::pH_cal_clear, &MenuSystem::pH2_temp_comp_set,
                          &MenuSystem::pH2_temp_comp_show, &MenuSystem::pH2_status_Vcc, &MenuSystem::pH2_status_restart_code,
                          &MenuSystem::pH2_import, &MenuSystem::pH2_export, &MenuSystem::pH2_factory_setting };
MenuPage pH2_page {
    &MenuSystem::pH_root,
    pH2_title,
    pH2_text,
    pH2_funcs
};

//Initialize EC sensor menu
char EC_title[20] = "EC Sensor";
char EC_text[10][20] = {"Reading         OFF", "LED             OFF", "Cal dry", "Cal num x10k:", "Cal num x100:",
                         "Cal num x1:", "Cal any", "Cal low", "Cal high", "Next Page"};
MenuFunc EC_funcs[10] = {&MenuSystem::EC_value, &MenuSystem::EC_toggle_LED, &MenuSystem::EC_cal_dry, &MenuSystem::EC_cal_num_rough,
                         &MenuSystem::EC_cal_num_medium, &MenuSystem::EC_cal_num_fine, &MenuSystem::EC_cal_any, 
                         &MenuSystem::EC_cal_low, &MenuSystem::EC_cal_high, &MenuSystem::EC_EC2 };
MenuPage EC_page {
    &MenuSystem::EC_root,
    EC_title,
    EC_text,
    EC_funcs
};

char EC2_title[20] = "EC Sensor";
char EC2_text[10][20] = {"Last Page", "Cal number", "TDS conv set", "TDS conv show", "K value set", "K value show",
                         "Temp set", "Temp show", "Cal !!!RESET!!!", "NextPage"};
MenuFunc EC2_funcs[10] = {&MenuSystem::EC2_EC, &MenuSystem::EC2_cal_number, &MenuSystem::EC2_TDS_conv_set, &MenuSystem::EC2_TDS_conv_show,
                          &MenuSystem::EC2_K_set, &MenuSystem::EC2_K_show, &MenuSystem::EC2_temp_comp_set, &MenuSystem::EC2_temp_comp_show,
                          &MenuSystem::EC2_cal_clear, &MenuSystem::EC2_EC3};
MenuPage EC2_page {
    &MenuSystem::EC_root,
    EC2_title,
    EC2_text,
    EC2_funcs
};

char EC3_title[20] = "EC Sensor";
char EC3_text[10][20] = {"Last Page", "Status   Vcc:", "Restart code:", "Import setting", "Export setting", "Factory Setting", "Enable all"};
MenuFunc EC3_funcs[10] = {&MenuSystem::EC3_EC2, &MenuSystem::EC3_Vcc, &MenuSystem::EC3_status_restart_code, &MenuSystem::EC3_import,
                          &MenuSystem::EC3_export, &MenuSystem::EC3_factory_setting, &MenuSystem::EC3_enable_all};
MenuPage EC3_page {
    &MenuSystem::EC_root,
    EC3_title,
    EC3_text,
    EC3_funcs
};

//Initialize Temperatre sensor menu
char Temp_title[20] = "Temp Sensor";
char Temp_text[10][20] = {"Reading         OFF", "LED             OFF", "Scale             C", "Cal Int:", "Cal float:", "Cal any", "Cal !!!RESET!!!",
                          "Status   Vcc:", "Restart code:", "Factory setting"};
MenuFunc Temp_funcs[10] = {&MenuSystem::Temp_value, &MenuSystem::Temp_toggle_LED, &MenuSystem::Temp_scale, &MenuSystem::Temp_cal_num_int, &MenuSystem::Temp_cal_num_float,
                            &MenuSystem::Temp_cal_any, &MenuSystem::Temp_cal_clear, &MenuSystem::Temp_Vcc, &MenuSystem::Temp_status_restart_code, &MenuSystem::Temp_factory_setting};
MenuPage Temp_page {
    &MenuSystem::Temp_root,
    Temp_title,
    Temp_text,
    Temp_funcs
};

char Temp2_title[20] = "Temp Sensor";
char Temp2_text[10][20] = {"LastPage", "Factory setting"};
MenuFunc Temp2_funcs[10] = {&MenuSystem::Temp2_Temp, &MenuSystem::Temp_factory_setting};
MenuPage Temp2_page {
    &MenuSystem::Temp_root,
    Temp2_title,
    Temp2_text,
    Temp2_funcs
};

//Initialize Turbidity sensor menu
char Turbidity_title[20] = "Turbidity Sensor";
char Turbidity_text[10][20] = {"Reading         OFF", "Cal num x100:", "Cal num x1:", "Cal num float:", "Cal low", "Cal high", "Cal status",
                               "Import setting", "Export setting"};
MenuFunc Turbidity_funcs[10] = {&MenuSystem::Turbidity_value, &MenuSystem::Turbidity_Cal_num_rough, &MenuSystem::Turbidity_Cal_num_fine, &MenuSystem::Turbidity_Cal_num_float,
                                &MenuSystem::Turbidity_Cal_low, &MenuSystem::Turbidity_Cal_high, &MenuSystem::Turbidity_Cal_status, &MenuSystem::Turbidity_import,
                                &MenuSystem::Turbidity_export};
MenuPage Turbidity_page {
    &MenuSystem::Turbidity_root,
    Turbidity_title,
    Turbidity_text,
    Turbidity_funcs
};

char Turbidity2_title[20] = "Turbidity Sensor";
char Turbidity2_text[10][20] = {"LastPage", "Default setting"};
MenuFunc Turbidity2_funcs[10] = {&MenuSystem::Turbidity2_Turbidity, &MenuSystem::Turbidity_WB_default_setting};
MenuPage Turbidity2_page {
    &MenuSystem::Turbidity_root,
    Turbidity2_title,
    Turbidity2_text,
    Turbidity2_funcs
};

//Initialize Nitrate sensor menu
char Nitrate_title[20] = "Nitrate Sensor";
char Nitrate_text[10][20] = {"Value"};
MenuFunc Nitrate_funcs[10] = {};
MenuPage Nitrate_page {
    &MenuSystem::Nitrate_root,
    Nitrate_title,
    Nitrate_text,
    Nitrate_funcs
};

//Initialize Ammonium sensor mehu
char Ammonium_title[20] = "Ammonium Sensor";
char Ammonium_text[10][20] = {"Value"};
MenuFunc Ammonium_funcs[10] = {};
MenuPage Ammonium_page {
    &MenuSystem::Ammonium_root,
    Ammonium_title,
    Ammonium_text,
    Ammonium_funcs
};

char SDcard_title[20] = "SD card";
char SDcard_text[10][20] = {"Folder:          pH", "Page:", "", "", "", "", "", "", "", ""};
MenuFunc SDcard_funcs[10] = {&MenuSystem::SDcard_choose_folder, &MenuSystem::SDcard_choose_page, &MenuSystem::SDcard_file1, &MenuSystem::SDcard_file2, 
                             &MenuSystem::SDcard_file3, &MenuSystem::SDcard_file4, &MenuSystem::SDcard_file5, &MenuSystem::SDcard_file6,
                             &MenuSystem::SDcard_file7, &MenuSystem::SDcard_file8};
MenuPage SDcard_page {
    &MenuSystem::SDcard_root,
    SDcard_title,
    SDcard_text,
    SDcard_funcs
};

char Info_title[20] = "Info";
char Info_text[10][20] = {"Version: 1.0", "Institution: LASS", "Battery: "};
MenuFunc Info_funcs[10] = {&MenuSystem::Info_version, &MenuSystem::Info_institution, &MenuSystem::Info_battery};
MenuPage Info_page{
    &MenuSystem::Info_root,
    Info_title, 
    Info_text,
    Info_funcs
};

MenuPage::MenuPage(MenuFunc Rootfunc_in, char title_in[20], char text_in[10][20], MenuFunc options_in[10]){
    Rootfunc = Rootfunc_in;
    memcpy(title, title_in, sizeof(title));
    memcpy(text, text_in, sizeof(text));
    memcpy(options, options_in, sizeof(options));
    for(int i=0; i<10; i++){
        if(options[i] == nullptr){
            func_num = i;
            break;
        } else if(i == 9){
            func_num = 10;
            break;
        }
    }
};

void MenuSystem::load_setting(){
    //Load general setting
    sprintf(file_path_buffer, "setting.json");
    import_sd_to_json();
    longitude = in_export_buffer["coordinate"]["longitude"];
    latitude = in_export_buffer["coordinate"]["latitude"];
    sprintf(APN, in_export_buffer["NBIOT"]["APN"]);
    interval = atoi(in_export_buffer["NBIOT"]["Interval"]);
    in_export_buffer.clear();

    //Load turbidity setting
    sprintf(file_path_buffer, "Turbidity/turb_current.json");
    import_sd_to_json();
    float load_coef = in_export_buffer["coef"];
    float load_offset = in_export_buffer["offset"];
    turb_sensor->import_calibration(load_coef, load_offset);
    in_export_buffer.clear();
    sprintf(file_path_buffer, "");
};


void MenuSystem::Main_Setting(int update_level){
    if(update_level==ACTIVATE_update){
        Current = &Setting_page;
    };
};

void MenuSystem::Main_Sensor(int update_level){
    if(update_level==ACTIVATE_update){
        Current = &Sensor_page;
    };
};

void MenuSystem::Main_SDcard(int update_level){
    if(update_level==ACTIVATE_update){
        Current = &SDcard_page;
    };
};

void MenuSystem::Main_Info(int update_level){
    if(update_level==ACTIVATE_update){
        Current = &Info_page;
    }
};

void MenuSystem::Main_root(int update_level){
    Current = &Main_page;
};

void MenuSystem::Setting_TimeSetting(int update_level){
    if(update_level==ACTIVATE_update){
        Current = &TimeSetting_page;
    };
};

void MenuSystem::Setting_root(int update_level){
    Current = &Main_page;
};

void MenuSystem::TimeSetting_Show(int update_level){
    sprintf(&(Current->text[0][0]), "%.4d-%.2d-%.2d", clock->now().year(), clock->now().month(), clock->now().day());
    sprintf(&(Current->text[0][10]), " %.2d:%.2d:%.2d", clock->now().hour(), clock->now().minute(), clock->now().second());
};

void MenuSystem::TimeSetting_Years(int update_level){
    DateTime CurrentTime = clock->now();
    switch(update_level){
        case PASSIVE_update:
            //When it's passive. The text shows current year.
            sprintf(&(Current->text[1][0]), "Years        %.4d", CurrentTime.year());
            break;
        case CHOSEN_update:
        {
            //When the text is chosen. It shows the year you wanna change to by reading knob_R.
            int set_year = 2022 + knob_portion(knob_R, 10, KNOB_MAX);   //Calculate the year offset from 2022.
            sprintf(&(Current->text[1][0]), "Years      %.4d", set_year);    //Show the year on oled
            if(!checkdate(set_year, CurrentTime.month(), CurrentTime.day())){
                sprintf(&(Current->text[1][0]), "Years      %.4d  X", set_year);    //If the date is invalid. Show a "X" at the end of line.
            };
            break;
        }
        case ACTIVATE_update:
            //When the text is activated (Button is pressed). It change the actual year in the system.
            int newyear = 2022 + knob_portion(knob_R, 10, KNOB_MAX);  //year_off is years from 2000.
            set_clock(clock, newyear, -1, -1, -1, -1, -1);
            break;
    };
};

void MenuSystem::TimeSetting_Months(int update_level){
    DateTime CurrentTime = clock->now();
    switch(update_level){
        case PASSIVE_update:
            //When it's passive. The text shows current month.
            sprintf(&(Current->text[2][0]), "Months       %.2d", CurrentTime.month());
            break;
        case CHOSEN_update:
        {
            //When the text is chose. It shows the month you wanna change to by reading knob_R.
            int set_month = 1 + knob_portion(knob_R, 11, KNOB_MAX);     
            //knob_portion starts from 0 so we reduce one portion and add one manually before hand.
            sprintf(&(Current->text[2][0]), "Months     %.2d  ", set_month);  //Show the month on oled.
            if(!checkdate(CurrentTime.year(), set_month, CurrentTime.day())){
                sprintf(&(Current->text[2][0]), "Months     %.2d    X", set_month);    //If the date is invalid. Show a "X" at the end of line.
            };
            break;
        }
        case ACTIVATE_update:
            int newmonth = 1 + knob_portion(knob_R, 11, KNOB_MAX);
            set_clock(clock, -1, newmonth, -1, -1, -1, -1);
            break;
    };
};

void MenuSystem::TimeSetting_Days(int update_level){
    DateTime CurrentTime = clock->now();
    switch(update_level){
        case PASSIVE_update:
            //When it's passive. The text shows current day.
            sprintf(&(Current->text[3][0]), "Days         %.2d", CurrentTime.day());
            break;
        case CHOSEN_update:
        {
            //When the text is chose. It shows the day you wanna change to by reading knob_R.
            int set_day = 1 + knob_portion(knob_R, 30, KNOB_MAX);
            //knob_portion starts from 0 so we reduce one portion and add one manually before hand.
            sprintf(&(Current->text[3][0]), "Days       %.2d  ", set_day);  //Show the day on oled.
            if(!checkdate(CurrentTime.year(), CurrentTime.month(), set_day)){
                sprintf(&(Current->text[3][0]), "Days       %.2d    X", set_day);    //If the date is invalid. Show a "X" at the end of line.
            };
            break;
        }
        case ACTIVATE_update:
            int newday = 1 + knob_portion(knob_R, 30, KNOB_MAX);
            set_clock(clock, -1, -1, newday, -1, -1, -1);
            break;
    };
};

void MenuSystem::TimeSetting_Hours(int update_level){
    switch(update_level){
        case PASSIVE_update:
            //When it's passive. The text shows current hour.
            sprintf(&(Current->text[4][0]), "Hours        %.2d", clock->now().hour());
            break;
        case CHOSEN_update:
            //When the text is chose. It shows the hour you wanna change to by reading knob_R.
            sprintf(&(Current->text[4][0]), "Hours      %.2d", knob_portion(knob_R, 24, KNOB_MAX));
            break;
        case ACTIVATE_update:
            int newhour = knob_portion(knob_R, 24, KNOB_MAX);
            set_clock(clock, -1, -1, -1, newhour, -1, -1);
            break;
    };
};

void MenuSystem::TimeSetting_Minutes(int update_level){
    switch(update_level){
        case PASSIVE_update:
            //When it's passive. The text shows current minute.
            sprintf(&(Current->text[5][0]), "Minutes      %.2d", clock->now().minute());
            break;
        case CHOSEN_update:
            //When the text is chose. It shows the minute you wanna change to by reading knob_R.
            sprintf(&(Current->text[5][0]), "Minutes    %.2d", knob_portion(knob_R, 59, KNOB_MAX));
            break;
        case ACTIVATE_update:
            int newminute = knob_portion(knob_R, 59, KNOB_MAX);
            set_clock(clock, -1, -1, -1, -1, newminute, -1);
            break;
    };
};

void MenuSystem::TimeSetting_Seconds(int update_level){
    switch(update_level){
        case PASSIVE_update:
            //When it's passive. The text shows current second.
            sprintf(&(Current->text[6][0]), "Seconds      %.2d", clock->now().second());
            break;
        case CHOSEN_update:
            //When the text is chose. It shows the second you wanna change to by reading knob_R.
            sprintf(&(Current->text[6][0]), "Seconds    %.2d", knob_portion(knob_R, 59, KNOB_MAX));
            break;
        case ACTIVATE_update:
            int newsecond = knob_portion(knob_R, 59, KNOB_MAX);
            set_clock(clock, -1, -1, -1, -1, -1, newsecond);
            break;
    };
};

void MenuSystem::TimeSetting_root(int update_level){
    Current = &Setting_page;
};

void MenuSystem::Sensor_pH(int update_level){
    if(update_level==ACTIVATE_update){
        Current = &pH_page;
    };
};

void MenuSystem::Sensor_EC(int update_level){
    if(update_level==ACTIVATE_update){
        Current = &EC_page;
    };
};

void MenuSystem::Sensor_Temp(int update_level){
    if(update_level==ACTIVATE_update){
        Current = &Temp_page;
    }
};

void MenuSystem::Sensor_Turbidity(int update_level){
    if(update_level==ACTIVATE_update){
        Current = &Turbidity_page;
    }
};

void MenuSystem::Sensor_Nitrate(int update_level){
    if(update_level==ACTIVATE_update){
        Current = &Nitrate_page;
    }
};

void MenuSystem::Sensor_Ammonium(int update_level){
    if(update_level==ACTIVATE_update){
        Current = &Ammonium_page;
    }
};

void MenuSystem::Sensor_root(int update_level){
    Current = &Main_page;
};

void MenuSystem::pH_value(int update_level){
    switch(update_level){
        case PASSIVE_update:
        case CHOSEN_update:{
            if(strcmp(&(Current->text[0][16]), " ON")==0){
                pH_sensor->receive_read_cmd();
                switch(pH_sensor->get_error()){
                    case Ezo_board::NO_DATA:
                        pH_sensor->send_read_cmd();
                        break;
                    case Ezo_board::NOT_READ_CMD:
                        break;
                    case Ezo_board::NOT_READY:
                        break;
                    case Ezo_board::FAIL:
                        sprintf(&(Current->title[0]), "pH Sensor  FAIL");
                        break;
                    case Ezo_board::SUCCESS:
                        sprintf(&(Current->title[0]), "pH Sensor  %.2f", pH_sensor->get_last_received_reading());
                        break;
                }
            };
            if(strcmp(&(Current->text[0][16]), "OFF")==0){
                sprintf(&(Current->title[0]), "pH Sensor");
            }
            break;
        }
        case ACTIVATE_update:{
            if(strcmp(&(Current->text[0][16]), "OFF")==0){
                sprintf(&(Current->text[0][16]), " ON");
            } else if (strcmp(&(Current->text[0][16]), " ON")==0){
                sprintf(&(Current->text[0][16]), "OFF");
            };
            break;
        }
    } 
};

void MenuSystem::pH_toggle_LED(int update_level){
    switch(update_level){
        case PASSIVE_update:{
            break;
        }
        case CHOSEN_update:{
            break;
        }
        case ACTIVATE_update:{
            delay(1000);
            pH_sensor->send_cmd("L,?");
            delay(500);
            pH_sensor->receive_cmd(ezo_buffer, ezo_buffer_len);
            int status = (int)(ezo_buffer[3]) - 48;     //Force char to ascii code and minus it by ascii-48 (number 0).
            status ^= 1;
            if(status == 0){
                sprintf(&(Current->text[1][0]), "LED             OFF");
            } else if(status == 1){
                sprintf(&(Current->text[1][0]), "LED              ON");
            }
            pH_sensor->send_cmd_with_num("L,", status, 0);
            break;
        }
    }
};

void MenuSystem::pH_cal_mid(int update_level){
    switch(update_level){
        case PASSIVE_update:
            sprintf(&(Current->text[2][0]), "Cal mid");
            break;
        case CHOSEN_update:
            sprintf(&(Current->text[2][0]), "Cal mid%12.2f", (float)(knob_portion(knob_R, 30, KNOB_MAX) + 55)/10);
            break;
        case ACTIVATE_update:
            delay(1000);
            float cal_ph = (float)(knob_portion(knob_R, 30, KNOB_MAX) + 55)/10;
            pH_sensor->send_cmd_with_num("Cal,mid,", cal_ph, 2);
            delay(1000);
            pH_sensor->receive_cmd(ezo_buffer, ezo_buffer_len);
            switch(pH_sensor->get_error()){
                case Ezo_board::SUCCESS:
                    sprintf(&(Current->text[2][0]), "Cal mid    SUCCESS");
                    break;
                case Ezo_board::FAIL:
                    sprintf(&(Current->text[2][0]), "Cal mid    FAIL");
                    break;
                default:
                    sprintf(&(Current->text[2][0]), "Cal mid    TRY AGAIN");
            };
            break;
    }
};

void MenuSystem::pH_cal_low(int update_level){
    switch(update_level){
        case PASSIVE_update:
            sprintf(&(Current->text[3][0]), "Cal low");
            break;
        case CHOSEN_update:
            sprintf(&(Current->text[3][0]), "Cal low%12.2f", (float)(knob_portion(knob_R, 30, KNOB_MAX) + 25)/10);
            break;
        case ACTIVATE_update:
            delay(1000);
            float cal_ph = (float)(knob_portion(knob_R, 30, KNOB_MAX) + 25)/10;
            pH_sensor->send_cmd_with_num("Cal,low,", cal_ph, 2);
            delay(1000);
            pH_sensor->receive_cmd(ezo_buffer, ezo_buffer_len);
            switch(pH_sensor->get_error()){
                case Ezo_board::SUCCESS:
                    sprintf(&(Current->text[3][0]), "Cal low    SUCCESS");
                    break;
                case Ezo_board::FAIL:
                    sprintf(&(Current->text[3][0]), "Cal low    FAIL");
                    break;
                default:
                    sprintf(&(Current->text[3][0]), "Cal low    TRY AGAIN");
            };
            break;
    }
};

void MenuSystem::pH_cal_high(int update_level){
    switch(update_level){
        case PASSIVE_update:
            sprintf(&(Current->text[4][0]), "Cal high");
            break;
        case CHOSEN_update:
            sprintf(&(Current->text[4][0]), "Cal high%11.2f", (float)(knob_portion(knob_R, 30, KNOB_MAX) + 85)/10);
            break;
        case ACTIVATE_update:
            delay(1000);
            float cal_ph = (float)(knob_portion(knob_R, 30, KNOB_MAX) + 85)/10;
            pH_sensor->send_cmd_with_num("Cal,high,", cal_ph, 2);
            delay(1000);
            pH_sensor->receive_cmd(ezo_buffer, ezo_buffer_len);
            switch(pH_sensor->get_error()){
                case Ezo_board::SUCCESS:
                    sprintf(&(Current->text[4][0]), "Cal high   SUCCESS");
                    break;
            };
            break;
    }
};

void MenuSystem::pH_cal_number(int update_level){
    switch(update_level){
        case PASSIVE_update:
            sprintf(&(Current->text[5][0]), "Cal number");
            break;
        case CHOSEN_update:
            break;
        case ACTIVATE_update:
            delay(1000);
            pH_sensor->send_cmd("Cal,?");
            delay(1000);
            pH_sensor->receive_cmd(ezo_buffer, ezo_buffer_len);
            switch(pH_sensor->get_error()){
                case Ezo_board::SUCCESS:
                    sprintf(&(Current->text[5][0]), "Cal number%9.c", ezo_buffer[5]);
            }
            break;
    };
};

void MenuSystem::pH_cal_slope_acid(int update_level){
    switch(update_level){
        case PASSIVE_update:
            break;
        case CHOSEN_update:
            break;
        case ACTIVATE_update:
            delay(1000);
            pH_sensor->send_cmd("Slope,?");
            delay(1000);
            pH_sensor->receive_cmd(ezo_buffer, ezo_buffer_len);
            strtok(ezo_buffer, ",");
            sprintf(&(Current->text[6][0]), "Slope Acid:%8.2f", atof(strtok(NULL, ",")));
            sprintf(&(Current->text[7][0]), "      Base:%8.2f", atof(strtok(NULL, ",")));
            sprintf(&(Current->text[8][0]), "      V-off:%7.2f", atof(strtok(NULL, ",")));
            break;
    }
};

void MenuSystem::pH_cal_slope_base(int update_level){
    pH_cal_slope_acid(update_level);
};

void MenuSystem::pH_cal_slope_Voff(int update_level){
    pH_cal_slope_acid(update_level);
};

void MenuSystem::pH_pH2(int update_level){
    switch(update_level){
        case PASSIVE_update:
            sprintf(&(Current->text[9][0]), " %c%c%cNext Page%c%c%c   ", (char)149, (char)149, (char)149, (char)149, (char)149, (char)149);
            break;
        case CHOSEN_update:
            sprintf(&(Current->text[9][0]), " %c%c%cNext Page%c%c%c  <", (char)149, (char)149, (char)149, (char)149, (char)149, (char)149);
            break;
        case ACTIVATE_update:
            char* title_status = &(Current->title[0]);
            Current = &pH2_page;
            sprintf(&(Current->title[0]), title_status);
            break;
    };
};

void MenuSystem::pH2_pH(int update_level){
    switch(update_level){
        case PASSIVE_update:
            sprintf(&(Current->text[0][0]), " %c%c%cLast Page%c%c%c   ", (char)149, (char)149, (char)149, (char)149, (char)149, (char)149);
            break;
        case CHOSEN_update:
            sprintf(&(Current->text[0][0]), " %c%c%cLast Page%c%c%c  <", (char)149, (char)149, (char)149, (char)149, (char)149, (char)149);
            break;
        case ACTIVATE_update:
            Current = &pH_page;
            break;
    }

    /*******************************************************************
     * Function Below is to maintain sensor reading if it's on in first page.
     * Code below shouldn't be here. But we have no choice.
     * It's here because we don't need a line of text on menu for it.
    ********************************************************************/
    if(strlen(&(Current->title[0])) > 9){
        pH_sensor->receive_read_cmd();
        switch(pH_sensor->get_error()){
            case Ezo_board::NO_DATA:
                pH_sensor->send_read_cmd();
                break;
            case Ezo_board::NOT_READ_CMD:
                break;
            case Ezo_board::NOT_READY:
                break;
            case Ezo_board::FAIL:
                sprintf(&(Current->title[0]), "pH Sensor  FAIL");
                break;
            case Ezo_board::SUCCESS:
                sprintf(&(Current->title[0]), "pH Sensor  %.2f", pH_sensor->get_last_received_reading());
                break;
        }
    }
};

void MenuSystem::pH_cal_clear(int update_level){
    switch(update_level){
        case PASSIVE_update:
            sprintf(&(Current->text[1][0]), "Cal !!!RESET!!!");
            break;
        case CHOSEN_update:
            break;
        case ACTIVATE_update:
            delay(1000);
            pH_sensor->send_cmd("Cal,clear");
            delay(1000);
    }
};

void MenuSystem::pH2_temp_comp_set(int update_level){
    switch(update_level){
        case PASSIVE_update:
            sprintf(&(Current->text[2][0]), "Temp set");
            break;
        case CHOSEN_update:
            sprintf(&(Current->text[2][0]), "Temp set%11.2f", (float)(knob_portion(knob_R, 200, KNOB_MAX) + 100)/10);
            break;
        case ACTIVATE_update:
            delay(1000);
            float temperature = (float)(knob_portion(knob_R, 200, KNOB_MAX) + 100)/10;
            pH_sensor->send_cmd_with_num("T,", temperature, 1);
            delay(500);
            pH_sensor->receive_cmd(ezo_buffer, ezo_buffer_len);
            switch(pH_sensor->get_error()){
                case Ezo_board::SUCCESS:
                    sprintf(&(Current->text[2][0]), "Temp set    SUCCESS");
                    break;
            };
            break;
    };
};

void MenuSystem::pH2_temp_comp_show(int update_level){
    switch(update_level){
        case ACTIVATE_update:
            delay(1000);
            pH_sensor->send_cmd("T,?");
            delay(500);
            pH_sensor->receive_cmd(ezo_buffer, ezo_buffer_len);
            strtok(ezo_buffer, ",");
            sprintf(&(Current->text[3][0]), "Temp show%10.2f", atof(strtok(NULL, ",")));
            break;
    };
}

void MenuSystem::pH2_status_Vcc(int update_level){
    switch(update_level){
        case ACTIVATE_update:
            delay(1000);
            pH_sensor->send_cmd("Status");
            delay(500);
            pH_sensor->receive_cmd(ezo_buffer, ezo_buffer_len);
            strtok(ezo_buffer, ",");
            sprintf(&(Current->text[5][0]), "Restart code:%6.c", *strtok(NULL, ","));
            sprintf(&(Current->text[4][0]), "Status   Vcc:%6.2f", atof(strtok(NULL, ",")));
            break;
    }
};

void MenuSystem::pH2_status_restart_code(int update_level){
    pH2_status_Vcc(update_level);
}

void MenuSystem::pH2_pHextend(int update_level){
    switch(update_level){
        case PASSIVE_update:{
            break;
        }
        case CHOSEN_update:{
            break;
        }
        case ACTIVATE_update:{
            delay(1000);
            pH_sensor->send_cmd("pHext,?");
            delay(500);
            pH_sensor->receive_cmd(ezo_buffer, ezo_buffer_len);
            int status = (int)(ezo_buffer[7]) - 48;     //Force char to ascii code and minus it by ascii-48 (number 0).
            status ^= 1;
            if(status == 0){
                sprintf(&(Current->text[6][0]), "Extend          OFF");
            } else if(status == 1){
                sprintf(&(Current->text[6][0]), "Extend           ON");
            }
            pH_sensor->send_cmd_with_num("pHext,", status, 0);
            break;
        }

    }
};

void MenuSystem::pH2_import(int update_level){
    switch(update_level){
        case ACTIVATE_update:
            //Wait 1 second till everything stop, like ezoboard reading.
            import_sd_to_json();
            Serial.print("Sensor name of file: ");
            const char* name = in_export_buffer["name"];
            Serial.println(name);
            if(!strcmp(&name[0], "pH")==0){
                Serial.println("This file is not for pH sensor");
                break;
            };
            
            bool json_led = in_export_buffer["setting"]["LED"];
            pH_sensor->send_cmd_with_num("L,", (int)json_led, 0);
            sprintf(&(pH_page.text[1][16]),"%s", (json_led)?" ON":"OFF");
            delay(400);
            bool json_extend = in_export_buffer["setting"]["extend"];
            pH_sensor->send_cmd_with_num("pHext,", (int)json_extend, 0);
            delay(400);

            /*
            float json_temper = in_export_buffer["setting"]["temp"];
            pH_sensor->send_cmd_with_num("T,", json_temper, 2);
            delay(400);
            */

            for(JsonVariant cal_string : in_export_buffer["calibration"].as<JsonArray>()){
                if(strcmp(cal_string.as<char*>(), "*DONE")==0){
                    Serial.println("Calibration import ends with *DONE.");
                    goto shortcalibration;
                };
                Serial.print("Sending calibration string: ");
                Serial.println(cal_string.as<char*>());
                char cal_command[20];
                sprintf(&cal_command[0], "Import,%s", cal_string.as<char*>());
                pH_sensor->send_cmd(cal_command);
                delay(400);
            };
            Serial.println("Calibration import ends with 10 lines input.");

            shortcalibration:

            //Make export buffer clear
            in_export_buffer.clear();
            in_export_buffer["name"] = "pH";                                           //Set the name of setting file.
            pH_sensor->send_cmd("L,?");
            delay(500);
            strtok(ezo_buffer, ",");
            in_export_buffer["setting"]["LED"] = (bool)(strtok(NULL, ","))?true:false;                           //Set LED on or off.
            in_export_buffer["setting"]["extend"] = false;                             //Set pHext off by default.
            
            /*Get information of tempperature compensation.                    
            pH_sensor->send_cmd("T,?");
            delay(500);
            pH_sensor->receive_cmd(ezo_buffer, ezo_buffer_len);
            strtok(ezo_buffer, ",");
            in_export_buffer["setting"]["temp"] = atof(strtok(NULL, ","));             
            */

            //Get calibration information.
            pH_sensor->send_cmd("Export,?");
            delay(350);
            pH_sensor->receive_cmd(ezo_buffer, ezo_buffer_len);
            Serial.println(ezo_buffer);
            strtok(ezo_buffer, ",");
            int num_char = atoi(strtok(NULL, ","));
            for(int i=0; i<num_char; i++){
                pH_sensor->send_cmd("Export");
                delay(350);
                pH_sensor->receive_cmd(ezo_buffer, ezo_buffer_len);
                Serial.println(ezo_buffer);
                in_export_buffer["calibration"][i] = ezo_buffer;
            };

            sprintf(&file_path_buffer[0], "pH/pH_Current.json");
            export_json_to_sd(true);
            break;
    }
};

void MenuSystem::pH2_export(int update_level){
    switch(update_level){
        case ACTIVATE_update:
            //Wait 1 second till everything stop, like ezoboard reading.
            delay(1000);

            //Make export buffer clear
            in_export_buffer.clear();
            in_export_buffer["name"] = "pH";                                           //Set the name of setting file.
            pH_sensor->send_cmd("L,?");
            delay(500);
            pH_sensor->receive_cmd(ezo_buffer, ezo_buffer_len);
            strtok(ezo_buffer, ",");
            in_export_buffer["setting"]["LED"] = (bool)(strtok(NULL, ","))?true:false;                           //Set LED on or off.
            in_export_buffer["setting"]["extend"] = false;                             //Set pHext off by default.
            
            /*Get information of tempperature compensation.                    
            pH_sensor->send_cmd("T,?");
            delay(500);
            pH_sensor->receive_cmd(ezo_buffer, ezo_buffer_len);
            strtok(ezo_buffer, ",");
            in_export_buffer["setting"]["temp"] = atof(strtok(NULL, ","));             
            */
            
            //Get calibration information.
            pH_sensor->send_cmd("Export,?");
            delay(350);
            pH_sensor->receive_cmd(ezo_buffer, ezo_buffer_len);
            strtok(ezo_buffer, ",");
            int num_char = atoi(strtok(NULL, ","));
            for(int i=0; i<num_char; i++){
                pH_sensor->send_cmd("Export");
                delay(350);
                pH_sensor->receive_cmd(ezo_buffer, ezo_buffer_len);
                in_export_buffer["calibration"][i] = ezo_buffer;
            };

            //Set file title based on current time.
            DateTime NOW = clock->now();
            sprintf(file_path_buffer, "pH/%.4d%.2d%.2d%.2d%.2d%.2d.json", NOW.year(), NOW.month(), NOW.day(),
                                                                    NOW.hour(), NOW.minute(), NOW.second());

            export_json_to_sd(true);
            break;
    }
};

void MenuSystem::pH2_WB_default_setting(int update_level){
    switch(update_level){
        case ACTIVATE_update:
            break;
    };
};

void MenuSystem::pH2_factory_setting(int update_level){
    switch(update_level){
        case ACTIVATE_update:
            delay(1000);
            pH_sensor->send_cmd("factory");
            break;
    }
};

void MenuSystem::pH_root(int update_level){
    Current = &Sensor_page;
};

void MenuSystem::EC_value(int update_level){
    switch(update_level){
        case PASSIVE_update:
        case CHOSEN_update:{
            if(strcmp(&(Current->text[0][16]), "OFF")==0){
                sprintf(&(Current->title[0]), "EC Sensor");
                return;
            } else {
                ec_sensor->receive_cmd(ezo_buffer, ezo_buffer_len);
                Serial.println(ezo_buffer);
                switch(ec_sensor->get_error()){
                    case Ezo_board::NO_DATA:
                        ec_sensor->send_read_cmd();
                        break;
                    case Ezo_board::NOT_READ_CMD:
                        break;
                    case Ezo_board::NOT_READY:
                        break;
                    case Ezo_board::FAIL:
                        sprintf(&(Current->title[0]), "EC Sensor  FAIL");
                        break;
                    case Ezo_board::SUCCESS:
                        sprintf(&(Current->title[0]), "EC  %6.i uS", atoi(strtok(ezo_buffer, ",")));
                        if(strcmp(&(Current->text[0][16]), " EC")==0){
                            break;
                        };
                        sprintf(&(Current->title[0]), "TDS  %6.i ppm", atoi(strtok(NULL, ",")));
                        if(strcmp(&(Current->text[0][16]), "TDS")==0){
                            break;
                        };
                        sprintf(&(Current->title[0]), "Salt  %5.2f ppt", atof(strtok(NULL, ",")));
                        if(strcmp(&(Current->text[0][16]), "SAT")==0){
                            break;
                        };
                        sprintf(&(Current->title[0]), "Specific G  %4.3f ", atof(strtok(NULL, ",")));
                        if(strcmp(&(Current->text[0][16]), " SG")==0){
                            break;
                        };
                        break;
                }
            };
            break;
        }
        case ACTIVATE_update:{
            if(strcmp(&(Current->text[0][16]), "OFF")==0){
                sprintf(&(Current->text[0][16]), " EC");
            } else if (strcmp(&(Current->text[0][16]), " EC")==0){
                sprintf(&(Current->text[0][16]), "TDS");
            } else if (strcmp(&(Current->text[0][16]), "TDS")==0){
                sprintf(&(Current->text[0][16]), "SAT");
            } else if (strcmp(&(Current->text[0][16]), "SAT")==0){
                sprintf(&(Current->text[0][16]), " SG");
            } else if (strcmp(&(Current->text[0][16]), " SG")==0){
                sprintf(&(Current->text[0][16]), "OFF");
            };
            break;
        }
    } 
};

void MenuSystem::EC_toggle_LED(int update_level){
    switch(update_level){
        case PASSIVE_update:{
            break;
        }
        case CHOSEN_update:{
            break;
        }
        case ACTIVATE_update:{
            delay(600);
            ec_sensor->send_cmd("L,?");
            delay(600);
            ec_sensor->receive_cmd(ezo_buffer, ezo_buffer_len);
            int status = (int)(ezo_buffer[3]) - 48;     //Force char to ascii code and minus it by ascii-48 (number 0).
            status ^= 1;
            if(status == 0){
                sprintf(&(Current->text[1][0]), "LED             OFF");
            } else if(status == 1){
                sprintf(&(Current->text[1][0]), "LED              ON");
            }
            ec_sensor->send_cmd_with_num("L,", status, 0);
            break;
        }
    }
};

void MenuSystem::EC_cal_dry(int update_level){
    switch(update_level){
        case PASSIVE_update:
            sprintf(&(Current->text[2][0]), "Cal dry");
            break;
        case CHOSEN_update:
            break;
        case ACTIVATE_update:
            delay(600);
            ec_sensor->send_cmd("Cal,dry");
            delay(600);
            ec_sensor->receive_cmd(ezo_buffer, ezo_buffer_len);
            switch(ec_sensor->get_error()){
                case Ezo_board::SUCCESS:
                    sprintf(&(Current->text[2][0]), "Cal dry    SUCCESS");
                    break;
                case Ezo_board::FAIL:
                    sprintf(&(Current->text[2][0]), "Cal dry    FAIL");
                    break;
                default:
                    sprintf(&(Current->text[2][0]), "Cal dry    TRY AGAIN");
            };
            break;
    }
};

void MenuSystem::EC_cal_num_rough(int update_level){
    switch(update_level){
        case PASSIVE_update:
            break;
        case CHOSEN_update:
            sprintf(&(Current->text[3][0]), "Cal num x10k:%6.i", (int)(knob_portion(knob_R, 49, KNOB_MAX)));
            break;
        case ACTIVATE_update:
            break;
    }
};

void MenuSystem::EC_cal_num_medium(int update_level){
    switch(update_level){
        case PASSIVE_update:
            break;
        case CHOSEN_update:
            sprintf(&(Current->text[4][0]), "Cal num x100:%6.i", (int)(knob_portion(knob_R, 99, KNOB_MAX)));
            break;
        case ACTIVATE_update:
            break;
    }
};

void MenuSystem::EC_cal_num_fine(int update_level){
    switch(update_level){
        case PASSIVE_update:
            break;
        case CHOSEN_update:
            sprintf(&(Current->text[5][0]), "Cal num x1:%8.i", (int)(knob_portion(knob_R, 99, KNOB_MAX)));
            break;
        case ACTIVATE_update:
            break;
    }
};

void MenuSystem::EC_cal_any(int update_level){
    switch(update_level){
        case PASSIVE_update:
            sprintf(&(Current->text[6][0]), "Cal any");
            break;
        case CHOSEN_update:
        {
            int cal_ec = atoi(&(Current->text[3][17]))*10000 + atoi(&(Current->text[4][17]))*100 + atoi(&(Current->text[5][17]));
            sprintf(&(Current->text[6][0]), "Cal any%12.i", cal_ec);
            break;
        }
        case ACTIVATE_update:
            delay(600);
            int cal_ec = atoi(&(Current->text[3][17]))*10000 + atoi(&(Current->text[4][17]))*100 + atoi(&(Current->text[5][17]));
            ec_sensor->send_cmd_with_num("Cal,", cal_ec, 0);
            delay(600);
            ec_sensor->receive_cmd(ezo_buffer, ezo_buffer_len);
            switch(ec_sensor->get_error()){
                case Ezo_board::SUCCESS:
                    sprintf(&(Current->text[6][0]), "Cal any    SUCCESS");
                    break;
                case Ezo_board::FAIL:
                    sprintf(&(Current->text[6][0]), "Cal any    FAIL");
                    break;
                default:
                    sprintf(&(Current->text[6][0]), "Cal any    TRY AGAIN");
            };
            break;
    }
};

void MenuSystem::EC_cal_low(int update_level){
    switch(update_level){
        case PASSIVE_update:
            sprintf(&(Current->text[7][0]), "Cal low");
            break;
        case CHOSEN_update:
        {
            int cal_num = atoi(&(Current->text[3][17]))*10000 + atoi(&(Current->text[4][17]))*100 + atoi(&(Current->text[5][17]));
            sprintf(&(Current->text[7][0]), "Cal low%12.i", cal_num);
            break;
        }
        case ACTIVATE_update:
            delay(600);
            int cal_ec = atoi(&(Current->text[3][17]))*10000 + atoi(&(Current->text[4][17]))*100 + atoi(&(Current->text[5][17]));
            ec_sensor->send_cmd_with_num("Cal,low,", cal_ec, 0);
            delay(600);
            ec_sensor->receive_cmd(ezo_buffer, ezo_buffer_len);
            switch(ec_sensor->get_error()){
                case Ezo_board::SUCCESS:
                    sprintf(&(Current->text[7][0]), "Cal low    SUCCESS");
                    break;
                case Ezo_board::FAIL:
                    sprintf(&(Current->text[7][0]), "Cal low    FAIL");
                    break;
                default:
                    sprintf(&(Current->text[7][0]), "Cal low    TRY AGAIN");
            };
            break;
    }
};

void MenuSystem::EC_cal_high(int update_level){
    switch(update_level){
        case PASSIVE_update:
            sprintf(&(Current->text[8][0]), "Cal high");
            break;
        case CHOSEN_update:
        {
            int cal_num = atoi(&(Current->text[3][17]))*10000 + atoi(&(Current->text[4][17]))*100 + atoi(&(Current->text[5][17]));
            sprintf(&(Current->text[8][0]), "Cal high%11.i", cal_num);
            break;
        }
        case ACTIVATE_update:
            delay(600);
            int cal_ec = atoi(&(Current->text[3][17]))*10000 + atoi(&(Current->text[4][17]))*100 + atoi(&(Current->text[5][17]));
            ec_sensor->send_cmd_with_num("Cal,high,", cal_ec, 0);
            delay(600);
            ec_sensor->receive_cmd(ezo_buffer, ezo_buffer_len);
            switch(ec_sensor->get_error()){
                case Ezo_board::SUCCESS:
                    sprintf(&(Current->text[8][0]), "Cal high    SUCCESS");
                    break;
                case Ezo_board::FAIL:
                    sprintf(&(Current->text[8][0]), "Cal high    FAIL");
                    break;
                default:
                    sprintf(&(Current->text[8][0]), "Cal high    TRY AGAIN");
            };
            break;
    }
};

void MenuSystem::EC_EC2(int update_level){
    switch(update_level){
    case PASSIVE_update:
        sprintf(&(Current->text[9][0]), " %c%c%cNext Page%c%c%c   ", (char)149, (char)149, (char)149, (char)149, (char)149, (char)149);
        break;
    case CHOSEN_update:
        sprintf(&(Current->text[9][0]), " %c%c%cNext Page%c%c%c  <", (char)149, (char)149, (char)149, (char)149, (char)149, (char)149);
        break;
    case ACTIVATE_update:
        char* title_status = &(Current->title[0]);
        Current = &EC2_page;
        sprintf(&(Current->title[0]), title_status);
        break;
    };
};

void MenuSystem::EC2_EC(int update_level){
    switch(update_level){
        case PASSIVE_update:
            sprintf(&(Current->text[0][0]), " %c%c%cLast Page%c%c%c   ", (char)149, (char)149, (char)149, (char)149, (char)149, (char)149);
            break;
        case CHOSEN_update:
            sprintf(&(Current->text[0][0]), " %c%c%cLast Page%c%c%c  <", (char)149, (char)149, (char)149, (char)149, (char)149, (char)149);
            break;
        case ACTIVATE_update:
            Current = &EC_page;
            break;
    }

    /*******************************************************************
     * Function Below is to maintain sensor reading if it's on in first page.
     * Code below shouldn't be here. But we have no choice.
     * It's here because we don't need a line of text on menu for it.
    ********************************************************************/
    if(strlen(&(Current->title[0])) > 9){
        ec_sensor->receive_cmd(ezo_buffer, ezo_buffer_len);
        switch(ec_sensor->get_error()){
            case Ezo_board::NO_DATA:
                ec_sensor->send_read_cmd();
                break;
            case Ezo_board::NOT_READ_CMD:
                break;
            case Ezo_board::NOT_READY:
                break;
            case Ezo_board::FAIL:
                sprintf(&(Current->title[0]), "EC Sensor  FAIL");
                break;
            case Ezo_board::SUCCESS:
                sprintf(&(Current->title[0]), "EC  %6.i uS", atoi(strtok(ezo_buffer, ",")));
                if(strcmp(&(EC_page.text[0][16]), " EC")==0){
                    break;
                };
                sprintf(&(Current->title[0]), "TDS  %6.i ppm", atoi(strtok(NULL, ",")));
                if(strcmp(&(EC_page.text[0][16]), "TDS")==0){
                    break;
                };
                sprintf(&(Current->title[0]), "Salt  %5.2f ppt", atof(strtok(NULL, ",")));
                if(strcmp(&(EC_page.text[0][16]), "SAT")==0){
                    break;
                };
                sprintf(&(Current->title[0]), "Specific G  %4.3f ", atof(strtok(NULL, ",")));
                if(strcmp(&(EC_page.text[0][16]), " SG")==0){
                    break;
                };
                break;
        }
    }
};

void MenuSystem::EC2_cal_number(int update_level){
    switch(update_level){
        case PASSIVE_update:
            sprintf(&(Current->text[1][0]), "Cal number");
            break;
        case CHOSEN_update:
            break;
        case ACTIVATE_update:
            delay(600);
            ec_sensor->send_cmd("Cal,?");
            delay(300);
            ec_sensor->receive_cmd(ezo_buffer, ezo_buffer_len);
            switch(ec_sensor->get_error()){
                case Ezo_board::SUCCESS:
                    sprintf(&(Current->text[1][0]), "Cal number%9.c", ezo_buffer[5]);
            }
            break;
    };
};

void MenuSystem::EC2_cal_clear(int update_level){
    switch(update_level){
        case PASSIVE_update:
            sprintf(&(Current->text[8][0]), "Cal !!!RESET!!!");
            break;
        case CHOSEN_update:
            break;
        case ACTIVATE_update:
            delay(600);
            ec_sensor->send_cmd("Cal,clear");
    }
};

void MenuSystem::EC2_TDS_conv_set(int update_level){
    switch(update_level){
        case PASSIVE_update:
            sprintf(&(Current->text[2][0]), "TDS conv set");
            break;
        case CHOSEN_update:
            sprintf(&(Current->text[2][0]), "TDS conv set%7.2f", (float)(knob_portion(knob_R, 100, KNOB_MAX))/100);
            break;
        case ACTIVATE_update:
            delay(600);
            float TDS_conversion_factor = (float)(knob_portion(knob_R, 100, KNOB_MAX))/100;
            ec_sensor->send_cmd_with_num("TDS,", TDS_conversion_factor, 2);
            delay(300);
            ec_sensor->receive_cmd(ezo_buffer, ezo_buffer_len);
            switch(ec_sensor->get_error()){
                case Ezo_board::SUCCESS:
                    sprintf(&(Current->text[2][0]), "TDS conv    SUCCESS");
                    break;
            };
            break;
    };
};

void MenuSystem::EC2_TDS_conv_show(int update_level){
    switch(update_level){
        case ACTIVATE_update:
            delay(600);
            ec_sensor->send_cmd("TDS,?");
            delay(300);
            ec_sensor->receive_cmd(ezo_buffer, ezo_buffer_len);
            strtok(ezo_buffer, ",");
            sprintf(&(Current->text[3][0]), "TDS conv show%6.2f", atof(strtok(NULL, ",")));
            break;
    };
};

void MenuSystem::EC2_K_set(int update_level){
    switch(update_level){
        case PASSIVE_update:
            sprintf(&(Current->text[4][0]), "K vaue set");
            break;
        case CHOSEN_update:
            sprintf(&(Current->text[4][0]), "K value set%8.1f", (float)(knob_portion(knob_R, 100, KNOB_MAX))/10);
            break;
        case ACTIVATE_update:
            delay(1000);
            float K_value = (float)(knob_portion(knob_R, 100, KNOB_MAX))/10;
            ec_sensor->send_cmd_with_num("K,", K_value, 1);
            delay(1000);
            ec_sensor->receive_cmd(ezo_buffer, ezo_buffer_len);
            switch(ec_sensor->get_error()){
                case Ezo_board::SUCCESS:
                    sprintf(&(Current->text[4][0]), "K value set SUCCESS");
                    break;
            };
            break;
    };
};

void MenuSystem::EC2_K_show(int update_level){
    switch(update_level){
        case ACTIVATE_update:
            delay(600);
            ec_sensor->send_cmd("K,?");
            delay(600);
            ec_sensor->receive_cmd(ezo_buffer, ezo_buffer_len);
            strtok(ezo_buffer, ",");
            sprintf(&(Current->text[5][0]), "K show%13.1f", atof(strtok(NULL, ",")));
            break;
    };
};

void MenuSystem::EC2_temp_comp_set(int update_level){
    switch(update_level){
        case PASSIVE_update:
            sprintf(&(Current->text[6][0]), "Temp set");
            break;
        case CHOSEN_update:
            sprintf(&(Current->text[6][0]), "Temp set%11.2f", (float)(knob_portion(knob_R, 200, KNOB_MAX) + 100)/10);
            break;
        case ACTIVATE_update:
            delay(600);
            float temperature = (float)(knob_portion(knob_R, 200, KNOB_MAX) + 100)/10;
            ec_sensor->send_cmd_with_num("T,", temperature, 1);
            delay(300);
            ec_sensor->receive_cmd(ezo_buffer, ezo_buffer_len);
            switch(ec_sensor->get_error()){
                case Ezo_board::SUCCESS:
                    sprintf(&(Current->text[6][0]), "Temp set    SUCCESS");
                    break;
            };
            break;
    };
};

void MenuSystem::EC2_temp_comp_show(int update_level){
    switch(update_level){
        case ACTIVATE_update:
            delay(600);
            ec_sensor->send_cmd("T,?");
            delay(300);
            ec_sensor->receive_cmd(ezo_buffer, ezo_buffer_len);
            strtok(ezo_buffer, ",");
            sprintf(&(Current->text[7][0]), "Temp show%10.1f", atof(strtok(NULL, ",")));
            break;
    };
};

void MenuSystem::EC2_EC3(int update_level){
    switch(update_level){
    case PASSIVE_update:
        sprintf(&(Current->text[9][0]), " %c%c%cNext Page%c%c%c   ", (char)149, (char)149, (char)149, (char)149, (char)149, (char)149);
        break;
    case CHOSEN_update:
        sprintf(&(Current->text[9][0]), " %c%c%cNext Page%c%c%c  <", (char)149, (char)149, (char)149, (char)149, (char)149, (char)149);
        break;
    case ACTIVATE_update:
        char* title_status = &(Current->title[0]);
        Current = &EC3_page;
        sprintf(&(Current->title[0]), title_status);
        break;
    };
};

void MenuSystem::EC3_EC2(int update_level){
    switch(update_level){
        case PASSIVE_update:
            sprintf(&(Current->text[0][0]), " %c%c%cLast Page%c%c%c   ", (char)149, (char)149, (char)149, (char)149, (char)149, (char)149);
            break;
        case CHOSEN_update:
            sprintf(&(Current->text[0][0]), " %c%c%cLast Page%c%c%c  <", (char)149, (char)149, (char)149, (char)149, (char)149, (char)149);
            break;
        case ACTIVATE_update:
            Current = &EC2_page;
            break;
    }

    /*******************************************************************
     * Function Below is to maintain sensor reading if it's on in first page.
     * Code below shouldn't be here. But we have no choice.
     * It's here because we don't need a line of text on menu for it.
    ********************************************************************/
    if(strlen(&(Current->title[0])) > 9){
        ec_sensor->receive_cmd(ezo_buffer, ezo_buffer_len);
        switch(ec_sensor->get_error()){
            case Ezo_board::NO_DATA:
                ec_sensor->send_read_cmd();
                break;
            case Ezo_board::NOT_READ_CMD:
                break;
            case Ezo_board::NOT_READY:
                break;
            case Ezo_board::FAIL:
                sprintf(&(Current->title[0]), "EC Sensor  FAIL");
                break;
            case Ezo_board::SUCCESS:
                sprintf(&(Current->title[0]), "EC  %6.i uS", atoi(strtok(ezo_buffer, ",")));
                if(strcmp(&(EC_page.text[0][16]), " EC")==0){
                    break;
                };
                sprintf(&(Current->title[0]), "TDS  %6.i ppm", atoi(strtok(NULL, ",")));
                if(strcmp(&(EC_page.text[0][16]), "TDS")==0){
                    break;
                };
                sprintf(&(Current->title[0]), "Salt  %5.2f ppt", atof(strtok(NULL, ",")));
                if(strcmp(&(EC_page.text[0][16]), "SAT")==0){
                    break;
                };
                sprintf(&(Current->title[0]), "Specific G  %4.3f ", atof(strtok(NULL, ",")));
                if(strcmp(&(EC_page.text[0][16]), " SG")==0){
                    break;
                };
                break;
        }
    }
};

void MenuSystem::EC3_Vcc(int update_level){
    switch(update_level){
        case ACTIVATE_update:
            delay(1000);
            ec_sensor->send_cmd("Status");
            delay(500);
            ec_sensor->receive_cmd(ezo_buffer, ezo_buffer_len);
            strtok(ezo_buffer, ",");
            sprintf(&(Current->text[2][0]), "Restart code:%6.c", *strtok(NULL, ","));
            sprintf(&(Current->text[1][0]), "Status   Vcc:%6.2f", atof(strtok(NULL, ",")));
            break;
    }
};

void MenuSystem::EC3_status_restart_code(int update_level){
    EC3_Vcc(update_level);
};

void MenuSystem::EC3_import(int update_level){
    switch(update_level){
        case ACTIVATE_update:
            //Wait 1 second till everything stop, like ezoboard reading.
            import_sd_to_json();
            Serial.print("Sensor name of file: ");
            const char* name = in_export_buffer["name"];
            Serial.println(name);
            if(!strcmp(&name[0], "EC")==0){
                Serial.println("This file is not for EC sensor");
                break;
            };

            Serial.println("Importing data............");

            //Get LED state
            bool json_led = in_export_buffer["setting"]["LED"];
            ec_sensor->send_cmd_with_num("L,", (int)json_led, 0);
            sprintf(&(EC_page.text[1][16]),"%s", (json_led)?" ON":"OFF");
            Serial.print("LED state is: ");
            Serial.println(json_led);
            delay(400);

            //Get TDS conversion factor
            float json_tds = in_export_buffer["setting"]["TDS"];
            ec_sensor->send_cmd_with_num("TDS,", json_tds, 2);
            Serial.print("TDS conversion factor is: ");
            Serial.println(json_tds);
            delay(400);

            //Get K value of sensor.
            float json_k = in_export_buffer["setting"]["K"];
            ec_sensor->send_cmd_with_num("K,", json_k, 2);
            Serial.print("K value is: ");
            Serial.println(json_k);
            delay(400);

            /*Get Temperature conpensation.
            float json_temp = in_export_buffer["setting"]["temp"];
            ec_sensor->send_cmd_with_num("T,", json_temp, 2);
            */

            //Get calibration strings.
            for(JsonVariant cal_string : in_export_buffer["calibration"].as<JsonArray>()){
                if(strcmp(cal_string.as<char*>(), "*DONE")==0){
                    Serial.println("Calibration import ends with *DONE.");
                    goto shortcalibration;
                };
                Serial.print("Importing calibration string: ");
                Serial.println(cal_string.as<char*>());
                char cal_command[20];
                sprintf(&cal_command[0], "Import,%s", cal_string.as<char*>());
                ec_sensor->send_cmd(cal_command);
                delay(400);
            };
            Serial.println("Calibration import ends with 10 lines input.");

            shortcalibration:

            //Make export buffer clear
            in_export_buffer.clear();
            in_export_buffer["name"] = "EC";            //Set the name of setting file.
            ec_sensor->send_cmd("L,?");
            delay(500);
            ec_sensor->receive_cmd(ezo_buffer, ezo_buffer_len);
            strtok(ezo_buffer, ",");
            in_export_buffer["setting"]["LED"] = (bool)(strtok(NULL, ","));                           //Set LED on or off.

            //Get information of TDS conversion factor.
            ec_sensor->send_cmd("TDS,?");
            delay(500);
            ec_sensor->receive_cmd(ezo_buffer, ezo_buffer_len);
            strtok(ezo_buffer, ",");
            in_export_buffer["setting"]["TDS"] = atof(strtok(NULL, ","));

            //Get information of K value of sensor.
            ec_sensor->send_cmd("K,?");
            delay(500);
            ec_sensor->receive_cmd(ezo_buffer, ezo_buffer_len);
            strtok(ezo_buffer, ",");
            in_export_buffer["setting"]["K"] = atof(strtok(NULL, ","));

            /*Get information of tempperature compensation.                    
            ec_sensor->send_cmd("T,?");
            delay(500);
            ec_sensor->receive_cmd(ezo_buffer, ezo_buffer_len);
            strtok(ezo_buffer, ",");
            in_export_buffer["setting"]["temp"] = atof(strtok(NULL, ","));             
            */

            //Get calibration information.
            ec_sensor->send_cmd("Export,?");
            delay(350);
            ec_sensor->receive_cmd(ezo_buffer, ezo_buffer_len);
            strtok(ezo_buffer, ",");
            int num_char = atoi(strtok(NULL, ","));
            for(int i=0; i<num_char; i++){
                ec_sensor->send_cmd("Export");
                delay(350);
                ec_sensor->receive_cmd(ezo_buffer, ezo_buffer_len);
                in_export_buffer["calibration"][i] = ezo_buffer;
            };

            //Set file title based on current time.
            sprintf(&file_path_buffer[0], "EC/EC_Current.json");
            export_json_to_sd(true);
            break;
    };
};

void MenuSystem::EC3_export(int update_level){
    switch(update_level){
        case ACTIVATE_update:
            //Wait 1 second till everything stop, like ezoboard reading.
            delay(1000);

            //Make export buffer clear
            in_export_buffer.clear();
            in_export_buffer["name"] = "EC";            //Set the name of setting file.
            ec_sensor->send_cmd("L,?");
            delay(500);
            ec_sensor->receive_cmd(ezo_buffer, ezo_buffer_len);
            strtok(ezo_buffer, ",");
            in_export_buffer["setting"]["LED"] = (bool)(strtok(NULL, ","));                           //Set LED on or off.

            //Get information of TDS conversion factor.
            ec_sensor->send_cmd("TDS,?");
            delay(500);
            ec_sensor->receive_cmd(ezo_buffer, ezo_buffer_len);
            strtok(ezo_buffer, ",");
            in_export_buffer["setting"]["TDS"] = atof(strtok(NULL, ","));

            //Get information of K value of sensor.
            ec_sensor->send_cmd("K,?");
            delay(500);
            ec_sensor->receive_cmd(ezo_buffer, ezo_buffer_len);
            strtok(ezo_buffer, ",");
            in_export_buffer["setting"]["K"] = atof(strtok(NULL, ","));

            /*Get information of tempperature compensation.                    
            ec_sensor->send_cmd("T,?");
            delay(500);
            ec_sensor->receive_cmd(ezo_buffer, ezo_buffer_len);
            strtok(ezo_buffer, ",");
            in_export_buffer["setting"]["temp"] = atof(strtok(NULL, ","));             
            */

            //Get calibration information.
            ec_sensor->send_cmd("Export,?");
            delay(350);
            ec_sensor->receive_cmd(ezo_buffer, ezo_buffer_len);
            strtok(ezo_buffer, ",");
            int num_char = atoi(strtok(NULL, ","));
            for(int i=0; i<num_char; i++){
                ec_sensor->send_cmd("Export");
                delay(350);
                ec_sensor->receive_cmd(ezo_buffer, ezo_buffer_len);
                in_export_buffer["calibration"][i] = ezo_buffer;
            };

            //Set file title based on current time.
            DateTime NOW = clock->now();
            sprintf(file_path_buffer, "EC/%.4d%.2d%.2d%.2d%.2d%.2d.json", NOW.year(), NOW.month(), NOW.day(),
                                                                    NOW.hour(), NOW.minute(), NOW.second());

            export_json_to_sd(true);
            break;
    }
};

void MenuSystem::EC3_WB_default_setting(int update_level){
    switch(update_level){
        case ACTIVATE_update:
            break;
    };
}

void MenuSystem::EC3_factory_setting(int update_level){
    switch(update_level){
        case ACTIVATE_update:
            delay(600);
            ec_sensor->send_cmd("factory");
            break;
    }
};

void MenuSystem::EC3_enable_all(int update_level){
    switch(update_level){
        case ACTIVATE_update:
            delay(600);
            ec_sensor->send_cmd_with_num("O,TDS,", 1, 0);
            delay(350);
            ec_sensor->send_cmd_with_num("O,S,", 1, 0);
            delay(350);
            ec_sensor->send_cmd_with_num("O,SG,", 1, 0);
            delay(350);
            break;
    }
}

void MenuSystem::EC_root(int update_level){
    Current = &Sensor_page;
};

void MenuSystem::Temp_value(int update_level){
    switch(update_level){
        case PASSIVE_update:
        case CHOSEN_update:{
            if(strcmp(&(Current->text[0][16]), " ON")==0){
                temp_sensor->receive_read_cmd();
                switch(temp_sensor->get_error()){
                    case Ezo_board::NO_DATA:
                        temp_sensor->send_read_cmd();
                        break;
                    case Ezo_board::NOT_READ_CMD:
                        break;
                    case Ezo_board::NOT_READY:
                        break;
                    case Ezo_board::FAIL:
                        sprintf(&(Current->title[0]), "Temp Sensor  FAIL");
                        break;
                    case Ezo_board::SUCCESS:
                        sprintf(&(Current->title[0]), "Temp   %.3f%c%c", temp_sensor->get_last_received_reading(), (char)176, Temp_page.text[2][18]);
                        break;
                }
            };
            if(strcmp(&(Current->text[0][16]), "OFF")==0){
                sprintf(&(Current->title[0]), "Temp Sensor");
            }
            break;
        }
        case ACTIVATE_update:{
            if(strcmp(&(Current->text[0][16]), "OFF")==0){
                sprintf(&(Current->text[0][16]), " ON");
            } else if (strcmp(&(Current->text[0][16]), " ON")==0){
                sprintf(&(Current->text[0][16]), "OFF");
            };
            break;
        }
    } 
};

void MenuSystem::Temp_toggle_LED(int update_level){
    switch(update_level){
        case PASSIVE_update:{
            break;
        }
        case CHOSEN_update:{
            break;
        }
        case ACTIVATE_update:{
            delay(1000);
            temp_sensor->send_cmd("L,?");
            delay(500);
            temp_sensor->receive_cmd(ezo_buffer, ezo_buffer_len);
            int status = (int)(ezo_buffer[3]) - 48;     //Force char to ascii code and minus it by ascii-48 (number 0).
            status ^= 1;
            if(status == 0){
                sprintf(&(Current->text[1][0]), "LED             OFF");
            } else if(status == 1){
                sprintf(&(Current->text[1][0]), "LED              ON");
            }
            temp_sensor->send_cmd_with_num("L,", status, 0);
            break;
        }
    }
};

void MenuSystem::Temp_scale(int update_level){
    switch(update_level){
        case ACTIVATE_update:
            if(strcmp(&(Current->text[2][18]), "C") == 0){
                delay(600);
                temp_sensor->send_cmd("S,f");
                sprintf(&(Current->text[2][0]), "Scale            %cF", (char)176);
            } else if(strcmp(&(Current->text[2][18]), "F") == 0){
                delay(600);
                temp_sensor->send_cmd("S,k");
                sprintf(&(Current->text[2][0]), "Scale            %cK", (char)176);
            } else if(strcmp(&(Current->text[2][18]), "K") == 0){
                delay(600);
                temp_sensor->send_cmd("S,c");
                sprintf(&(Current->text[2][0]), "Scale            %cC", (char)176);
            }
    };
};

void MenuSystem::Temp_cal_num_int(int update_level){
    switch(update_level){
        case CHOSEN_update:
            sprintf(&(Current->text[3][0]), "Cal int:%11.i", (int)(knob_portion(knob_R, 50, KNOB_MAX)));
            break;
    }
};

void MenuSystem::Temp_cal_num_float(int update_level){
    switch(update_level){
        case CHOSEN_update:
            sprintf(&(Current->text[4][0]), "Cal float:%9.2f", (float)(knob_portion(knob_R, 99, KNOB_MAX))/100);
            break;
    }
};

void MenuSystem::Temp_cal_any(int update_level){
    switch(update_level){
        case PASSIVE_update:
            sprintf(&(Current->text[5][0]), "Cal any");
            break;
        case CHOSEN_update:
        {
            float cal_temp = atoi(&(Current->text[3][17])) + atof(&(Current->text[4][15]));
            sprintf(&(Current->text[5][0]), "Cal any%12.3f", cal_temp);
            break;
        }
        case ACTIVATE_update:
            delay(600);
            float cal_temp = atoi(&(Current->text[3][17])) + atof(&(Current->text[4][15]));
            temp_sensor->send_cmd_with_num("Cal,", cal_temp, 3);
            delay(600);
            temp_sensor->receive_cmd(ezo_buffer, ezo_buffer_len);
            switch(temp_sensor->get_error()){
                case Ezo_board::SUCCESS:
                    sprintf(&(Current->text[5][0]), "Cal any      SUCCESS");
                    break;
                case Ezo_board::FAIL:
                    sprintf(&(Current->text[5][0]), "Cal any         FAIL");
                    break;
                default:
                    sprintf(&(Current->text[5][0]), "Cal any    TRY AGAIN");
            };
            break;
    }
};

void MenuSystem::Temp_cal_clear(int update_level){
    switch(update_level){
        case PASSIVE_update:
            sprintf(&(Current->text[6][0]), "Cal !!!RESET!!!");
            break;
        case CHOSEN_update:
            break;
        case ACTIVATE_update:
            delay(600);
            temp_sensor->send_cmd("Cal,clear");
    }
};

void MenuSystem::Temp_Vcc(int update_level){
    switch(update_level){
        case ACTIVATE_update:
            delay(1000);
            temp_sensor->send_cmd("Status");
            delay(500);
            temp_sensor->receive_cmd(ezo_buffer, ezo_buffer_len);
            strtok(ezo_buffer, ",");
            sprintf(&(Current->text[8][0]), "Restart code:%6.c", *strtok(NULL, ","));
            sprintf(&(Current->text[7][0]), "Status   Vcc:%6.2f", atof(strtok(NULL, ",")));
            break;
    }
};

void MenuSystem::Temp_status_restart_code(int update_level){
    Temp_Vcc(update_level);
};

void MenuSystem::Temp_Temp2(int update_level){
    switch(update_level){
    case PASSIVE_update:
        sprintf(&(Current->text[9][0]), " %c%c%cNext Page%c%c%c   ", (char)149, (char)149, (char)149, (char)149, (char)149, (char)149);
        break;
    case CHOSEN_update:
        sprintf(&(Current->text[9][0]), " %c%c%cNext Page%c%c%c  <", (char)149, (char)149, (char)149, (char)149, (char)149, (char)149);
        break;
    case ACTIVATE_update:
        char* title_status = &(Current->title[0]);
        Current = &Temp2_page;
        sprintf(&(Current->title[0]), title_status);
        break;
    };
};

void MenuSystem::Temp2_Temp(int update_level){
    switch(update_level){
        case PASSIVE_update:
            sprintf(&(Current->text[0][0]), " %c%c%cLast Page%c%c%c   ", (char)149, (char)149, (char)149, (char)149, (char)149, (char)149);
            break;
        case CHOSEN_update:
            sprintf(&(Current->text[0][0]), " %c%c%cLast Page%c%c%c  <", (char)149, (char)149, (char)149, (char)149, (char)149, (char)149);
            break;
        case ACTIVATE_update:
            Current = &Temp_page;
            break;
    }

    /*******************************************************************
     * Function Below is to maintain sensor reading if it's on in first page.
     * Code below shouldn't be here. But we have no choice.
     * It's here because we don't need a line of text on menu for it.
    ********************************************************************/
    if(strlen(&(Current->title[0])) > 11){
        temp_sensor->receive_read_cmd();
        switch(temp_sensor->get_error()){
            case Ezo_board::NO_DATA:
                temp_sensor->send_read_cmd();
                break;
            case Ezo_board::NOT_READ_CMD:
                break;
            case Ezo_board::NOT_READY:
                break;
            case Ezo_board::FAIL:
                sprintf(&(Current->title[0]), "Temp Sensor  FAIL");
                break;
            case Ezo_board::SUCCESS:
                sprintf(&(Current->title[0]), "Temp   %.3f%c%c", temp_sensor->get_last_received_reading(), (char)176, Temp_page.text[2][18]);
                break;
        }
    }
};

void MenuSystem::Temp_import(int update_level){
    switch(update_level){
        case ACTIVATE_update:
            //Wait 1 second till everything stop, like ezoboard reading.
            import_sd_to_json();
            Serial.print("Sensor name of file: ");
            const char* name = in_export_buffer["name"];
            Serial.println(name);
            if(!strcmp(&name[0], "Temp")==0){
                Serial.println("This file is not for Temperature sensor");
                break;
            };

            Serial.println("Importing data............");

            //Get LED state
            bool json_led = in_export_buffer["setting"]["LED"];
            temp_sensor->send_cmd_with_num("L,", (int)json_led, 0);
            sprintf(&(Temp_page.text[1][16]),"%s", (json_led)?" ON":"OFF");
            Serial.print("LED state is: ");
            Serial.println(json_led);
            delay(400);

            //Get calibration strings.
            for(JsonVariant cal_string : in_export_buffer["calibration"].as<JsonArray>()){
                Serial.print("Importing calibration string: ");
                Serial.println(cal_string.as<char*>());
                char cal_command[20];
                sprintf(&cal_command[0], "Import,%s", cal_string.as<char*>());
                temp_sensor->send_cmd(cal_command);
                delay(400);
            };
            Serial.println("Calibration import ends.");

            //Make export buffer clear
            in_export_buffer.clear();
            in_export_buffer["name"] = "Temp";            //Set the name of setting file.
            temp_sensor->send_cmd("L,?");
            delay(500);
            temp_sensor->receive_cmd(ezo_buffer, ezo_buffer_len);
            strtok(ezo_buffer, ",");
            in_export_buffer["setting"]["LED"] = (bool)(strtok(NULL, ","));                           //Set LED on or off.

            //Get calibration information.
            temp_sensor->send_cmd("Export,?");
            delay(350);
            temp_sensor->receive_cmd(ezo_buffer, ezo_buffer_len);
            strtok(ezo_buffer, ",");
            int num_char = atoi(strtok(NULL, ","));
            for(int i=0; i<num_char+1; i++){
                temp_sensor->send_cmd("Export");
                delay(350);
                temp_sensor->receive_cmd(ezo_buffer, ezo_buffer_len);
                in_export_buffer["calibration"][i] = ezo_buffer;
            };

            //Set file title based on current time.
            sprintf(file_path_buffer, "Temperature/Temp_current.json");
            export_json_to_sd(true);
            
            break;
    };
};

void MenuSystem::Temp_export(int update_level){
    switch(update_level){
        case ACTIVATE_update:
            //Wait 1 second till everything stop, like ezoboard reading.
            delay(1000);

            //Make export buffer clear
            in_export_buffer.clear();
            in_export_buffer["name"] = "Temp";            //Set the name of setting file.
            temp_sensor->send_cmd("L,?");
            delay(500);
            temp_sensor->receive_cmd(ezo_buffer, ezo_buffer_len);
            strtok(ezo_buffer, ",");
            in_export_buffer["setting"]["LED"] = (bool)(strtok(NULL, ","));                           //Set LED on or off.

            //Get calibration information.
            temp_sensor->send_cmd("Export,?");
            delay(350);
            temp_sensor->receive_cmd(ezo_buffer, ezo_buffer_len);
            Serial.println(ezo_buffer);
            strtok(ezo_buffer, ",");
            int num_char = atoi(strtok(NULL, ","));
            for(int i=0; i<num_char+1; i++){
                temp_sensor->send_cmd("Export");
                delay(350);
                temp_sensor->receive_cmd(ezo_buffer, ezo_buffer_len);
                in_export_buffer["calibration"][i] = ezo_buffer;
            };

            //Set file title based on current time.
            DateTime NOW = clock->now();
            sprintf(file_path_buffer, "Temperature/%.4d%.2d%.2d%.2d%.2d%.2d.json", NOW.year(), NOW.month(), NOW.day(),
                                                                    NOW.hour(), NOW.minute(), NOW.second());

            export_json_to_sd(true);
            break;
    }
};

void MenuSystem::Temp_WB_default_setting(int update_level){
    switch(update_level){
        case ACTIVATE_update:
            break;
    };
};

void MenuSystem::Temp_factory_setting(int update_level){
    switch(update_level){
        case ACTIVATE_update:
            break;
    };
};

void MenuSystem::Temp_root(int update_level){
    Current = &Sensor_page;
};

void MenuSystem::Turbidity_value(int update_level){
    switch(update_level){
        case PASSIVE_update:
        case CHOSEN_update:{
            if(strcmp(&(Current->text[0][16]), " ON")==0){
                sprintf(&(Current->title[0]), "Turb%8.2fNTU", turb_sensor->get_value((float)sen_L/4));
                Serial.print("Turb :");
                Serial.println(turb_sensor->get_value((float)sen_L/4));
            };
            if(strcmp(&(Current->text[0][16]), "OFF")==0){
                sprintf(&(Current->title[0]), "Turbidity Sensor");
            }
            break;
        }
        case ACTIVATE_update:{
            if(strcmp(&(Current->text[0][16]), "OFF")==0){
                sprintf(&(Current->text[0][16]), " ON");
            } else if (strcmp(&(Current->text[0][16]), " ON")==0){
                sprintf(&(Current->text[0][16]), "OFF");
            };
            break;
        }
    }
    /*
    /Serial.print("Real Value: ");
    /Serial.println(turb_sensor->get_value());
    /Serial.print("Magni_turb: ");
    /Serial.println(turb_sensor->slope_magnificant());
    /Serial.print("Offset: ");
    /Serial.println(turb_sensor->slope_offset());
    */
};

void MenuSystem::Turbidity_Cal_num_rough(int update_level){
    switch(update_level){
        case PASSIVE_update:
            break;
        case CHOSEN_update:
            sprintf(&(Current->text[1][0]), "Cal num x100:%6.i", (int)(knob_portion(knob_R, 49, KNOB_MAX)));
            break;
        case ACTIVATE_update:
            break;
    }
};

void MenuSystem::Turbidity_Cal_num_fine(int update_level){
    switch(update_level){
        case PASSIVE_update:
            break;
        case CHOSEN_update:
            sprintf(&(Current->text[2][0]), "Cal num x1:%8.i", (int)(knob_portion(knob_R, 99, KNOB_MAX)));
            break;
        case ACTIVATE_update:
            break;
    }
};

void MenuSystem::Turbidity_Cal_num_float(int update_level){
    switch(update_level){
        case PASSIVE_update:
            break;
        case CHOSEN_update:
            sprintf(&(Current->text[3][0]), "Cal num float:%5.2f", (float)(knob_portion(knob_R, 99, KNOB_MAX))/100);
            break;
        case ACTIVATE_update:
            break;
    }
};

void MenuSystem::Turbidity_Cal_low(int update_level){
    switch(update_level){
        case PASSIVE_update:
            sprintf(&(Current->text[4][0]), "Cal low");
            break;
        case CHOSEN_update:
        {
            float cal_turb_ntu = atoi(&(Current->text[1][17]))*100 + atoi(&(Current->text[2][17])) + atof(&(Current->text[3][15]));
            sprintf(&(Current->text[4][0]), "Cal low%9.2fNTU", cal_turb_ntu);
            break;
        }
        case ACTIVATE_update:
            delay(10);
            read_adc(ads1015, &knob_L, &knob_R, &sen_L, &sen_R);
            float cal_turb_ntu = atoi(&(Current->text[1][17]))*100 + atoi(&(Current->text[2][17])) + atof(&(Current->text[3][15]));
            turb_sensor->calibrate_low(cal_turb_ntu, (float)sen_L/4);           
            break;
    }
};

void MenuSystem::Turbidity_Cal_high(int update_level){
    switch(update_level){
        case PASSIVE_update:
            sprintf(&(Current->text[5][0]), "Cal high");
            break;
        case CHOSEN_update:
        {
            float cal_turb_ntu = atoi(&(Current->text[1][17]))*100 + atoi(&(Current->text[2][17])) + atof(&(Current->text[3][15]));
            sprintf(&(Current->text[5][0]), "Cal high%8.2fNTU", cal_turb_ntu);
            break;
        }
        case ACTIVATE_update:
            delay(10);
            read_adc(ads1015, &knob_L, &knob_R, &sen_L, &sen_R);
            float cal_turb_ntu = atoi(&(Current->text[1][17]))*100 + atoi(&(Current->text[2][17])) + atof(&(Current->text[3][15]));
            turb_sensor->calibrate_high(cal_turb_ntu, (float)sen_L/4);           
            break;
    }
};

void MenuSystem::Turbidity_Cal_status(int update_level){
    sprintf(&(Current->text[6][0]), "Cal status%9.i", turb_sensor->calibrate_status());
};

void MenuSystem::Turbidity_import(int update_level){
     switch(update_level){
        case ACTIVATE_update:
            //Wait 1 second till everything stop, like ezoboard reading.
            import_sd_to_json();
            Serial.print("Sensor name of file: ");
            const char* name = in_export_buffer["name"];
            Serial.println(name);
            if(!strcmp(&name[0], "Turb")==0){
                Serial.println("This file is not for Turbidity sensor");
                break;
            };

            Serial.println("Importing data............");

            float json_coef = in_export_buffer["coef"];
            Serial.print("Import coef: ");
            Serial.println(json_coef);
            float json_offset = in_export_buffer["offset"];
            turb_sensor->import_calibration(json_coef, json_offset);
            Serial.print("Import offset: ");
            Serial.println(json_offset);

            //Make export buffer clear
            in_export_buffer.clear();
            in_export_buffer["name"] = "Turb";            //Set the name of setting file.
            in_export_buffer["coef"] = turb_sensor->slope_coef();
            Serial.print("Slope of volt-NTU is");
            Serial.println(turb_sensor->slope_coef());
            in_export_buffer["offset"] = turb_sensor->slope_offset();
            Serial.print("Offset of volt-NTU is");
            Serial.println(turb_sensor->slope_offset());

            //Set file title based on current time.
            DateTime NOW = clock->now();
            sprintf(file_path_buffer, "Turbidity/turb_current.json");;

            export_json_to_sd(true);
            break;
    };
};

void MenuSystem::Turbidity_Turbidity2(int update_level){
    switch(update_level){
    case PASSIVE_update:
        sprintf(&(Current->text[9][0]), " %c%c%cNext Page%c%c%c   ", (char)149, (char)149, (char)149, (char)149, (char)149, (char)149);
        break;
    case CHOSEN_update:
        sprintf(&(Current->text[9][0]), " %c%c%cNext Page%c%c%c  <", (char)149, (char)149, (char)149, (char)149, (char)149, (char)149);
        break;
    case ACTIVATE_update:
        char* title_status = &(Current->title[0]);
        Current = &Turbidity2_page;
        sprintf(&(Current->title[0]), title_status);
        break;
    };
};

void MenuSystem::Turbidity2_Turbidity(int update_level){
    switch(update_level){
        case PASSIVE_update:
            sprintf(&(Current->text[0][0]), " %c%c%cLast Page%c%c%c   ", (char)149, (char)149, (char)149, (char)149, (char)149, (char)149);
            break;
        case CHOSEN_update:
            sprintf(&(Current->text[0][0]), " %c%c%cLast Page%c%c%c  <", (char)149, (char)149, (char)149, (char)149, (char)149, (char)149);
            break;
        case ACTIVATE_update:
            Current = &Turbidity_page;
            break;
    ;}

    /*******************************************************************
     * Function Below is to maintain sensor reading if it's on in first page.
     * Code below shouldn't be here. But we have no choice.
     * It's here because we don't need a line of text on menu for it.
    ********************************************************************/
    if(strlen(&(Current->title[0])) > 17){
        sprintf(&(Current->title[0]), "Turb%8.2fNTU", abs(turb_sensor->get_value((float)sen_L/4)));
    }
};

void MenuSystem::Turbidity_export(int update_level){
    switch(update_level){
        case ACTIVATE_update:
            //Wait 1 second till everything stop, like ezoboard reading.
            delay(1000);

            //Make export buffer clear
            in_export_buffer.clear();
            in_export_buffer["name"] = "Turb";            //Set the name of setting file.
            in_export_buffer["coef"] = turb_sensor->slope_coef();
            Serial.print("Slope of volt-NTU is");
            Serial.println(turb_sensor->slope_coef());
            in_export_buffer["offset"] = turb_sensor->slope_offset();
            Serial.print("Offset of volt-NTU is");
            Serial.println(turb_sensor->slope_offset());

            //Set file title based on current time.
            DateTime NOW = clock->now();
            sprintf(file_path_buffer, "Turbidity/%.4d%.2d%.2d%.2d%.2d%.2d.json", NOW.year(), NOW.month(), NOW.day(),
                                                                    NOW.hour(), NOW.minute(), NOW.second());

            export_json_to_sd(true);
            break;
    }
};

void MenuSystem::Turbidity_WB_default_setting(int update_level){

};

void MenuSystem::Turbidity_root(int update_level){
    Current = &Sensor_page;
};

void MenuSystem::Nitrate_value(int update_level){};
void MenuSystem::Nitrate_root(int update_level){
    Current = &Sensor_page;
};

void MenuSystem::Ammonium_value(int update_level){};
void MenuSystem::Ammonium_root(int update_level){
    Current = &Sensor_page;
};

void MenuSystem::SDcard_allfile(int update_level){
    switch(update_level){
        case ACTIVATE_update:
            SPI_select(SPI_SD);
            SDcard->ls(LS_R);
            SPI_select(SPI_OLED);
            break;
    };
};

void MenuSystem::SDcard_choose_folder(int update_level){
    switch(update_level){
        case PASSIVE_update:
            break;
        case CHOSEN_update:
            char sensor_folder[15];
            if(knob_portion(knob_R, 3, KNOB_MAX) == 0){
                sprintf(&sensor_folder[0], "pH");
            } else if(knob_portion(knob_R, 3, KNOB_MAX) == 1){
                sprintf(&sensor_folder[0], "EC");
            } else if(knob_portion(knob_R, 3, KNOB_MAX) == 2){
                sprintf(&sensor_folder[0], "Temperature");
            } else if(knob_portion(knob_R, 3, KNOB_MAX) == 3){
                sprintf(&sensor_folder[0], "Turbidity");
            };
            sprintf(&(Current->text[0][0]), "Folder:%12s", sensor_folder);
            break;
        case ACTIVATE_update:
            break;
    }
};

void MenuSystem::SDcard_choose_page(int update_level){
    switch(update_level){
        case PASSIVE_update:
            break;
        case CHOSEN_update:
            sprintf(&(Current->text[1][0]), "Page:%14.i", (int)(knob_portion(knob_R, 9, KNOB_MAX) + 1));
            break;
        case ACTIVATE_update:
            //Clear menu text
            for(int i = 2; i < 10; i++){
                sprintf(&(Current->text[i][0]), "");
            }

            for(int i = 0; i < strlen(&(Current->text[0][7])); i++){
                if(strncmp(&(Current->text[0][i+7]), " ", 1)){
                    sprintf(&file_path_buffer[0], "/%s/", &(Current->text[0][i+7]));
                    break;
                };
            };

            //Activate sdcard.
            SPI_select(SPI_SD);
            if(!dir.open(file_path_buffer)){
                Serial.println("Open foler failed.");
                break;
            };

            for(int i = 0; i < (atoi(&(Current->text[1][18]))-1)*8; i++){
                CurrentFile.openNext(&dir, O_RDONLY);
            } ;
            for(int i = 0; i < 8; i++){
                if(!CurrentFile.openNext(&dir, O_RDONLY)){
                    break;
                };
                CurrentFile.getName(file_path_buffer, size_t(file_path_buffer));
                sprintf(&(Current->text[i+2][0]), &file_path_buffer[0]);
            };  

            SPI_select(SPI_OLED);        
            break;
    }
};

void MenuSystem::SDcard_file1(int update_level){
    switch(update_level){
        case ACTIVATE_update:
            for(int i = 0; i < strlen(&(Current->text[0][7])); i++){
                if(strncmp(&(Current->text[0][i+7]), " ", 1)){
                    sprintf(&file_path_buffer[0], "/%s/%s", &(Current->text[0][i+7]), &(Current->text[2][0]));
                    break;
                };
            };
            Serial.print("File selected: ");
            Serial.println(file_path_buffer);
            break;
    };
};

void MenuSystem::SDcard_file2(int update_level){
    switch(update_level){
        case ACTIVATE_update:
            for(int i = 0; i < strlen(&(Current->text[0][7])); i++){
                if(strncmp(&(Current->text[0][i+7]), " ", 1)){
                    sprintf(&file_path_buffer[0], "/%s/%s", &(Current->text[0][i+7]), &(Current->text[3][0]));
                    break;
                };
            };
            Serial.print("File selected: ");
            Serial.println(file_path_buffer);
            break;
    };
};

void MenuSystem::SDcard_file3(int update_level){
    switch(update_level){
        case ACTIVATE_update:
            for(int i = 0; i < strlen(&(Current->text[0][7])); i++){
                if(strncmp(&(Current->text[0][i+7]), " ", 1)){
                    sprintf(&file_path_buffer[0], "/%s/%s", &(Current->text[0][i+7]), &(Current->text[4][0]));
                    break;
                };
            };
            Serial.print("File selected: ");
            Serial.println(file_path_buffer);
            break;
    };
};

void MenuSystem::SDcard_file4(int update_level){
    switch(update_level){
        case ACTIVATE_update:
            for(int i = 0; i < strlen(&(Current->text[0][7])); i++){
                if(strncmp(&(Current->text[0][i+7]), " ", 1)){
                    sprintf(&file_path_buffer[0], "/%s/%s", &(Current->text[0][i+7]), &(Current->text[5][0]));
                    break;
                };
            };
            Serial.print("File selected: ");
            Serial.println(file_path_buffer);
            break;
    };
};

void MenuSystem::SDcard_file5(int update_level){
    switch(update_level){
        case ACTIVATE_update:
            for(int i = 0; i < strlen(&(Current->text[0][7])); i++){
                if(strncmp(&(Current->text[0][i+7]), " ", 1)){
                    sprintf(&file_path_buffer[0], "/%s/%s", &(Current->text[0][i+7]), &(Current->text[6][0]));
                    break;
                };
            };
            Serial.print("File selected: ");
            Serial.println(file_path_buffer);
            break;
    };
};

void MenuSystem::SDcard_file6(int update_level){
    switch(update_level){
        case ACTIVATE_update:
            for(int i = 0; i < strlen(&(Current->text[0][7])); i++){
                if(strncmp(&(Current->text[0][i+7]), " ", 1)){
                    sprintf(&file_path_buffer[0], "/%s/%s", &(Current->text[0][i+7]), &(Current->text[7][0]));
                    break;
                };
            };
            Serial.print("File selected: ");
            Serial.println(file_path_buffer);
            break;
    };
};

void MenuSystem::SDcard_file7(int update_level){
    switch(update_level){
        case ACTIVATE_update:
            for(int i = 0; i < strlen(&(Current->text[0][7])); i++){
                if(strncmp(&(Current->text[0][i+7]), " ", 1)){
                    sprintf(&file_path_buffer[0], "/%s/%s", &(Current->text[0][i+7]), &(Current->text[8][0]));
                    break;
                };
            };
            Serial.print("File selected: ");
            Serial.println(file_path_buffer);
            break;
    };
};

void MenuSystem::SDcard_file8(int update_level){
    switch(update_level){
        case ACTIVATE_update:
            for(int i = 0; i < strlen(&(Current->text[0][7])); i++){
                if(strncmp(&(Current->text[0][i+7]), " ", 1)){
                    sprintf(&file_path_buffer[0], "/%s/%s", &(Current->text[0][i+7]), &(Current->text[9][0]));
                    break;
                };
            };
            Serial.print("File selected: ");
            Serial.println(file_path_buffer);
            break;
    };
};

void MenuSystem::SDcard_root(int update_level){
    Current = &Main_page;
};

void MenuSystem::Info_version(int update_level){

};

void MenuSystem::Info_institution(int update_level){

};

void MenuSystem::Info_battery(int update_level){
    switch(update_level){
        case PASSIVE_update:
        case CHOSEN_update:
        {
            sprintf(&(Current->text[2][0]), "Battery:%9.2f V", ina219->getBusVoltage_V());
            break;
        }
        case ACTIVATE_update:
            Serial.print("Voltage of battery: ");
            Serial.println(ina219->getBusVoltage_V());
            Serial.print("Current of battery: ");
            Serial.println(ina219->getCurrent_mA());
            break;
    };
};

void MenuSystem::Info_root(int update_level){
    Current = &Main_page;
};

void MenuSystem::update_button_knob(){
    read_adc(ads1015, &knob_L, &knob_R, &sen_L, &sen_R);
    float button_time = pull_time(10);
    if(button_time > PRESS_TIME_1 && button_time < PRESS_TIME_2){
        button = 1;     //Comfirm option
    } else if(button_time > PRESS_TIME_2){
        button = 2;     //Back to rootpage
    } else {
        button = 0;     //Nothing
    };
};

void MenuSystem::update_page(){
    int index_L = (int)(1660-knob_L)/(1660/(Current->func_num));
    if(index_L == Current->func_num){
        index_L -= 1;   //When knob_L = 0, index will just reach func_num. But it shouldn't reach a new integer. So we minus it by one.
    };
    //Slice the voltage measured from two knobs (1657~0) into N section. Depends on how many functions are there in a page.
    //This index will be used in determine which page functino are chosen.
    if(button==2){
        (this->*(Current->Rootfunc))(0);
        menu_window_index = 0;
        return;
    };
    if(Current->func_num == 0){
        return;
    };
    for(int i = 0; i < Current->func_num; i++){
        if(i==index_L){
            (this->*(Current->options[i]))(CHOSEN_update);
            if(button==1){
                (this->*(Current->options[i]))(ACTIVATE_update);
                index_L = (int)(1660-knob_L)/(1660/(Current->func_num));
                if(index_L > menu_window_index + 3){
                    menu_window_index = 0;
                };
                if(index_L < menu_window_index){
                    menu_window_index = 0;
                };
                break;
            }
        } else {
            (this->*(Current->options[i]))(PASSIVE_update);
        };
    }
};

void MenuSystem::update_oled(){
    int index_L = (int)(1660-knob_L)/(1660/(Current->func_num));
    if(index_L == Current->func_num){
        index_L -= 1;   //When knob_L = 0, index will just reach func_num. But it shouldn't reach a new integer. So we minus it by one.
    };
    int index_R = (int)(1660-knob_R);
    waterbox_oled->firstPage();
    do{
        //Draw title
        int title_x_position = (int)(64-strlen(Current->title)*6/2);
        waterbox_oled->drawStr(title_x_position, 8, Current->title);
        /*
        for(int i = index_L; i < index_L + 4; i++){
            if(i >= Current->func_num){
                break;
            } else {
                if(i == index_L){
                    waterbox_oled->drawStr(0, 24, ">");
                    waterbox_oled->drawStr(12, 24,Current->text[i]);
                } else{
                    waterbox_oled->drawStr(12, (i-index_L+1)*12 + 12,Current->text[i]);
                }
            };
        };*/
        if(index_L > menu_window_index + 3){
            menu_window_index = index_L - 3;
        };
        if(index_L < menu_window_index){
            menu_window_index = index_L;
        };
        for(int i = menu_window_index; i < menu_window_index + 4; i++){
            if(i == index_L){
                waterbox_oled->drawStr(0, (i-menu_window_index+2)*12, ">");
                waterbox_oled->drawStr(12, (i-menu_window_index+2)*12,Current->text[i]);
            } else {
                waterbox_oled->drawStr(12, (i-menu_window_index+2)*12, Current->text[i]);
            };
        };

    }while(waterbox_oled->nextPage());
};

void MenuSystem::runMenu(){

    update_button_knob();
    update_page();
    update_oled();
};

int MenuSystem::SPI_select(SPI_device device){
    switch(device){
        case SPI_LORA:
            break;
        case SPI_SD:
            digitalWrite(LORA_CS, HIGH);
            digitalWrite(SD_CS, LOW);
            digitalWrite(OLED_CS, HIGH);
            SDcard->begin(SD_CHIP_SELECT, SPI_HALF_SPEED);
            break;
        case SPI_OLED:
            digitalWrite(LORA_CS, HIGH);
            digitalWrite(SD_CS, HIGH);
            digitalWrite(OLED_CS, LOW);
            waterbox_oled->begin();
            break;
    }
    return 0;
};

void MenuSystem::export_json_to_sd(bool serialize){
    /*
        This function will export json object in in_export_buffer to SD card.
        And the file name is based on file_path_buffer.
        Make sure you set both of them properly before calling this function
    */

    //Seriallize the json to char array
    if(serialize = true){
        serializeJson(in_export_buffer, json_char_buffer);
        Serial.println(json_char_buffer);                    //Print serialized json file to Serial.
    }
    

    //Activate sdcard.
    SPI_select(SPI_SD);
    
    //Clear file.
    if(!SDcard->remove(file_path_buffer)){
        Serial.print("SD file remove fail for file: ");
        Serial.println(file_path_buffer);
    };

    //Open(Create) the file and write data in. 
    CurrentFile.open(file_path_buffer, FILE_WRITE);
    if(CurrentFile){
        Serial.print("Export pH setting to file: ");
        Serial.println(file_path_buffer);
        CurrentFile.println(json_char_buffer);
        CurrentFile.close();
    } else {
        Serial.print("Cannot open file ");
        Serial.println(file_path_buffer);
    };

    sprintf(&file_path_buffer[0], "");

    //Activate oled
    SPI_select(SPI_OLED);
};

void MenuSystem::import_sd_to_json(){
    /*
        This function will import file. And the path is based on file_path_buffer.
        Json object will be stored in in_export_buffer.
        Please set file_path_buffer to correct value before calling this function.
    */

    //Activate SD card.
    SPI_select(SPI_SD);

    in_export_buffer.clear();
    //Open file and read it to json_char_buffer.
    CurrentFile.open(file_path_buffer, FILE_READ);
    if(CurrentFile){
        CurrentFile.read(json_char_buffer, 1024);
        Serial.print("File ");
        Serial.print(file_path_buffer);
        Serial.println("successfully opened");
        Serial.print("Content is: ");
        Serial.println(json_char_buffer);
        CurrentFile.close();
    } else {
        Serial.print("Cannot open file ");
        Serial.println(file_path_buffer);
    };

    DeserializationError err = deserializeJson(in_export_buffer, json_char_buffer);
    if(err){
        Serial.print("Json deserializationn Error: ");
        Serial.println(err.c_str());
    }
    SPI_select(SPI_OLED);
};

String MenuSystem::pmu_date(){
    char command_date[14];
    sprintf(command_date, "F1,%.4d-%.2d-%.2d", clock->now().year(), clock->now().month(), clock->now().day());
    return String(command_date);
};

String MenuSystem::pmu_time(){
    char command_time[12];
    sprintf(command_time, "F2,%.2d:%.2d:%.2d", clock->now().hour(), clock->now().minute(), clock->now().second());
    return String(command_time);
};

String MenuSystem::pmu_lon(){
    char command_longitude[15];
    sprintf(command_longitude, "F4,%3.6f", longitude);
    return String(command_longitude);
};

String MenuSystem::pmu_lat(){
    char command_latitude[14];
    sprintf(command_latitude, "F5,%2.6f", latitude);
    return String(command_latitude);
};

String MenuSystem::pmu_ec(){
    char command_ec[13];
    Serial.println(temp_cache);
    if(temp_cache == NULL){
        temp_sensor->send_cmd("S,c");
        delay(300);
        temp_sensor->send_read_cmd();
        delay(900);
        temp_sensor->receive_read_cmd();
        temp_cache = temp_sensor->get_last_received_reading();
    };
    float now_ec;
    ec_sensor->send_read_with_temp_comp(temp_cache);
    delay(900);
    ec_sensor->receive_read_cmd();
    now_ec = ec_sensor->get_last_received_reading();
    sprintf(command_ec, "F6,%.2f", now_ec);
    delay(100);
    return String(command_ec);
};

String MenuSystem::pmu_ph(){
    char command_ph[9];
    Serial.println(temp_cache);
    if(temp_cache == NULL){
        temp_sensor->send_cmd("S,c");
        delay(300);
        temp_sensor->send_read_cmd();
        delay(900);
        temp_sensor->receive_read_cmd();
        temp_cache = temp_sensor->get_last_received_reading();
    };
    float now_ph;
    pH_sensor->send_read_with_temp_comp(temp_cache);
    delay(900);
    pH_sensor->receive_read_cmd();
    now_ph = pH_sensor->get_last_received_reading();
    sprintf(command_ph, "F7,%.2f", now_ph);
    delay(100);
    return String(command_ph);
};

String MenuSystem::pmu_temp(){
    char command_temp[10];
    if(temp_cache == NULL){
        temp_sensor->send_cmd("S,c");
        delay(300);
        temp_sensor->send_read_cmd();
        delay(900);
        temp_sensor->receive_read_cmd();
        temp_cache = temp_sensor->get_last_received_reading();
    };
    Serial.println(temp_cache);
    sprintf(command_temp, "F8,%.2f", temp_cache);
    delay(100);
    return String(command_temp);
};

String MenuSystem::pmu_turb(){
    char command_turb[12];
    update_button_knob();
    sprintf(command_turb, "F9,%.2f", turb_sensor->get_value((float)sen_L/4));
    return String(command_turb);
};

String MenuSystem::pmu_volt(){
    char command_volt[9];
    sprintf(command_volt, "F10,%.2f", ina219->getBusVoltage_V());
    delay(50);
    return String(command_volt);
};

String MenuSystem::pmu_current(){
    char command_mA[12];
    sprintf(command_mA, "F11,%.2f", ina219->getCurrent_mA());
    delay(50);
    return String(command_mA);
};

String MenuSystem::pmu_APN(){
    return String(APN);
}

String MenuSystem::pmu_sleeptime(){
    char sleep_command[9];
    int today_second = DateTime(clock->now().year(), clock->now().month(), clock->now().day()).secondstime();
    int now_second = clock->now().secondstime();
    Serial.println(today_second);
    Serial.println(now_second);
    Serial.println(interval);
    Serial.println(interval - (now_second - today_second)%interval);

    sprintf(sleep_command, "TIME,%i", interval - (now_second - today_second)%interval);
    return String(sleep_command);
};
