# Self-balancing robot

A two-wheeled self-balancing robot built on the ESP32-C6, with an ICM-20948 IMU over I²C.

Status: project skeleton only. The firmware boots and idles; no sensor or motor code yet.

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
