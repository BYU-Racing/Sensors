#ifndef DIGITALSENSOR_H
#define DIGITALSENSOR_H

#include <Arduino.h>
#include "Sensor.h"

class DigitalSensor : public Sensor
{
    uint8_t pin = 0;
public:
    DigitalSensor(uint32_t id, bool criticality, uint8_t pin, uint32_t readInterval);
    SensorData read() override;
};

class Switch : public DigitalSensor
{
    const char* name;
public:
    Switch(uint32_t id, bool criticality, uint8_t pin, uint32_t readInterval, const char* name);
    void debugPrint(const CAN_message_t& canMsg) const override;
};

#endif //DIGITALSENSOR_H
