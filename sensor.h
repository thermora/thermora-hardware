#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_BME280.h>

struct SensorData {
  float temperature;
  float humidity;
  float pressure;
  bool valid;
};

class Sensor {
public:
  Sensor();
  bool initSensor();
  SensorData readData();
  float readTemperature();
  float readHumidity();
  float readPressure();
  bool isConnected();

private:
  Adafruit_BME280 bme;
  bool initialized;
  SensorData readWithRetry();
};

#endif

