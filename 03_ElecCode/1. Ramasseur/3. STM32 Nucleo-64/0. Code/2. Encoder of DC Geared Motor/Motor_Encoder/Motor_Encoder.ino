/*
  ENCODER CQGB37Y001:
  
  Le codeur du moteur CQRobot DC Geared Motor est conçu pour mesurer
  la rotation d'un moteur pour contrôler le nombre de tour et la 
  distance parcourue par chaque moteurs.

  Comme nous l’utiliserons sur une STM32 on définira les 
  les inputs de la manière suivante

*/

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

void setup() {
  Serial.begin(57600); // Serial comunication pour debuger 
  EncoderInit();       // Initialisation des modules
}

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
}

void loop() {
  Serial.print("Pulse:");
  Serial.println(M1duration);
  Serial.println(M2duration);
  Serial.println(M3duration);
  Serial.println(M4duration);
  
  M1nb_tours +=  abs(M1duration)/1600;   
  M2nb_tours +=  abs(M2duration)/1600;
  M3nb_tours +=  abs(M3duration)/1600;
  M4nb_tours +=  abs(M4duration)/1600; 

  Serial.print("\nNombre de tours M1: \r");
  Serial.println(M1nb_tours);
  Serial.print("\nNombre de tours M2: \r");
  Serial.println(M2nb_tours);
  Serial.print("\nNombre de tours M3: \r");
  Serial.println(M3nb_tours);
  Serial.print("\nNombre de tours M4: \r");
  Serial.println(M4nb_tours);
  M1duration = 0;
  M2duration = 0;
  M3duration = 0;
  M4duration = 0;
  delay(100);
}