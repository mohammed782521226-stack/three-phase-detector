# Three-Phase AC Phase Sequence Detector

This project implements a **Three-Phase AC Phase Sequence Detector** using
zero-crossing detection and a microcontroller (Arduino / ESP32).
The system determines whether the phase sequence is **ABC (clockwise)** or
**ACB (counterclockwise)** by measuring the time difference between zero-crossing
events of the three phases.

---

## 1. Introduction
In a single-phase AC system, voltage measurement is straightforward because
only one phase exists and phase angle comparison is unnecessary.
However, in a **three-phase AC system**, each phase has the same frequency and
amplitude but differs in **phase angle**, making phase angle measurement
essential.

In a balanced three-phase system:
- Each phase is separated by **120 electrical degrees**
- The third phase is **240 degrees** apart from the first phase
- Phase angles may be expressed as (0°, ±120°) depending on the chosen reference

Correct identification of the phase sequence is critical in:
- Three-phase motor rotation
- Industrial power systems
- Protection and control circuits

---

## 2. Operating Principle
The phase sequence is determined by measuring the **relative timing of zero
crossings** of the three phase voltages.

Each phase voltage is:
1. Isolated using an **optocoupler (PC817)**
2. Converted into a digital pulse near its zero-crossing point
3. Fed into the microcontroller as a digital input

One phase (Phase A) is selected as the **reference phase**.
The controller measures:
- `tAB`: Time difference between Phase A and Phase B zero crossings
- `tAC`: Time difference between Phase A and Phase C zero crossings

For a 50 Hz system:
- One full cycle = 20 ms
- 120 degrees ≈ 6.67 ms
- 240 degrees ≈ 13.33 ms

Decision logic:
- If `tAB < tAC` → Phase sequence is **A-B-C (clockwise)**
- If `tAC < tAB` → Phase sequence is **A-C-B (counterclockwise)**

To improve reliability, the measurement is repeated over multiple cycles and
the final result is determined using majority voting.

---

## 3. Hardware Overview
### Main Components
- Microcontroller (Arduino UNO / ESP32)
- 3 × PC817 Optocouplers (one per phase)
- High-value resistors for current limiting and voltage division
- Pull-up resistors for digital outputs
- Two indicator LEDs for phase sequence result

### Isolation and Safety
- Each phase is electrically isolated from the microcontroller
- Neutral is used as the reference to measure 220 VAC per phase
- No direct electrical connection exists between mains voltage and logic ground

Refer to the schematic files for detailed wiring information.

---

## 4. Software Overview
The firmware performs the following tasks:
1. Detects zero-crossing pulses using digital interrupts
2. Records precise timestamps using `micros()`
3. Computes time differences between phase crossings
4. Determines the phase sequence
5. Activates the corresponding output LED

The code is written to be:
- Deterministic
- Noise tolerant
- Easily portable between Arduino and ESP32 platforms

---

## 5. Output Indication
- **LED 1 ON** → Phase sequence is **ABC (clockwise)**
- **LED 2 ON** → Phase sequence is **ACB (counterclockwise)**
- Both LEDs OFF → Invalid or unstable measurement

---

## 6. Applications
- Three-phase motor direction detection
- Industrial automation systems
- Power quality monitoring
- Educational and training platforms

---

## 7. Notes
- Phase labels (A, B, C) depend on physical wiring.
  The detected direction reflects the relative phase order, not absolute motor
  rotation unless wiring is standardized.
- For ESP32, all GPIOs support interrupts, simplifying the implementation.
- For Arduino UNO, a hybrid interrupt/polling approach is used due to limited
  external interrupt pins.

---

## 8. License
This project is open-source and can be used, modified, and distributed freely
for educational and non-commercial purposes.

---
## 9. Author
Designed and implemented by Mohammed Alashari as an open-source reference
project for three-phase AC phase sequence detection.
