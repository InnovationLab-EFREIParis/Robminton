// SN74HC595N Register Example
#define PIN_DS PA9   //pin 14  75HC595 and D8     
#define PIN_STCP PC7  //pin 12  75HC595 and D9   
#define PIN_SHCP PB6 //pin 11  75HC595 and D10 

#define numberOf74hc595 1 
#define numOfRegisterPins numberOf74hc595 * 8
boolean registers[numOfRegisterPins];

void setup(){
  pinMode(PIN_DS, OUTPUT);
  pinMode(PIN_STCP, OUTPUT);
  pinMode(PIN_SHCP, OUTPUT);
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
  setRegisterPin(0, HIGH);
  setRegisterPin(1, HIGH);
  setRegisterPin(2, HIGH);
  setRegisterPin(3, HIGH);
  setRegisterPin(4, HIGH);
  setRegisterPin(5, HIGH);
  setRegisterPin(6, HIGH);
  setRegisterPin(7, HIGH);
  writeRegisters();
}