# Thermora Hardware Documentation

Complete hardware assembly guide for the Thermora sensor system.

## Component List

### Main Components

1. **ESP32 DevKit V1**
   - Microcontroller with WiFi and Bluetooth
   - Operating voltage: 3.3V
   - Input voltage: 5V (via USB) or 7-12V (via VIN)
   - Price: ~R$ 35,00

2. **BME280 Environmental Sensor**
   - Temperature range: -40°C to +85°C
   - Humidity range: 0-100% RH
   - Pressure range: 300-1100 hPa
   - Interface: I2C (address 0x76 or 0x77)
   - Operating voltage: 3.3V
   - Price: ~R$ 15,00

3. **NEO-6M GPS Module**
   - UART interface
   - Baud rate: 9600 (default)
   - Operating voltage: 3.3V (with 3.3V regulator)
   - Active antenna recommended
   - Price: ~R$ 25,00

### Supporting Components

4. **LEDs (4x)**
   - 5mm standard LEDs
   - Colors: Power (blue), WiFi (green), GPS (yellow), Error (red)
   - Price: ~R$ 2,00

5. **Resistors (4x 220Ω)**
   - For LED current limiting
   - Price: ~R$ 1,00

6. **Breadboard**
   - Half-size or full-size breadboard
   - Price: ~R$ 5,00

7. **Jumper Wires**
   - Male-to-male jumper wires
   - Male-to-female for connections
   - Price: ~R$ 5,00

8. **Power Supply**
   - 5V USB power bank or adapter
   - Minimum 500mA output
   - USB-A to Micro-USB cable
   - Price: ~R$ 15,00

9. **Enclosure**
   - Weather-resistant box (IP65 recommended)
   - Size: ~120x80x50mm minimum
   - Clear window for GPS antenna
   - Price: ~R$ 20,00

**Total Hardware Cost: ~R$ 123,00**

## Wiring Diagram

### Power Connections
```
5V Power Source (USB)
  └── ESP32 VIN (5V input)
  └── ESP32 GND (Ground)
```

### BME280 Sensor (I2C)
```
BME280 VCC  → ESP32 3.3V
BME280 GND  → ESP32 GND
BME280 SDA  → ESP32 GPIO 21
BME280 SCL  → ESP32 GPIO 22
```

### NEO-6M GPS Module (UART)
```
GPS VCC  → ESP32 3.3V
GPS GND  → ESP32 GND
GPS TX   → ESP32 GPIO 17 (RX)
GPS RX   → ESP32 GPIO 16 (TX)
```

Note: GPS TX connects to ESP32 RX, GPS RX connects to ESP32 TX.

### Status LEDs
```
Power LED:
  Anode → ESP32 GPIO 2 → 220Ω resistor → LED → GND

WiFi LED:
  Anode → ESP32 GPIO 4 → 220Ω resistor → LED → GND

GPS LED:
  Anode → ESP32 GPIO 5 → 220Ω resistor → LED → GND

Error LED:
  Anode → ESP32 GPIO 18 → 220Ω resistor → LED → GND
```

### Common Ground
All components must share a common ground connection. Connect all GND pins together.

## Complete Wiring Table

| Component | Pin/Function | ESP32 Connection | Notes |
|-----------|--------------|-----------------|-------|
| BME280 | VCC | 3.3V | Power |
| BME280 | GND | GND | Ground |
| BME280 | SDA | GPIO 21 | I2C Data |
| BME280 | SCL | GPIO 22 | I2C Clock |
| GPS | VCC | 3.3V | Power |
| GPS | GND | GND | Ground |
| GPS | TX | GPIO 17 | GPS transmit → ESP32 receive |
| GPS | RX | GPIO 16 | GPS receive ← ESP32 transmit |
| Power LED | Anode | GPIO 2 → 220Ω → LED → GND | Status indicator |
| WiFi LED | Anode | GPIO 4 → 220Ω → LED → GND | Status indicator |
| GPS LED | Anode | GPIO 5 → 220Ω → LED → GND | Status indicator |
| Error LED | Anode | GPIO 18 → 220Ω → LED → GND | Status indicator |

## Assembly Steps

### Step 1: Prepare Breadboard
1. Place ESP32 DevKit on breadboard
2. Ensure it's centered and stable
3. Note pin positions for reference

### Step 2: Connect Power
1. Connect 5V power source to ESP32 VIN pin
2. Connect ground to ESP32 GND pin
3. Verify power LED lights up (if built-in)

### Step 3: Install BME280 Sensor
1. Place BME280 module on breadboard
2. Connect VCC to ESP32 3.3V
3. Connect GND to ESP32 GND
4. Connect SDA to GPIO 21
5. Connect SCL to GPIO 22
6. Double-check I2C address (most modules use 0x76)

### Step 4: Install GPS Module
1. Place NEO-6M module on breadboard
2. Connect VCC to ESP32 3.3V
3. Connect GND to ESP32 GND
4. Connect GPS TX to ESP32 GPIO 17
5. Connect GPS RX to ESP32 GPIO 16
6. Ensure GPS antenna is connected and positioned upward

