#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>
#include "SensorData.h"

/**
 * Interface Sensors must implement
 */
class Sensor
{
protected:
    /** the sensor id */
    uint32_t id = 0;
    /** the sensor criticality */
    bool criticality = false;
    /** the interval a sensor should read at */
    uint32_t readInterval = 0;
    /** the last time a read occured */
    uint32_t lastRead = 0;
public:
    virtual ~Sensor() = default;
    /** @return the sensor id */
    [[nodiscard]] uint32_t getId() const;
    /** @return the sensor criticality */
    [[nodiscard]] bool isCritical() const;
    /**
     * Get the ready state of the sensor prior to reading
     * @return the ready state of the sensor
     */
    virtual bool ready();
    /**
     * "Fill" a sensorData object with sensor information for sending over a CAN bus
     * @return sensorData object containing the sensor data and CAN msgs
     */
    virtual SensorData read() = 0;
};

#endif //SENSOR_H
