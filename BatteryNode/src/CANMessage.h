#ifndef CAN_MESSAGE_H
#define CAN_MESSAGE_H
#include <stdlib.h>
#include <FlexCAN.h>
#include <Arduino.h>
class CANMessage{
private:
  uint32_t messageID;
  uint8_t message[8];
public:
	CANMessage();
	CANMessage(uint32_t id, uint8_t buff[]);

  uint32_t getMessageID();
  void setMessageID(uint32_t id);
  //Functions for translation to/from FlexCAN and our CAN message format
  void translateFromFlexCAN(CAN_message_t from);
  void translateToFlexCAN(CAN_message_t &from);
  //Functions tot read signed and unsigned integers
  uint64_t readUnsignedInt(uint8_t start, uint8_t end);
  int64_t readSignedInt(uint8_t start, uint8_t end);
  //Functions to store signed and unsigned integers
  int storeUnsignedInt(uint64_t num, uint8_t start, uint8_t end);
  int storeSignedInt(int64_t num, uint8_t start, uint8_t end);

  //Functions to read and store boolean values
  int storeBool(bool val ,uint8_t position);
  bool readBool(uint8_t position);
};
#endif
