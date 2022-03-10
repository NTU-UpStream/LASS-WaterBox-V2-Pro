#include <dfrobot_turbidity.h>

Dfrobot_Turbidity::Dfrobot_Turbidity(){
    this->coef = 1;
    this->offset = 0;

    this->calibrated_low_point = {0, 0};
    this->calibrated_high_point = {0, 0};
};

float Dfrobot_Turbidity::get_value(float voltage){
    float ntu = coef * voltage + offset;
    return ntu;
};

void Dfrobot_Turbidity::calibrate_low(float NTU, float voltage){
    calibrated_low_point = {voltage, NTU};
    cal_update();
    Serial.print("Low voltage:");
    Serial.println(calibrated_low_point.voltage);
    Serial.print("Low NTU:");
    Serial.println(calibrated_low_point.NTU);
    Serial.print("High voltege:");
    Serial.println(calibrated_high_point.voltage);
    Serial.print("High NTU:");
    Serial.println(calibrated_high_point.NTU);
    return;
};

void Dfrobot_Turbidity::calibrate_high(float NTU, float voltage){
    calibrated_high_point = {voltage, NTU};
    cal_update();
    Serial.print("Low voltage:");
    Serial.println(calibrated_low_point.voltage);
    Serial.print("Low NTU:");
    Serial.println(calibrated_low_point.NTU);
    Serial.print("High voltege:");
    Serial.println(calibrated_high_point.voltage);
    Serial.print("High NTU:");
    Serial.println(calibrated_high_point.NTU);
    return;
};

void Dfrobot_Turbidity::cal_update(){
    float low_V = calibrated_low_point.voltage;
    float low_NTU = calibrated_low_point.NTU;
    float high_V = calibrated_high_point.voltage;
    float high_NTU = calibrated_high_point.NTU;

    coef = (low_NTU - high_NTU)/(low_V - high_V);
    offset = (low_V*high_NTU - high_V*low_NTU)/
                (low_V - high_V);
    Serial.print("low V:");
    Serial.println(low_V);
    Serial.print("low_NTU:");
    Serial.println(low_NTU);
    Serial.print("high_V:");
    Serial.println(high_V);
    Serial.print("high_NTU:");
    Serial.println(high_NTU);
    Serial.print("coef:");
    Serial.println(coef);
    Serial.print("offset:");
    Serial.println(offset);
};

int Dfrobot_Turbidity::calibrate_status(){
    int calibrated_num = 0;
    if( calibrated_low_point.voltage != 0){
        calibrated_num++;
    }
    if( calibrated_high_point.voltage != 0){
        calibrated_num++;
    }
    return calibrated_num;
};

float Dfrobot_Turbidity::slope_coef(){
    return this->coef;
};

float Dfrobot_Turbidity::slope_offset(){
    return this->offset;
};

void Dfrobot_Turbidity::import_calibration(float coef, float offset){
    this->coef = coef;
    this->offset = offset;
    return;
};