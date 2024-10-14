#include "RotationSensor.h"
#include <BufferPacker.h>

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

void RotationSensor::updateYPR(euler_t* ypr, const RotationVector& rv)
{
    const auto [roll, pitch, yaw] = quaternionToEuler(rv.real, rv.i, rv.j, rv.k);
    ypr->roll = updateAngle(roll, ypr->roll);
    ypr->pitch = updateAngle(pitch, ypr->pitch);
    ypr->yaw = updateAngle(yaw, ypr->yaw);
}

void RotationSensor::setMsg(SensorData* sensorData, uint8_t* msgIndex, const float value, const RVCSubIDs subSensorId)
{
    uint8_t buf[sizeof(float) + sizeof(uint8_t)];
    BufferPacker<sizeof(float) + sizeof(uint8_t)> packer;
    packer.pack(subSensorId);
    packer.pack(value);
    packer.deepCopyTo(buf);
    sensorData->setMsg(buf, sizeof(float) + sizeof(uint8_t), *msgIndex);
    (*msgIndex)++;
}

SensorData RotationSensor::read()
{
    lastRead = millis();
    // Defaulted to "empty"" state in case a switch on sensorValue.sensorId isn't matched
    SensorData sensorData;

    if (sensorValue->sensorId == report) // TODO: Remove multiple options after testing which Rotation Vector is best
    {
        RotationVector rv;

        switch (sensorValue->sensorId)
        {
        case SH2_ROTATION_VECTOR:
            /// Sensor fusion of Accelerometer, Gyroscope, and Magnetometer
            /// Referenced to Gravity *and* Magnetic North

            rv = RotationVector(sensorValue->un.rotationVector);
            break;
        case SH2_GAME_ROTATION_VECTOR:
            /// Sensor fusion of Accelerometer and Gyroscope
            /// Referenced to Gravity
            /// Avoids sudden jumps due to Magnetometer based corrections

            rv = RotationVector(sensorValue->un.gameRotationVector);
            break;
        case SH2_GEOMAGNETIC_ROTATION_VECTOR:
            /// Sensor fusion of Accelerometer and Magnetometer

            rv = RotationVector(sensorValue->un.geoMagRotationVector);
            break;
        case SH2_GYRO_INTEGRATED_RV:
            /// Sensor fusion of Accelerometer and Gyroscope
            /// Made for rotation?

            rv = RotationVector(sensorValue->un.gyroIntegratedRV);
            break;
        default:
            // A missing match will lead to an "empty" sensorData with no values
            return sensorData;
        }
        sensorData = SensorData(id, 3);

        updateYPR(ypr, rv);

        uint8_t msgIndex = 0; // used for prependId and message index

        setMsg(&sensorData, &msgIndex, ypr->roll, RVCSubIDs::Roll);
        setMsg(&sensorData, &msgIndex, ypr->pitch, RVCSubIDs::Pitch);
        setMsg(&sensorData, &msgIndex, ypr->yaw, RVCSubIDs::Yaw);
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
    BufferPacker<sizeof(float) + sizeof(uint8_t)> unpacker(canMsg.buf);
    const RVCSubIDs id = unpacker.unpack<RVCSubIDs>();
    const float value = unpacker.unpack<float>();
    switch (id)
    {
    case RVCSubIDs::Roll:
        printValue("Roll", value, "deg");
        break;
    case RVCSubIDs::Pitch:
        printValue("Pitch", value, "deg");
        break;
    case RVCSubIDs::Yaw:
        printValue("Yaw", value, "deg");
        break;
    default:
        break;
    }
    Serial.println();
}
