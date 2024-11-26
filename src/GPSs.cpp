#include "GPSs.h"
#include <Adafruit_GPS.h>
#include <FlexCAN_T4.h>

GPSs::GPSs(int id, HardwareSerial* serialIn) {
    this->id = id;
    this->wGPS = Adafruit_GPS(serialIn);
}

void GPSs::begin() {
    this->wGPS.begin(9600);
    this->wGPS.sendCommand(PMTK_API_SET_FIX_CTL_5HZ);
    this->wGPS.sendCommand(PMTK_ENABLE_WAAS);
}

Health GPSs::healthCheck() const {
    return Health::HEALTHY;
}

bool GPSs::ready() {
    char c = this->wGPS.read();
    return this->wGPS.newNMEAreceived();
}

SensorData GPSs::read() {
    SensorData sensorData = SensorData(id, 1);
    uint8_t buf[8];
    BufferPacker<8> packer;
    packer.reset();

    if(this->wGPS.parse(this->wGPS.lastNMEA())) {
        if(this->wGPS.fix) {
            Serial.println("LOCKED");
            packer.pack(float(this->wGPS.latitudeDegrees));
            packer.pack(float(abs(this->wGPS.longitudeDegrees)));
            packer.deepCopyTo(buf);

        } else {
            packer.pack(0);
            packer.deepCopyTo(buf);
        }
    }
    sensorData.setMsg(buf, 8, 0);
    return sensorData;
}