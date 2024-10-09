#include "DigitalSensor.h"

DigitalSensor::DigitalSensor(const uint32_t id, const bool criticality, const uint8_t pin, const uint32_t readInterval)
{
    this->id = id;
    this->criticality = criticality;
    this->readInterval = readInterval;
    this->pin = pin;
    pinMode(pin, INPUT_PULLDOWN);
}

Health DigitalSensor::healthCheck() const
{
    pinMode(pin, INPUT_PULLDOWN);
    if (digitalRead(pin) == 0) {
        pinMode(pin, INPUT_PULLUP);
        if(digitalRead(pin) == 0) {
            pinMode(pin, INPUT_PULLDOWN);
            return Health::HEALTHY;
        }
    }
    pinMode(pin, INPUT_PULLDOWN);
    if (criticality) { return Health::CRITICAL; }
    return Health::UNRESPONSIVE;
}

SensorData DigitalSensor::read()
{
    lastRead = millis();
    SensorData sensorData = SensorData(id, 1);

    constexpr size_t bufferLen = sizeof(uint8_t);
    const uint8_t buf[bufferLen] = { digitalRead(pin) };
    sensorData.setMsg(buf, bufferLen);

    return sensorData;
}
