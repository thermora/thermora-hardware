#include "sensor.h"
#include "config.h"

Sensor::Sensor() : initialized(false) {
}

bool Sensor::initSensor() {
  Wire.begin(BME280_SDA, BME280_SCL);
  
  if (!bme.begin(BME280_I2C_ADDRESS)) {
    Serial.println("ERROR: BME280 sensor not found!");
    Serial.println("Try changing BME280_I2C_ADDRESS to 0x77 in config.h");
    return false;
  }
  
  bme.setSampling(Adafruit_BME280::MODE_NORMAL,
                  Adafruit_BME280::SAMPLING_X1,
                  Adafruit_BME280::SAMPLING_X1,
                  Adafruit_BME280::SAMPLING_X1,
                  Adafruit_BME280::FILTER_OFF);
  
  initialized = true;
  Serial.println("OK: BME280 sensor initialized");
  return true;
}

SensorData Sensor::readData() {
  return readWithRetry();
}

SensorData Sensor::readWithRetry() {
  SensorData data;
  data.valid = false;
  
  for (int i = 0; i < SENSOR_READ_RETRIES; i++) {
    if (isConnected()) {
      data.temperature = bme.readTemperature();
      data.humidity = bme.readHumidity();
      data.pressure = bme.readPressure() / 100.0F;  // Convert to hPa
      
      if (!isnan(data.temperature) && !isnan(data.humidity) && !isnan(data.pressure)) {
        data.valid = true;
        return data;
      }
    }
    
    if (i < SENSOR_READ_RETRIES - 1) {
      delay(SENSOR_READ_DELAY);
    }
  }
  
  Serial.println("ERROR: Failed to read sensor data after retries");
  return data;
}

float Sensor::readTemperature() {
  SensorData data = readData();
  return data.temperature;
}

float Sensor::readHumidity() {
  SensorData data = readData();
  return data.humidity;
}

float Sensor::readPressure() {
  SensorData data = readData();
  return data.pressure;
}

bool Sensor::isConnected() {
  return initialized;
}

