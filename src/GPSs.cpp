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
    if(this->wGPS.fix) {

        //Return a Sensor Data saying no fix
    }

    this->wGPS.latitudeDegrees;
    this->wGPS.longitudeDegrees;

    //PACK


    //SEND

}