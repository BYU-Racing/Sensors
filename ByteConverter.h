#ifndef BYTECONVERTER_H
#define BYTECONVERTER_H

#include <Arduino.h>

/**
 * Helper for converting types to and from raw bytes
 */
class ByteConverter
{
public:
    static void uint32ToBytes(uint32_t value, uint8_t* buf);
    static uint32_t uint32FromBytes(const uint8_t* bytes);

    static void floatToBytes(float value, uint8_t* buf);
    static float floatFromBytes(const uint8_t* bytes);

    static void intToBytes(int value, uint8_t* buf);
    static int intFromBytes(const uint8_t* bytes);
};

#endif //BYTECONVERTER_H
