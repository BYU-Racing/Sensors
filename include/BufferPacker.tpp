#ifndef BUFFERPACKER_H
#define BUFFERPACKER_H

#include <Arduino.h>

constexpr uint8_t NO_ID = 0xFF;
constexpr uint8_t MASK = 0xFF;
constexpr uint8_t BYTE_SIZE = 8;

class BufferPacker
{
public:
    /**
     * Generic function for packing any integral type (<8 bytes) and an optional ID into a bytes buffer
     * @param buf Buffer to fill with the value (and ID if provided)
     * @param offset Pointer offset for use when chaining pack() calls
     * @param value Value to store in buffer
     * @param id Optional ID to store in buffer
     */
    template <typename T>
    static void pack(uint8_t* buf, size_t* offset = nullptr, const T value, const uint8_t id = NO_ID)
    {
        const size_t TSize = sizeof(T);
        const size_t localOffset = offset != nullptr ? *offset : 0;
        if (id == NO_ID)
        {
            memcpy(buf + localOffset, &value, TSize);
        } else
        {
            buf[localOffset] = id;
            memcpy(&buf[localOffset + 1], &value, TSize);
        }
        if (offset != nullptr) { *offset += (id != NO_ID) ? TSize + 1 : TSize; }
    }
    /**
     * Generic function for packing any integral type (<8 bytes) and an optional ID into a bytes buffer
     * @param buf Buffer to fill with the value (and ID if provided)
     * @param value Value to store in buffer
     * @param id Optional ID to store in buffer
     */
    template <typename T>
    static void pack(uint8_t* buf, const T value, uint8_t id = NO_ID)
    {
        // Overload for calling without an offset
        pack(buf, value, nullptr, id);
    }
    /**
     * Generic function for unpacking any integral type (<8 bytes) and an optional ID in size into a bytes buffer
     * @param buf Buffer to extract value (and ID if provided) from
     * @param offset Pointer offset for use when chaining unpack() calls
     * @param hasId Flag for if the first byte is an ID byte
     * @param id Optional pointer to location to store ID
     * @return The desired return type declared in the template (i.e. <float>)
     */
    template <typename T>
    static T unpack(const uint8_t* buf, size_t* offset = nullptr, const bool hasId = false, uint8_t* id = nullptr)
    {
        T value {};
        const size_t TSize = sizeof(T);
        const size_t localOffset = offset != nullptr ? *offset : 0;
        if (hasId)
        {
            if (id != nullptr) { *id = buf[localOffset]; }
            memcpy(&value, &buf[localOffset + 1], TSize);
        } else
        {
            memcpy(&value, &buf[localOffset], TSize);
        }
        if (offset != nullptr) { *offset += hasId ? TSize + 1 : TSize; }
        return value;
    }
    /**
     * Generic function for unpacking any integral type (<8 bytes) and an optional ID in size into a bytes buffer
     * @param buf Buffer to extract value (and ID if provided) from
     * @param hasId Flag for if the first byte is an ID byte
     * @param id Optional pointer to location to store ID
     * @return The desired return type declared in the template (i.e. <float>)
     */
    template <typename T>
    static T unpack(const uint8_t* buf, const bool hasId = false, uint8_t* id = nullptr)
    {
        // Overload for calling without an offset
        return unpack<T>(buf, nullptr, hasId, id);
    }
};

#endif //BUFFERPACKER_H
