#include "DigitalSensor.h"

DigitalSensor::DigitalSensor(const uint32_t id, const bool criticality, const uint8_t pin, const uint32_t readInterval)
{
    this->id = id;
    this->criticality = criticality;
    this->readInterval = readInterval;
    this->pin = pin;
}

SensorData DigitalSensor::read()
{
    lastRead = millis();
    SensorData sensorData = SensorData(id, 1);

    const uint8_t buf[1] = { digitalRead(pin) };
    sensorData.setMsg(buf, 1);

    return sensorData;
}
