#ifndef ROTATIONVECTORS_H
#define ROTATIONVECTORS_H

#include <Arduino.h>

/** Struct containing Euler angles in Degrees */
struct euler_t
{
    // The x-axis angle
    float roll;
    // The y-axis angle
    float pitch;
    // The z-axis angle
    float yaw;
};

/**
 * Calculates the roll (x-axis) angle from a normalized quaternion
 * @return the roll
 */
float quaternionToRoll(float q0, float q1, float q2, float q3);

/**
 * Calculates the pitch (y-axis) angle from a normalized quaternion
 * @return the pitch
 */
float quaternionToPitch(float q0, float q1, float q2, float q3);

/**
 * Calculates the yaw (z-axis) angle from a normalized quaternion
 * @return the yaw
 */
float quaternionToYaw(float q0, float q1, float q2, float q3);

/**
 * Calculates the Euler angles from an unnormalized quaternion.
 * @return the Euler angles
 */
euler_t quaternionToEuler(float qr, float qi, float qj, float qk);

/** @return an angle normalized between [-180, 180] */
float normalize180(float angle);

/** @return an angle normalized between [-360, 360] */
float normalize360(float angle);

/**
 * Gets the change in theta between two angles in degrees and applies it
 * The angle is wrapped across the -180/180 degree boundary and normalized to [-360, 360].
 * @param newAngle expected to be in range [-180, 180]
 * @param prevAngle expected to be in range [-360, 360]
 * @return the result angle in range [-360, 360]
 */
float updateAngle(float newAngle, float prevAngle);

/** Quaternion representation of an absolute rotation vector */
struct RotationVector
{
    float i = 0.0f;
    float j = 0.0f;
    float k = 0.0f;
    float real = 0.0f;
    RotationVector() = default;
    /**
     * @tparam T Any class that has implements floats: i, j, k, and real
     * @param rv rotation vector to create from
     */
    template<typename T>
    explicit RotationVector(const T& rv)
    {
        i = rv.i;
        j = rv.j;
        k = rv.k;
        real = rv.real;
    }
};

#endif //ROTATIONVECTORS_H
