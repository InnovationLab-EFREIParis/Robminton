// Programme de balayage de la surface du terrain de badbigton par un robot équipé de roue mecanum

// ********************************************************************************************************************************************************************************
// Déclaration des constantes et des variables
// ********************************************************************************************************************************************************************************

// Déclaration des données du pont en H **************************************************************
#include <AFMotor.h>
AF_DCMotor motorAvDr(1);
AF_DCMotor motorAvGa(2);
AF_DCMotor motorArGa(3);
AF_DCMotor motorArDr(4);

// Déclaration des données des interruptions **********************************************************
int interruptPin_Dist = 2  ;
int interruptPin_MotAvGa = 18  ;
int interruptPin_MotAvDr = 19  ;
int interruptPin_MotArGa = 20  ;
int interruptPin_MotArDr = 21  ;
int compt_Dist = 1 ;
int compt_MotAvGa = 0;
int compt_MotAvDr = 0;
int compt_MotArGa = 0;
int compt_MotArDr = 0;
const int frequence_echantillonnage = 5;  // Fréquence d'exécution de l'asservissement (Xxx fois par seconde)

// Déclaration des caractéristique du moteur ***********************************************************
const int rapport_reducteur_MoteursDeplacement = 15;   // Rapport entre le nombre de tours de l'arbre moteur et de la roue
const int tick_par_tour_codeuse_MoteursDeplacement = 26;  //26 tick sur deux capteurs hall par capteur pour front montant et descendant, 13 sur un capteur 
const float DiametreRoue = 6.34 ;  // diametre de la roue 6,34 cm
float k_reductionMoteurs =(float)tick_par_tour_codeuse_MoteursDeplacement*(float)rapport_reducteur_MoteursDeplacement ;
float k_reductionDistance = 3.141592654*DiametreRoue/(float)tick_par_tour_codeuse_MoteursDeplacement/(float)rapport_reducteur_MoteursDeplacement ;

// Déclaration des caractéristique du robot ***********************************************************
int largeur_robot = 40 ; // cm
int longueur_demi_tour = 30 ; // cm

// Déclaration timer ***********************************************************************************
#include <SimpleTimer.h>
SimpleTimer timer_MoteursDeplacement;


// Déclaration des constantes et variables du PID *******************************************************
// Déclaration des constantes ********************************
float kp_MotAvGa = 8;     // coefficient proportionnel
float ki_MotAvGa = 5;       // Coefficient intégrateur
float kd_MotAvGa = 2.5;     // Coefficient dérivateur

float kp_MotAvDr = 8;     // coefficient proportionnel
float ki_MotAvDr= 5;       // Coefficient intégrateur
float kd_MotAvDr = 2.5;     // Coefficient dérivateur

float kp_MotArGa = 8;     // coefficient proportionnel
float ki_MotArGa = 5;       // Coefficient intégrateur
float kd_MotArGa = 2.5;     // Coefficient dérivateur

float kp_MotArDr = 8;     // coefficient proportionnel
float ki_MotArDr= 5;       // Coefficient intégrateur
float kd_MotArDr = 2.5;     // Coefficient dérivateur

// Initialisation des variables *******************************
float consigne_Dist = 100; //en cm
float Dist = 0;//en cm
bool Arret_Moteurs = false;
float consigne_des_moteurs = 5;

int vit_MotAvGa = 0;
float consigne_MotAvGa = 5; // consigne en nombre de tours de roue par seconde
float erreur_precedente_MotAvGa = consigne_MotAvGa;
float somme_erreur_MotAvGa = 0;

int vit_MotAvDr = 0;
float consigne_MotAvDr = consigne_MotAvGa; // consigne en nombre de tours de roue par seconde
float erreur_precedente_MotAvDr = consigne_MotAvDr;
float somme_erreur_MotAvDr = 0;

int vit_MotArGa = 0;
float consigne_MotArGa = consigne_MotAvGa; // consigne en nombre de tours de roue par seconde
float erreur_precedente_MotArGa = consigne_MotArGa;
float somme_erreur_MotArGa = 0;

