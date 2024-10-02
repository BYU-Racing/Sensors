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

bool IMU::ready() { return rvc->read(heading); }

void IMU::pack(uint8_t* buf, const float value, const uint8_t id)
{
    buf[0] = id;
    ByteConverter::toBytes(value, &buf[1]);
}

float IMU::unpack(const uint8_t* buf)
{
    uint8_t bytes[sizeof(float)];
    for (size_t i = 0; i < sizeof(float); i++)
    {
        bytes[i] = buf[i + 1]; // All bytes after first (prepended id)
    }
    return ByteConverter::toFloat(bytes);
}

SensorData IMU::read()
{
    lastRead = millis();
    SensorData sensorData = SensorData(id, 6);

    uint8_t x_accelBuf[sizeof(float) + 1];
    pack(x_accelBuf, heading->x_accel, 0);
    sensorData.setMsg(x_accelBuf, 5, 0);

    uint8_t y_accelBuf[sizeof(float) + 1];
    pack(y_accelBuf, heading->y_accel, 1);
    sensorData.setMsg(y_accelBuf, 5, 1);

    uint8_t z_accelBuf[sizeof(float) + 1];
    pack(z_accelBuf, heading->z_accel, 2);
    sensorData.setMsg(z_accelBuf, 5, 2);

    uint8_t yawBuf[sizeof(float) + 1];
    pack(yawBuf, heading->yaw, 3);
    sensorData.setMsg(yawBuf, 5, 3);

    uint8_t pitchBuf[sizeof(float) + 1];
    pack(pitchBuf, heading->pitch, 4);
    sensorData.setMsg(pitchBuf, 5, 4);

    uint8_t rollBuf[sizeof(float) + 1];
    pack(rollBuf, heading->roll, 5);
    sensorData.setMsg(rollBuf, 5, 5);

    return sensorData;
}
