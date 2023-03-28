//ESC
#define PWM_PIN1 PC7 //ESC1
#define PWM_PIN2 PB3 //ESC2
void setup() {
  Serial.begin(9600);
  pinMode(PWM_PIN1, OUTPUT); // ESC1 PWM1
  pinMode(PWM_PIN2, OUTPUT); // ESC1 PWM2
}
int x = 0;
void loop() {
  char RxedByte = 0;
  if(Serial.available()){
    RxedByte = Serial.read();
    if(RxedByte == 'z')
      x=x+1;
    if(RxedByte == 's')
      x=x-1;              
  }
  analogWrite(PWM_PIN1, x);
  analogWrite(PWM_PIN2, x);
}
