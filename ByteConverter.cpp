#include "ByteConverter.h"

/**
 * Bit unshifts and combines all bytes from Big-Endian format
 * @param bytes array with 4 Big-Endian bytes
 * @return
 */
uint32_t ByteConverter::uint32FromBytes(const uint8_t* bytes)
{
    return static_cast<uint32_t>(bytes[0]) << 24 |
        static_cast<uint32_t>(bytes[1]) << 16 |
        static_cast<uint32_t>(bytes[2]) << 8 |
        static_cast<uint32_t>(bytes[3]);
}
/**
 * Bit shifts and masks each byte then stores in Big-Endian format
 * @param value value to convert
 * @param buf array to store 4 Big-Endian bytes
 */
void ByteConverter::uint32ToBytes(const uint32_t value, uint8_t* buf)
{
    buf[0] = static_cast<uint8_t>(value >> 24 & 0xFF);
    buf[1] = static_cast<uint8_t>(value >> 16 & 0xFF);
    buf[2] = static_cast<uint8_t>(value >> 8 & 0xFF);
    buf[3] = static_cast<uint8_t>(value & 0xFF);
}
/**
 * Extract uint32_t from bytes then reinterpret as float
 * @param bytes array with 4 Big-Endian bytes
 * @return
 */
float ByteConverter::floatFromBytes(const uint8_t* bytes)
{
    const uint32_t intValue = uint32FromBytes(bytes);
    float floatValue;
    memcpy(&floatValue, &intValue, sizeof(float));
    return floatValue;
}
/**
 * Reinterprets as uint32_t then stores bytes in buf
 * @param value float to convert
 * @param buf array to store 4 Big-Endian bytes
 */
void ByteConverter::floatToBytes(const float value, uint8_t* buf)
{
    uint32_t intValue;
    memcpy(&intValue, &value, sizeof(float));
    uint32ToBytes(intValue, buf);
}
/**
 * Extracts uint32_t from bytes then casts to int
 * @param bytes array with 4 Big-Endian bytes
 * @return
 */
int ByteConverter::intFromBytes(const uint8_t* bytes)
{
    return static_cast<int>(uint32FromBytes(bytes));
}
/**
 * Casts to int then stores bytes in buf
 * @param value
 * @param buf
 */
void ByteConverter::intToBytes(const int value, uint8_t* buf)
{
    uint32ToBytes(static_cast<uint32_t>(value), buf);
}