int vit_MotArDr = 0;
float consigne_MotArDr = consigne_MotAvGa; // consigne en nombre de tours de roue par seconde
float erreur_precedente_MotArDr = consigne_MotArDr;
float somme_erreur_MotArDr = 0;


// Initialisation des données du séquenciel de balayage de la surface.
int Phase = 0;
int ref_Temps = 0 ;
int mes_Temps = 0 ;
int duration = 0 ;
int coef_compensation_arret = 7 ; // coefficient pour compenser l'arrêt. Après arret de l'alimenattion des des moteurs, du fait de l'inertie le robot continue d'avancer quelques secondes.
int nb_passage = 0 ;
int compt_nb_passage = 1 ;

//################################################################################################################################################################################
// Initialisation des dimension du terrain de badminton
int largeur_terrain = 120 ; //cm
int longueur_terrain = 100 ; //cm
//################################################################################################################################################################################


// ********************************************************************************************************************************************************************************
// Setpup - initialisation des fonctions
// ********************************************************************************************************************************************************************************

void setup() {
  // initialize serial communication at 9600 bits per second:
      Serial.begin(9600);
      Serial.println("DébutSetup");
  // Intialisation des interruptions pour le calcul des compteurs des encodeurs de chaque moteur pour alimenter la fonction d'asservissement des moteurs -----------------------------
  attachInterrupt(digitalPinToInterrupt(interruptPin_MotAvGa), F_cpt_MotAvGa, CHANGE);
  attachInterrupt(digitalPinToInterrupt(interruptPin_MotAvDr), F_cpt_MotAvDr, CHANGE);
  attachInterrupt(digitalPinToInterrupt(interruptPin_MotArGa), F_cpt_MotArGa, CHANGE);
  attachInterrupt(digitalPinToInterrupt(interruptPin_MotArDr), F_cpt_MotArDr, CHANGE);  
  // Intialisation de l'interruption pour le calcul du compteur de l'encodeur du moteur pour alimenter la fonction de calcul de la distance parcoure par le robot -------------------
  attachInterrupt(digitalPinToInterrupt(interruptPin_Dist), F_cpt_Dist, CHANGE);

  // Initialisation du timer pour le calcul périodique de l'asservissement des 4 moteurs d'netrainement du robot --------------------------------------------------------------------
  int FreqTimer=1000/frequence_echantillonnage;
  timer_MoteursDeplacement.setInterval(FreqTimer,asservissement_MoteursDeplacement); // Interruption pour calcul du PID et asservissement; toutes les 10ms, on recommence la routine

  // Initialisation de l'algorithme de balayage -------------------------------------------------------------------------------------------------------------------------------------
  //Calcul du nombre de passage
  nb_passage = largeur_terrain / largeur_robot;
  ref_Temps = millis(); // Initialisation  du compteur temps pour calculer un temps d'attente avant le mouvement suivant
      Serial.print("Consigne du nbre de passage");
      Serial.println(nb_passage);
    
      Serial.println("FinSetup");
}

// ********************************************************************************************************************************************************************************
// Loop - Boucle principale
// ********************************************************************************************************************************************************************************

