#ifndef SENSORDATA_H
#define SENSORDATA_H

#include <Arduino.h>
#include <FlexCAN_T4.h>
#include "Reserved.h"

class SensorData
{
    uint32_t id = INVALID;
    size_t msgCount = 0;
    CAN_message_t* msgs = nullptr;
    CAN_message_t newMsg(const uint8_t* buf, size_t len) const;
public:
    SensorData() = default;
    SensorData(uint32_t id, size_t msgCount);
    ~SensorData() { delete[] msgs; }
    size_t getMsgCount() const;
    CAN_message_t* getMsgs() const;
    bool setMsg(const uint8_t* buf, size_t len, size_t msgIndex=0);
};

#endif //SENSORDATA_H
