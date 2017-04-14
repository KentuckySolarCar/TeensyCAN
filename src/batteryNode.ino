#include <Arduino.h>
#include <FlexCAN.h>
#include <kinetis_flexcan.h>
#include "CANMessage.h"
#include "TeensyNode.h"
#include "BatteryFilter.h"
#include "batteryNode.h"
#include "message_ids.h"

//TODO
uint32_t batteryNode::checkForError(int data[], uint32_t messageID){
  return 0;
}

batteryNode::batteryNode() : TeensyNode(){
  for(int i =0; i < CURRENT_DATA_ROWS; i++){
    for(int j = 0; j < CURRENT_DATA_COLUMNS; j++){
      this->currentData[i][j] = 0;

    }
  }
}
void batteryNode::interpretData(uint32_t messageID){
  int index, errormsg, datalen = 8;
  int data[datalen];
  CANMessage CANmsg;

  if(messageID == BATTERY_BC_AC_BP_AP){
    for(int i =0; i<4; i++){
      data[i] = currentFilter.getX(i);
    }
    errormsg = checkForError(data, BATTERY_BC_AC_BP_AP);
    if(errormsg){
      CANmsg.setMessageID(errormsg);
    }
    else{
      CANmsg.setMessageID(messageID);
    }
    //Pack the data
    int start=0,end=16;
    for(int i = 0; i< datalen; i++){
        CANmsg.storeSignedInt(int64_t(data[i]),start, end);
        start += 16;
        end += 16;
    }
  }
  else{
    //if it is a Voltage, set the correct index for the filter
    if(messageID >=BATTERY_VOLTAGE_1 && messageID <= BATTERY_VOLTAGE_10){
      index = messageID - BATTERY_VOLTAGE_1;
    }
      //if it is a Temperature, set the correct index for the filter
    else if(messageID >= BATTERY_TEMPERATURE_1 && messageID <= BATTERY_TEMPERATURE_10){
      index = messageID - BATTERY_TEMPERATURE_1;
    }
    else{
      //if we get here, we were processing data with an id that does't belong to the batteryNode
      return;
    }
    for(int i =0; i<4; i++){
      data[i] = this->cellFilters[index].getX(i);
    }
    errormsg = checkForError(data, messageID);
    if(errormsg){
      CANmsg.setMessageID(errormsg);
    }
    else{
      CANmsg.setMessageID(messageID);
    }
    //Pack the data
    int start=0,end=16;
    for(int i = 0; i< datalen; i++){
        CANmsg.storeSignedInt(int64_t(data[i]),start, end);
        start += 16;
        end += 16;
      }
    }
  this->write(CANmsg);
}

void batteryNode::kalmanStep(int data[], int id, int arrLen){
  int index;

  if(id == BATTERY_BC_AC_BP_AP){
    this->currentFilter.step(data);
  }
  else{
    if(id >=BATTERY_VOLTAGE_1 && id <= BATTERY_VOLTAGE_10){
      index = id - BATTERY_VOLTAGE_1;
      //Record the voltage values in the first 4 indices of currentData[index]
      for(int i = 0; i< 4; i++){
        currentData[index][i] = data[i];
      }

    }

    else if(id >=BATTERY_TEMPERATURE_1 && id <= BATTERY_TEMPERATURE_10){
      index = id - BATTERY_TEMPERATURE_1;
      //Record the voltage values in the first 4 indices of currentData[index]
      for(int i =0; i< 4; i++){
        currentData[index][i+4] = data[i];
      }
    }
    this->cellFilters[index].step(currentData[index]);
  }

}
