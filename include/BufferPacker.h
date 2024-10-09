#ifndef BUFFERPACKER_H
#define BUFFERPACKER_H

#include <Arduino.h>

/** Value to represt a subSensorId that won't be used */
constexpr uint8_t INVALID_ID = 0xFF;

class BufferPacker {
public:
    /**
     * Bit shifts and masks each byte then stores in Big-Endian format
     * @param value value to convert
     * @param buf array to store 4 Big-Endian bytes
     */
    static void uint32ToBytes(uint32_t value, uint8_t* buf);
    /**
     * Bit unshifts and combines all bytes from Big-Endian format
     * @param bytes array with 4 Big-Endian bytes
     * @return
     */
    static uint32_t uint32FromBytes(const uint8_t* bytes);
    /**
     * Reinterprets as uint32_t then stores bytes in buf
     * @param value float to convert
     * @param buf array to store 4 Big-Endian bytes
     */
    static void floatToBytes(float value, uint8_t* buf);
    /**
     * Extract uint32_t from bytes then reinterpret as float
     * @param bytes array with 4 Big-Endian bytes
     * @return
     */
    static float floatFromBytes(const uint8_t* bytes);
    /**
     * Casts to int then stores bytes in buf
     * @param value
     * @param buf
     */
    static void intToBytes(int value, uint8_t* buf);
    /**
     * Extracts uint32_t from bytes then casts to int
     * @param bytes array with 4 Big-Endian bytes
     * @return
     */
    static int intFromBytes(const uint8_t* bytes);
    /**
     * Stores an int in a buffer in Big Endian format
     * @param buf buffer to pack the int into.
     * This buffer must be sizeof(int) long (sizeof(int) + 1 if an id is *not* 0xFF)
     * @param value value to store
     * @param id [Optional] value to store in the first slot as an id. 0xFF is invalid.
     */
    static void packInt(uint8_t* buf, int value, uint8_t id = INVALID_ID);
    /**
     * Extracts an int from a Big Endian format buffer.
     * @param buf buffer to extract an int from.
     * This buffer must be sizeof(int) long (sizeof(int) + 1 if an hasId is true)
     * @param hasId [Optional] flag for if the first bytes of the buffer contains an id
     * @param id [Optional] pointer to a byte to store the id in if desired
     * @return
     */
    static int unpackInt(const uint8_t* buf, bool hasId = false, uint8_t* id = nullptr);
    /**
     * Stores a float in a buffer in Big Endian format
     * @param buf buffer to pack the float into.
     * This buffer must be sizeof(float) long (sizeof(float) + 1 if an id is *not* 0xFF)
     * @param value value to store
     * @param id [Optional] value to store in the first slot as an id. 0xFF is invalid.
     */
    static void packFloat(uint8_t* buf, float value, uint8_t id = INVALID_ID);
    /**
     * Extracts a float from a Big Endian format buffer.
     * @param buf buffer to extract a float from.
     * This buffer must be sizeof(float) long (sizeof(float) + 1 if an hasId is true)
     * @param hasId [Optional] flag for if the first bytes of the buffer contains an id
     * @param id [Optional] pointer to a float to store the id in if desired
     * @return
     */
    static float unpackFloat(const uint8_t* buf, bool hasId = false, uint8_t* id = nullptr);
};



#endif //BUFFERPACKER_H