void loop() {
 // Lancement du timer d'asservissement des moteurs avec encodeurs ---------------------------------------------------------------------------------------------------------------
 timer_MoteursDeplacement.run();  //on fait tourner l'horloge

 // Algorithme de traitement du séquentiel de déplacement du robot  ---------------------------------------------------------------------------------------------------------------
 // Définit la consigne de vitesse, la distance à pourcourir, le type de mouvement à réaliser  ------------------------------------------------------------------------------------
 // Le balayage du terrain est effectué en X traversées (appelées passage) chaque passage est composer de 3 mouvements : Marche avant pour balayer la longueur du terrain,*
 // déplacement latéral de la largeur du robot - x cm (pour garantir un balayage complet et pour finir une rotation de 180°.
 
  if (compt_nb_passage < nb_passage+1)
  { 
     if (Phase == 0)  //  Phase 0 : Traversée du terrain
      {
        mes_Temps = millis ();
        duration = mes_Temps-ref_Temps ;
        if( (duration) > 500)
        {
              Serial.println("J'avance");
          consigne_Dist = longueur_terrain;
          consigne_des_moteurs = 5;
          goForward();
        }
      }
    
      if (Phase== 1)  // Phase 1 : déplacement latéral droit ou gauche en du modulo du n° de traversé du terrain
      {
        mes_Temps = millis ();
        duration = mes_Temps-ref_Temps ;
        if( (duration) > 500)
        {
            if (compt_nb_passage%2 ==1)
          {
               Serial.println("Je me déplace à droite");
            consigne_Dist = largeur_robot;
            consigne_des_moteurs = 6;
            moveRight();
            Arret_Moteurs = false ;    }
           else if (compt_nb_passage%2 ==0)
           {
                Serial.println("Je me déplace à gauche");
            consigne_Dist = 50;
            consigne_des_moteurs = 6;
            moveLeft();
            Arret_Moteurs = false ; 
           }
        }
      }
    
      if (Phase== 2)  // Phase 2 : retournement complet du robot(180°) 
      {
        mes_Temps = millis ();
        duration = mes_Temps-ref_Temps ;
        if( (duration) > 500)
        {
              Serial.println("Je fais demi-tour");
          consigne_Dist = longueur_demi_tour;
          consigne_des_moteurs = 4.5;
          turnRight();
          Arret_Moteurs = false ; 
        }
      }
      if (Phase== 3) // Phase 3 : phase de cloture et d'incrément du compteur de nbre de passage
        {
              Serial.print("Nbre de passage = ");
              Serial.println(compt_nb_passage);
          compt_nb_passage++;
          Phase = 0;
        }
  }

 // Calcul de la distance parcourue par le robot pour arrêter déplacement du robot------------------------------------------------------------------------------------------------------
  Fct_CalculDistance ();

// Fin Boucle de traitement du séquentiel de gestion du balayage de la surface du terrain-----------------------------------------------------------------------------------------------
}

// ********************************************************************************************************************************************************************************
// Déclaration de toutes les fonctions
// ********************************************************************************************************************************************************************************


