#include <Servo.h>

Servo myServo;  // create servo object
int servoPin = 9; // change to your signal pin

void setup() {
  myServo.attach(servoPin);  // attach servo to pin
  Serial.begin(9600);
  Serial.println("Starting MG90S test...");
}

void loop() {
  // Move to 0 degrees
  myServo.write(0);
  Serial.println("Position: 0°");
  delay(1000);

  // Move to max (180 degrees)
  myServo.write(180);
  Serial.println("Position: 180°");
  delay(1000);

}
