#ifndef ANALOGSENSOR_H
#define ANALOGSENSOR_H

#include <Arduino.h>
#include "Sensor.h"

class AnalogSensor : public Sensor
{
    uint8_t pin = 0;
public:
    AnalogSensor(uint32_t id, bool criticality, uint8_t pin, uint32_t readInterval);
    SensorData read() override;
};

class Pot : public AnalogSensor
{
    const char* name;
public:
    Pot(uint32_t id, bool criticality, uint8_t pin, uint32_t readInterval, const char* name);
    void debugPrint(const CAN_message_t& canMsg) const override;
};

#endif //ANALOGSENSOR_H
