#ifndef DFROBOT_TURBIDITY
#define DFROBOT_TURBIDITY

#include <Arduino.h>

struct turb_cal{
    float voltage;
    float NTU;
};

class Dfrobot_Turbidity{
    public:
        Dfrobot_Turbidity();

        float get_value(float voltage);
        void calibrate_low(float NTU, float voltage);
        void calibrate_high(float NTU, float voltage);
        int calibrate_status();
        void calibrate_reset();
        float slope_coef();
        float slope_offset();
        void import_calibration(float, float);

    protected:
        void cal_update();

        //Turbidity = coef * Turbidity + offset
        float coef;
        float offset;

        turb_cal calibrated_low_point;
        turb_cal calibrated_high_point;
};

#endif