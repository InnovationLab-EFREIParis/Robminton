int M1_1A = 4;
int M1_1B = 5;
int M1_P_1 = 9;
int M2_2A = 7;
int M2_2B = 8;
int M2_P_2 = 10;
int M1et2_Enable = 6;
int i=0;
//interruption
int interruptCaptIRGa=2;
int interruptCaptIRDr=3;
int interruptMotTapisGa=18;
int interruptMotTapisDr=19;
int Cpt_CaptIRGa=0;
int Cpt_CaptIRDr=0;
int Cpt_EncodeurMotTapisGa=0;
int Cpt_EncodeurMotTapisDr=0;
int erreur_Synchro=0;
float correction_dif=0;
float kDif =0.01;
boolean CoteSyncGa = false;
boolean CoteSyncDr = false;
unsigned long time_MillisGa=0;
unsigned long time_MillisDr=0;
int erreur_synchro_Millis =0;
int k_time_MillisGa = 0 ;
int k_time_MillisDr = 0 ;
int Offset_comptGa = 0;

// Déclaration timer ***********************************************************************************
#include <SimpleTimer.h>
SimpleTimer timerElevateur;
const int frequence_echantillonnage = 20;  // Fréquence d'exécution de l'asservissement (Xxx fois par seconde)
int periode_echantillonage=1000/frequence_echantillonnage;
// Déclaration des caractéristique du moteur ***********************************************************
const int rapport_reducteur_Mots = 40;   // Rapport entre le nombre de tours de l'arbre moteur et de la roue
const int tick_par_tour_codeuse_Mots = 12;  //12 tick sur deux capteurs hall, ici on a pris un seul capteur mais 2 front
// Déclaration des constantes et variables du PID *******************************************************
// Déclaration des constantes ********************************
// Moteur 1 = moteur Droit = Port M1 sur le pont en H.
float kp_Mot1 = 50;     // coefficient proportionnel
float ki_Mot1 = 20;       // Coefficient intégrateur
float kd_Mot1 = 10;     // Coefficient dérivateur
float kp_Mot2 = 50;     // coefficient proportionnel
float ki_Mot2 = 20;       // Coefficient intégrateur
float kd_Mot2 = 10;     // Coefficient dérivateur

// Initialisation des variables *******************************
int vitMot_1 = 0;
float consigne_Mot1 = 2.4; // consigne en nombre de tours de roue par seconde
float erreur_precedente_Mot1 = consigne_Mot1;
float somme_erreur_Mot1 = 0;
int vitMot_2 = 0;
float consigne_Mot2 = 2.4;
float erreur_precedente_Mot2 = consigne_Mot2;
float somme_erreur_Mot2 = 0;


void setup() { 
  
  // Motor_1 controll pin initiate; 
  pinMode(M1_1A, OUTPUT); 
  pinMode(M1_1B, OUTPUT); 
  pinMode(M1_P_1, OUTPUT);   // Speed control 
  // Motor_2 controll pin initiate; 
  pinMode(M2_2A, OUTPUT); 
  pinMode(M2_2B, OUTPUT); 
  pinMode(M2_P_2, OUTPUT); // Speed control 
  //Enable the Motor Shield output; 
  pinMode(M1et2_Enable, OUTPUT); 
  digitalWrite(M1et2_Enable, HIGH);
   
  // Initialisation liaison série
  Serial.begin(19200);
  Serial.println("Begin");

  //synchronisation tapis
  
  syncTapis();
  delay(3000);

  // initialialisation interruption pour compensation des écarts de courroies
  attachInterrupt(digitalPinToInterrupt(interruptCaptIRGa), fct_SynchroIRGa, FALLING);
  attachInterrupt(digitalPinToInterrupt(interruptCaptIRDr), fct_SynchroIRDr, FALLING);
  time_MillisGa = millis();
  time_MillisDr = millis();

  attachInterrupt(digitalPinToInterrupt(interruptMotTapisGa), fct_EncodeurMotTapisGa, CHANGE);
  attachInterrupt(digitalPinToInterrupt(interruptMotTapisDr), fct_EncodeurMotTapisDr, CHANGE);
    // Initialisation du timer-----------------------------------------------------------------------------
 
  Serial.print("periode_echantillonage = ");
  Serial.println(periode_echantillonage);
  timerElevateur.setInterval(periode_echantillonage,asservissement_Mots); // Interruption pour calcul du PID et asservissement; toutes les 10ms, on recommence la routine

  } 
 
  void loop() { 
  
  // Boucle pour asservissement des moteurs avec encodeurs -----------------------------------------------
  timerElevateur.run();  //on fait tourner l'horloge
  delay(10);
// Fin Boucle pour asservissement des moteurs avec encodeurs ---------------------------------------------
  }


