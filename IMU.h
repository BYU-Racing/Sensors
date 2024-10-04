#ifndef IMU_H
#define IMU_H

#include <Arduino.h>
#include <Adafruit_BNO08x_RVC.h>
#include "Sensor.h"

class IMU final : public Sensor
{
    Adafruit_BNO08x_RVC* rvc = nullptr;
    HardwareSerial* serial = nullptr;
    BNO08x_RVC_Data* heading = nullptr;
    static void pack(uint8_t* buf, float value, uint8_t id);
    static float unpack(const uint8_t* buf);
    static void printValue(const char label[], float value, const char units[]);
public:
    IMU(uint32_t id, bool criticality, uint32_t readInterval, Adafruit_BNO08x_RVC* rvc);
    ~IMU() override;
    bool ready() override;
    SensorData read() override;
    void debugPrint(const CAN_message_t& canMsg) const override;
};

#endif //IMU_H
