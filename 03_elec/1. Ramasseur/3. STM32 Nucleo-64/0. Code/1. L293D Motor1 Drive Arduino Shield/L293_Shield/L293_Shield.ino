// SN74HC595N Register Example
#define PIN_DS PA9   //pin 14  75HC595 and D8     
#define PIN_STCP PA6  //pin 12  75HC595 and D12   
#define PIN_SHCP PB5 //pin 11  75HC595 and D10 
#define OE PA8 //
#define PWM_PIN1 PA7 // D11
#define PWM_PIN2 PB3 // D3
#define PWM_PIN3 PB10 // D6
#define PWM_PIN4 PB4 // D5

#define numberOf74hc595 1 
#define numOfRegisterPins numberOf74hc595 * 8
boolean registers[numOfRegisterPins];

void setup(){
  pinMode(PWM_PIN1, OUTPUT); //First Motor PWM
  pinMode(PWM_PIN2, OUTPUT); //Second Motor PWM
  pinMode(PWM_PIN3, OUTPUT); //Third Motor PWM
  pinMode(PWM_PIN4, OUTPUT); //Fourth Motor PWM
  pinMode(PIN_DS, OUTPUT);
  pinMode(PIN_STCP, OUTPUT);
  pinMode(PIN_SHCP, OUTPUT);
  pinMode(OE, OUTPUT);
  digitalWrite(OE, LOW);  
  clearRegisters();
  writeRegisters();
}               

void clearRegisters(){
  for(int i = numOfRegisterPins - 1; i >=  0; i--){
     registers[i] = LOW;
  }
} 

void writeRegisters(){
  digitalWrite(PIN_STCP, LOW);
  for(int i = numOfRegisterPins - 1; i >=  0; i--){
    digitalWrite(PIN_SHCP, LOW);
    int val = registers[i];
    digitalWrite(PIN_DS, val);
    digitalWrite(PIN_SHCP, HIGH);
  }
  digitalWrite(PIN_STCP, HIGH);
}

void setRegisterPin(int index, int value){
  registers[index] = value;
}

void loop(){
  setRegisterPin(0, LOW); //Motor4
  setRegisterPin(1, LOW);
  setRegisterPin(2, LOW);
  setRegisterPin(3, LOW);
  setRegisterPin(4, LOW);
  setRegisterPin(5, HIGH); // Motor3
  setRegisterPin(6, LOW);
  setRegisterPin(7, LOW);
  writeRegisters();

  //First Motor Output  
  //analogWrite(PWM_PIN1, 190);  
  //delay(3000);
  //analogWrite(PWM_PIN1, 50);
  //delay(3000);

  //Second Motor Output  
  //analogWrite(PWM_PIN2, 190);  
  //delay(3000);
  //analogWrite(PWM_PIN2, 50);
  //delay(3000);  

  //Third Motor Output  
  //analogWrite(PWM_PIN3, 190);  
  //delay(3000);
  //analogWrite(PWM_PIN3, 50);
  //delay(3000);

  //Fourth Motor Output  
  //analogWrite(PWM_PIN4, 190);  
  //delay(3000);
  //analogWrite(PWM_PIN4, 50);
  //delay(3000);

}