void syncTapis()
{

  //interruption 
  attachInterrupt(digitalPinToInterrupt(interruptCaptIRGa), fct_CaptIRGa, FALLING);
  attachInterrupt(digitalPinToInterrupt(interruptCaptIRDr), fct_CaptIRDr, FALLING);

  //lancement rotation à vitess maxi pour synchronisation
  analogWrite(M2_P_2,255); // set the motor_2 speed ; 
  analogWrite(M1_P_1,255); // set the motor_1 speed ;
  digitalWrite(M2_2A, HIGH); 
  digitalWrite(M1_1A, LOW); 
  digitalWrite(M2_2B, LOW); // Set the rotation of motor_2 
  digitalWrite(M1_1B, HIGH); // Set the rotation of motor_1 

  
   //interruption
   Serial.println("Lancement synchronisation");
    do 
    {
     /*
      Serial.print("Cpt_CaptIRGa=");
      Serial.print(Cpt_CaptIRGa);
      Serial.print("  Cpt_CaptIRDr=");
      Serial.println(Cpt_CaptIRDr);
    */  
          if (Cpt_CaptIRGa >= 1)
        {
          analogWrite(M2_P_2,0); // set the motor_2 stop ; 
         // Serial.println("Mousse sous capteur Gauche");
          Cpt_CaptIRGa=0;
          CoteSyncGa = true;
        }
        if (Cpt_CaptIRDr >= 1)
        {
          analogWrite(M1_P_1,0); // set the motor_1 stop ; 
         // Serial.println("Mousse sous capteur Droit");
          Cpt_CaptIRDr=0;
          CoteSyncDr = true;
        }
        delay (100);
    }while(CoteSyncGa == false || CoteSyncDr == false);
    Serial.println("Fin de synchro");
    
    //detacher interruption 
    detachInterrupt(digitalPinToInterrupt(interruptCaptIRGa));
    detachInterrupt(digitalPinToInterrupt(interruptCaptIRDr));
}

void moveTapis ()
{
  //lancement rotation à vitess maxi pour synchronisation
  analogWrite(M2_P_2,255); // set the motor_2 speed ; 
  analogWrite(M1_P_1,255); // set the motor_1 speed ;
  digitalWrite(M2_2A, HIGH); 
  digitalWrite(M1_1A, LOW); 
  digitalWrite(M2_2B, LOW); // Set the rotation of motor_2 
  digitalWrite(M1_1B, HIGH); // Set the rotation of motor_1 

}

