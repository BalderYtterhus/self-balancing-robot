# Self-balancing robot

A two-wheeled self-balancing robot built on the ESP32-C6, with an ICM-20948 IMU over I²C.

### Implemented so far
- ESP-IDF project setup is working
- I2C communication with the IMU is working
- The **ICM-20948 driver is implemented**
- The IMU is successfully detected at address **`0x69`**
- Sensor data is currently being **streamed at 100 Hz**

### In progress / next steps
- Clean up and modularize the firmware further
- Add sensor fusion / tilt estimation
- Add motor driver integration
- Implement PID control for balancing

## Wiring

Current IMU wiring (reference for this repo):

- **SDA** → **GPIO 7**
- **SCL** → **GPIO 5**
- **I2C address** → **`0x69`**

If the hardware is rewired later, this section should be updated to stay in sync with the actual build.

## Circuit overview

```text
ESP32-C6                         ICM-20948 IMU
---------                        -------------
GPIO 7   ----------------------> SDA
GPIO 5   ----------------------> SCL
3.3V     ----------------------> VCC
GND      ----------------------> GND

I2C address: 0x69
Streaming rate: 100 Hz 
```


## Layout

- `firmware/` — ESP-IDF project that runs on the robot
- `experiments/` — Python scripts for sensor data analysis and control simulations
- `docs/` — notes, wiring, diagrams

## Build and flash

Requires [ESP-IDF v6.1](https://docs.espressif.com/projects/esp-idf/en/latest/esp32c6/get-started/).
The target (ESP32-C6) is set in `firmware/sdkconfig.defaults`, so no `set-target` step is needed.

```bash
cd firmware
idf.py build
idf.py -p <PORT> flash monitor
```
