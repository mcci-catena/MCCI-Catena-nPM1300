# MCCI-Catena-nPM1300 Library

The `MCCI_Catena_nPM1300` library provides an interface to the nPM1300 power management IC, allowing users to control and monitor various features such as voltage regulation, load switches, battery charging, GPIO configuration, and LED control. This library is designed for Arduino-compatible platforms and simplifies communication with the nPM1300 via the I2C protocol.

## Features

- Control buck regulators (enable/disable, set output voltage).
- Configure load switches and LDO regulators.
- Measure system parameters like:
  - VBAT (Battery Voltage)
  - VBUS (Input Voltage)
  - VSYS (System Voltage)
  - Temperature
- Control LED modes and states.
- Manage battery charger settings, including current limits and termination voltages.
- Configure GPIOs:
  - Set GPIO modes.
  - Configure drive strength, pull-up, pull-down, and open-drain.
  - Enable/disable debounce functionality.
  - Retrieve GPIO status.
- Comprehensive error handling and state management.

## Installation

1. Download or clone this repository.
2. Copy the `MCCI_Catena_nPM1300` folder into your Arduino `libraries` directory.
3. Restart the Arduino IDE to recognize the library.

## Getting Started

### Include the Library
Add the library to your sketch:
```cpp
#include <MCCI_Catena_nPM1300.h>
```

### Initialize the Device
```cpp
#include <Wire.h>
#include <MCCI_Catena_nPM1300.h>

McciCatenaNpm1300::cNPM1300 npm1300;

void setup() {
    Wire.begin();
    if (npm1300.begin()) {
        Serial.println("nPM1300 initialized successfully.");
    } else {
        Serial.println("Failed to initialize nPM1300.");
    }
}

void loop() {
    // Example logic here
}
```

## API Reference

### Initialization and State
- **`bool begin()`**: Initialize the nPM1300 device.
- **`void end()`**: Terminate the device operation.
- **`bool reset()`**: Perform a soft reset on the nPM1300.

### Voltage and Current Measurement
- **`float measureVbat()`**: Measure battery voltage.
- **`float measureVbus()`**: Measure input voltage.
- **`float measureVsys()`**: Measure system voltage.
- **`float measureTemperature()`**: Measure die temperature.

### Voltage Regulators
- **`bool setBuck_1(bool state)`**: Enable or disable Buck 1.
- **`bool setBuck_2(bool state)`**: Enable or disable Buck 2.
- **`bool setBuckVoltage_1(Voltage voltage)`**: Set output voltage for Buck 1.
- **`bool setBuckVoltage_2(Voltage voltage)`**: Set output voltage for Buck 2.

### Load Switch and LDO
- **`bool setLoadSwitch_1(bool state)`**: Control Load Switch 1.
- **`bool setLoadSwitch_2(bool state)`**: Control Load Switch 2.
- **`bool selectLdoLoadSwitchMode_1(bool mode)`**: Select mode between LDO and Load Switch for LDO 1.
- **`bool selectLdoLoadSwitchMode_2(bool mode)`**: Select mode between LDO and Load Switch for LDO 2.
- **`bool setLdoVoltage_1(Voltage voltage)`**: Set voltage for LDO 1.
- **`bool setLdoVoltage_2(Voltage voltage)`**: Set voltage for LDO 2.

### LEDs
- **`bool ledSetMode(Led led, LedMode mode)`**: Set LED mode.
- **`bool ledOn(Led led)`**: Turn on an LED.
- **`bool ledOff(Led led)`**: Turn off an LED.
- **`bool ledToggle(Led led)`**: Toggle the LED state.

### GPIO Configuration
- **`bool setGpioMode(GpioMode mode, uint8_t gpioNumber)`**: Configure GPIO mode.
- **`bool setGpioDrive(GpioDrive current, uint8_t gpioNumber)`**: Set GPIO drive strength.
- **`bool setGpioPullUp(bool enable, uint8_t gpioNumber)`**: Enable or disable pull-up resistor for GPIO.
- **`bool setGpioPullDown(bool enable, uint8_t gpioNumber)`**: Enable or disable pull-down resistor for GPIO.
- **`bool setGpioOpenDrain(bool enable, uint8_t gpioNumber)`**: Enable or disable open-drain mode for GPIO.
- **`bool setGpioDebounce(bool enable, uint8_t gpioNumber)`**: Enable or disable debounce for GPIO.
- **`uint8_t getGpioStatus()`**: Retrieve the current status of GPIOs.

### Buck Regulator GPIO Control
- **`bool setBuckGpioControl(uint8_t buckNumber, uint8_t gpioNumber, bool invert)`**: Configure GPIO for Buck regulator control.

### Load Switch GPIO Control
- **`bool setLoadSwitchGpioControl_1(uint8_t gpioNumber, bool invert)`**: Configure GPIO control for Load Switch 1.
- **`bool setLoadSwitchGpioControl_2(uint8_t gpioNumber, bool invert)`**: Configure GPIO control for Load Switch 2.

### Battery Charger
- **`bool chargerOn()`**: Enable the charger.
- **`bool chargerOff()`**: Disable the charger.
- **`bool setChargeCurrent(uint16_t limit)`**: Set charge current (32-800 mA).
- **`uint8_t getChargeStatus()`**: Get the charging status.
- **`uint8_t getErrorReason()`**: Retrieve the last error reason.
- **`uint8_t getErrorSensor()`**: Retrieve error related to external sensor.

### Error Handling
- **`Error getLastError()`**: Retrieve the last error code.
- **`const char* getLastErrorName()`**: Get a human-readable error message.

## Example

### Measure VBAT and Toggle an LED
```cpp
#include <Wire.h>
#include <MCCI_Catena_nPM1300.h>

McciCatenaNpm1300::cNPM1300 npm1300;

void setup() {
    Wire.begin();
    Serial.begin(9600);

    if (!npm1300.begin()) {
        Serial.println("Failed to initialize nPM1300!");
        while (true);
    }

    float vbat = npm1300.measureVbat();
    Serial.print("Battery Voltage: ");
    Serial.println(vbat, 2);

    if (npm1300.ledToggle(McciCatenaNpm1300::cNPM1300::Led::LED_0)) {
        Serial.println("LED toggled successfully.");
    } else {
        Serial.println("Failed to toggle LED.");
    }
}

void loop() {
    // Add your logic
}
```

## Supported Devices
- nPM1300 Power Management IC

## Meta

### License

This repository is released under the [MIT](./LICENSE.md) license. Commercial licenses are also available from MCCI Corporation.

### Support Open Source Hardware and Software

MCCI invests time and resources providing this open source code, please support MCCI and open-source hardware by purchasing products from MCCI, Adafruit and other open-source hardware/software vendors!

For information about MCCI's products, please visit [store.mcci.com](https://store.mcci.com/).

### Trademarks

MCCI and MCCI Catena are registered trademarks of MCCI Corporation. All other marks are the property of their respective owners.