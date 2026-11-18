# Imports
import argparse
import time
from datetime import datetime
from pathlib import Path

import serial

parser = argparse.ArgumentParser(
    description="Record IMU-data from ESP32-C6"
)

parser.add_argument(
    "--port", 
    type=str, 
    required=True, 
    help="Serial port connected to the ESP32-C6"
)

parser.add_argument(
    "--name",
    required=True,
    help="Short description of the recording, e.g. still_flat",
)

parser.add_argument(
    "--seconds",
    type=float, 
    default=60,
    help="How many seconds you wish to record"
)
args = parser.parse_args()

DATA_DIR = Path(__file__).resolve().parent.parent / "data"
DATA_DIR.mkdir(exist_ok=True)
path = DATA_DIR / f"{args.name}_{datetime.now():%Y-%m-%d_%H%M}.csv"

with serial.Serial(args.port, 115200, timeout=1) as ser:
    ser.dtr = False
    ser.rts = True
    time.sleep(0.1)
    ser.rts = False

    deadline = time.monotonic() + 5

    while True: 
        if time.monotonic() > deadline:
            raise SystemExit("No header from the board within 5 s. Is the IMU connected?")
        line = ser.readline().decode(errors="replace").strip()
        if line.startswith("time_us"):
            header = line
            break

    with open(path, "w", encoding="utf-8") as f:
        f.write(header + "\n")

        end = time.monotonic() + args.seconds
        count = 0

        while time.monotonic() < end:
            line = ser.readline().decode(errors="replace").strip()
            parts = line.split(",")
            if len(parts)==7 and parts[0].isdigit():
                f.write(line + "\n")
                count += 1