#ifndef BATTERY_NODE_H
#define BATTERY_NODE_H

#include <Arduino.h>
#include <FlexCAN.h>
#include <kinetis_flexcan.h>
#include "CANMessage.h"
#include "TeensyNode.h"
#include "BatteryFilter.h"
#include <vector>
#define CURRENT_DATA_ROWS 10
#define CURRENT_DATA_COLUMNS 8
#define CELL_FILTERS_LEN 10

class batteryNode : protected TeensyNode {
private:
BatteryFilter cellFilters[CELL_FILTERS_LEN];
BatteryFilter currentFilter;
int currentData[CURRENT_DATA_ROWS][CURRENT_DATA_COLUMNS];

public:
  batteryNode();
  void interpretData( uint32_t messageID);

  void kalmanStep(int data[], int id, int arrLen);
  uint32_t  checkForError(int data[], uint32_t messageID);


};
#endif
