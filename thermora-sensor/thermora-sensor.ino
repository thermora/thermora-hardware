#include "config.h"
#include "sensor.h"
#include "gps.h"
#include "network.h"

Sensor sensor;
GPSModule gps;
NetworkManager network;

struct BufferedReading {
  ReadingData data;
  bool used;
};

BufferedReading readingBuffer[READING_BUFFER_SIZE];
int bufferIndex = 0;
bool bufferFull = false;

GPSData lastMeasurementLocation;
bool hasLastLocation = false;
unsigned long lastGPSUpdate = 0;
unsigned long lastMeasurementTime = 0;

unsigned long lastLEDUpdate = 0;
unsigned long lastErrorBlink = 0;
bool errorLEDState = false;
bool wifiLEDState = false;
bool gpsLEDState = false;
unsigned long lastWiFiBlink = 0;
unsigned long lastGPSBlink = 0;

bool errorState = false;

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n=== Thermora Sensor System ===");
  Serial.println("Initializing...");
  
  pinMode(LED_POWER, OUTPUT);
  pinMode(LED_WIFI, OUTPUT);
  pinMode(LED_GPS, OUTPUT);
  pinMode(LED_ERROR, OUTPUT);
  
  digitalWrite(LED_POWER, HIGH);
  
  if (!sensor.initSensor()) {
    Serial.println("CRITICAL: Sensor initialization failed!");
    errorState = true;
  }
  
  if (!gps.initGPS()) {
    Serial.println("CRITICAL: GPS initialization failed!");
    errorState = true;
  }
  
  for (int i = 0; i < READING_BUFFER_SIZE; i++) {
    readingBuffer[i].used = false;
  }
  
  bool wifiSuccess = network.connectWiFi();
  if (!wifiSuccess) {
    Serial.println("WARNING: WiFi connection failed, will retry");
  }
  
  Serial.println("System ready!");
  Serial.println("Waiting for GPS fix...");
}

void loop() {
  unsigned long currentTime = millis();
  
  if (currentTime - lastGPSUpdate >= GPS_UPDATE_INTERVAL) {
    gps.update();
    lastGPSUpdate = currentTime;
  }
  
  updateLEDs(currentTime);
  network.checkConnection();
  
  GPSData currentGPS = gps.getCoordinates();
  
  if (currentGPS.hasFix && currentGPS.valid) {
    if (!hasLastLocation) {
      lastMeasurementLocation = currentGPS;
      hasLastLocation = true;
      Serial.println("GPS fix acquired! Starting measurements...");
    }
    
    double distance = gps.calculateDistance(
      lastMeasurementLocation.latitude,
      lastMeasurementLocation.longitude,
      currentGPS.latitude,
      currentGPS.longitude
    );
    
    if (distance >= DISTANCE_TRIGGER_METERS) {
      Serial.print("Distance threshold reached: ");
      Serial.print(distance);
      Serial.println(" meters");
      
      performMeasurement(currentGPS);
      lastMeasurementLocation = currentGPS;
    }
  }
  
  processBufferedReadings();
  
  if (errorState) {
    errorLEDState = !errorLEDState;
    digitalWrite(LED_ERROR, errorLEDState);
    delay(100);
  }
}

void performMeasurement(GPSData gpsData) {
  Serial.println("--- Taking measurement ---");
  
  SensorData sensorData = sensor.readData();
  
  if (!sensorData.valid) {
    Serial.println("ERROR: Invalid sensor data, skipping measurement");
    errorState = true;
    return;
  }
  
  String timestamp = gps.getGPSTime();
  if (timestamp.length() == 0) {
    timestamp = getCurrentTimestamp();
  }
  
  ReadingData reading;
  reading.busId = String(BUS_ID);
  reading.timestamp = timestamp;
  reading.latitude = gpsData.latitude;
  reading.longitude = gpsData.longitude;
  reading.temperature = sensorData.temperature;
  reading.humidity = sensorData.humidity;
  reading.pressure = sensorData.pressure;
  
  Serial.print("Temperature: ");
  Serial.print(reading.temperature);
  Serial.println(" °C");
  Serial.print("Humidity: ");
  Serial.print(reading.humidity);
  Serial.println(" %");
  Serial.print("Pressure: ");
  Serial.print(reading.pressure);
  Serial.println(" hPa");
  
  bool sent = network.sendDataToAPI(reading);
  
  if (!sent) {
    Serial.println("Buffering reading for later transmission");
    bufferReading(reading);
    errorState = true;
  } else {
    errorState = false;
    Serial.println("Measurement sent successfully!");
  }
  
  lastMeasurementTime = millis();
}

