#include "AnalogSensor.h"
#include <BufferPacker.h>

AnalogSensor::AnalogSensor(const ReservedIDs id, const bool criticality, const uint8_t pin, const uint32_t readInterval)
{
    this->id = id;
    this->criticality = criticality;
    this->readInterval = readInterval;
    this->pin = pin;
}

Health AnalogSensor::healthCheck() const
{
    if (analogRead(pin) != 0) { return HEALTHY; }
    if (criticality) { return CRITICAL; }
    return UNRESPONSIVE;
}

SensorData AnalogSensor::read()
{
    lastRead = millis();
    SensorData sensorData = SensorData(id, 1);
    uint8_t buf[sizeof(int)] = {};
    BufferPacker<sizeof(int)> packer;
    packer.pack(analogRead(pin));
    packer.deepCopyTo(buf);
    sensorData.setMsg(buf, sizeof(int));

    return sensorData;
}
