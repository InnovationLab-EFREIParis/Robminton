#include <AFMotor.h>


AF_DCMotor motorAvDr(1);
AF_DCMotor motorAvGa(2);
AF_DCMotor motorArGa(3);
AF_DCMotor motorArDr(4);

//declaration des variables 

int tension_nominale = 12; //V
int vitesse_tn = 120; //tr/min
float tension_moteur = 11.7; //V
int k1 = 25; //cm
int largeur_terrain = 160; //cm
int longueur_terrain = 1000; //cm
int largeur_robot = 40;
int k2 = 0; //cm
int nb_passage;
int t_traversee;
int t_deplacement_lateral; //microsec
float v_rotation; //tr/min
float d_roues = 6; //cm
int mspeed = 120; 
float circonference;
float vitesse_lineaire; 
int i;
int t_demi_tour =9000;

void setup() {

// initialize serial communication at 9600 bits per second:
  Serial.begin(9600);

  
    //Calcul du nombre de passage
  nb_passage = largeur_terrain / largeur_robot;
    //Calcul vitesse de rotation 
  v_rotation = ( vitesse_tn * tension_moteur ) / tension_nominale;
  //Serial.print("v_rotation : ");
  //Serial.println(v_rotation);
    //Calcul vitesse linéaire en marche avant
  circonference = 3.1415 * d_roues; //cm
  vitesse_lineaire = (circonference * v_rotation)/60000; // cm/microsec
  //Serial.print("vitesse_lineaire : ");
  //Serial.println(vitesse_lineaire);
    //Calcul du temps de traversée
  t_traversee = (longueur_terrain + k1) / vitesse_lineaire; //microsec
  //Serial.print("temps traversée: ");
  //Serial.print(t_traversee);
  //Serial.println("micro secondes");
    //Calcul du temps de déplacement lateral
  t_deplacement_lateral = (largeur_robot + k2 )/ vitesse_lineaire;
  //Serial.print("deplacement lateral: ");
  //Serial.println(t_deplacement_lateral);
  for (i=1; i<nb_passage+1; i++)
  
  {
    //Serial.print ("nombre de passage: ");
    //Serial.println(i);
    uGoForward(mspeed, t_traversee);
    //Serial.println("J'avance");
    hardStop();
    delay(50);
    uTurnRight(mspeed, t_demi_tour);
    //Serial.println("Demi tour");
    hardStop();
    delay(50);
    if (i%2 ==1)
    {
       uMoveLeft(mspeed, t_deplacement_lateral);
       //Serial.println("Je vais a gauche");
       hardStop();
       delay(50);
    }
     else if (i%2 ==0)
     {
      uMoveRight(mspeed, t_deplacement_lateral);
      //Serial.println("Je vais a droite");
      hardStop();
      delay(50);
     }
  }
}



void loop() {
 
//uTurnRight(255);
//hardStop();
//delay(5000);
}


void uTurnRight(int mspeed, int a) {
for (int Speed=55;Speed<=mspeed;Speed=Speed+10) 
  {
    turnRight(Speed);
    delay(10);
    }
delay(a);
}

void uGoForward(int mspeed, int a) {
for (int Speed=55;Speed<=mspeed;Speed=Speed+10) 
  {
    goForward(Speed);
    delay(10);
    }
delay(a);
}

void uMoveLeft(int mspeed, int b) {
for (int Speed=55;Speed<=mspeed;Speed=Speed+10) 
  {
    moveLeft(Speed);
    delay(10);
    }
delay(b);
}

void uMoveRight(int mspeed, int b) {
for (int Speed=55;Speed<=mspeed;Speed=Speed+10) 
  {
    moveRight(Speed);
    delay(10);
    }
delay(b);
}

void goForward(int mspeed){
  motorAvDr.setSpeed(mspeed);
  motorAvGa.setSpeed(mspeed);
  motorArDr.setSpeed(mspeed);
  motorArGa.setSpeed(mspeed);
  motorAvDr.run(FORWARD);
  motorAvGa.run(FORWARD);
  motorArDr.run(FORWARD);
  motorArGa.run(FORWARD);
}// void goForward(int mspeed)

