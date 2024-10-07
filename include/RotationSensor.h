#ifndef ROTATIONSENSOR_H
#define ROTATIONSENSOR_H

#include <Arduino.h>
#include <Adafruit_BNO08x.h>
#include "Sensor.h"
#include "RotationVectors.h"

class RotationSensor final : public Sensor {
    Adafruit_BNO08x* imu = nullptr;
    sh2_SensorId_t report = 0;
    sh2_SensorValue_t* sensorValue = nullptr;
    euler_t* ypr = nullptr;
    static void updateYPR(euler_t* ypr, const RotationVector& rv);
    static void setMsg(SensorData* sensorData, uint8_t* index, float value);
    static void printValue(const char label[], float value, const char units[]);
public:
    RotationSensor(
        uint32_t id, bool criticality, uint32_t readInterval, Adafruit_BNO08x* imu,
        sh2_SensorId_t report = SH2_GAME_ROTATION_VECTOR
    );
    ~RotationSensor() override;
    void begin(HardwareSerial* serial);
    [[nodiscard]] bool healthCheck() const override;
    bool ready() override;
    SensorData read() override;
    void debugPrint(const CAN_message_t& canMsg) const override;
};

#endif //ROTATIONSENSOR_H
