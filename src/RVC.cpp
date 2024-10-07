#include "RVC.h"

#include <BufferPacker.h>

RVC::RVC(const uint32_t id, const bool criticality, const uint32_t readInterval, Adafruit_BNO08x_RVC* rvc)
{
    this->id = id;
    this->criticality = criticality;
    this->readInterval = readInterval;
    this->rvc = rvc;
    heading = new BNO08x_RVC_Data;
}

RVC::~RVC() { delete heading; }

void RVC::begin(HardwareSerial* serial) { rvc->begin(serial); }

bool RVC::healthCheck() const { return rvc; } // RVC has a bool operator override so this is OK

bool RVC::ready()
{
    if (millis() - lastRead >= readInterval)
    {
        return rvc->read(heading);
    }
    return false;
}

void RVC::setMsg(SensorData* sensorData, uint8_t* index, const float value)
{
    constexpr size_t bufferLen = sizeof(float) + 1;
    uint8_t buf[bufferLen];
    BufferPacker::packFloat(buf, value, *index);
    sensorData->setMsg(buf, bufferLen, *index);
    (*index)++;
}

SensorData RVC::read()
{
    lastRead = millis();
    SensorData sensorData = SensorData(id, 6);

    uint8_t bufferIndex = 0; // used for prependId and message index

    setMsg(&sensorData, &bufferIndex, heading->x_accel);
    setMsg(&sensorData, &bufferIndex, heading->y_accel);
    setMsg(&sensorData, &bufferIndex, heading->z_accel);
    setMsg(&sensorData, &bufferIndex, heading->roll);
    setMsg(&sensorData, &bufferIndex, heading->pitch);
    setMsg(&sensorData, &bufferIndex, heading->yaw);

    return sensorData;
}

void RVC::printValue(const char label[], const float value, const char units[])
{
    Serial.print(label);
    Serial.print(": ");
    Serial.print(value);
    Serial.print(" ");
    Serial.println(units);
}

void RVC::debugPrint(const CAN_message_t& canMsg) const
{
    Serial.println("RVC CAN Message:");
    Serial.print("Timestamp: ");
    Serial.println(canMsg.timestamp);
    uint8_t* id = new uint8_t(0xFF);
    const float value = BufferPacker::unpackFloat(canMsg.buf, true, id);
    switch (*id)
    {
    case 0:
        printValue("X Acceleration", value, "m/s^2");
        break;
    case 1:
        printValue("Y Acceleration", value, "m/s^2");
        break;
    case 2:
        printValue("Z Acceleration", value, "m/s^2");
        break;
    case 3:
        printValue("Roll", value, "deg");
        break;
    case 4:
        printValue("Pitch", value, "deg");
        break;
    case 5:
        printValue("Yaw", value, "deg");
        break;
    default:
        break;
    }
    Serial.println();
    delete id;
}
