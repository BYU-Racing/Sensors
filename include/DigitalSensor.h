#ifndef DIGITALSENSOR_H
#define DIGITALSENSOR_H

#include <cstdint>
#include "Sensor.h"

class DigitalSensor : public Sensor
{
    uint8_t pin = 0;
public:
    DigitalSensor(uint32_t id, bool criticality, uint8_t pin, uint32_t readInterval);
    [[nodiscard]] Health healthCheck() const override;
    SensorData read() override;
};

#endif //DIGITALSENSOR_H
