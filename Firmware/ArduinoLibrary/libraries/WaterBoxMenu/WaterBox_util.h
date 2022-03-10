
#ifndef WATERBOX_UTIL_H
#define WATERBOX_UTIL_H


#include <Arduino.h>
#include <DS3231M.h>
#include <Ezo_i2c.h>
#include <Ezo_i2c_util.h>
#include <Adafruit_ADS1015.h>

#define sensorSwitch 6
#define modulePower 14


#define LORA_CS 10
#define SD_CS 4
#define OLED_CS 15

#define knob_R_Ch 0 //Channel number of the knob on the right side when we call ads1115.readADC_SingleEnded(CH)
#define sen_L_Ch  2
#define sen_R_Ch  1
#define knob_L_Ch 3

#define volt_measure_delay_ms 20

struct EZO_RETURN
{
    float normal;
    float temp_compensated;
};

enum SPI_device {
    SPI_LORA,
    SPI_OLED,
    SPI_SD
};

void ezo_read(Ezo_board &sensor);
//Send read command by I2C to ezo sensors
//Read the sensor data in two way ( normal read and temperature compensate )
//Automatically print on lcd and Serial port

void ezo_clear_cal(Ezo_board &sensor);

void ezo_factory(Ezo_board &sensor);

void ezo_cal_H(Ezo_board &sensor);
void ezo_cal_H(Ezo_board &sensor, float correct);

void ezo_cal_M(Ezo_board &sensor);
void exo_cal_M(Ezo_board &sensor, float correct);

void ezo_cal_L(Ezo_board &sensor);
void ezo_cal_L(Ezo_board &sensor, float correct);

void ezo_cal_custom(Ezo_board &sensor, const char* command);

void ezo_print_status(Ezo_board &sensor);

float pull_time(int _limit);

void read_adc(Adafruit_ADS1015* volt_chip, int* knob_L, int* knob_R, int* sen_L, int* sen_R);
int knob_portion(int, int, int);
void set_clock(DS3231M_Class*, int, int, int, int, int, int);
bool checkdate(int y, int m, int d);

#endif