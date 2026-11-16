# Experiments

Python notebooks for analysing sensor data and simulating the control system of the self-balancing robot. Nothing here runs on the robot; this is where ideas are tested before they are written in C in `../firmware/`.

## Layout

- `notebooks/` — Jupyter notebooks, one per experiment
- `data/` — sensor logs and results collected locally (not committed)
- `pyproject.toml`, `uv.lock` — dependencies and their exact locked versions

## Setup

Requires [uv](https://docs.astral.sh/uv/). From this directory:

```bash
uv sync
```

This creates `.venv/` with Python 3.13 and the locked dependencies.

## Run Jupyter

```bash
uv run jupyter lab
```

## Add a dependency

```bash
uv add <package>
```

This updates both `pyproject.toml` and `uv.lock`; commit both.

## Planned experiments

1. **IMU analysis** — accelerometer and gyroscope noise and bias
2. **Sensor fusion** — complementary filter for tilt angle estimation
3. **PID simulation** — inverted pendulum model of the robot
4. **PID tuning** — comparing controller parameters

## Relation to the firmware

The firmware logs IMU samples over serial; those logs are saved to `data/` and analysed here. Results flow back the other way: filter coefficients and PID gains found in the notebooks become constants in the firmware.