// Fonction asservissement des moteurs de déplacement  *********************************************************************************************************************************
void asservissement_MoteursDeplacement()
{
  if (Arret_Moteurs == false)  // Si la distance parcourue est supérieure à la consigne, le flag Arret_Moteurs est à vrai 
    //et le calcul d'asservissement ne doit pas s'executer pour ne pas envoyer de nouvelle consignes aux moteurs
    {
       // Calcul asservissement Moteur AvGa ------------------------------------------------------------------------
      // Calcul du nombre de tick de la codeuse du moteur par seconde : Frequence Echan * tick_codeuse
      int frequence_codeuse_MotAvGa = frequence_echantillonnage*compt_MotAvGa; 
      // Calcul de la vitesse de la roue
      float vit_roue_tour_sec_MotAvGa = (float)frequence_codeuse_MotAvGa/k_reductionMoteurs ;  
      // calcul erreur pour chaque composante du PID
      float erreur_MotAvGa = consigne_des_moteurs - vit_roue_tour_sec_MotAvGa; // pour le proportionnel
      somme_erreur_MotAvGa += erreur_MotAvGa; // pour l'intégrateur
      float delta_erreur_MotAvGa = somme_erreur_MotAvGa-erreur_precedente_MotAvGa;  // pour le dérivateur
      erreur_precedente_MotAvGa = erreur_MotAvGa;
      // PID : calcul de la commande pour chaque moteur -----------------------------------
      vit_MotAvGa = kp_MotAvGa*erreur_MotAvGa + ki_MotAvGa*somme_erreur_MotAvGa + kd_MotAvGa*delta_erreur_MotAvGa;  //somme des tois erreurs
       // Réinitialisation du nombre de tick de la codeuse ----------------------------------
      compt_MotAvGa = 0;
      // Normalisation et contrôle des moteurs --------------------------------------------
      if (vit_MotAvGa > 255) {
          vit_MotAvGa = 255;  // sachant que l'on est branché sur un pont en H L293D
        }   else if (vit_MotAvGa <0) {
          vit_MotAvGa = 0;
        }
      
      // Calcul asservissement Moteur AvDr ------------------------------------------------------------------------
      // Calcul du nombre de tick de la codeuse du moteur par seconde : Frequence Echan * tick_codeuse
      int frequence_codeuse_MotAvDr = frequence_echantillonnage*compt_MotAvDr; 
      // Calcul de la vitesse de la roue
      float vit_roue_tour_sec_MotAvDr = (float)frequence_codeuse_MotAvDr/k_reductionMoteurs;  
      // calcul erreur pour chaque composante du PID
      float erreur_MotAvDr = consigne_des_moteurs - vit_roue_tour_sec_MotAvDr; // pour le proportionnel
      somme_erreur_MotAvDr += erreur_MotAvDr; // pour l'intégrateur
      float delta_erreur_MotAvDr = somme_erreur_MotAvGa-erreur_precedente_MotAvDr;  // pour le dérivateur
      erreur_precedente_MotAvDr = erreur_MotAvDr;
      // PID : calcul de la commande pour chaque moteur -----------------------------------
      vit_MotAvDr = kp_MotAvDr*erreur_MotAvDr + ki_MotAvDr*somme_erreur_MotAvDr + kd_MotAvDr*delta_erreur_MotAvDr;  //somme des tois erreurs
       // Réinitialisation du nombre de tick de la codeuse ----------------------------------
      compt_MotAvDr = 0;
      // Normalisation et contrôle des moteurs --------------------------------------------
      if (vit_MotAvDr > 255) {
          vit_MotAvDr = 255;  // sachant que l'on est branché sur un pont en H L293D
        }   else if (vit_MotAvDr <0) {
          vit_MotAvDr = 0;
        }
    
      // Calcul asservissement Moteur ArGa ------------------------------------------------------------------------
      // Calcul du nombre de tick de la codeuse du moteur par seconde : Frequence Echan * tick_codeuse
      int frequence_codeuse_MotArGa = frequence_echantillonnage*compt_MotArGa; 
      // Calcul de la vitesse de la roue
      float vit_roue_tour_sec_MotArGa = (float)frequence_codeuse_MotArGa/k_reductionMoteurs;  
      // calcul erreur pour chaque composante du PID
      float erreur_MotArGa = consigne_des_moteurs - vit_roue_tour_sec_MotArGa; // pour le proportionnel
      somme_erreur_MotArGa += erreur_MotArGa; // pour l'intégrateur
      float delta_erreur_MotArGa = somme_erreur_MotArGa-erreur_precedente_MotArGa;  // pour le dérivateur
      erreur_precedente_MotArGa = erreur_MotArGa;
      // PID : calcul de la commande pour chaque moteur -----------------------------------
      vit_MotArGa = kp_MotArGa*erreur_MotArGa + ki_MotArGa*somme_erreur_MotArGa + kd_MotArGa*delta_erreur_MotArGa;  //somme des tois erreurs
       // Réinitialisation du nombre de tick de la codeuse ----------------------------------
      compt_MotArGa = 0;
      // Normalisation et contrôle des moteurs --------------------------------------------
      if (vit_MotArGa > 255) {
          vit_MotArGa = 255;  // sachant que l'on est branché sur un pont en H L293D
        } 
        else if (vit_MotArGa <0) {
          vit_MotArGa = 0;
        } 
      
      // Calcul asservissement Moteur ArDr ------------------------------------------------------------------------
      // Calcul du nombre de tick de la codeuse du moteur par seconde : Frequence Echan * tick_codeuse
      int frequence_codeuse_MotArDr = frequence_echantillonnage*compt_MotArDr; 
      // Calcul de la vitesse de la roue
      float vit_roue_tour_sec_MotArDr = (float)frequence_codeuse_MotArDr/k_reductionMoteurs ;  
      // calcul erreur pour chaque composante du PID
      float erreur_MotArDr = consigne_des_moteurs - vit_roue_tour_sec_MotArDr;      // pour le proportionnel
      somme_erreur_MotArDr += erreur_MotArDr;                                       // pour l'intégrateur
      float delta_erreur_MotArDr = somme_erreur_MotArGa-erreur_precedente_MotArDr;  // pour le dérivateur
      erreur_precedente_MotArDr = erreur_MotArDr;
      // PID : calcul de la commande pour chaque moteur -----------------------------------
      vit_MotArDr = kp_MotArDr*erreur_MotArDr + ki_MotArDr*somme_erreur_MotArDr + kd_MotArDr*delta_erreur_MotArDr;  //somme des tois erreurs
       // Réinitialisation du nombre de tick de la codeuse ----------------------------------
      compt_MotArDr = 0;
      // Normalisation et contrôle des vitesses moteurs --------------------------------------------
      if (vit_MotArDr > 255) {
          vit_MotArDr = 255;  // sachant que l'on est branché sur un pont en H L293D
        }   else if (vit_MotArDr <0) {
          vit_MotArDr = 0;
        } 
      // Envoie de la nouvelle référence vitesse à chaque moteur ----------------
      motorArDr.setSpeed(vit_MotArDr);
      motorArGa.setSpeed(vit_MotArGa);
      motorAvDr.setSpeed(vit_MotAvDr);
      motorAvGa.setSpeed(vit_MotAvGa);
  }
}

