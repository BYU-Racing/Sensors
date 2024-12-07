#ifndef ROTATIONVECTORS_H
#define ROTATIONVECTORS_H

#define D_180 180.0f
#define D_360 360.0f
/** Multiply with F_RAD_TO_DEG to convert from radians to degrees */
#define TO_DEG static_cast<float>(180.0 / M_PI)

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