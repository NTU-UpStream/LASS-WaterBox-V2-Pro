#include "MQTT_Packer.h"

MQTT_Packer::MQTT_Packer(char _package[]){
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

void MQTT_Packer::initConnect(char* _id, char* _user, char* _pw, uint16_t _keep){
    clientid = _id;
    username = _user;
    password = _pw;
    keepalive=_keep;

    if(_DEBUG){
      _refSerial.println(F("[PACKER]\tInit Connect"));
      _refSerial.print(F("\t\tClient ID:\t"));
      _refSerial.println(clientid);
      _refSerial.print(F("\t\tUername:\t"));
      _refSerial.println(username);
      _refSerial.print(F("\t\tPassword:\t"));
      _refSerial.println(password);
      _refSerial.print(F("\t\tKeepalive:\t"));
      _refSerial.println(keepalive);
    }
}

void MQTT_Packer::showPackage(uint8_t _isACIIS){
  if(_DEBUG){
    
  }
}