#include "GPSs.h"
#include <Adafruit_GPS.h>

GPSs::GPSs(ReservedIDs id, HardwareSerial* serialIn) {
    this->id = id;
    this->wGPS = Adafruit_GPS(serialIn);
}

void GPSs::begin() {
    this->wGPS.begin(9600);
    this->wGPS.sendCommand(PMTK_API_SET_FIX_CTL_5HZ);
    this->wGPS.sendCommand(PMTK_ENABLE_WAAS);
}

bool GPSs::ready() {
    this->wGPS.read(); // TODO: See if we can throw this away
    return this->wGPS.newNMEAreceived();
}

SensorData GPSs::read() {
    SensorData sensorData = SensorData(id, 1);
    uint8_t buf[8];
    BufferPacker<8> packer;

    if(this->wGPS.fix) {
        packer.pack(float(this->wGPS.latitudeDegrees));
        packer.pack(float(this->wGPS.longitudeDegrees));
        packer.deepCopyTo(buf);
    } else {
        packer.pack(0);
        packer.deepCopyTo(buf);
    }
    return sensorData;
}