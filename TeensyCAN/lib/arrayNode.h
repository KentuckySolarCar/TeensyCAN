#ifndef ARRAY_NODE_H
#define ARRAY_NODE_H

#include <Arduino.h>
#include <FlexCAN.h>
#include <kinetis_flexcan.h>
#include "CANMessage.h"
#include "TeensyNode.h"
#include "ArrayFilter.h"

#define DATA_ENTRIES 5

class arrayNode : public TeensyNode {
private:

	ArrayFilter arrayEKF;
	int currentData[DATA_ENTRIES];
	
public:
  arrayNode();
  
  void interpretData( uint32_t messageID);

  void kalmanStep(int data[], int id, int arrLen);
  void  checkForError(int data[], int datalen, uint32_t messageID);
  void sendMessage(uint32_t writeMessageID, int data[], int datalen);
  
  void updateStateCalculations();


};

#endif
