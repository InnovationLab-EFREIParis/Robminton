/*
  SHIELD L293 Arduino sur STM32:
  
  Le shield Arduino L293 est conçu pour contrôler des moteurs DC 
  ou des moteurs pas à pas en utilisant des signaux de commande 
  provenant d'un microcontrôleur Arduino.

  Comme nous l’utiliserons sur une STM32 on définira les 
  signaux PWM et les inputs de sens de la manière suivante

*/

//SIGNAUX PWM
#define PWM_PIN1 PA7  // Moteur 1 PIN-D11
#define PWM_PIN2 PB3  // Moteur 2 PIN-D3
#define PWM_PIN3 PB10 // Moteur 3 PIN-D6
#define PWM_PIN4 PB4  // Moteur 4 PIN-D5

//INPUT 74HC595N
#define PIN_DS   PA9 // SER   PIN-D8     
#define PIN_STCP PA6 // RCLK  PIN-D12   
#define PIN_SHCP PB5 // SRCLK PIN-D4
#define OE PA8       // OE  PIN-D7

//VARIABLES
#define numberOf74hc595 1                      // nb 74HC595N
#define numOfRegisterPins numberOf74hc595 * 8  // nb Pins
boolean registers[numOfRegisterPins];          //

void setup(){
  //PWM  
  pinMode(PWM_PIN1, OUTPUT); // Moteur 1
  pinMode(PWM_PIN2, OUTPUT); // Moteur 2
  pinMode(PWM_PIN3, OUTPUT); // Moteur 3
  pinMode(PWM_PIN4, OUTPUT); // Moteur 4
  
  //74HC595N
  pinMode(PIN_DS, OUTPUT);   // SER
  pinMode(PIN_STCP, OUTPUT); // RCLK
  pinMode(PIN_SHCP, OUTPUT); // SRCLK
  pinMode(OE, OUTPUT);       // OE
  digitalWrite(OE, LOW);     // OE = LOW
  
  //INITIALISATION DE 74HC595N
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

void motorsDirections(int M1, int M2, int M3, int M4){
  if(M1 == 1){
    setRegisterPin(2, HIGH); // Moteur 1 A
    setRegisterPin(3, LOW);  // Moteur 1 B
  } else if(M1 == -1){
    setRegisterPin(2, LOW);  // Moteur 1 A
    setRegisterPin(3, HIGH); // Moteur 1 B
  } else{
    setRegisterPin(2, LOW); // Moteur 1 A
    setRegisterPin(3, LOW); // Moteur 1 A
  }

  if(M2 == 1){
    setRegisterPin(1, HIGH);  // Moteur 2 A
    setRegisterPin(4, LOW);   // Moteur 2 B
  } else if(M2 == -1){
    setRegisterPin(1, LOW);   // Moteur 2 A
    setRegisterPin(4, HIGH);  // Moteur 2 B
  } else{
    setRegisterPin(1, LOW);  // Moteur 2 A
    setRegisterPin(4, LOW);  // Moteur 2 B
  }

  if(M3 == 1){
    setRegisterPin(0, HIGH);  // Moteur 3 A
    setRegisterPin(6, LOW);   // Moteur 3 B
  } else if(M3 == -1){
    setRegisterPin(0, LOW);   // Moteur 3 A
    setRegisterPin(6, HIGH);  // Moteur 3 B
  } else{
    setRegisterPin(0, LOW);  // Moteur 3 A
    setRegisterPin(6, LOW);  // Moteur 3 B
  }

  if(M4 == 1){
    setRegisterPin(5, HIGH); // Moteur 4 A
    setRegisterPin(7, LOW);  // Moteur 4 B
  } else if(M4 == -1){
    setRegisterPin(5, LOW);  // Moteur 4 A
    setRegisterPin(7, HIGH); // Moteur 4 B
  } else{
    setRegisterPin(5, LOW); // Moteur 4 A
    setRegisterPin(7, LOW); // Moteur 4 B
  }

  writeRegisters(); //Write Registers
}

void motorsSpeed(int spM1, int spM2, int spM3, int spM4){
  analogWrite(PWM_PIN1, spM1); // Vitesse Moteur 1
  analogWrite(PWM_PIN2, spM2); // Vitesse Moteur 2
  analogWrite(PWM_PIN3, spM3); // Vitesse Moteur 3
  analogWrite(PWM_PIN4, spM4); // Vitesse Moteur 4
}

void loop(){

  motorsDirections(1,1,0,1); // Sens de rotations des Moteurs
  motorsSpeed(190,190,190,0);  // Vitesse des Moteurs
  delay(3000);
  motorsSpeed(100,100,190,0);  // Vitesse des Moteurs
  delay(3000);

  motorsDirections(-1,-1,0,-1); // Sens de rotations des Moteurs
  motorsSpeed(190,190,190,0);    // Vitesse des Moteurs
  delay(3000);
  motorsSpeed(100,100,190,0);    // Vitesse des Moteurs
  delay(3000);

}