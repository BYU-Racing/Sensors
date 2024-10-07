#include "BufferPacker.h"

void BufferPacker::uint32ToBytes(const uint32_t value, uint8_t* buf)
{
    buf[0] = static_cast<uint8_t>(value >> 24 & 0xFF);
    buf[1] = static_cast<uint8_t>(value >> 16 & 0xFF);
    buf[2] = static_cast<uint8_t>(value >> 8 & 0xFF);
    buf[3] = static_cast<uint8_t>(value & 0xFF);
}

uint32_t BufferPacker::uint32FromBytes(const uint8_t* bytes)
{
    return static_cast<uint32_t>(bytes[0]) << 24 |
        static_cast<uint32_t>(bytes[1]) << 16 |
        static_cast<uint32_t>(bytes[2]) << 8 |
        static_cast<uint32_t>(bytes[3]);
}

void BufferPacker::floatToBytes(const float value, uint8_t* buf)
{
    uint32_t intValue;
    memcpy(&intValue, &value, sizeof(float));
    uint32ToBytes(intValue, buf);
}

float BufferPacker::floatFromBytes(const uint8_t* bytes)
{
    const uint32_t intValue = uint32FromBytes(bytes);
    float floatValue;
    memcpy(&floatValue, &intValue, sizeof(float));
    return floatValue;
}

void BufferPacker::intToBytes(const int value, uint8_t* buf)
{
    uint32ToBytes(static_cast<uint32_t>(value), buf);
}

int BufferPacker::intFromBytes(const uint8_t* bytes)
{
    return static_cast<int>(uint32FromBytes(bytes));
}

void BufferPacker::packInt(uint8_t* buf, const int value, const uint8_t id)
{
    if (id == 0xFF)
    {
        intToBytes(value, buf);
    } else
    {
        buf[0] = id;
        intToBytes(value, &buf[1]);
    }
}

int BufferPacker::unpackInt(const uint8_t* buf, const bool hasId, uint8_t* id)
{
    uint8_t bytes[sizeof(int)];
    if (hasId)
    {
        if (id != nullptr) { *id = buf[0]; }
        for (size_t i = 1; i < sizeof(int) + 1; i++)
        {
            bytes[i - 1] = buf[i]; // All bytes after id bytes
        }
    } else
    {
        for (size_t i = 0; i < sizeof(int); i++)
        {
            bytes[i] = buf[i];
        }
    }
    return intFromBytes(bytes);
}


void BufferPacker::packFloat(uint8_t* buf, const float value, const uint8_t id)
{
    if (id == 0xFF)
    {
        floatToBytes(value, buf);
    } else
    {
        buf[0] = id;
        floatToBytes(value, &buf[1]);
    }
}

float BufferPacker::unpackFloat(const uint8_t* buf, const bool hasId, uint8_t* id)
{
    uint8_t bytes[sizeof(float)];
    if (hasId)
    {
        if (id != nullptr) { *id = buf[0]; }
        for (size_t i = 1; i < sizeof(float) + 1; i++)
        {
            bytes[i - 1] = buf[i]; // All bytes after id bytes
        }
    } else
    {
        for (size_t i = 0; i < sizeof(float); i++)
        {
            bytes[i] = buf[i];
        }
    }
    return floatFromBytes(bytes);
}
