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

Sensor* sensors[] = {
    &steeringWheel,
};
size_t numSensors = 1;

void setup() {
    Serial.begin(115200);
    Serial1.begin(115200);
    steeringWheel.begin(&Serial1);
}

uint64_t i = 1;
void loop() {
    for (size_t sensorIndex = 0; sensorIndex < numSensors; sensorIndex++)
    {
        if (Sensor* sensor = sensors[sensorIndex]; sensor->healthCheck() && sensor->ready())
        {
            Serial.print("Sensor Read ");
            Serial.print(i);
            Serial.println(": ");
            const SensorData data = sensor->read();
            const size_t msgCount = data.getMsgCount();
            const CAN_message_t* msgs = data.getMsgs();
            for (size_t msgIndex = 0; msgIndex < msgCount; msgIndex++)
            {
                Serial.print("Debugging Message ");
                Serial.print(msgIndex + 1);
                Serial.print(" of ");
                Serial.println(msgCount);
                sensor->debugPrint(msgs[msgIndex]);
            }
            i++;
        }
    }
}
