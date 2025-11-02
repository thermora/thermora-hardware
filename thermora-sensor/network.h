#ifndef NETWORK_H
#define NETWORK_H

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

struct ReadingData {
  String busId;
  String timestamp;
  double latitude;
  double longitude;
  float temperature;
  float humidity;
  float pressure;
};

class NetworkManager {
public:
  NetworkManager();
  bool connectWiFi();
  bool isConnected();
  bool sendDataToAPI(const ReadingData& data);
  void disconnect();
  void checkConnection();
  int getRSSI();

private:
  bool wifiConnected;
  unsigned long lastReconnectAttempt;
  static const unsigned long RECONNECT_INTERVAL = 30000;  // 30 seconds
  
  String createJSONPayload(const ReadingData& data);
  bool performHTTPRequest(const String& jsonPayload);
};

#endif

