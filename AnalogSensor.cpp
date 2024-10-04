#include "AnalogSensor.h"
#include "ByteConverter.h"

AnalogSensor::AnalogSensor(const uint32_t id, const bool criticality, const uint8_t pin, const uint32_t readInterval)
{
    this->id = id;
    this->criticality = criticality;
    this->readInterval = readInterval;
    this->pin = pin;
}

SensorData AnalogSensor::read()
{
    lastRead = millis();
    SensorData sensorData = SensorData(id, 1);

    uint8_t buf[sizeof(int)];
    ByteConverter::intToBytes(analogRead(pin), buf);
    sensorData.setMsg(buf, 4);

    return sensorData;
}
