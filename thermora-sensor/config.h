#ifndef CONFIG_H
#define CONFIG_H

// WiFi Configuration
#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"
#define WIFI_CONNECT_TIMEOUT 30000  // 30 seconds

// API Configuration
#define API_ENDPOINT "https://api.thermora.example.com/v1/readings"
#define API_KEY "YOUR_API_KEY_HERE"
#define API_TIMEOUT 10000  // 10 seconds
#define API_RETRY_COUNT 3

// Bus Configuration
#define BUS_ID "BUS_001"

// Measurement Configuration
#define DISTANCE_TRIGGER_METERS 600  // Measure every 600 meters
#define READING_BUFFER_SIZE 100      // Buffer up to 100 readings

// Hardware Pins
#define BME280_SDA 21
#define BME280_SCL 22
#define BME280_I2C_ADDRESS 0x76  // Some modules use 0x77

#define GPS_TX 16
#define GPS_RX 17

#define LED_POWER 2
#define LED_WIFI 4
#define LED_GPS 5
#define LED_ERROR 18

// Sensor Configuration
#define SENSOR_READ_RETRIES 3
#define SENSOR_READ_DELAY 100  // ms between retries

// GPS Configuration
#define GPS_BAUD_RATE 9600
#define GPS_UPDATE_INTERVAL 1000  // Update GPS every 1 second
#define GPS_FIX_TIMEOUT 30000     // 30 seconds to get fix

// LED Blink Intervals (ms)
#define LED_BLINK_FAST 200
#define LED_BLINK_SLOW 1000

// Watchdog Configuration
#define WATCHDOG_TIMEOUT 60000  // 60 seconds

#endif

