# Thermora API Documentation

API endpoint documentation for the Thermora sensor data collection system.

## Base URL

```
https://api.thermora.example.com/v1
```

**Note**: Replace with your actual API endpoint URL in `config.h`.

## Authentication

All API requests require authentication using an API key sent in the request header.

### Header
```
X-API-Key: YOUR_API_KEY_HERE
```

Set your API key in `config.h`:
```cpp
#define API_KEY "YOUR_API_KEY_HERE"
```

## Endpoints

### POST /readings

Submit a sensor reading from a Thermora device.

#### Request

**URL**: `POST /readings`

**Headers**:
```
Content-Type: application/json
X-API-Key: YOUR_API_KEY_HERE
```

**Body** (JSON):
```json
{
  "bus_id": "BUS_001",
  "timestamp": "2025-11-01T14:23:45Z",
  "latitude": -23.5505,
  "longitude": -46.6333,
  "temperature": 32.5,
  "humidity": 65.2,
  "pressure": 1013.25
}
```

#### Field Descriptions

| Field | Type | Required | Description |
|-------|------|----------|-------------|
| `bus_id` | string | Yes | Unique identifier for the bus/sensor unit |
| `timestamp` | string (ISO 8601) | Yes | UTC timestamp of the measurement |
| `latitude` | float | Yes | GPS latitude in decimal degrees (-90 to 90) |
| `longitude` | float | Yes | GPS longitude in decimal degrees (-180 to 180) |
| `temperature` | float | Yes | Temperature in Celsius |
| `humidity` | float | Yes | Relative humidity percentage (0-100) |
| `pressure` | float | Yes | Atmospheric pressure in hectopascals (hPa) |

#### Response

**Success (200 OK)**:
```json
{
  "status": "success",
  "message": "Reading saved",
  "id": "reading_12345",
  "timestamp": "2025-11-01T14:23:45Z"
}
```

**Success (201 Created)**:
```json
{
  "status": "created",
  "message": "Reading created",
  "id": "reading_12345",
  "timestamp": "2025-11-01T14:23:45Z"
}
```

**Error (400 Bad Request)**:
```json
{
  "status": "error",
  "error_code": "VALIDATION_ERROR",
  "message": "Invalid field: temperature must be between -40 and 85",
  "fields": ["temperature"]
}
```

**Error (401 Unauthorized)**:
```json
{
  "status": "error",
  "error_code": "UNAUTHORIZED",
  "message": "Invalid or missing API key"
}
```

**Error (429 Too Many Requests)**:
```json
{
  "status": "error",
  "error_code": "RATE_LIMIT_EXCEEDED",
  "message": "Rate limit exceeded. Try again in 60 seconds."
}
```

**Error (500 Internal Server Error)**:
```json
{
  "status": "error",
  "error_code": "SERVER_ERROR",
  "message": "Internal server error. Please try again later."
}
```

## Error Codes

| HTTP Code | Error Code | Description |
|-----------|------------|-------------|
| 400 | `VALIDATION_ERROR` | Invalid request data or missing required fields |
| 401 | `UNAUTHORIZED` | Missing or invalid API key |
| 403 | `FORBIDDEN` | API key valid but insufficient permissions |
| 404 | `NOT_FOUND` | Endpoint not found |
| 429 | `RATE_LIMIT_EXCEEDED` | Too many requests, rate limit exceeded |
| 500 | `SERVER_ERROR` | Internal server error |
| 503 | `SERVICE_UNAVAILABLE` | Service temporarily unavailable |

## Request Examples

### cURL Example
```bash
curl -X POST https://api.thermora.example.com/v1/readings \
  -H "Content-Type: application/json" \
  -H "X-API-Key: YOUR_API_KEY_HERE" \
  -d '{
    "bus_id": "BUS_001",
    "timestamp": "2025-11-01T14:23:45Z",
    "latitude": -23.5505,
    "longitude": -46.6333,
    "temperature": 32.5,
    "humidity": 65.2,
    "pressure": 1013.25
  }'
```

### Python Example
```python
import requests
import json

url = "https://api.thermora.example.com/v1/readings"
headers = {
    "Content-Type": "application/json",
    "X-API-Key": "YOUR_API_KEY_HERE"
}
data = {
    "bus_id": "BUS_001",
    "timestamp": "2025-11-01T14:23:45Z",
    "latitude": -23.5505,
    "longitude": -46.6333,
    "temperature": 32.5,
    "humidity": 65.2,
    "pressure": 1013.25
}

response = requests.post(url, headers=headers, json=data)
print(response.json())
```

### JavaScript Example
```javascript
const url = "https://api.thermora.example.com/v1/readings";
const headers = {
    "Content-Type": "application/json",
    "X-API-Key": "YOUR_API_KEY_HERE"
};
const data = {
    bus_id: "BUS_001",
    timestamp: "2025-11-01T14:23:45Z",
    latitude: -23.5505,
    longitude: -46.6333,
    temperature: 32.5,
    humidity: 65.2,
    pressure: 1013.25
};

fetch(url, {
    method: "POST",
    headers: headers,
    body: JSON.stringify(data)
})
.then(response => response.json())
.then(data => console.log(data))
.catch(error => console.error("Error:", error));
```

