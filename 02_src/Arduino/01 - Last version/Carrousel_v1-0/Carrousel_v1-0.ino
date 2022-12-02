#include <HCSR04.h>
#include <Stepper.h>

int nombredepas = 48*64;
int trig = 7;
int echo = 5;
long lecture_echo;
long cm;
int i = 0;
int dist = 0;
int signalcapteur = 2;
int nb_volants = 3;
Stepper moteur (nombredepas,9,11,10,6);

void setup(){

pinMode(trig, OUTPUT);
digitalWrite(trig, LOW);
pinMode(echo, INPUT);
Serial.begin(9600);
 moteur.setSpeed(10);




}

void loop(){

digitalWrite(trig, HIGH);
delayMicroseconds(10);
digitalWrite(trig, LOW);
lecture_echo = pulseIn(echo,HIGH);
cm = lecture_echo /58;
Serial.print("Distance en cm :");
Serial.println(cm);
if (cm < 10){
    i++;
    Serial.println(i);
    delay(2000);
}
  if (i == nb_volants){
    moteur.step(6200);
    
    i=0;
  
  }

}
