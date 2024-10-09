#include <Arduino.h>
#include "Sensor.h"
#include "RotationSensor.h"

constexpr uint32_t STEERING_WHEEL_ID = 5;
constexpr bool STEERING_WHEEL_CRITICALITY = false;
constexpr uint32_t STEERING_WHEEL_INTERVAL = 100;

Adafruit_BNO08x imu;
RotationSensor steeringWheel = RotationSensor(
    STEERING_WHEEL_ID, STEERING_WHEEL_CRITICALITY, STEERING_WHEEL_INTERVAL, &imu, SH2_ROTATION_VECTOR
);
size_t numSensors = 1;

constexpr uint32_t BAUD_RATE = 115200;

void setup() {
    Serial.begin(BAUD_RATE);
    Serial1.begin(BAUD_RATE);
    steeringWheel.begin(&Serial1);
}

uint64_t i = 1;
void loop() {
    if (steeringWheel.healthCheck() && steeringWheel.ready())
    {
        Serial.print("Sensor Read ");
        Serial.print(i);
        Serial.println(": ");
        const SensorData data = steeringWheel.read();
        const size_t msgCount = data.getMsgCount();
        const CAN_message_t* msgs = data.getMsgs();
        for (size_t msgIndex = 0; msgIndex < msgCount; msgIndex++)
        {
            Serial.print("Debugging Message ");
            Serial.print(msgIndex + 1);
            Serial.print(" of ");
            Serial.println(msgCount);
            steeringWheel.debugPrint(msgs[msgIndex]);
        }
        i++;
    }
}
