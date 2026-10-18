# OPEN-Hand

An affordable, open-source robotic hand for prosthetic applications, designed to be reproduced for roughly $90. Modeled on the proportions of a human hand, 3D printed in PLA+, PETG and resin, and driven by four 9 g servos that wind monofilament cables to close the fingers. Elastic shock cords return them open.

This repository holds the **Arduino code** for the hand. The CAD, drawings and schematics are documented separately (see [Design package](#design-package)).

## Background

OPEN-Hand started as an ENGR-107 group project at George Mason University: a 3D-printed hand driven by a single stepper motor, with insufficient torque, a rigid palm that could not form a fist, and clunky Bluetooth app control. I rebuilt it from scratch to fix those problems:

| ENGR-107 original | OPEN-Hand |
|---|---|
| One forearm-mounted stepper motor | Four palm-mounted 9 g servos |
| Rigid one-piece palm | Two-piece palm with an elastic hinge (full fist) |
| Single-pivot finger joints | Dual-pivot finger joints |
| Bluetooth app control | Sensor control (accelerometer, with EMG tested) |

The project was presented at the Mason Innovation Exchange, supported by the Office of Student Creative Activities and Research.

## Repository contents

| File | What it does |
|---|---|
| `4_servo_accelerometer_code.ino` | **Final hand control.** Tilting the GY-61 accelerometer drives all four servos one way or the other. In the neutral zone the hand winds back toward its starting position and stops. |
| `1_servo_forward-reverse_code.ino` | Single-servo version of the same logic, used for bench testing before building the four-servo hand. |
| `1-sevor_controle_code.ino` | First single-servo test. Maps accelerometer tilt to a servo command and holds neutral outside a set range. |
| `emg_sketch_v1.ino` | First EMG (muscle signal) test. Smooths the signal over 10 samples and switches a servo between relaxed and flexed using two fixed thresholds. |
| `emg_servo_control_v2.ino` | Improved EMG test. Compares a fast moving average to a slow baseline, so a flex is detected as a rise above baseline instead of an absolute level. |

## How the accelerometer control works

1. The X axis of the GY-61 accelerometer is read on analog pin `A0` (values 0 to 1023).
2. If the reading is above `thresholdHigh` (555), all four servos rotate clockwise to close the fingers. If it is below `thresholdLow` (380), they rotate counter-clockwise to open them.
3. Between the two thresholds the sensor is neutral. The sketch keeps a running count of how long the servos have moved in each direction (`moveUnits`, updated every 100 ms) and drives them back until the count returns to zero, then stops.

The servos are 360° continuous-rotation types, so they are commanded by speed and direction (`write(70)` counter-clockwise, `write(110)` clockwise, `write(90)` stop), not by angle. Because the return trip is timed and there is no position sensor, the hand finds its way home by counting time, not by measuring position. That works for a demonstration but can drift over many cycles.

## Hardware

- Arduino UNO R3
- GY-61 accelerometer (X axis used)
- 4× MG90S 9 g servos, 360° continuous rotation
- Monofilament cable and elastic shock cord for the fingers
- For the EMG tests: an EMG amplifier module and a servo on pin `D9`

### Pin assignments (as used in the sketches)

| Sketch | Signal | Pin |
|---|---|---|
| `4_servo_accelerometer_code.ino` | Accelerometer X | `A0` |
| | Servos 1 to 4 | `D2`, `D3`, `D4`, `D5` |
| `1_servo_*` and `1-sevor_*` | Accelerometer X | `A0` |
| | Servo | `D2` |
| `emg_*` | EMG signal | `A0` |
| | Servo | `D9` |

## Running a sketch

1. Install the [Arduino IDE](https://www.arduino.cc/en/software). The `Servo` library ships with it.
2. Open the sketch you want, select **Arduino Uno**, and upload.
3. For the EMG sketches, open the Serial Plotter at 115200 baud to see the signal while tuning.

## Tuning

- **Accelerometer thresholds** (`thresholdLow`, `thresholdHigh`): set from the sensor's resting reading, so they depend on your sensor and mounting. Print `analogRead(A0)` to the serial monitor with the sensor level and tilted, then adjust.
- **EMG** (`emg_servo_control_v2.ino`): `activationDelta` sets how far above the baseline counts as a flex, and `samplesFast` and `samplesSlow` set the two averaging windows.

## EMG notes

EMG was the original control goal. I tested a commercial off-the-shelf amplifier module, then built a custom discrete amplifier, but could not get a reliable signal before the presentation deadline, so I switched to the accelerometer. After the deadline I revisited the problem and detected a clean EMG signal on my third amplifier attempt. The two EMG sketches here are the control tests from that work.

## Design package

The full design package includes Onshape CAD modeled from measurements of my own hand (14 unique parts, 21-sheet drawing set), KiCAD schematics, and a bill of materials of about $90.
