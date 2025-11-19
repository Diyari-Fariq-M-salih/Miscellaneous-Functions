int potPin = A0;     // Potentiometer input
int pwmPin = 9;      // PWM output pin
int potValue = 0;    // Raw analog value
int pwmValue = 0;    // Mapped PWM value (0–255)

void setup() {
  Serial.begin(9600);      // Open serial monitor
  pinMode(pwmPin, OUTPUT); // Pin 9 as PWM output
}

void loop() {
  potValue = analogRead(potPin);        // Read 0–1023
  pwmValue = map(potValue, 0, 1023, 0, 255); // Convert to PWM range

  analogWrite(pwmPin, pwmValue);        // Output PWM

  // Print values to Serial Monitor
  Serial.print("Potentiometer: ");
  Serial.print(potValue);
  Serial.print("   PWM: ");
  Serial.println(pwmValue);

  delay(50);
}
