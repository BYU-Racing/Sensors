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

bool AnalogSensor::ready() {
    counter++;
    runner += analogRead(pin);
    return millis() - lastRead >= readInterval;
}

SensorData AnalogSensor::read()
{
    lastRead = millis();
    uint8_t buf[sizeof(int)] = {};
    BufferPacker<sizeof(int)> packer;
    packer.pack(int(runner / counter));
    packer.deepCopyTo(buf);
    SensorData sensorData = SensorData(id, 1);
    sensorData.setMsg(buf, sizeof(int));
    counter = 0;
    runner = 0;
    return sensorData;
}
