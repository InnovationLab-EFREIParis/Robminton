/*
  ROBMINTON:
  
  Voici le code de ROBMINTON qui mélange le code du Motor Shield 293D, des Encoders 
  et de la comunication Sérial avec la Raspberry Pi 3
*/

/*-----------------------------------ENCODERS--------------------------------------*/
#define M1ENCA  PA0 // Moteur 1 AVout 
#define M1ENCB  PA1 // Moteur 1 BVout 

#define M2ENCA  PC1 // Moteur 2 AVout
#define M2ENCB  PC0 // Moteur 2 BVout 

#define M3ENCA  PB2 // Moteur 3 AVout 
#define M3ENCB  PA10  // Moteur 3 BVout 

#define M4ENCA  PA13 // Moteur 4 AVout 
#define M4ENCB  PA14 // Moteur 4 BVout 

byte M1ENCALast;
byte M2ENCALast;
byte M3ENCALast;
byte M4ENCALast;

//NOMBRE DE PULSES 
float M1duration; // Moteur 1
float M2duration; // Moteur 2
float M3duration; // Moteur 3
float M4duration; // Moteur 4

//NOMBRE DE TOURS
float M1nb_tours=0.0; // Moteur 1
float M2nb_tours=0.0; // Moteur 2
float M3nb_tours=0.0; // Moteur 3
float M4nb_tours=0.0; // Moteur 4

//DIRECTION DE ROTATION
boolean M1Direction; // Moteur 1
boolean M2Direction; // Moteur 2
boolean M3Direction; // Moteur 3
boolean M4Direction; // Moteur 4

void EncoderInit() {
  //PAR DEFAUT : True -> Foward
  M1Direction = true; // Moteur 1
  M2Direction = true; // Moteur 2  
  M3Direction = true; // Moteur 3  
  M4Direction = true; // Moteur 4  

  //INITIALISATION DES ENCB
  pinMode(M1ENCB,INPUT); // Moteur 1
  pinMode(M2ENCB,INPUT); // Moteur 2
  pinMode(M3ENCB,INPUT); // Moteur 3
  pinMode(M4ENCB,INPUT); // Moteur 4

  //INITIALISATION DES ENCA
  attachInterrupt(M1ENCA, wheelSpeedM1, CHANGE); // Moteur 1
  attachInterrupt(M2ENCA, wheelSpeedM2, CHANGE); // Moteur 2
  attachInterrupt(M3ENCA, wheelSpeedM3, CHANGE); // Moteur 3
  attachInterrupt(M4ENCA, wheelSpeedM4, CHANGE); // Moteur 4
}

void wheelSpeedM1() {
  int Lstate1 = digitalRead(M1ENCA);
  if((M1ENCALast == LOW) && Lstate1==HIGH)
  {
    int val = digitalRead(M1ENCB);
    if(val == LOW && M1Direction)
      M1Direction = false; //Reverse
    else if(val == HIGH && !M1Direction)
      M1Direction = true;  //Forward
  }
  M1ENCALast = Lstate1;
  if(!M1Direction)  
    M1duration++;
  else  
    M1duration--;
}

void wheelSpeedM2() {
  int Lstate2 = digitalRead(M2ENCA);
  if((M2ENCALast == LOW) && Lstate2==HIGH)
  {
    int val = digitalRead(M2ENCB);
    if(val == LOW && M2Direction)
      M2Direction = false; //Reverse
    else if(val == HIGH && !M2Direction)
      M2Direction = true;  //Forward
  }
  M2ENCALast = Lstate2;
  if(!M2Direction)  
    M2duration++;
  else  
    M2duration--; 
}

void wheelSpeedM3() {
  int Lstate3 = digitalRead(M3ENCA);
  if((M3ENCALast == LOW) && Lstate3==HIGH)
  {
    int val = digitalRead(M3ENCB);
    if(val == LOW && M3Direction)
      M3Direction = false; //Reverse
    else if(val == HIGH && !M3Direction)
      M3Direction = true;  //Forward
  }
  M3ENCALast = Lstate3;
  if(!M3Direction)  
    M3duration++;
  else  
    M3duration--; 
}

void wheelSpeedM4() {
  int Lstate4 = digitalRead(M4ENCA);
  if((M4ENCALast == LOW) && Lstate4==HIGH)
  {
    int val = digitalRead(M4ENCB);
    if(val == LOW && M4Direction)
      M4Direction = false; //Reverse
    else if(val == HIGH && !M4Direction)
      M4Direction = true;  //Forward
  }
  M4ENCALast = Lstate4;
  if(!M4Direction)  
    M4duration++;
  else  
    M4duration--;
/*--------------------------------------------------------------------------------*/


/*------------------------------------SHIELD--------------------------------------*/

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

void setRegisterPin(int index, int value){
  registers[index] = value;
}
/*--------------------------------------------------------------------------------*/


void setup() {
  // SERIAL COMMUNICATION
  Serial.begin(9600);

  // ENCODERS
  EncoderInit();       // Initialisation des modules

  // SHIELD  
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

// SHIELD VARIABLES
int ms1=0;  
int ms2=0;
int ms3=0;
int ms4=0;
int mr1=0;
int mr2=0;
int mr3=0;
int mr4=0;

void loop() {
  
  // SERIAL COMMUNICATION
  char RxedByte = 0;
  if(Serial.available()){
    RxedByte = Serial.read();
    if(RxedByte == 's'){
      ms1++;
      ms2++;
      ms3++;
      ms4++;
    }else if(RxedByte == 'z'){
      ms1--;
      ms2--;
      ms3--;
      ms4--;
    }else if(RxedByte == 'a'){
      ms1++;
      ms4++;
    }else if(RxedByte == 'e'){
      ms2++;
      ms3++;
    }
    if(ms1<0)
      mr1=-1
    else
      mr1=+1

    if(ms2<0)
      mr2=-1
    else
      mr2=+1

    if(ms3<0)
      mr3=-1
    else
      mr3=+1

    if(ms4<0)
      mr4=-1
    else
      mr4=+1              
  }

  // SHIELD
  motorsDirections(mr1,mr2,mr3,mr4); // Sens de rotations des Moteurs
  motorsSpeed(ms1,ms2,ms3,ms4);  // Vitesse des Moteurs

  // Encoders
  M1nb_tours +=  abs(M1duration)/1600;   
  M2nb_tours +=  abs(M2duration)/1600;
  M3nb_tours +=  abs(M3duration)/1600;
  M4nb_tours +=  abs(M4duration)/1600;
  M1duration = 0;
  M2duration = 0;
  M3duration = 0;
  M4duration = 0;
  delay(100);
  

}
