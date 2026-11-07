/*
 * Three-Phase AC Phase Sequence Detector
 *
 * Description:
 *   Detects the phase sequence (ABC or ACB) of a three-phase AC system
 *   using zero-crossing detection with opto-isolated inputs (PC817).
 *   The algorithm measures time differences between phase zero crossings
 *   to determine the rotation order.
 *
 * Hardware:
 *   - Microcontroller: Arduino / ESP32
 *   - Optocouplers: PC817 (one per phase)
 *   - Input: 3-phase AC (220V phase-to-neutral)
 *
 * License:
 *   MIT License
 *
 * Copyright (c) 2026 Mohammed Alashari
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 */

const byte PIN_A = 2;  // INT0
const byte PIN_B = 3;  // INT1
const byte PIN_C = 4;  // Pin-change (we'll poll via interrupt on A only)

const byte LED_CW  = 8; // ABC (clockwise)
const byte LED_CCW = 9; // ACB (counterclockwise)

volatile unsigned long tA = 0, tB = 0, tC = 0;
volatile bool haveA = false, haveB = false, haveC = false;

void isrA() {
  tA = micros();
  haveA = true;
  haveB = false;
  haveC = false;
}


volatile unsigned long lastB = 0;
void isrB() {
  if (haveA && !haveB) {
    tB = micros();
    haveB = true;
    lastB = tB;
  }
}

bool waitRising(byte pin, unsigned long start_us, unsigned long timeout_us, unsigned long &tOut) {
  unsigned long t0 = micros();
  // wait for LOW (in case already HIGH)
  while (digitalRead(pin) == HIGH) {
    if ((micros() - t0) > timeout_us) return false;
  }
  while (digitalRead(pin) == LOW) {
    if ((micros() - t0) > timeout_us) return false;
  }
  tOut = micros();
  if (tOut < start_us) return false;
  return true;
}

void setup() {
  pinMode(PIN_A, INPUT_PULLUP);
  pinMode(PIN_B, INPUT_PULLUP);
  pinMode(PIN_C, INPUT_PULLUP);

  pinMode(LED_CW, OUTPUT);
  pinMode(LED_CCW, OUTPUT);

  digitalWrite(LED_CW, LOW);
  digitalWrite(LED_CCW, LOW);

  attachInterrupt(digitalPinToInterrupt(PIN_A), isrA, RISING);
  attachInterrupt(digitalPinToInterrupt(PIN_B), isrB, RISING);
}

void loop() {
  const int SAMPLES = 8;
  int cwCount = 0, ccwCount = 0;

  for (int i = 0; i < SAMPLES; i++) {
    unsigned long tStart = micros();
    while (!haveA) {
      if (micros() - tStart > 40000UL) break; // 40ms timeout
    }
    if (!haveA) continue;

    unsigned long localA = tA;

    unsigned long localC = 0;
    bool gotC = waitRising(PIN_C, localA, 25000UL, localC);

    unsigned long t0 = micros();
    while (!haveB && (micros() - t0) < 25000UL) { /* wait */ }

    if (!(haveB && gotC)) {
      haveA = false;
      continue;
    }

    unsigned long localB = tB;

    unsigned long dAB = localB - localA;
    unsigned long dAC = localC - localA;

    if (dAB < dAC) cwCount++;
    else ccwCount++;

    haveA = false;
  }

  // Decide and light LEDs
  if (cwCount > ccwCount) {
    digitalWrite(LED_CW, HIGH);
    digitalWrite(LED_CCW, LOW);
  } else if (ccwCount > cwCount) {
    digitalWrite(LED_CW, LOW);
    digitalWrite(LED_CCW, HIGH);
  } else {
    // turn both LEDs off
    digitalWrite(LED_CW, LOW);
    digitalWrite(LED_CCW, LOW);
  }

  delay(200);
}