## Data Validation

### Temperature
- Range: -40.0 to 85.0 °C
- Precision: 1 decimal place
- Invalid values will be rejected

### Humidity
- Range: 0.0 to 100.0 %
- Precision: 1 decimal place
- Values outside range will be rejected

### Pressure
- Range: 300.0 to 1100.0 hPa
- Precision: 2 decimal places
- Values outside typical atmospheric range will trigger warning

### Coordinates
- Latitude: -90.0 to 90.0
- Longitude: -180.0 to 180.0
- Precision: 4-6 decimal places recommended
- Invalid coordinates (e.g., 0,0) may be flagged

### Timestamp
- Format: ISO 8601 (YYYY-MM-DDTHH:MM:SSZ)
- Must be UTC timezone
- Future timestamps (> current time + 5 minutes) will be rejected
- Old timestamps (> 24 hours) may trigger warning

### Bus ID
- Format: Alphanumeric string
- Recommended: "BUS_" prefix followed by identifier
- Case-sensitive
- Maximum length: 50 characters

## Rate Limiting

API requests are rate-limited to prevent abuse:

- **Default limit**: 100 requests per minute per API key
- **Burst limit**: 10 requests per second
- **Response**: 429 Too Many Requests when exceeded
- **Retry-After header**: Included in 429 responses

The firmware includes automatic retry logic with exponential backoff.

## Retry Logic

The firmware implements retry logic for failed requests:

- **Retry count**: 3 attempts (configurable in `config.h`)
- **Timeout**: 10 seconds per request (configurable)
- **Behavior**: Failed requests are buffered locally
- **Buffer size**: 100 readings (configurable)

When a request fails:
1. First retry: Immediate
2. Second retry: After 1 second delay
3. Third retry: After 1 second delay
4. If all fail: Reading is buffered for later transmission

Buffered readings are automatically sent when WiFi connection is restored.

## Data Format Standards

### Timestamp Format
- **Standard**: ISO 8601
- **Format**: `YYYY-MM-DDTHH:MM:SSZ`
- **Example**: `2025-11-01T14:23:45Z`
- **Timezone**: Always UTC (Z suffix)

### Coordinate Format
- **Type**: Decimal degrees
- **Precision**: 6 decimal places recommended (meter-level accuracy)
- **Example**: `-23.550520, -46.633309`

### Numeric Precision
- **Temperature**: 1 decimal place (°C)
- **Humidity**: 1 decimal place (%)
- **Pressure**: 2 decimal places (hPa)

## Security Considerations

1. **API Key Security**
   - Never commit API keys to version control
   - Rotate keys regularly
   - Use different keys for development/production

2. **HTTPS Required**
   - All endpoints must use HTTPS
   - HTTP connections will be rejected
   - Valid SSL certificate required

3. **Input Validation**
   - All input is validated server-side
   - Malformed requests are rejected
   - Invalid data ranges trigger errors

4. **Rate Limiting**
   - Prevents abuse and DoS attacks
   - Protects server resources
   - Monitors unusual patterns

## Testing

### Test Endpoint (if available)
```
https://api.thermora.example.com/v1/test
```

Returns server status and API key validation.

### Test Request
```bash
curl -X GET https://api.thermora.example.com/v1/test \
  -H "X-API-Key: YOUR_API_KEY_HERE"
```

### Expected Response
```json
{
  "status": "ok",
  "message": "API is operational",
  "timestamp": "2025-11-01T14:23:45Z"
}
```

## Firmware Configuration

Configure API settings in `config.h`:

```cpp
#define API_ENDPOINT "https://api.thermora.example.com/v1/readings"
#define API_KEY "YOUR_API_KEY_HERE"
#define API_TIMEOUT 10000  // 10 seconds
#define API_RETRY_COUNT 3
```

## Troubleshooting

### 401 Unauthorized
- Verify API key is correct
- Check API key is set in `config.h`
- Ensure API key hasn't expired
- Verify header name is `X-API-Key`

### 400 Bad Request
- Check JSON format is valid
- Verify all required fields are present
- Check data ranges (temperature, humidity, etc.)
- Verify timestamp format (ISO 8601)

### 429 Rate Limit
- Reduce measurement frequency temporarily
- Increase `DISTANCE_TRIGGER_METERS` in config
- Wait for rate limit window to reset
- Contact API administrator for limit increase

### Connection Timeout
- Check internet connectivity
- Verify API endpoint URL is correct
- Check firewall settings
- Increase `API_TIMEOUT` if network is slow

### SSL/TLS Errors
- Verify API endpoint uses valid SSL certificate
- Check ESP32 SSL certificate bundle is updated
- Ensure HTTPS (not HTTP) is used

## Support

For API issues:
- Check Serial Monitor output for detailed error messages
- Review HTTP response codes
- Verify API endpoint is operational
- Contact API administrator with:
  - Bus ID
  - Timestamp of failure
  - HTTP response code
  - Error message from Serial Monitor

