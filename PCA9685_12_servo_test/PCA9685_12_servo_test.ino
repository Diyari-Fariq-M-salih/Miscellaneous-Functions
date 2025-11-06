// Arduino Uno R3 + PCA9685 (Adafruit PWM Servo Driver)
// Moves 12 servos: all to 90°, wait 5s; all to 0°, wait 5s; loop.

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

// Tune these to your servos. Start with 500–2500 µs, then refine.
const uint16_t MIN_PULSE_US = 500;   // microseconds at 0°
const uint16_t MAX_PULSE_US = 2500;  // microseconds at 180°
const float    SERVO_FREQ   = 50.0;  // Hz

// Channels 0–11 used for 12 servos
const uint8_t SERVO_CH[12] = {
  0, 1, 2,   // Leg 1: servos 1–3
  3, 4, 5,   // Leg 2: servos 4–6
  6, 7, 8,   // Leg 3 (rear): servos 7–9
  9,10,11    // Leg 4 (rear): servos 10–12
};

uint16_t usToTicks(uint16_t us) {
  // PCA9685: 4096 ticks per period
  // ticks = us * 4096 / (1e6 / freq) = us * freq * 4096 / 1e6
  float ticks = (float)us * SERVO_FREQ * 4096.0f / 1000000.0f;
  if (ticks < 0)   ticks = 0;
  if (ticks > 4095) ticks = 4095;
  return (uint16_t)(ticks + 0.5f);
}

uint16_t angleToUs(float angleDeg) {
  if (angleDeg < 0) angleDeg = 0;
  if (angleDeg > 180) angleDeg = 180;
  return (uint16_t)(MIN_PULSE_US + (MAX_PULSE_US - MIN_PULSE_US) * (angleDeg / 180.0f));
}

void writeAngle(uint8_t ch, float angleDeg) {
  uint16_t us = angleToUs(angleDeg);
  uint16_t ticks = usToTicks(us);
  pwm.setPWM(ch, 0, ticks);
}

void writeAll(float angleDeg) {
  for (uint8_t i = 0; i < 12; i++) {
    writeAngle(SERVO_CH[i], angleDeg);
  }
}

void setup() {
  Wire.begin();                 // Uno R3: SDA=A4, SCL=A5
  pwm.begin();
  pwm.setPWMFreq(SERVO_FREQ);   // 50 Hz for hobby servos
  delay(10);

  // Optional: move to a safe midpoint on boot
  writeAll(90);
  delay(1000);
}

void loop() {
  // Stand
  writeAll(0);
  delay(5000);  // 5 s

  // Rest
  writeAll(0);
  delay(5000);  // 5 s
}
