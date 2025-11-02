#ifndef GPS_H
#define GPS_H

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>

struct GPSData {
  double latitude;
  double longitude;
  bool valid;
  bool hasFix;
  unsigned long time;
  int satellites;
};

class GPSModule {
public:
  GPSModule();
  bool initGPS();
  GPSData getCoordinates();
  double calculateDistance(double lat1, double lon1, double lat2, double lon2);
  String getGPSTime();
  void update();
  bool hasFix();

private:
  SoftwareSerial gpsSerial;
  TinyGPSPlus gps;
  bool initialized;
  GPSData currentData;
  
  static const double EARTH_RADIUS_KM;
};

#endif

