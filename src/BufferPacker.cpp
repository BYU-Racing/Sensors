#include "BufferPacker.h"

constexpr uint8_t BYTE_MASK = 0xFF;
constexpr uint8_t OFFSET_3_BYTE = 24;
constexpr uint8_t OFFSET_2_BYTE = 16;
constexpr uint8_t OFFSET_1_BYTE = 8;

void BufferPacker::uint32ToBytes(const uint32_t value, uint8_t* buf)
{
    buf[0] = static_cast<uint8_t>(value >> OFFSET_3_BYTE & BYTE_MASK);
    buf[1] = static_cast<uint8_t>(value >> OFFSET_2_BYTE & BYTE_MASK);
    buf[2] = static_cast<uint8_t>(value >> OFFSET_1_BYTE & BYTE_MASK);
    buf[3] = static_cast<uint8_t>(value & BYTE_MASK);
}

uint32_t BufferPacker::uint32FromBytes(const uint8_t* bytes)
{
    return static_cast<uint32_t>(bytes[0]) << OFFSET_3_BYTE |
        static_cast<uint32_t>(bytes[1]) << OFFSET_2_BYTE |
        static_cast<uint32_t>(bytes[2]) << OFFSET_1_BYTE |
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
    if (id == INVALID_ID)
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
    if (id == INVALID_ID)
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
        if (id != nullptr) { *id = buf[0]; } // caller wants to store the id
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
