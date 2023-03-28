// L293D Registers Example
#define PWM_PIN PB3 // PIN D3
#define DIR1  PB5 // PIN D4
#define DIR2  PA8 // PIN D7
void setup() {
  pinMode(PWM_PIN, OUTPUT);
  pinMode(DIR1, OUTPUT);
  pinMode(DIR2, OUTPUT);
  digitalWrite(DIR1, 0);
  digitalWrite(DIR2, 1);  
}

void loop() {
  analogWrite(PWM_PIN, 255);
  delay(3000);
  analogWrite(PWM_PIN, 1);
  delay(3000);
}