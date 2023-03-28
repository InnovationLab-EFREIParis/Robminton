//ESC
#define PWM_PIN1 PC7
#define PWM_PIN2 PB3
void setup() {
  pinMode(PWM_PIN1, OUTPUT);
  pinMode(PWM_PIN2, OUTPUT);
}
void loop() {
  analogWrite(PWM_PIN1, 90);
  analogWrite(PWM_PIN2, 45);
}
