#include "RotationVectors.h"

#include <cmath>
#include "Arduino.h"

float quaternionToRoll(const float q0, const float q1, const float q2, const float q3)
{
    // Combined Sine of Roll and Cosine of Pitch
    const float sinRcosP = 2.0f * (q0 * q1 + q2 * q3);
    // Cosine of Roll and Cosine of Pitch
    const float cosRcosP = 1.0f - 2.0f * (q1 * q1 + q2 * q2);
    // Roll (x-axis) in degrees
    return atan2f(sinRcosP, cosRcosP) * TO_DEG;
}

float quaternionToPitch(const float q0, const float q1, const float q2, const float q3)
{
    // Sine of Pitch
    const float sinP = 2.0f * (q0 * q2 - q3 * q1);
    // Floating Point Rounding Check
    if (fabsf(sinP) >= 1)
    {
        // Clamp pitch (y-axis) to ± 90 degrees
        return static_cast<float>(copysign(M_PI / 2, sinP)) * TO_DEG;
    }
    // Pitch (y-axis) in degrees
    return asinf(sinP) * TO_DEG;
}

float quaternionToYaw(const float q0, const float q1, const float q2, const float q3)
{
    // Combined Sine of Yaw and Cosine of Pitch
    const float sinYcosP = 2.0f * (q0 * q3 + q1 * q2);
    // Cosine of Yaw and Cosine of Pitch
    const float cosYcosP = 1.0f - 2.0f * (q2 * q2 + q3 * q3);
    // Yaw (z-axis) in degrees
    return atan2f(sinYcosP, cosYcosP) * TO_DEG;
}

euler_t quaternionToEuler(const float qr, const float qi, const float qj, const float qk)
{
    // Normalize quaternion

    // The magnitude of the quaternion (Euclidean norm)
    const float norm = 1.0f / sqrtf(qr * qr + qi * qi + qj * qj + qk * qk);
    // Real scalar part of the quaternion
    const float q0 = qr * norm;
    // Imaginary vector part of the quaternion
    const float q1 = qi * norm;
    // Imaginary vector part of the quaternion
    const float q2 = qj * norm;
    // Imaginary vector part of the quaternion
    const float q3 = qk * norm;

    // Compute Euler Angles

    // X-axis
    const float roll = quaternionToRoll(q0, q1, q2, q3);
    // Y-axis
    const float pitch = quaternionToPitch(q0, q1, q2, q3);
    // Z-axis
    const float yaw = quaternionToYaw(q0, q1, q2, q3);

    return euler_t { yaw, pitch, roll };
}
