#include "RotationVectors.h"

constexpr float D_180 = 180.0f;
constexpr float D_360 = 360.0f;

float quaternionToRoll(const float q0, const float q1, const float q2, const float q3)
{
    // Combined Sine of Roll and Cosine of Pitch
    const float sinRcosP = 2.0f * (q0 * q1 + q2 * q3);
    // Cosine of Roll and Cosine of Pitch
    const float cosRcosP = 1.0f - 2.0f * (q1 * q1 + q2 * q2);
    // Roll (x-axis) in degrees
    return atan2(sinRcosP, cosRcosP) * F_RAD_TO_DEG;
}

float quaternionToPitch(const float q0, const float q1, const float q2, const float q3)
{
    // Sine of Pitch
    const float sinP = 2.0f * (q0 * q2 - q3 * q1);
    // Floating Point Rounding Check
    if (fabs(sinP) >= 1)
    {
        // Clamp pitch (y-axis) to ± 90 degrees
        return static_cast<float>(copysign(M_PI / 2, sinP)) * F_RAD_TO_DEG;
    }
    // Pitch (y-axis) in degrees
    return asin(sinP) * F_RAD_TO_DEG;
}

float quaternionToYaw(const float q0, const float q1, const float q2, const float q3)
{
    // Combined Sine of Yaw and Cosine of Pitch
    const float sinYcosP = 2.0f * (q0 * q3 + q1 * q2);
    // Cosine of Yaw and Cosine of Pitch
    const float cosYcosP = 1.0f - 2.0f * (q2 * q2 + q3 * q3);
    // Yaw (z-axis) in degrees
    return atan2(sinYcosP, cosYcosP) * F_RAD_TO_DEG;
}

float delta(const float newAngle, float prevAngle)
{
    // Adjust for previous angle that is outside of -180, 180 bounds
    if (prevAngle > D_180)
    {
        prevAngle -= D_360;
    } else if (prevAngle < -D_180)
    {
        prevAngle += D_360;
    }
    float delta = newAngle - prevAngle;
    // Get delta inside of -360, 360 bounds
    if (delta > D_180)
    {
        delta -= D_360;
    } else if (delta < -D_180)
    {
        delta += D_360;
    }
    return delta;
}

euler_t quaternionToEuler(const float qr, const float qi, const float qj, const float qk)
{
    // Normalize quaternion

    // The magnitude of the quaternion (Euclidean norm)
    const float norm = 1.0f / sqrt(qr * qr + qi * qi + qj * qj + qk * qk);
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