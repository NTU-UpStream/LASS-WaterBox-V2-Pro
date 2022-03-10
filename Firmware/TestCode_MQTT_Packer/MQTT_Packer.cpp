#include "MQTT_Packer.h"

MQTT_Packer::MQTT_Packer(char* _package){
  _p_package = _package;
}

MQTT_Packer::~MQTT_Packer(){
  _DEBUG=0;
  free(_p_package);
}

void MQTT_Packer::setDebuger(Stream &refSer){
  _refSerial=refSer;
  _DEBUG=1;
}


void MQTT_Packer::showPackage(uint8_t _isACIIS){
}
