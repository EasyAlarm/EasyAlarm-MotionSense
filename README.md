## EasyAlarm MotionSense (Arduino)

PIR motion sensor unit that reports TRIGGERED events to the hub via NRF24.

### Hardware
- Arduino Nano (`nanoatmega328new`)
- NRF24L01+
- PIR sensor
- Activity LED
- Pair button

### Behavior
- Debounces motion using `Chrono` (1s window).
- On motion, sends `TRIGGERED` to hub, lights LED briefly.
- Pair button toggles pairing: if paired → reset/unpair; if not → start pairing.
