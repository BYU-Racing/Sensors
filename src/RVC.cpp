#include "RVC.h"
#include <BufferPacker.h>

RVC::RVC(const ReservedIDs id, const bool criticality, const uint32_t readInterval, Adafruit_BNO08x_RVC* rvc)
{
    this->id = id;
    this->criticality = criticality;
    this->readInterval = readInterval;
    this->rvc = rvc;
    heading = new BNO08x_RVC_Data;
}

RVC::~RVC() { delete heading; }

void RVC::begin(HardwareSerial* serial) { rvc->begin(serial); }

Health RVC::healthCheck() const
{
    if (rvc != nullptr) { return HEALTHY; } // Definitely not a true inidication of *healthy* operation
    if (criticality) { return CRITICAL; }
    return UNRESPONSIVE;
}

bool RVC::ready()
{
    if (millis() - lastRead >= readInterval)
    {
        return rvc->read(heading);
    }
    return false;
}

void RVC::setMsg(SensorData* sensorData, uint8_t* msgIndex, const float value, const uint8_t subSensorId)
{
    uint8_t buf[sizeof(uint8_t) + sizeof(float)];
    BufferPacker<sizeof(uint8_t) + sizeof(float)> packer;
    packer.pack(subSensorId);
    packer.pack(value);
    packer.deepCopyTo(buf);
    sensorData->setMsg(buf, sizeof(uint8_t) + sizeof(float), *msgIndex);
    (*msgIndex)++;
}

SensorData RVC::read()
{
    lastRead = millis();
    SensorData sensorData = SensorData(id, rvcMsgCount);

    uint8_t msgIndex = 0; // used for prependId and message index

    // rvc->read is called in ready() check
    setMsg(&sensorData, &msgIndex, heading->x_accel, RVCSubIDs::X_Accel);
    setMsg(&sensorData, &msgIndex, heading->y_accel, RVCSubIDs::Y_Accel);
    setMsg(&sensorData, &msgIndex, heading->z_accel, Z_Accel);
    setMsg(&sensorData, &msgIndex, heading->roll, RVCSubIDs::Roll);
    setMsg(&sensorData, &msgIndex, heading->pitch, RVCSubIDs::Pitch);
    setMsg(&sensorData, &msgIndex, heading->yaw, RVCSubIDs::Yaw);

    return sensorData;
}

void RVC::printValue(const char label[], const float value, const char units[])
{
    Serial.print(label);
    Serial.print(": ");
    Serial.print(value);
    Serial.print(" ");
    Serial.println(units);
}

void RVC::debugPrint(const CAN_message_t& canMsg) const
{
    Serial.println("RVC CAN Message:");
    Serial.print("Timestamp: ");
    Serial.println(canMsg.timestamp);
    BufferPacker<sizeof(uint8_t) + sizeof(float)> unpacker(canMsg.buf);
    const RVCSubIDs id = unpacker.unpack<RVCSubIDs>();
    const float value = unpacker.unpack<float>();
    switch (id)
    {
    case RVCSubIDs::X_Accel:
        printValue("X Acceleration", value, "m/s^2");
        break;
    case RVCSubIDs::Y_Accel:
        printValue("Y Acceleration", value, "m/s^2");
        break;
    case RVCSubIDs::Z_Accel:
        printValue("Z Acceleration", value, "m/s^2");
        break;
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
