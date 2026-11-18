# Roadmap

Progress towards a balancing robot, phase by phase. Tick a box by changing `[ ]` to `[x]`.

## Parts

| Component | Model | Status |
|---|---|---|
| Microcontroller | ESP32-C6 | have |
| IMU | Adafruit ICM-20948 (9-DoF) | have |
| DC motors | 2 × TT DC gear motor | have |
| Wheels | 2 × TT motor wheel (≈ 65 mm) | have |
| Motor driver | DRV8833 dual H-bridge | ordered |
| Voltage regulator | MP1584EN buck converter, adjustable | ordered |
| Prototype board | Perfboard, 20 pcs | ordered |
| Switches | On/off switch, 10 pcs | ordered |
| Pin headers | Male pin headers | ordered |
| Battery | 2 × 18650 in holder (protected cells), or 2S LiPo | **to order** |
| Charger | matching the battery | **to order** |
| Chassis | plates, spacers, TT motor brackets | **to order** |
| Female headers | for removable ESP32 and IMU | **to order** |
| Capacitors | 0.1 µF ceramic × 2, 100 µF electrolytic × 1 | **to order** |
| Wire and heat shrink | thick for power and motors, thin for signals | **to order** |
| Encoders | TT encoder disc + optical sensor kit | optional |

Tools: soldering iron, solder, multimeter.

## Done

- [x] Repository, ESP-IDF project for the ESP32-C6, uv-managed Python environment
- [x] IMU driver: init, `WHO_AM_I` check, wake-up, reading at 100 Hz
- [x] Serial logger (`experiments/scripts/log_imu.py`)
- [x] Notebook 1: IMU bias, noise and gyro drift
- [x] Sample data committed so notebook 1 runs from a fresh clone

## Phase 1: Parts

- [ ] Receive: DRV8833, MP1584, perfboard, switches, male headers
- [ ] Order: battery and charger
- [ ] Order: chassis or materials, TT motor brackets
- [ ] Order: female headers, capacitors, wire and heat shrink
- [ ] Decide: encoders now or later

## Phase 2: Sensor fusion

- [x] Record `tilt_slow` (slow tilts to ~30° and back, one axis)
- [x] Record `tilt_fast` (quick tilts, same axis)
- [x] Notebook 2: complementary filter in Python
- [x] Notebook 2: compare α values and choose one
- [ ] `sensor_fusion` component in C
- [ ] Gyro bias calibration at startup (average while standing still)
- [ ] Print and log the filtered angle; compare with notebook 2
- [ ] `imu_init`: retry `WHO_AM_I` a few times before giving up

## Phase 3: Mechanics

- [ ] Mount the TT motors with brackets, wheels parallel
- [ ] Mount the IMU firmly on the centre line, near the axle
- [ ] Place the battery high (higher centre of gravity falls more slowly)
- [ ] Secure everything so nothing can shift

## Phase 4: Electronics and soldering

- [ ] **Set the MP1584 to 5.0 V with a multimeter before connecting anything to its output**
- [ ] Solder headers on DRV8833, MP1584 and IMU
- [ ] Solder 0.1 µF capacitors onto the motor terminals
- [ ] Solder motor wires, with heat shrink
- [ ] Choose the 4 motor PWM pins (avoid strapping and USB pins; check the board pinout)
- [ ] Check whether GPIO 5 (SCL) is a strapping pin (MTDI) and whether that matters
- [ ] Perfboard layout: ESP32 and IMU in female headers, DRV8833, MP1584, switch
- [ ] Power path: battery → switch → DRV8833 VM (with 100 µF) and → MP1584 → ESP32 5V
- [ ] Common ground for battery, DRV8833, MP1584 and ESP32
- [ ] Multimeter check before first power-up: no short, 5 V at the ESP32
- [ ] Wiring diagram updated in the README and `docs/`

## Phase 5: Motor control

- [ ] `motor_driver` component: 4 LEDC PWM channels at ~20 kHz
- [ ] Direction control: PWM one input, hold the other
- [ ] Cap PWM at ~70 % (6 V motors on a battery up to 8.4 V)
- [ ] Test both directions and a range of speeds, wheels off the ground
- [ ] Measure the dead zone of each motor
- [ ] Safety stop: motors off when the angle passes ~45°
- [ ] Encoders, if added: pulse counting with PCNT

## Phase 6: Simulation (Python)

- [ ] Measure the robot: mass, height of centre of gravity, wheel radius
- [ ] Notebook 3: inverted pendulum model
- [ ] Notebook 3: add dead zone and delay to match TT motors
- [ ] Notebook 3: PID in simulation, starting gains
- [ ] Notebook 4: compare PID gains

## Phase 7: Balancing

- [ ] `controller` component: PID on the angle, with dead-zone compensation
- [ ] Control loop at 200–500 Hz: `CONFIG_FREERTOS_HZ=1000`, dedicated task
- [ ] Find the balance point angle
- [ ] Tune P, then D, then a little I
- [ ] Change gains at runtime without reflashing
- [ ] Handle drift: trim the balance point (no encoders) or outer speed loop (encoders)
- [ ] Log balancing runs and analyse them in a notebook

## Phase 8: Extras

- [ ] Battery voltage monitoring with low-voltage cut-off
- [ ] Remote control over Bluetooth or Wi-Fi
- [ ] Wireless telemetry

## Phase 9: Showcase

- [ ] README: photos, video, parts list, wiring diagram, result plots
- [ ] Design write-up: filter choice, PID tuning, problems and solutions
- [ ] Version tags for the milestones
- [ ] Make the repository public
