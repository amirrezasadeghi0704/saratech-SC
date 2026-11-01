# Saratech SC

### STM32 Robot Control Library

[![Platform](https://img.shields.io/badge/Platform-STM32-blue.svg)](https://www.st.com/)
[![Framework](https://img.shields.io/badge/Framework-Arduino-00979D.svg)](https://www.arduino.cc/)
[![Language](https://img.shields.io/badge/Language-C%2B%2B-orange.svg)](https://isocpp.org/)
[![Status](https://img.shields.io/badge/Status-Under%20Development-yellow.svg)]()

**Saratech SC** is an embedded control library for STM32-based robotic systems. It provides a reusable software interface for motor control, sensor acquisition, gyroscope communication, and OLED display management.

The library is derived from the original `Secondry_ST_02` project. Its purpose is to separate reusable hardware-control functionality from application-specific behavior.

## Overview

Saratech SC is designed to provide a common control layer for STM32 robot projects.

Its responsibilities include:

- Four-motor control.
- Sensor data acquisition.
- Gyroscope communication.
- OLED display management.
- Hardware initialization.
- Motion-control functions.
- Sensor calibration and configuration.

The library focuses on hardware interaction and reusable control functions.

**Application-specific algorithms do not belong in this library.**

In particular, ball detection, ball tracking, and ball-related decision-making are outside the scope of Saratech SC.

## Design Philosophy

Saratech SC follows a hardware-oriented design.

The library should:

1. Provide reusable hardware interfaces.
2. Keep hardware initialization and control in one place.
3. Expose clear functions to the application.
4. Avoid application-specific decision-making.
5. Keep sensor acquisition separate from application logic.
6. Support independent development of robot applications.

The application is responsible for deciding what the robot should do. The library provides the hardware functions needed to execute those decisions.

## Hardware Platform

The primary target is:

| Component | Description |
|---|---|
| MCU | STM32F103C8T6 |
| Development framework | Arduino |
| Motor system | Four DC motors |
| Motion control | Four-channel motor control |
| Sensor interface | GPIO and ADC |
| Motion sensor | I2C gyroscope |
| Display | I2C OLED |
| Communication | I2C |

Other STM32 devices may require pin and peripheral changes.

## Hardware Pinout

The following pin assignments are documented for the current hardware configuration.

### Motor Control

| Function | STM32 Pin |
|---|---|
| Front-right motor direction | PB12 |
| Rear-right motor direction | PB13 |
| Rear-left motor direction | PB14 |
| Front-left motor direction | PB15 |
| Front-right motor PWM | PB6 |
| Rear-right motor PWM | PB7 |
| Rear-left motor PWM | PB8 |
| Front-left motor PWM | PB9 |

### Sensor Inputs

| Function | STM32 Pin |
|---|---|
| Bottom sensor | PA1 |
| Right sensor | PA2 |
| Left sensor | PA3 |
| Kicker-related hardware input | PA4 |
| Front LDR | PA5 |
| Right LDR | PA6 |
| Rear LDR | PA7 |
| Left LDR | PB0 |

### Communication and Display

| Function | STM32 Pin |
|---|---|
| I2C SDA | PB11 |
| I2C SCL | PB10 |
| OLED I2C address | 0x3C |
| Gyroscope I2C address | 0x10 |

These assignments describe the documented board configuration. Check the source and the physical board before using a different hardware revision.

## Installation

### Method 1: Arduino IDE

1. Download or clone this repository.
2. Extract the repository.
3. Copy the `saratech_sc` library folder into the Arduino libraries directory.

Typical directory:

```text
Documents/
└── Arduino/
    └── libraries/
        └── saratech_sc/
```

4. Restart Arduino IDE.
5. Open one of the provided examples.

### Method 2: ZIP Installation

1. Download the repository as a ZIP archive.
2. Open Arduino IDE.
3. Select:

```text
Sketch
  → Include Library
  → Add .ZIP Library
```

4. Select the downloaded ZIP archive.

## Quick Start

The following example shows the basic library initialization and sensor-update flow.

```cpp
#include <saratech_sc.h>

void setup() {
    saratech.begin();
}

void loop() {
    saratech.sensor();
    saratech.printall();
}
```

This example demonstrates the basic interface. Use `Complete_Reference` for the complete and current API usage.

## API Reference

The public API is defined in:

```text
src/saratech_sc.h
```

The main implementation is located in:

```text
src/saratech_sc.cpp
```

The main API areas include:

| Category | Purpose |
|---|---|
| Initialization | Configure hardware and peripherals |
| Motor Control | Control motor direction and speed |
| Motion Control | Execute movement commands |
| Sensor Management | Read sensor data |
| Gyroscope Interface | Access orientation data |
| OLED Management | Display system information |
| Configuration | Set operating parameters |
| Calibration | Adjust sensor values |

Refer to the current header and `Complete_Reference` example for exact function signatures and parameter requirements.

## Examples

The `examples/` directory contains usage examples.

### Complete_Reference

The primary reference example.

It demonstrates the intended use of the current library API and should be treated as the reference for library behavior and application integration.

### Original_Code

If included in the repository, this example provides a reference for the original application structure.

Example availability and names depend on the current repository version.

## Application Separation

Saratech SC is a hardware-control library.

The following functions must remain outside the library:

- Ball detection.
- Ball position estimation.
- Ball tracking.
- Ball-related decision-making.
- Robot strategy.
- Application-specific autonomous behavior.

These functions belong in the main application or in independent application-level modules.

This separation keeps Saratech SC reusable across different robot projects.

## Development and Verification

The project is under development.

Before using the library in a production robot, verify:

- Compilation for the target STM32 board.
- Motor direction and PWM behavior.
- Sensor readings.
- I2C communication.
- OLED initialization.
- Gyroscope communication.
- Timing and resource usage.

A successful compilation does not prove that the hardware functions correctly.

## Repository Structure

```text
saratech_sc/
│
├── src/
│   ├── saratech_sc.h
│   └── saratech_sc.cpp
│
├── examples/
│   ├── Complete_Reference/
│   │   └── Complete_Reference.ino
│   └── ...
│
├── library.properties
└── README.md
```

This is a conceptual structure. Refer to the actual repository for the current file list.

## Contributing

Contributions are welcome.

When modifying the library:

1. Preserve the separation between hardware and application logic.
2. Do not add ball-detection functionality.
3. Keep public API changes consistent with the header.
4. Update the reference example when the API changes.
5. Verify compilation before submitting changes.
6. Document hardware requirements and behavior.

## License

No license terms are specified here. Refer to the repository's license file or contact the repository owner before redistributing or modifying the library.

## Author

**Saratech**

Repository: [saratech-SC](https://github.com/amirrezasadeghi0704/saratech-SC)