// Fonction de calcul de la distance parcourue par le robot. Cette fonction est déclenchée par une interruption connectée au codeur numéro 2 d'une des roue du robot ******************
void Fct_CalculDistance ()
{
 //Calcul de la distance
Dist = compt_Dist * k_reductionDistance ;
  if (Dist >= (consigne_Dist-coef_compensation_arret)) // Arrêt du robot si la distance parcourue est >= à la consigne
  {
          Serial.print("Distance calculée = ");
          Serial.print(Dist);
          Serial.print("  Nombre d'impulsion du compteur = ");
          Serial.println (compt_Dist);
    hardStop(); // déclenchement de l'arrêt des moteurs
    Arret_Moteurs = true; // Flag d'arrêt des moteurs à "VRAI" ... permet de courcuiter le traitement de la fonction asservissement
    compt_Dist=0; // remise à zéro du compteur de distance
    ref_Temps = millis(); // Initialisation  du compteur temps pour calculer un temps d'attente avant le mouvement suivant
    Phase++; // Incrément du compteur de phase pour l'algorithme séquenciel de balayage
  } 
}

// Fonctions comptage des tick des encodeurs des moteurs. Ces fonctions sont déclenchées sur interruption ******************************************************************************
void F_cpt_MotAvGa() {  compt_MotAvGa++; }
void F_cpt_MotAvDr() {  compt_MotAvDr++; }
void F_cpt_MotArGa() {  compt_MotArGa++; }
void F_cpt_MotArDr() {  compt_MotArDr++; }
void F_cpt_Dist()    {  compt_Dist++;    }

// Fonctions definissant les mouvements élémentaires du robot ***********************************************************************************************************************

void goForward(){
  motorAvDr.run(FORWARD);
  motorAvGa.run(FORWARD);
  motorArDr.run(FORWARD);
  motorArGa.run(FORWARD);
}// void goForward()

void goBackward(){
  motorAvDr.run(BACKWARD);
  motorAvGa.run(BACKWARD);
  motorArDr.run(BACKWARD);
  motorArGa.run(BACKWARD);  
}// void goBackwad()

void moveRight(){
  motorAvDr.run(BACKWARD);
  motorAvGa.run(FORWARD);
  motorArDr.run(FORWARD);
  motorArGa.run(BACKWARD);
}// void moveRight()

void moveLeft(){
  motorAvDr.run(FORWARD);
  motorAvGa.run(BACKWARD);
  motorArDr.run(BACKWARD);
  motorArGa.run(FORWARD);
}// void moveLeft()

void turnRight(){
  motorAvDr.run(BACKWARD);
  motorAvGa.run(FORWARD);
  motorArDr.run(BACKWARD);
  motorArGa.run(FORWARD);
}// void turnRight()

void turnLeft(){
  motorAvDr.run(FORWARD);
  motorAvGa.run(BACKWARD);
  motorArDr.run(FORWARD);
  motorArGa.run(BACKWARD);
}// void turnRight()

void hardStop(){
  motorAvDr.run(RELEASE);
  motorAvGa.run(RELEASE);
  motorArDr.run(RELEASE);
  motorArGa.run(RELEASE);
}// void hardRobot()
