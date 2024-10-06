#include "DigitalSensor.h"

DigitalSensor::DigitalSensor(const uint32_t id, const bool criticality, const uint8_t pin, const uint32_t readInterval)
{
    this->id = id;
    this->criticality = criticality;
    this->readInterval = readInterval;
    this->pin = pin;
    pinMode(pin, INPUT_PULLDOWN);
}

bool DigitalSensor::healthCheck() const
{
    pinMode(pin, INPUT_PULLDOWN);
    if (digitalRead(pin) == 0) {
        pinMode(pin, INPUT_PULLUP);
        if(digitalRead(pin == 0)) {
            pinMode(pin, INPUT_PULLDOWN);
            return true;
        }
    }
    pinMode(pin, INPUT_PULLDOWN);
    return false;
}

SensorData DigitalSensor::read()
{
    lastRead = millis();
    SensorData sensorData = SensorData(id, 1);

    const uint8_t buf[1] = { digitalRead(pin) };
    sensorData.setMsg(buf, 1);

    return sensorData;
}