### Step 5: Install Status LEDs
1. For each LED:
   - Connect anode (long leg) through 220Ω resistor to GPIO pin
   - Connect cathode (short leg) to GND
2. Power LED: GPIO 2
3. WiFi LED: GPIO 4
4. GPS LED: GPIO 5
5. Error LED: GPIO 18

### Step 6: Verify Connections
1. Check all power connections (VCC, GND)
2. Verify I2C connections (SDA, SCL)
3. Check UART connections (TX, RX)
4. Verify LED polarity (anode/cathode)
5. Ensure all components share common ground

### Step 7: Initial Test
1. Connect USB cable to ESP32
2. Open Serial Monitor (115200 baud)
3. Upload firmware
4. Verify initialization messages
5. Check LED status indicators

## Enclosure Recommendations

### Requirements
- Weather-resistant (IP65 or better)
- Clear window or opening for GPS antenna
- Ventilation for sensor accuracy
- Mounting points for bus installation
- Access to USB port for power

### Suggested Enclosure
- Material: ABS plastic or polycarbonate
- Size: 120mm x 80mm x 50mm minimum
- Features:
  - Gasket-sealed lid
  - Clear top panel for GPS
  - Cable gland for power cable
  - Mounting brackets or adhesive pad
  - LED indicators visible through transparent sections

### Installation Considerations
- Mount with GPS antenna facing upward
- Ensure good airflow around BME280 sensor
- Protect from direct sunlight (affects temperature readings)
- Secure mounting to prevent vibration damage
- Keep power cable accessible

## Bus Installation Guide

### Pre-Installation Checklist
- [ ] All components tested and working
- [ ] Firmware uploaded and configured
- [ ] WiFi credentials set correctly
- [ ] API endpoint and key configured
- [ ] Bus ID assigned
- [ ] Enclosure properly sealed

### Installation Location
Recommended locations on bus:
1. **Interior roof** (best for GPS signal)
   - Center of bus interior
   - Clear view of sky through windows
   - Protected from passengers

2. **Exterior roof** (if weatherproof)
   - Maximum GPS signal strength
   - Better environmental readings
   - Requires IP67+ enclosure

3. **Driver's cabin** (alternative)
   - Protected location
   - Easy access for maintenance
   - May have reduced GPS signal

### Mounting Steps
1. **Choose mounting location**
   - Ensure GPS antenna has sky view
   - Avoid metal obstructions
   - Consider passenger safety

2. **Prepare surface**
   - Clean mounting area
   - Mark drill holes if needed
   - Ensure surface is flat and stable

3. **Install enclosure**
   - Use provided mounting brackets or adhesive
   - Ensure secure attachment
   - Verify enclosure is sealed

4. **Route power cable**
   - Connect to bus 5V/12V system via USB adapter
   - Secure cable to prevent damage
   - Ensure connection is weatherproof

5. **Final verification**
   - Power on device
   - Check LED indicators
   - Verify Serial Monitor output
   - Test GPS fix acquisition
   - Confirm WiFi connection

### Maintenance
- Regular visual inspection (monthly)
- Check LED status indicators
- Verify GPS fix acquisition time
- Monitor API transmission success rate
- Clean enclosure and GPS antenna (quarterly)
- Check power connection security

## Power Considerations

### Power Requirements
- ESP32: ~80-240mA (peak ~500mA during WiFi transmission)
- BME280: ~3.6µA (standby), ~338µA (active)
- GPS: ~45mA
- LEDs: ~20mA each (4x = 80mA)
- **Total: ~200-400mA typical, ~600mA peak**

### Power Supply Options
1. **USB Power Bank** (recommended for testing)
   - Portable and easy to test
   - Minimum 5000mAh for all-day operation
   - USB-A output with Micro-USB cable

2. **Bus 12V System** (production)
   - 12V to 5V USB adapter
   - Minimum 1A output
   - Fused connection recommended

3. **Solar Power** (future enhancement)
   - 5V solar panel with battery backup
   - Charge controller required
   - Weatherproof battery enclosure

## Troubleshooting Hardware Issues

### No Power
- Check USB cable connection
- Verify power supply output (use multimeter)
- Check for short circuits
- Verify ESP32 VIN pin connection

### Sensor Not Detected
- Verify I2C wiring (SDA/SCL)
- Check power to BME280 (3.3V)
- Try different I2C address (0x77 vs 0x76)
- Check for loose connections

### GPS Not Working
- Verify TX/RX connections (may be swapped)
- Check GPS power (should have LED indicator)
- Ensure antenna is connected
- Move to location with sky view
- Wait 5-10 minutes for cold start

### LEDs Not Working
- Check LED polarity (anode/cathode)
- Verify resistor values (220Ω)
- Check GPIO pin connections
- Test LED with direct power (with resistor)

### WiFi Issues
- ESP32 requires 2.4GHz WiFi (not 5GHz)
- Check signal strength
- Verify SSID and password
- Try different WiFi network for testing

## Safety Considerations

- Use appropriate fuse for bus power connection
- Ensure all connections are secure
- Protect from water ingress
- Avoid short circuits
- Use proper wire gauge for power connections
- Follow bus electrical system guidelines
- Ensure enclosure doesn't obstruct passenger areas

