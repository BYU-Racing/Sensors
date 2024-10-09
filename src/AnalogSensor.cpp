#include "AnalogSensor.h"
#include "BufferPacker.h"

AnalogSensor::AnalogSensor(const uint32_t id, const bool criticality, const uint8_t pin, const uint32_t readInterval)
{
    this->id = id;
    this->criticality = criticality;
    this->readInterval = readInterval;
    this->pin = pin;
}

Health AnalogSensor::healthCheck() const
{
    if (analogRead(pin) == 0) { return Health::HEALTHY; }
    if (criticality) { return Health::CRITICAL; }
    return Health::UNRESPONSIVE;
}

SensorData AnalogSensor::read()
{
    lastRead = millis();
    SensorData sensorData = SensorData(id, 1);

    constexpr size_t bufferLen = sizeof(int);
    uint8_t buf[bufferLen];
    BufferPacker::packInt(buf, analogRead(pin));
    sensorData.setMsg(buf, bufferLen);

    return sensorData;
}