void fct_CaptIRGa()
{
  Cpt_CaptIRGa++;
}
void fct_CaptIRDr()
{
  Cpt_CaptIRDr++;
}
void fct_EncodeurMotTapisGa()
{
  Cpt_EncodeurMotTapisGa++;
}
void fct_EncodeurMotTapisDr()
{
  Cpt_EncodeurMotTapisDr++;
}
void fct_SynchroIRGa()
{
  time_MillisGa=millis();
  k_time_MillisGa=1; 
}
void fct_SynchroIRDr()
{
  time_MillisDr=millis();
  k_time_MillisDr=1;
}
/* Interruption pour calcul du PID */
void asservissement_Mots()
{
  //Serial.println("Deb Asserv");
  // Moteur 1 ------------------------------------------------------------------------
  // Calcul du nombre de tick de la codeuse du moteur par seconde : Frequence Echan * tick_codeuse
  int frequence_codeuse_Mot1 = frequence_echantillonnage*Cpt_EncodeurMotTapisGa; 
  // Calcul de la vitesse de la poulie en fonction du réducteur du moteur et du nbre de tick de la roue codeuse
  float vit_roue_tour_sec_Mot1 = (float)frequence_codeuse_Mot1/(float)tick_par_tour_codeuse_Mots/(float)rapport_reducteur_Mots;  
  // calcul erreur pour chaque composante du PID
  float erreur_Mot1 = consigne_Mot1 - correction_dif - vit_roue_tour_sec_Mot1; // pour le proportionnel
  //Serial.print(vit_roue_tour_sec_Mot1*50);
  //Serial.print(",");
  somme_erreur_Mot1 += erreur_Mot1; // pour l'intégrateur
  float delta_erreur_Mot1 = erreur_Mot1- erreur_precedente_Mot1;  // pour le dérivateur
  erreur_precedente_Mot1 = erreur_Mot1;
  // idem pour Moteur 2 ----------------------------------------------------------------
  int frequence_codeuse_Mot2 = frequence_echantillonnage*Cpt_EncodeurMotTapisDr; //100*tick_codeuse
  float vit_roue_tour_sec_Mot2 = (float)frequence_codeuse_Mot2/(float)tick_par_tour_codeuse_Mots/(float)rapport_reducteur_Mots;  
  //Serial.print(vit_roue_tour_sec_Mot2*50);
  //Serial.print(",");
  float erreur_Mot2 = consigne_Mot2 + correction_dif - vit_roue_tour_sec_Mot2; // pour le proportionnel
  somme_erreur_Mot2 += erreur_Mot2; // pour l'intégrateur
  float delta_erreur_Mot2 = erreur_Mot2-erreur_precedente_Mot2;  // pour le dérivateur
  erreur_precedente_Mot2 = erreur_Mot2;
 // Calcul de la désynchronisation des moteurs ----------------------------------
  erreur_Synchro = erreur_Synchro + (Cpt_EncodeurMotTapisGa + Offset_comptGa - Cpt_EncodeurMotTapisDr);
  correction_dif = kDif * erreur_Synchro;
 //    Serial.println(erreur_Synchro);
 // calcul de désynchronisation de passage sous les capteur IR
 if (k_time_MillisGa==1 && k_time_MillisDr==1)
 {
  Serial.print("k_time_MillisGa=");
  Serial.print(k_time_MillisGa);
  Serial.print(" k_time_MillisDr=");
  Serial.print(k_time_MillisDr);  
  Serial.print(" time_MillisGa=");
  Serial.print(time_MillisGa);
  Serial.print(" time_MillisDr=");
  Serial.println(time_MillisDr);
  //erreur_synchro_Millis = erreur_synchro_Millis + (time_MillisGa - time_MillisDr);
  erreur_synchro_Millis = (time_MillisGa - time_MillisDr);
  Serial.print("erreur_synchro_Millis=");
  Serial.println(erreur_synchro_Millis);
  k_time_MillisGa = 0;
  k_time_MillisDr = 0;
 }
 if (erreur_synchro_Millis >= 1500)
 {
  Offset_comptGa=0;
  Offset_comptGa=1;
  
  Serial.print(" offset=");
  Serial.println(Offset_comptGa);
 } 
 else if (erreur_synchro_Millis <= -1500)
 {
 
  Offset_comptGa=0;
  Offset_comptGa=-1;
  
  Serial.print(" offset=");
  Serial.println(Offset_comptGa);
 } 
 else
 {
  Offset_comptGa=0;
 }
 
 //  Serial.print("Offset_comptGa=");
 // Serial.println(Offset_comptGa);
 // Réinitialisation du nombre de tick de la codeuse ----------------------------------
  Cpt_EncodeurMotTapisGa = 0;
  Cpt_EncodeurMotTapisDr = 0;
  

  // PID : calcul de la commande pour chaque moteur -----------------------------------
  vitMot_1 = kp_Mot1*erreur_Mot1 + ki_Mot1*somme_erreur_Mot1 + kd_Mot1*delta_erreur_Mot1;  //somme des tois erreurs
  vitMot_2 = kp_Mot2*erreur_Mot2 + ki_Mot2*somme_erreur_Mot2 + kd_Mot2*delta_erreur_Mot2;  //somme des tois erreurs
  // Normalisation et contrôle des moteurs --------------------------------------------
  if (vitMot_1 > 255) {
    vitMot_1 = 255;  // sachant que l'on est branché sur un pont en H L293D
  } 
  else if (vitMot_1 <0) {
    vitMot_1 = 0;
  }
  if (vitMot_2 > 255) {
    vitMot_2 = 255;  // sachant que l'on est branché sur un pont en H L293D
  } 
  else if (vitMot_2 <0) {
    vitMot_2 = 0;
  }
 /* 
 Serial.print("vitMot1 = ");
 Serial.print(vitMot_1);
 Serial.print(",");
 Serial.print("  vitMot2 = ");
  Serial.println(vitMot_2);
 */ 

  // Commande de la rotation de l'élévateur avec une vitesse par moteur ----------------
  RotationElevateur (vitMot_1, vitMot_2);
}
/* Rotation du tapis */
void RotationElevateur (int powerRateMot_1, int powerRateMot_2) {
  // Sens de rotation du moteur 1
  digitalWrite(M1_1A, LOW);
  digitalWrite(M1_1B, HIGH);
  // Sens de rotation du moteur 2
  digitalWrite(M2_2A, HIGH); 
  digitalWrite(M2_2B, LOW); 
  // Charger la vitesse des moteurs 1 et 2
  analogWrite(M1_P_1, powerRateMot_1);
  analogWrite(M2_P_2, powerRateMot_2);
}
