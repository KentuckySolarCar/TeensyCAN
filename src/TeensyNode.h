#ifndef TEENSY_NODE_H
#define TEENSY_NODE_H

#include <Arduino.h>
#include <FlexCAN.h>
#include <kinetis_flexcan.h>
#include "CANMessage.h"

#define MAX_ERRORS 10

class TeensyNode {

public:
  FlexCAN CANBus;
  TeensyNode(){
    this->CANBus = FlexCAN();
    //this->CANBus.begin();
  }
  TeensyNode(uint32_t baudrate){
    this->CANBus = FlexCAN(baudrate);
    //this->CANBus.begin();
  }
  int read(CANMessage &message){
    CAN_message_t msg;
    int result = this->CANBus.read(msg);
    Serial.println(msg.id);
    message.translateFromFlexCAN(msg);
    return result;
  }
  int write(CANMessage message){
    CAN_message_t msg;
    message.translateToFlexCAN(msg);
    int result = this->CANBus.write(msg);
    return result;
  }



};

#endif
