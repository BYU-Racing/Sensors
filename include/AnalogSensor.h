#ifndef ANALOGSENSOR_H
#define ANALOGSENSOR_H

#include <Arduino.h>
#include "Sensor.h"

class AnalogSensor : public Sensor
{
    uint8_t pin = 0;
public:
    AnalogSensor(uint32_t id, bool criticality, uint8_t pin, uint32_t readInterval);
    [[nodiscard]] bool healthCheck() const override;
    SensorData read() override;
};

#endif //ANALOGSENSOR_H