#include <Arduino.h>
#include <FlexCAN_T4.h>
#include <Adafruit_BNO08x_RVC.h>
#include "AnalogSensor.h"
#include "DigitalSensor.h"
#include "RVC.h"
#include "RotationSensor.h"

constexpr uint32_t R_DAMPER_ID = 1;
constexpr bool R_DAMPER_CRITICALITY = false;
constexpr uint8_t R_DAMPER_PIN = 22;

constexpr uint32_t L_DAMPER_ID = 2;
constexpr bool L_DAMPER_CRITICALITY = false;
constexpr uint8_t L_DAMPER_PIN = 23;

constexpr uint32_t DAMPER_INTERVAL = 100;

constexpr uint32_t SWITCH_ID = 3;
constexpr bool SWITCH_CRITICALITY = true;
constexpr uint8_t SWITCH_PIN = 38;
constexpr uint32_t SWITCH_INTERVAL = 100;

constexpr uint32_t RVC_ID = 4;
constexpr bool RVC_CRITICALITY = false;
constexpr uint32_t RVC_INTERVAL = 100;

constexpr uint32_t STEERING_WHEEL_ID = 5;
constexpr bool STEERING_WHEEL_CRITICALITY = false;
constexpr uint32_t STEERING_WHEEL_INTERVAL = 100;

AnalogSensor rightDamperPot = AnalogSensor(R_DAMPER_ID, R_DAMPER_CRITICALITY, R_DAMPER_PIN, DAMPER_INTERVAL);
AnalogSensor leftDamperPot = AnalogSensor(L_DAMPER_ID, L_DAMPER_CRITICALITY, L_DAMPER_PIN, DAMPER_INTERVAL);
DigitalSensor startSwitch = DigitalSensor(SWITCH_ID, SWITCH_CRITICALITY, SWITCH_PIN, SWITCH_INTERVAL);
Adafruit_BNO08x_RVC bno1;
RVC rvc = RVC(RVC_ID, RVC_CRITICALITY, RVC_INTERVAL, &bno1);
Adafruit_BNO08x bno2;
RotationSensor steeringWheel = RotationSensor(
    STEERING_WHEEL_ID, STEERING_WHEEL_CRITICALITY, STEERING_WHEEL_INTERVAL, &bno2
);

Sensor* sensors[] = {
    &rightDamperPot,
    &leftDamperPot,
    &startSwitch,
    &rvc,
    &steeringWheel,
};
size_t numSensors = sizeof(sensors) / sizeof(sensors[0]);

void setup() {
    Serial.begin(115200);
    Serial1.begin(115200);
    rvc.begin(&Serial1);
    steeringWheel.begin(&Serial1);
}

void loop() {
    for (size_t sensorIndex = 0; sensorIndex < numSensors; sensorIndex++)
    {
        if (Sensor* sensor = sensors[sensorIndex]; sensor->healthCheck() && sensor->ready())
        {
            const SensorData data = sensor->read();
            const size_t msgCount = data.getMsgCount();
            const CAN_message_t* msgs = data.getMsgs();
            for (size_t msgIndex = 0; msgIndex < msgCount; msgIndex++)
            {
                sensor->debugPrint(msgs[msgIndex]);
            }
        }
    }
}
