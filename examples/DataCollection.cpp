#include <Arduino.h>
#include <FlexCAN_T4.h>
#include <Adafruit_BNO08x_RVC.h>
#include <Reserved.h>
#include "AnalogSensor.h"
#include "DigitalSensor.h"
#include "RVC.h"
#include "RotationSensor.h"


constexpr uint32_t BAUD_RATE = 115200;

constexpr bool THROTTLE1_CRITICALITY = false;
constexpr uint8_t THROTTLE1_PIN = 22;

constexpr bool THROTTLE2_CRITICALITY = false;
constexpr uint8_t THROTTLE2_PIN = 23;

constexpr uint32_t THROTTLE_INTERVAL = 100;

constexpr bool SWITCH_CRITICALITY = true;
constexpr uint8_t SWITCH_PIN = 38;
constexpr uint32_t SWITCH_INTERVAL = 100;

constexpr bool RVC_CRITICALITY = false;
constexpr uint32_t RVC_INTERVAL = 100;

constexpr bool STEERING_WHEEL_CRITICALITY = false;
constexpr uint32_t STEERING_WHEEL_INTERVAL = 100;

AnalogSensor throttle1 = AnalogSensor(ReservedIDs::Throttle1PositionId, THROTTLE1_CRITICALITY, THROTTLE1_PIN, THROTTLE_INTERVAL);
AnalogSensor throttle2 = AnalogSensor(ReservedIDs::Throttle2PositionId, THROTTLE2_CRITICALITY, THROTTLE2_PIN, THROTTLE_INTERVAL);
DigitalSensor startSwitch = DigitalSensor(ReservedIDs::StartSwitchId, SWITCH_CRITICALITY, SWITCH_PIN, SWITCH_INTERVAL);
Adafruit_BNO08x_RVC bno1;
RVC rvc = RVC(ReservedIDs::RVCId, RVC_CRITICALITY, RVC_INTERVAL, &bno1);
// rvc cannot share a sensor with rotationsensor
Adafruit_BNO08x bno2;
RotationSensor steeringWheel = RotationSensor(
    ReservedIDs::SteeringWheelAngleId, STEERING_WHEEL_CRITICALITY, STEERING_WHEEL_INTERVAL, &bno2
);

Sensor* sensors[] = {
    &throttle1,
    &throttle2,
    &startSwitch,
    &rvc,
    &steeringWheel
};
size_t numSensors = sizeof(sensors) / sizeof(sensors[0]);

void setup() {
    Serial.begin(BAUD_RATE);
    Serial1.begin(BAUD_RATE);
    Serial2.begin(BAUD_RATE);
    rvc.begin(&Serial1);
    steeringWheel.begin(&Serial2);
}

void loop() {
    for (size_t sensorIndex = 0; sensorIndex < numSensors; sensorIndex++)
    {
        if (Sensor* sensor = sensors[sensorIndex]; sensor->ready())
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
