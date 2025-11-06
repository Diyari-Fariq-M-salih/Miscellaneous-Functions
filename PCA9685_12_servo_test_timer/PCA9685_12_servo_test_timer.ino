// Uno R3 + PCA9685: 12-servo sequential sweeps
// Each servo: 0→180→0 for 2 cycles, then move to next servo.

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm(0x40);

// Tune per your servos if needed
const uint16_t MIN_PULSE_US = 500;    // 0°
const uint16_t MAX_PULSE_US = 2500;   // 180°
const float    SERVO_FREQ   = 50.0;   // Hz

// PCA9685 channel mapping for servos 1..12
const uint8_t SERVO_CH[12] = {
  0,1,2,    // Leg 1: servos 1–3
  3,4,5,    // Leg 2: servos 4–6
  6,7,8,    // Leg 3 (rear): servos 7–9
  9,10,11   // Leg 4 (rear): servos 10–12
};

// Sweep tuning
const float STEP_DEG   = 1.0;   // degrees per step
const uint16_t STEP_MS = 15;    // delay per step (speed)
const uint16_t DWELL_MS = 250;  // pause at ends

// ---------- helpers ----------
uint16_t usToTicks(uint16_t us){
  float t = us * SERVO_FREQ * 4096.0f / 1000000.0f;
  if (t < 0) t = 0;
  if (t > 4095) t = 4095;
  return (uint16_t)(t + 0.5f);
}
uint16_t angleToUs(float deg){
  if (deg < 0) deg = 0;
  if (deg > 180) deg = 180;
  return (uint16_t)(MIN_PULSE_US + (MAX_PULSE_US - MIN_PULSE_US) * (deg/180.0f));
}
void writeAngle(uint8_t idx, float deg){
  uint16_t ticks = usToTicks(angleToUs(deg));
  pwm.setPWM(SERVO_CH[idx], 0, ticks);
}
// Sweep helper
void sweep(uint8_t idx, float a0, float a1){
  if (a1 >= a0){
    for (float a=a0; a<=a1; a+=STEP_DEG){ writeAngle(idx,a); delay(STEP_MS); }
  } else {
    for (float a=a0; a>=a1; a-=STEP_DEG){ writeAngle(idx,a); delay(STEP_MS); }
  }
  delay(DWELL_MS);
}
// ---------- /helpers ----------

void setup(){
  Serial.begin(115200);
  Wire.begin();
  pwm.begin();
  pwm.setPWMFreq(SERVO_FREQ);
  delay(10);

  // Initialize all to 90°
  for (uint8_t i=0;i<12;i++) writeAngle(i, 90);
  Serial.println("Sequential sweep: each servo 0→180→0 x2 cycles.");
}

void loop(){
  for (uint8_t i=0; i<12; i++){
    Serial.print("Servo "); Serial.print(i+1);
    Serial.print(" (CH "); Serial.print(SERVO_CH[i]); Serial.println(") start");

    for (uint8_t cycle=1; cycle<=2; cycle++){
      Serial.print("  Cycle "); Serial.println(cycle);
      sweep(i, 0, 180);
      sweep(i, 180, 0);
    }

    Serial.print("Servo "); Serial.print(i+1); Serial.println(" done");
    delay(300); // small gap before next servo
  }
  // after servo 12, repeats from servo 1
}
