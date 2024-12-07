#include "RotationSensor.h"
#include <BufferPacker.h>

#define BUFFER_SIZE 8

RotationSensor::RotationSensor(
    const ReservedIDs id, const bool criticality, const uint32_t readInterval, Adafruit_BNO08x* imu,
    const sh2_SensorId_t report
)
{
    this->id = id;
    this->criticality = criticality;
    this->readInterval = readInterval;
    this->imu = imu;
    this->report = report;
    sensorValue = new sh2_SensorValue_t;
    ypr = new euler_t;
}

RotationSensor::~RotationSensor()
{
    delete sensorValue;
    delete ypr;
}

void RotationSensor::begin(HardwareSerial* serial)
{
    constexpr uint16_t millisToMicros = 1000;
    imu->begin_UART(serial, static_cast<int32_t>(id));
    imu->enableReport(report, readInterval * millisToMicros);
}

Health RotationSensor::healthCheck() const
{
    if (imu != nullptr)
    {
        return HEALTHY;
    }
    if (criticality)
    {
        return CRITICAL;
    }
    return UNRESPONSIVE;
} // Unsure if this is best health check?

bool RotationSensor::ready()
{
    if (millis() - lastRead >= readInterval)
    {
        return imu->getSensorEvent(sensorValue);
    }
    return false;
}

float RotationSensor::updateAngle(const float newAngle, const float prevAngle)
{
    const float normNewAngle = fmod(newAngle, D_180);
    const float normPrevAngle = fmod(prevAngle, D_360);
    float delta = normNewAngle - normPrevAngle;
    if (delta > D_180)
    {
        delta -= D_360;
    } else if (delta < -D_180)
    {
        delta += D_360;
    }
    return fmod(normPrevAngle + delta, D_360); // Don't allow more than 1 full rotation
}

void RotationSensor::updateYPR(euler_t* ypr, const RotationVector& rv)
{
    const auto [roll, pitch, yaw] = quaternionToEuler(rv.real, rv.i, rv.j, rv.k);
    ypr->roll = updateAngle(roll, ypr->roll);
    ypr->pitch = updateAngle(pitch, ypr->pitch);
    ypr->yaw = updateAngle(yaw, ypr->yaw);
}


void RotationSensor::setMsg(SensorData* sensorData, uint8_t* msgIndex, const float value, const RVCSubIDs subSensorId)
{
    uint8_t buf[sizeof(float) + sizeof(RVCSubIDs)];
    BufferPacker<sizeof(float) + sizeof(RVCSubIDs)> packer;
    packer.pack(subSensorId);
    packer.pack(value);
    packer.deepCopyTo(buf);
    sensorData->setMsg(buf, sizeof(float) + sizeof(RVCSubIDs), *msgIndex);
    (*msgIndex)++;
}

SensorData RotationSensor::read()
{
    lastRead = millis();
    // Defaulted to "empty"" state in case a switch on sensorValue.sensorId isn't matched
    SensorData sensorData;

    if (sensorValue->sensorId == report) // This is only necessary if the bno0x is used for multiple sensor reports
    {
        sensorData = SensorData(id, 3);
        const RotationVector rv(sensorValue->un.gameRotationVector);
        updateYPR(ypr, rv);
        uint8_t msgIndex = 0;
        setMsg(&sensorData, &msgIndex, ypr->roll, RVCSubIDs::RollId);
        setMsg(&sensorData, &msgIndex, ypr->pitch, RVCSubIDs::PitchId);
        setMsg(&sensorData, &msgIndex, ypr->yaw, RVCSubIDs::YawId);
    }
    return sensorData;
}

void RotationSensor::printValue(const char label[], const float value, const char units[])
{
    Serial.print(label);
    Serial.print(": ");
    Serial.print(value);
    Serial.print(" ");
    Serial.println(units);
}

void RotationSensor::debugPrint(const CAN_message_t& canMsg) const
{
    Serial.println("Rotation Sensor CAN Message:");
    Serial.print("Timestamp: ");
    Serial.println(canMsg.timestamp);
    BufferPacker<BUFFER_SIZE> unpacker(canMsg.buf);
    const RVCSubIDs id = unpacker.unpack<RVCSubIDs>();
    const float value = unpacker.unpack<float>();
    switch (id)
    {
    case RVCSubIDs::RollId:
        printValue("Roll", value, "deg");
        break;
    case RVCSubIDs::PitchId:
        printValue("Pitch", value, "deg");
        break;
    case RVCSubIDs::YawId:
        printValue("Yaw", value, "deg");
        break;
    default:
        break;
    }
    Serial.println();
}
