#include "RotationSensor.h"
#include "BufferPacker.h"

RotationSensor::RotationSensor(
    const uint32_t id, const bool criticality, const uint32_t readInterval, Adafruit_BNO08x* imu,
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
    imu->begin_UART(serial, static_cast<int32_t>(id));
    imu->enableReport(report, readInterval);
}

bool RotationSensor::healthCheck() const { return imu; } // imu has bool operator override so this is OK

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
    const auto [yaw, pitch, roll] = quaternionToEuler(rv.real, rv.i, rv.j, rv.k);
    ypr->yaw += delta(yaw, ypr->yaw);
    ypr->pitch += delta(pitch, ypr->pitch);
    ypr->roll += delta(roll, ypr->roll);
}

void RotationSensor::setMsg(SensorData* sensorData, uint8_t* index, const float value)
{
    constexpr size_t bufferLen = sizeof(float) + 1;
    uint8_t buf[bufferLen];
    BufferPacker::packFloat(buf, value, *index);
    sensorData->setMsg(buf, bufferLen, *index);
    (*index)++;
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

        uint8_t bufferIndex = 0; // used for prependId and message index

        setMsg(&sensorData, &bufferIndex, ypr->roll);
        setMsg(&sensorData, &bufferIndex, ypr->pitch);
        setMsg(&sensorData, &bufferIndex, ypr->yaw);
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
    uint8_t* id = new uint8_t(0xFF);
    const float value = BufferPacker::unpackFloat(canMsg.buf, true, id);
    switch (*id)
    {
    case 0:
        printValue("Roll", value, "deg");
        break;
    case 1:
        printValue("Pitch", value, "deg");
        break;
    case 2:
        printValue("Yaw", value, "deg");
        break;
    default:
        break;
    }
    Serial.println();
    delete id;
}
