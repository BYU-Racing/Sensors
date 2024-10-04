#include "IMU.h"
#include "ByteConverter.h"

IMU::IMU(const uint32_t id, const bool criticality, const uint32_t readInterval, Adafruit_BNO08x_RVC* rvc)
{
    this->id = id;
    this->criticality = criticality;
    this->readInterval = readInterval;
    this->rvc = rvc;
    this->serial = serial;
    heading = new BNO08x_RVC_Data;
}

IMU::~IMU() { delete heading; }

bool IMU::ready()
{
    if (millis() - lastRead >= readInterval)
    {
        return rvc->read(heading);
    }
    return false;
}

void IMU::pack(uint8_t* buf, const float value, const uint8_t id)
{
    buf[0] = id;
    ByteConverter::floatToBytes(value, &buf[1]);
}

float IMU::unpack(const uint8_t* buf)
{
    uint8_t bytes[sizeof(float)];
    for (size_t i = 0; i < sizeof(float); i++)
    {
        bytes[i] = buf[i + 1]; // All bytes after first (prepended id)
    }
    return ByteConverter::floatFromBytes(bytes);
}

SensorData IMU::read()
{
    lastRead = millis();
    SensorData sensorData = SensorData(id, 6);

    constexpr size_t len = sizeof(float) + 1;
    uint8_t bufferIndex = 0; // used for prependId and message index

    uint8_t x_accelBuf[len];
    pack(x_accelBuf, heading->x_accel, bufferIndex);
    sensorData.setMsg(x_accelBuf, len, bufferIndex);

    uint8_t y_accelBuf[len];
    bufferIndex = 1;
    pack(y_accelBuf, heading->y_accel, bufferIndex);
    sensorData.setMsg(y_accelBuf, len, bufferIndex);

    uint8_t z_accelBuf[len];
    bufferIndex = 2;
    pack(z_accelBuf, heading->z_accel, bufferIndex);
    sensorData.setMsg(z_accelBuf, len, bufferIndex);

    uint8_t yawBuf[len];
    bufferIndex = 3;
    pack(yawBuf, heading->yaw, bufferIndex);
    sensorData.setMsg(yawBuf, len, bufferIndex);

    uint8_t pitchBuf[len];
    bufferIndex = 4;
    pack(pitchBuf, heading->pitch, bufferIndex);
    sensorData.setMsg(pitchBuf, len, bufferIndex);

    uint8_t rollBuf[len];
    bufferIndex = 5;
    pack(rollBuf, heading->roll, bufferIndex);
    sensorData.setMsg(rollBuf, len, bufferIndex);

    return sensorData;
}

void IMU::printValue(const char label[], const float value, const char units[])
{
    Serial.print(label);
    Serial.print(": ");
    Serial.print(value);
    Serial.print(" ");
    Serial.println(units);
}

void IMU::debugPrint(const CAN_message_t& canMsg) const
{
    Serial.println("IMU CAN Message:");
    Serial.print("Timestamp: ");
    Serial.println(canMsg.timestamp);
    const float value = unpack(canMsg.buf);
    switch (canMsg.buf[0])
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
        printValue("Yaw", value, "deg");
        break;
    case 4:
        printValue("Pitch", value, "deg");
        break;
    case 5:
        printValue("Roll", value, "deg");
        break;
    default:
        break;
    }
    Serial.println();
}
