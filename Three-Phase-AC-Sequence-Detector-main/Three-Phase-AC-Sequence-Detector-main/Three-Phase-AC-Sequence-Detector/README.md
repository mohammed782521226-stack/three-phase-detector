# Three-Phase AC Phase Sequence Detector
A simple and reliable three-phase AC phase sequeqce detector based on
zero-crossing detection and opto-isolated inputs.
The project determines whether the phase order is **ABC (clockwise)** or
**ACB (counterclockwise)** by measuring the relative timing between zero
crossings of the three phases.
---
## How It Works
- Each phase is isolated using a PC817 optocoupler
- Zero-crossing events are converted into digital pulses
- One phase is used as a reference
- Time differences between phase crossings are measured
- Phase order is determined based on which phase arrives first
For a 50 Hz system:
- 120° ≈ 6.67 ms
- 240° ≈ 13.33 ms
---
## Hardware
- Arduino or ESP32
- 3 × PC817 optocouplers
- High-value resistors for AC input
- Pull-up resistors
- 2 × indicator LEDs
---
## Output
- **LED 1 ON** → ABC (clockwise)
- **LED 2 ON** → ACB (counterclockwise)
- **Both OFF** → Invalid or unstable input
---
## Applications
- Three-phase motor direction detection
- Industrial power systems
- Educational projects
---
## License
This project is released under the MIT License.
