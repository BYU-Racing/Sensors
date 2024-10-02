#include "ByteConverter.h"

float ByteConverter::toFloat(const uint8_t* bytes)
{
    float value;
    uint8_t* valueBytes = reinterpret_cast<uint8_t*>(&value);
    for (size_t i = 0; i < sizeof(float); i++)
    {
        valueBytes[i] = bytes[i];
    }
    return value;
}

void ByteConverter::toBytes(float value, uint8_t* buf)
{
    const uint8_t* valueBytes = reinterpret_cast<uint8_t*>(&value);
    for (size_t i = 0; i < sizeof(float); i++)
    {
        buf[i] = valueBytes[i];
    }
}

int ByteConverter::toInt(const uint8_t* bytes)
{
    int value;
    uint8_t* valueBytes = reinterpret_cast<uint8_t*>(&value);
    for (size_t i = 0; i < sizeof(int); i++)
    {
        valueBytes[i] = bytes[i];
    }
    return value;
}

void ByteConverter::toBytes(int value, uint8_t* buf)
{
    const uint8_t* valueBytes = reinterpret_cast<uint8_t*>(&value);
    for (size_t i = 0; i < sizeof(int); i++)
    {
        buf[i] = valueBytes[i];
    }
}
