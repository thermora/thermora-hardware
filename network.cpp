#include "network.h"
#include "config.h"

NetworkManager::NetworkManager() : wifiConnected(false), lastReconnectAttempt(0) {
}

bool NetworkManager::connectWiFi() {
  if (WiFi.status() == WL_CONNECTED) {
    wifiConnected = true;
    return true;
  }
  
  Serial.print("Connecting to WiFi: ");
  Serial.println(WIFI_SSID);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  unsigned long startTime = millis();
  while (WiFi.status() != WL_CONNECTED && (millis() - startTime) < WIFI_CONNECT_TIMEOUT) {
    delay(500);
    Serial.print(".");
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    wifiConnected = true;
    Serial.println();
    Serial.print("WiFi connected! IP: ");
    Serial.println(WiFi.localIP());
    Serial.print("RSSI: ");
    Serial.println(WiFi.RSSI());
    return true;
  } else {
    wifiConnected = false;
    Serial.println();
    Serial.println("ERROR: WiFi connection failed");
    return false;
  }
}

bool NetworkManager::isConnected() {
  return WiFi.status() == WL_CONNECTED;
}

void NetworkManager::checkConnection() {
  if (!isConnected()) {
    wifiConnected = false;
    unsigned long now = millis();
    if (now - lastReconnectAttempt > RECONNECT_INTERVAL) {
      lastReconnectAttempt = now;
      connectWiFi();
    }
  } else {
    wifiConnected = true;
  }
}

int NetworkManager::getRSSI() {
  if (isConnected()) {
    return WiFi.RSSI();
  }
  return 0;
}

void NetworkManager::disconnect() {
  WiFi.disconnect();
  wifiConnected = false;
}

String NetworkManager::createJSONPayload(const ReadingData& data) {
  StaticJsonDocument<512> doc;
  
  doc["bus_id"] = data.busId;
  doc["timestamp"] = data.timestamp;
  doc["latitude"] = data.latitude;
  doc["longitude"] = data.longitude;
  doc["temperature"] = data.temperature;
  doc["humidity"] = data.humidity;
  doc["pressure"] = data.pressure;
  
  String jsonString;
  serializeJson(doc, jsonString);
  return jsonString;
}

bool NetworkManager::performHTTPRequest(const String& jsonPayload) {
  HTTPClient http;
  
  http.begin(API_ENDPOINT);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("X-API-Key", API_KEY);
  http.setTimeout(API_TIMEOUT);
  
  int httpResponseCode = http.POST(jsonPayload);
  
  bool success = false;
  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    
    if (httpResponseCode == 200 || httpResponseCode == 201) {
      success = true;
      String response = http.getString();
      Serial.println("Response: " + response);
    } else {
      Serial.print("ERROR: HTTP error code ");
      Serial.println(httpResponseCode);
    }
  } else {
    Serial.print("ERROR: HTTP request failed: ");
    Serial.println(http.errorToString(httpResponseCode));
  }
  
  http.end();
  return success;
}

bool NetworkManager::sendDataToAPI(const ReadingData& data) {
  if (!isConnected()) {
    Serial.println("ERROR: WiFi not connected, cannot send data");
    return false;
  }
  
  String jsonPayload = createJSONPayload(data);
  Serial.println("Sending data to API:");
  Serial.println(jsonPayload);
  
  for (int attempt = 0; attempt < API_RETRY_COUNT; attempt++) {
    if (attempt > 0) {
      Serial.print("Retry attempt ");
      Serial.print(attempt);
      Serial.println("...");
      delay(1000);
    }
    
    if (performHTTPRequest(jsonPayload)) {
      return true;
    }
  }
  
  Serial.println("ERROR: Failed to send data after all retries");
  return false;
}

