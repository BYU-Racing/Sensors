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

Switch::Switch(
    const uint32_t id, const bool criticality, const uint8_t pin, const uint32_t readInterval, const char* name
) : DigitalSensor(id, criticality, pin, readInterval), name(name) {  }

void Switch::debugPrint(const CAN_message_t& canMsg) const
{
    Serial.print(name);
    Serial.println(" Switch CAN Message:");
    Serial.print("Timestamp: ");
    Serial.println(canMsg.timestamp);
    const uint8_t value = canMsg.buf[0];
    Serial.print("State: ");
    Serial.println(value == 1 ? "On" : "Off");
    Serial.println();
}
