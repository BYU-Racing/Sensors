#ifndef RVC_H
#define RVC_H

#include <Arduino.h>
#include <Adafruit_BNO08x_RVC.h>
#include "Sensor.h"

class RVC final : public Sensor
{
    Adafruit_BNO08x_RVC* rvc = nullptr;
    BNO08x_RVC_Data* heading = nullptr;
    static void printValue(const char label[], float value, const char units[]);
    static void setMsg(SensorData* sensorData, uint8_t* index, float value);
public:
    RVC(uint32_t id, bool criticality, uint32_t readInterval, Adafruit_BNO08x_RVC* rvc);
    ~RVC() override;
    void begin(HardwareSerial* serial);
    [[nodiscard]] bool healthCheck() const override;
    bool ready() override;
    SensorData read() override;
    void debugPrint(const CAN_message_t& canMsg) const override;
};

#endif //RVC_H
