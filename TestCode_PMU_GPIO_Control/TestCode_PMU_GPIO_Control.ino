#include "LowPower.h"

#define LinkIt7697  2
#define SIMCOM7000E 3
#define INA219      4
#define LED 13

void initGPIO(void){
  pinMode(INA219, OUTPUT);
  pinMode(SIMCOM7000E, OUTPUT);
  pinMode(LinkIt7697, OUTPUT);
  pinMode(LED, OUTPUT);
}

void blink(uint8_t _times=3, float _rate=0.5){
  for(uint8_t _i=0;_i<_times;_i++){
    digitalWrite(LED, HIGH);
    delay(1000*_rate);
    digitalWrite(LED, LOW);
    delay(1000*(1-_rate));
  }
}


void setup() {
  Serial.begin(9600);
  Serial.println("序列埠初始化：9600");
  
  Serial.println("初始化控制腳位");
  initGPIO();

  digitalWrite(INA219, LOW);
  digitalWrite(SIMCOM7000E, LOW);
  digitalWrite(LinkIt7697, LOW);
  blink(2);
}

void loop() {
  Serial.println("啟動LinkIt7697：5 秒");
  digitalWrite(LinkIt7697, HIGH);
  delay(5000);
  digitalWrite(LinkIt7697, LOW);
  blink(1);
  
  Serial.println("啟動SIMCOM7000E：5 秒");
  digitalWrite(SIMCOM7000E, HIGH);
  delay(5000);
  digitalWrite(SIMCOM7000E, LOW);
  delay(1000);
  
  Serial.println("啟動INA219：5 秒");
  digitalWrite(INA219, HIGH);
  delay(5000);
  digitalWrite(INA219, LOW);
  delay(1000);

 Serial.println("進入省電睡眠：8 秒");
 LowPower.idle(SLEEP_8S, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF, SPI_OFF, USART0_OFF, TWI_OFF);
}