void bufferReading(ReadingData data) {
  int index = bufferIndex;
  
  if (!bufferFull) {
    readingBuffer[index].data = data;
    readingBuffer[index].used = true;
    bufferIndex++;
    
    if (bufferIndex >= READING_BUFFER_SIZE) {
      bufferIndex = 0;
      bufferFull = true;
    }
  } else {
    readingBuffer[index].data = data;
    readingBuffer[index].used = true;
    bufferIndex = (bufferIndex + 1) % READING_BUFFER_SIZE;
  }
  
  Serial.print("Buffered reading. Buffer usage: ");
  Serial.print(getBufferCount());
  Serial.print("/");
  Serial.println(READING_BUFFER_SIZE);
}

int getBufferCount() {
  int count = 0;
  for (int i = 0; i < READING_BUFFER_SIZE; i++) {
    if (readingBuffer[i].used) {
      count++;
    }
  }
  return count;
}

void processBufferedReadings() {
  if (!network.isConnected()) {
    return;
  }
  
  for (int i = 0; i < READING_BUFFER_SIZE; i++) {
    if (readingBuffer[i].used) {
      Serial.println("Attempting to send buffered reading...");
      
      if (network.sendDataToAPI(readingBuffer[i].data)) {
        readingBuffer[i].used = false;
        Serial.println("Buffered reading sent successfully");
      } else {
        break;
      }
    }
  }
  
  if (getBufferCount() == 0 && bufferFull) {
    bufferFull = false;
    bufferIndex = 0;
  }
}

void updateLEDs(unsigned long currentTime) {
  digitalWrite(LED_POWER, HIGH);
  
  if (network.isConnected()) {
    digitalWrite(LED_WIFI, HIGH);
    wifiLEDState = true;
  } else {
    if (currentTime - lastWiFiBlink >= LED_BLINK_FAST) {
      wifiLEDState = !wifiLEDState;
      digitalWrite(LED_WIFI, wifiLEDState);
      lastWiFiBlink = currentTime;
    }
  }
  
  GPSData gpsData = gps.getCoordinates();
  if (gpsData.hasFix) {
    digitalWrite(LED_GPS, HIGH);
    gpsLEDState = true;
  } else {
    if (currentTime - lastGPSBlink >= LED_BLINK_SLOW) {
      gpsLEDState = !gpsLEDState;
      digitalWrite(LED_GPS, gpsLEDState);
      lastGPSBlink = currentTime;
    }
  }
  
  if (errorState) {
    if (currentTime - lastErrorBlink >= LED_BLINK_FAST) {
      errorLEDState = !errorLEDState;
      digitalWrite(LED_ERROR, errorLEDState);
      lastErrorBlink = currentTime;
    }
  } else {
    digitalWrite(LED_ERROR, LOW);
  }
}

String getCurrentTimestamp() {
  unsigned long currentTime = millis();
  unsigned long seconds = currentTime / 1000;
  unsigned long minutes = seconds / 60;
  unsigned long hours = minutes / 60;
  unsigned long days = hours / 24;
  
  seconds = seconds % 60;
  minutes = minutes % 60;
  hours = hours % 24;
  
  char timestamp[25];
  snprintf(timestamp, sizeof(timestamp), "2025-01-01T%02lu:%02lu:%02luZ", hours, minutes, seconds);
  
  return String(timestamp);
}
