#include "gps.h"
#include "config.h"

const double GPSModule::EARTH_RADIUS_KM = 6371.0;

GPSModule::GPSModule() : gpsSerial(GPS_RX, GPS_TX), initialized(false) {
  currentData.valid = false;
  currentData.hasFix = false;
}

bool GPSModule::initGPS() {
  gpsSerial.begin(GPS_BAUD_RATE);
  initialized = true;
  Serial.println("OK: GPS module initialized");
  return true;
}

void GPSModule::update() {
  while (gpsSerial.available() > 0) {
    gps.encode(gpsSerial.read());
  }
  
  if (gps.location.isValid()) {
    currentData.latitude = gps.location.lat();
    currentData.longitude = gps.location.lng();
    currentData.valid = true;
    currentData.hasFix = true;
    currentData.satellites = gps.satellites.value();
    
    if (gps.time.isValid()) {
      currentData.time = gps.time.value();
    }
  } else {
    currentData.hasFix = false;
    currentData.valid = false;
  }
}

GPSData GPSModule::getCoordinates() {
  update();
  return currentData;
}

bool GPSModule::hasFix() {
  update();
  return currentData.hasFix;
}

double GPSModule::calculateDistance(double lat1, double lon1, double lat2, double lon2) {
  double dLat = radians(lat2 - lat1);
  double dLon = radians(lon2 - lon1);
  
  double a = sin(dLat / 2) * sin(dLat / 2) +
             cos(radians(lat1)) * cos(radians(lat2)) *
             sin(dLon / 2) * sin(dLon / 2);
  
  double c = 2 * atan2(sqrt(a), sqrt(1 - a));
  double distance = EARTH_RADIUS_KM * c;
  
  return distance * 1000.0;  // Return in meters
}

String GPSModule::getGPSTime() {
  if (!gps.time.isValid() || !gps.date.isValid()) {
    return "";
  }
  
  char timeStr[25];
  snprintf(timeStr, sizeof(timeStr), "%04d-%02d-%02dT%02d:%02d:%02dZ",
           gps.date.year(),
           gps.date.month(),
           gps.date.day(),
           gps.time.hour(),
           gps.time.minute(),
           gps.time.second());
  
  return String(timeStr);
}

