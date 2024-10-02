#include "Sensor.h"

uint32_t Sensor::getId() const { return id; }

bool Sensor::isCritical() const { return criticality; }

bool Sensor::ready() { return millis() - lastRead >= readInterval; }