void goBackward(int mspeed){
  motorAvDr.setSpeed(mspeed);
  motorAvGa.setSpeed(mspeed);
  motorArDr.setSpeed(mspeed);
  motorArGa.setSpeed(mspeed);
  motorAvDr.run(BACKWARD);
  motorAvGa.run(BACKWARD);
  motorArDr.run(BACKWARD);
  motorArGa.run(BACKWARD);  
}// void goBackwad(int mspeed)

void moveRight(int mspeed){
  motorAvDr.setSpeed(mspeed);
  motorAvGa.setSpeed(mspeed);
  motorArDr.setSpeed(mspeed);
  motorArGa.setSpeed(mspeed);
  motorAvDr.run(BACKWARD);
  motorAvGa.run(FORWARD);
  motorArDr.run(FORWARD);
  motorArGa.run(BACKWARD);
}// void moveRight(int mspeed)

void moveLeft(int mspeed){
  motorAvDr.setSpeed(mspeed);
  motorAvGa.setSpeed(mspeed);
  motorArDr.setSpeed(mspeed);
  motorArGa.setSpeed(mspeed);
  motorAvDr.run(FORWARD);
  motorAvGa.run(BACKWARD);
  motorArDr.run(BACKWARD);
  motorArGa.run(FORWARD);
}// void moveLeft(int mspeed)

void moveRightForward(int mspeed){
  motorAvDr.setSpeed(0);
  motorAvGa.setSpeed(mspeed);
  motorArDr.setSpeed(mspeed);
  motorArGa.setSpeed(0);
  motorAvDr.run(FORWARD);
  motorAvGa.run(FORWARD);
  motorArDr.run(FORWARD);
  motorArGa.run(FORWARD);
}// void  moveRightForward(int mspeed)

void moveRightBackward(int mspeed){
  motorAvDr.setSpeed(mspeed);
  motorAvGa.setSpeed(0);
  motorArDr.setSpeed(0);
  motorArGa.setSpeed(mspeed);
  motorAvDr.run(BACKWARD);
  motorAvGa.run(FORWARD);
  motorArDr.run(FORWARD);
  motorArGa.run(BACKWARD);
}// void  moveRightBackward(int mspeed)

void moveLeftForward(int mspeed){
  motorAvDr.setSpeed(mspeed);
  motorAvGa.setSpeed(0);
  motorArDr.setSpeed(0);
  motorArGa.setSpeed(mspeed);
  motorAvDr.run(FORWARD);
  motorAvGa.run(FORWARD);
  motorArDr.run(FORWARD);
  motorArGa.run(FORWARD);
}// void  moveLeftForward(int mspeed)

void moveLeftBackward(int mspeed){
  motorAvDr.setSpeed(0);
  motorAvGa.setSpeed(mspeed);
  motorArDr.setSpeed(mspeed);
  motorArGa.setSpeed(0);
  motorAvDr.run(FORWARD);
  motorAvGa.run(BACKWARD);
  motorArDr.run(BACKWARD);
  motorArGa.run(FORWARD);
}// void  moveLeftBackward(int mspeed)

void turnRight(int mspeed){
  motorAvDr.setSpeed(mspeed);
  motorAvGa.setSpeed(mspeed);
  motorArDr.setSpeed(mspeed);
  motorArGa.setSpeed(mspeed);
  motorAvDr.run(BACKWARD);
  motorAvGa.run(FORWARD);
  motorArDr.run(BACKWARD);
  motorArGa.run(FORWARD);
}// void turnRight(int mspeed)

void turnLeft(int mspeed){
  motorAvDr.setSpeed(mspeed);
  motorAvGa.setSpeed(mspeed);
  motorArDr.setSpeed(mspeed);
  motorArGa.setSpeed(mspeed);
  motorAvDr.run(FORWARD);
  motorAvGa.run(BACKWARD);
  motorArDr.run(FORWARD);
  motorArGa.run(BACKWARD);
}// void turnRight(int mspeed)

void stopRobot(int delay_ms){
  delay(delay_ms);
  motorAvDr.run(RELEASE);
  motorAvGa.run(RELEASE);
  motorArDr.run(RELEASE);
  motorArGa.run(RELEASE);
}// void stopRobot(int delay_ms)

void hardStop(){
  motorAvDr.run(RELEASE);
  motorAvGa.run(RELEASE);
  motorArDr.run(RELEASE);
  motorArGa.run(RELEASE);
}// void stopRobot()
