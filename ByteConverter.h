#ifndef BYTECONVERTER_H
#define BYTECONVERTER_H

#include <Arduino.h>

/**
 * Helper for converting to and from raw bytes
 */
class ByteConverter
{
public:
    static float toFloat(const uint8_t* bytes);
    static void toBytes(float value, uint8_t* buf);
    static int toInt(const uint8_t* bytes);
    static void toBytes(int value, uint8_t* buf);
};

#endif //BYTECONVERTER_H
