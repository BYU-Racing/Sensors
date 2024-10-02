#ifndef DIGITALSENSOR_H
#define DIGITALSENSOR_H

#include <Arduino.h>
#include "Sensor.h"

class DigitalSensor final : public Sensor
{
    uint8_t pin = 0;
public:
    DigitalSensor(uint32_t id, bool criticality, uint8_t pin, uint32_t readInterval);
    SensorData read() override;
};

#endif //DIGITALSENSOR_H
