#ifndef DIGITALSENSOR_H
#define DIGITALSENSOR_H

#include <Arduino.h>
#include "Sensor.h"

class DigitalSensor : public Sensor
{
    uint8_t pin = 0;
public:
    DigitalSensor(uint32_t id, bool criticality, uint8_t pin, uint32_t readInterval);
    [[nodiscard]] bool healthCheck() const override;
    SensorData read() override;
};

#endif //DIGITALSENSOR_H
