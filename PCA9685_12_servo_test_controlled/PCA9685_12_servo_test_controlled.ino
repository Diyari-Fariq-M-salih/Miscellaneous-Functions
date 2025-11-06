// Uno R3 + PCA9685: 12 keypairs control 12 servos in 30° steps
// Down: a s d f g h j k l ; ' /   -> servos 1..12  (-30°)
// Up:   q w e r t y u i o p [ ]   -> servos 1..12  (+30°)

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
Adafruit_PWMServoDriver pwm(0x40);

// Tune if needed
const uint16_t MIN_PULSE_US = 500;    // 0°
const uint16_t MAX_PULSE_US = 2500;   // 180°
const float    SERVO_FREQ   = 50.0;

const uint8_t SERVO_CH[12] = {0,1,2,3,4,5,6,7,8,9,10,11};

const char DOWN_KEYS[12] = {'a','s','d','f','g','h','j','k','l',';','\'','/'};
const char  UP_KEYS[12]  = {'q','w','e','r','t','y','u','i','o','p','[',']'};

float angleDeg[12];
const float STEP = 30.0;  // 30° per keypress

uint16_t usToTicks(uint16_t us){
  float t = us * SERVO_FREQ * 4096.0f / 1000000.0f;
  if (t < 0) t = 0; if (t > 4095) t = 4095;
  return (uint16_t)(t + 0.5f);
}
uint16_t angleToUs(float deg){
  if (deg < 0) deg = 0; if (deg > 180) deg = 180;
  return (uint16_t)(MIN_PULSE_US + (MAX_PULSE_US - MIN_PULSE_US) * (deg/180.0f));
}
void apply(uint8_t i){
  pwm.setPWM(SERVO_CH[i], 0, usToTicks(angleToUs(angleDeg[i])));
}
void changeAngle(uint8_t i, float delta){
  angleDeg[i] += delta;
  if (angleDeg[i] < 0) angleDeg[i] = 0;
  if (angleDeg[i] > 180) angleDeg[i] = 180;
  apply(i);
  Serial.print("S"); Serial.print(i+1);
  Serial.print(" -> "); Serial.print(angleDeg[i],0);
  Serial.println(" deg");
}
int16_t findIndexIn(const char* arr, char key){
  for (uint8_t i=0;i<12;i++) if (arr[i]==key) return i;
  return -1;
}

void setup(){
  Serial.begin(115200);
  Wire.begin();
  pwm.begin();
  pwm.setPWMFreq(SERVO_FREQ);
  delay(10);

  for (uint8_t i=0;i<12;i++){ angleDeg[i]=90; apply(i); }

  Serial.println("Down: a s d f g h j k l ; ' /   |   Up: q w e r t y u i o p [ ]   (30° steps)");
  Serial.println("c=center all 90°, p=print CSV.");
}

void loop(){
  while (Serial.available()){
    char c = Serial.read();
    if (c >= 'A' && c <= 'Z') c = c - 'A' + 'a';

    int16_t idx = findIndexIn(DOWN_KEYS, c);
    if (idx >= 0){ changeAngle((uint8_t)idx, -STEP); continue; }

    idx = findIndexIn(UP_KEYS, c);
    if (idx >= 0){ changeAngle((uint8_t)idx, +STEP); continue; }

    if (c=='c'){
      for (uint8_t i=0;i<12;i++){ angleDeg[i]=90; apply(i); }
      Serial.println("CENTER_ALL -> 90 deg");
    } else if (c=='p'){
      Serial.print("POSE,");
      for (uint8_t i=0;i<12;i++){ Serial.print(angleDeg[i],0); if(i<11) Serial.print(','); }
      Serial.println();
    }
  }
}
