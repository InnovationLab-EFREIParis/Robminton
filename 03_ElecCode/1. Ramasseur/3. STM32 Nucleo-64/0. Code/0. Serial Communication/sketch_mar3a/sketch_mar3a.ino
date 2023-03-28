void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  pinMode(PD_1,OUTPUT);
  digitalWrite(PD_1,HIGH);
}

void loop() {
  char RxedByte = 0;
  if(Serial.available()){
    RxedByte = Serial.read();
    if(RxedByte == 'f')
      digitalWrite(LED_BUILTIN, HIGH);
    else
      digitalWrite(LED_BUILTIN, LOW);              
  }
}
