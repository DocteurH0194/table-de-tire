#include <Arduino.h>
#include <Keypad.h>// Bibliothèque pour le clavier matriciel
#include <SPI.h>// Bibliothèque pour la communication SPI
#include <TM1637Display.h>// Bibliothèque pour les afficheurs à 7 segments TM1637
#include <Wire.h>// Bibliothèque pour la communication I2C
#include <LiquidCrystal_I2C.h>// Bibliothèque pour les écrans LCD I2C
void displayMenu();// Fonction pour afficher le menu sur l'écran LCD
void testalllines();// Fonction pour tester toutes les lignes de sortie
void manuelfire();// Fonction pour gérer les sorties manuelles
void setOutput();// Fonction pour définir les sorties
void setTimers();// Fonction pour définir les timers des sorties
void voyant();// Fonction pour gérer les voyants des sorties
void afficheur();// Fonction pour gérer les afficheurs à 7 segments
void LaunchMenu();// Fonction pour gérer le menu de lancement
void triggersequence();// Fonction pour déclencher la séquence de sorties
//////////////////////////////////////////////initialisation des Variable/////////////////////////////////////////
unsigned long AVTimer1 = 0;// Variable pour le timer de la sortie 1
unsigned long AVTimer2 = 0;// Variable pour le timer de la sortie 2
unsigned long AVTimer3 = 0;// Variable pour le timer de la sortie 3
unsigned long AVTimer4 = 0;// Variable pour le timer de la sortie 4
unsigned long AVTimer5 = 0;// Variable pour le timer de la sortie 5
unsigned long AVTimer6 = 0;// Variable pour le timer de la sortie 6
unsigned long AVTimer7 = 0;// Variable pour le timer de la sortie 7
unsigned long AVTimer8 = 0;// Variable pour le timer de la sortie 8

unsigned int TIMER_OUT_ONE = 0;// Timer pour la sortie 1
unsigned int TIMER_OUT_TWO = 0;// Timer pour la sortie 2
unsigned int TIMER_OUT_THREE = 0;// Timer pour la sortie 3
unsigned int TIMER_OUT_FOUR = 0;// Timer pour la sortie 4
unsigned int TIMER_OUT_FIVE = 0;// Timer pour la sortie 5
unsigned int TIMER_OUT_SIX = 0;// Timer pour la sortie 6
unsigned int TIMER_OUT_SEVEN = 0;// Timer pour la sortie 7
unsigned int TIMER_OUT_EIGHT = 0;// Timer pour la sortie 8

bool ManOne = false;// Variable pour la sortie manuelle 1
bool ManTwo = false;// Variable pour la sortie manuelle 2
bool ManThree = false;// Variable pour la sortie manuelle 3
bool ManFour = false;// Variable pour la sortie manuelle 4
bool ManFive = false;// Variable pour la sortie manuelle 5
bool ManSix = false;// Variable pour la sortie manuelle 6
bool ManSeven = false;// Variable pour la sortie manuelle 7
bool ManEight = false;// Variable pour la sortie manuelle 8

bool One = false;// Variable pour la sortie 1
bool Two = false;// Variable pour la sortie 2
bool Three = false;// Variable pour la sortie 3
bool Four = false;// Variable pour la sortie 4
bool Five = false;// Variable pour la sortie 5
bool Six = false;// Variable pour la sortie 6
bool Seven = false;// Variable pour la sortie 7
bool Eight = false;// Variable pour la sortie 8

//////////////////////////////////////////////initialisation du clavier/////////////////////////////////////////

const byte ROWS = 4; // 4 lignes
const byte COLS = 4; // 4 colonnes
// Définition des touches selon la position dans la matrice (4x4)
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {12, 13, 14, 15};    // Lignes connectées à ces pins Arduino
byte colPins[COLS] = {16, 17, 18, 19};    // Colonnes connectées à ces pins Arduino
// Création de l’objet Keypad pour interagir avec le clavier
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

//////////////////////////////////////////////initialisation des afficheur/////////////////////////////////////////
// Déclaration de chaque afficheur
TM1637Display disp1(10, 30); // CLK, DIO
TM1637Display disp2(10, 32);
TM1637Display disp3(10, 34);
TM1637Display disp4(10, 36);
TM1637Display disp5(10, 38);  
TM1637Display disp6(10, 40);
TM1637Display disp7(10, 42);
TM1637Display disp8(10, 44);

//////////////////////////////////////////////initialisation de lécrant LCD/////////////////////////////////////////
// Déclaration d’un écran LCD à l’adresse I2C 0x27 avec 20 colonnes et 4 lignes
LiquidCrystal_I2C lcd(0x27, 20, 4);
// Énumération des différents écrans (menus) de l’interface utilisateur
enum MenuState {
  MENU_MAIN,
  MENU_OPTIONS,
  MENU_LAUNCH
};
// Le menu actif au démarrage est le menu principal
MenuState currentMenu = MENU_MAIN;

//////////////////////////////////////////////initialisation des pin/////////////////////////////////////////

int master_lauch = 53; // Broche pour le bouton maître de lancement
int buzzer = 11;// Broche pour le buzzer
int radio_in = 51;// Broche pour la radio
int radio_out = 52;// Broche pour la sortie de la radio

int out_one = 2;// Broche pour la sortie 1
int out_two = 3;// Broche pour la sortie 2
int out_three = 4;// Broche pour la sortie 3
int out_four = 5;// Broche pour la sortie 4
int out_five = 6;// Broche pour la sortie 5
int out_six = 7;// Broche pour la sortie 6
int out_seven = 8;// Broche pour la sortie 7
int out_eight = 9;// Broche pour la sortie 8

int voyant_one = 31;// Broche pour le voyant de la sortie 1
int voyant_two = 33;// Broche pour le voyant de la sortie 2
int voyant_three = 35;// Broche pour le voyant de la sortie 3
int voyant_four = 37;// Broche pour le voyant de la sortie 4
int voyant_five = 39;// Broche pour le voyant de la sortie 5
int voyant_six = 41;// Broche pour le voyant de la sortie 6
int voyant_seven = 43;// Broche pour le voyant de la sortie 7
int voyant_eight = 45;// Broche pour le voyant de la sortie 8

int test_out_onein = A8;// Broche analogique A8 pour le test de la sortie 1
int test_out_twoin = A9;// Broche analogique A9 pour le test de la sortie 2
int test_out_threein = A10;// Broche analogique A10 pour le test de la sortie 3
int test_out_fourin = A11;// Broche analogique A11 pour le test de la sortie 4
int test_out_fivein = A12;// Broche analogique A12 pour le test de la sortie 5
int test_out_sixin = A13;// Broche analogique A13 pour le test de la sortie 6
int test_out_sevenin = A14;// Broche analogique A14 pour le test de la sortie 7
int test_out_eightin = A15;// Broche analogique A15 pour le test de la sortie 8

int test_out_oneout = 22;// Broche pour la sortie de test 1
int test_out_twoout = 23;// Broche pour la sortie de test 2
int test_out_threeout = 24;// Broche pour la sortie de test 3
int test_out_fourout = 25;// Broche pour la sortie de test 4
int test_out_fiveout = 26;// Broche pour la sortie de test 5
int test_out_sixout = 27;  // Broche pour la sortie de test 6
int test_out_sevenout = 28;// Broche pour la sortie de test 7
int test_out_eightout = 29;// Broche pour la sortie de test 8

int man_one = A0;// Broche analogique A0 pour le bouton manuel de la sortie 1
int man_two = A1;// Broche analogique A1 pour le bouton manuel de la sortie 2
int man_three = A2;// Broche analogique A2 pour le bouton manuel de la sortie 3
int man_four = A3;// Broche analogique A3 pour le bouton manuel de la sortie 4
int man_five = A4;// Broche analogique A4 pour le bouton manuel de la sortie 5
int man_six = A5;// Broche analogique A5 pour le bouton manuel de la sortie 6
int man_seven = A6;// Broche analogique A6 pour le bouton manuel de la sortie 7
int man_eight = A7;// Broche analogique A7 pour le bouton manuel de la sortie 8
//////////////////////////////////////////////////////////////////setup/////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(9600);// Initialisation de la communication série pour débogage
  //////////////////////////////////////////////écrant LCD/////////////////////////////////////////
  lcd.init();// Initialisation de l'écran LCD
  lcd.backlight();// Allume le rétroéclairage de l'écran LCD
  displayMenu();// Affiche le menu principal sur l'écran LCD
  //////////////////////////////////////////////afficheur/////////////////////////////////////////
  disp1.setBrightness(7);// Définit la luminosité de chaque afficheur à 7 (maximum)
  disp2.setBrightness(7);
  disp3.setBrightness(7);
  disp4.setBrightness(7);
  disp5.setBrightness(7);
  disp6.setBrightness(7);
  disp7.setBrightness(7);
  disp8.setBrightness(7);

  disp1.showNumberDec(TIMER_OUT_ONE);// Affiche le timer de chaque sortie sur l'afficheur correspondant
  disp2.showNumberDec(TIMER_OUT_TWO);
  disp3.showNumberDec(TIMER_OUT_THREE);
  disp4.showNumberDec(TIMER_OUT_FOUR);
  disp5.showNumberDec(TIMER_OUT_FIVE);
  disp6.showNumberDec(TIMER_OUT_SIX);
  disp7.showNumberDec(TIMER_OUT_SEVEN);
  disp8.showNumberDec(TIMER_OUT_EIGHT);
  
  //////////////////////////////////////////////pin/////////////////////////////////////////
  pinMode(master_lauch, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);
  pinMode(radio_in, INPUT);
  pinMode(radio_out, OUTPUT);

  pinMode(out_one, OUTPUT);
  pinMode(out_two, OUTPUT);
  pinMode(out_three, OUTPUT);
  pinMode(out_four, OUTPUT);
  pinMode(out_five, OUTPUT);
  pinMode(out_six, OUTPUT);
  pinMode(out_seven, OUTPUT);
  pinMode(out_eight, OUTPUT);

  digitalWrite(out_one, LOW);
  digitalWrite(out_two, LOW);
  digitalWrite(out_three, LOW);
  digitalWrite(out_four, LOW);
  digitalWrite(out_five, LOW);
  digitalWrite(out_six, LOW);
  digitalWrite(out_seven, LOW);
  digitalWrite(out_eight, LOW);
  
  pinMode(test_out_oneout, OUTPUT);
  pinMode(test_out_twoout, OUTPUT);
  pinMode(test_out_threeout, OUTPUT);
  pinMode(test_out_fourout, OUTPUT);
  pinMode(test_out_fiveout, OUTPUT);
  pinMode(test_out_sixout, OUTPUT);
  pinMode(test_out_sevenout, OUTPUT);
  pinMode(test_out_eightout, OUTPUT);

  digitalWrite(test_out_oneout, LOW);
  digitalWrite(test_out_twoout, LOW); 
  digitalWrite(test_out_threeout, LOW);
  digitalWrite(test_out_fourout, LOW);
  digitalWrite(test_out_fiveout, LOW);
  digitalWrite(test_out_sixout, LOW);
  digitalWrite(test_out_sevenout, LOW);
  digitalWrite(test_out_eightout, LOW);

  pinMode(man_one, INPUT_PULLUP);
  pinMode(man_two, INPUT_PULLUP);
  pinMode(man_three, INPUT_PULLUP);
  pinMode(man_four, INPUT_PULLUP);
  pinMode(man_five, INPUT_PULLUP);
  pinMode(man_six, INPUT_PULLUP);
  pinMode(man_seven, INPUT_PULLUP);
  pinMode(man_eight, INPUT_PULLUP);
}

//////////////////////////////////////////////////////////////////loop/////////////////////////////////////////////////////////////
void loop() { //fonction principale qui tourne en boucle en continu une fois que le système est allumé.
  char key = keypad.getKey(); //Récupère la touche actuellement pressée sur le clavier.
  manuelfire(); // apelle de la fonction manuelfire
  voyant(); // apelle de la fonction voyant
  afficheur(); // apelle de la fonction afficheur

  if (key) { // Si une touche est appuyée...
    switch (currentMenu) { // En fonction du menu affiché actuellement (MENU_MAIN, MENU_OPTIONS, etc.)...
      case MENU_MAIN: // Si on est dans le menu principal :
        if (key == 'A') { // Si on appuie sur A, on passe au menu "Launch".
          currentMenu = MENU_LAUNCH; // on change le menu courant
          displayMenu(); // Affiche les options du menu "Launch"
          LaunchMenu();  // Lance la gestion du menu "Launch"
        } 

        else if (key == 'B') { // Si on appuie sur B, on passe au menu "Options".
          currentMenu = MENU_OPTIONS; // on change le menu courant
          displayMenu(); // appel de la fonction displayMenu
          
        }
        else if (key == 'C') { // Si on appuie sur C,...
          testalllines();  // apelle de la fonction testalllines
        }
      break; // fin du cas MENU_MAIN

      case MENU_OPTIONS: // Si on est dans le menu des options :
        if (key == 'A') { // Si on appuie sur A,...
          lcd.clear(); // efface l'écran LCD
          lcd.setCursor(0, 0); // positionne le curseur en haut à gauche
          lcd.print("Set Output..."); // affiche "Set Output..."
          setOutput(); // appel de la fonction setOutput
          displayMenu(); // appel de la fonction displayMenu
        }
        else if (key == 'B') { // si on appuie sur B,...
          lcd.clear(); // efface l'écran LCD
          lcd.setCursor(0, 0); // positionne le curseur en haut à gauche
          lcd.print("Set Timer..."); // affiche "Set Timer..."
          setTimers(); // appel de la fonction setTimers
          displayMenu(); // appel de la fonction displayMenu
        }
        else if (key == 'D') { // Si on appuie sur D, ...
          currentMenu = MENU_MAIN; // on revient au menu principal
          displayMenu(); // appel de la fonction displayMenu
        }
      break; // fin du cas MENU_OPTIONS
    }
  }
}

//////////////////////////////////////////////////////////////////manuelfire/////////////////////////////////////////////////////////////
void manuelfire() { // Fonction manuelfire
  //sortie 1
  if(analogRead(man_one) < 512) ManOne = true; // Si la valeur lue est inférieure à 512, on défini ManOne à true
  else ManOne = false; // Sinon, on défini ManOne à false
  //sortie 2
  if(analogRead(man_two) < 512) ManTwo = true; // Si la valeur lue est inférieure à 512, on défini ManTwo à true
  else ManTwo = false; // Sinon, on défini ManTwo à false
  //sortie 3
  if(analogRead(man_three) < 512) ManThree = true; // Si la valeur lue est inférieure à 512, on défini ManThree à true
  else ManThree = false; // Sinon, on défini ManThree à false
  //sortie 4
  if(analogRead(man_four) < 512) ManFour = true; // Si la valeur lue est inférieure à 512, on défini ManFour à true
  else ManFour = false; // Sinon, on défini ManFour à false
  //sortie 5
  if(analogRead(man_five) < 512) ManFive = true; // Si la valeur lue est inférieure à 512, on défini ManFive à true
  else ManFive = false; // Sinon, on défini ManFive à false
  //sortie 6
  if(analogRead(man_six) < 512) ManSix = true; // Si la valeur lue est inférieure à 512, on défini ManSix à true
  else ManSix = false; // Sinon, on défini ManSix à false
  //sortie 7
  if(analogRead(man_seven) < 512) ManSeven = true; // Si la valeur lue est inférieure à 512, on défini ManSeven à true
  else ManSeven = false; // Sinon, on défini ManSeven à false
  //sortie 8
  if(analogRead(man_eight) < 512) ManEight = true; // Si la valeur lue est inférieure à 512, on défini ManEight à true
  else ManEight = false; // Sinon, on défini ManEight à false
}

//////////////////////////////////////////////////////////////////voyant/////////////////////////////////////////////////////////////
void  voyant() { // Fonction voyant
  // sortie 1
  if(One == true && ManOne == false) digitalWrite(voyant_one, HIGH); // Si One est true et ManOne est false, on allume le voyant
  else digitalWrite(voyant_one, LOW); // Sinon, on éteint le voyant
  // sortie 2
  if(Two == true && ManTwo == false) digitalWrite(voyant_two, HIGH); // Si Two est true et ManTwo est false, on allume le voyant
  else digitalWrite(voyant_two, LOW); // Sinon, on éteint le voyant
  // sortie 3
  if(Three == true && ManThree == false) digitalWrite(voyant_three, HIGH); // Si Three est true et ManThree est false, on allume le voyant
  else digitalWrite(voyant_three, LOW); // Sinon, on éteint le voyant
  // sortie 4
  if(Four == true && ManFour == false) digitalWrite(voyant_four, HIGH); // Si Four est true et ManFour est false, on allume le voyant
  else digitalWrite(voyant_four, LOW); // Sinon, on éteint le voyant
  // sortie 5
  if(Five == true && ManFive == false) digitalWrite(voyant_five, HIGH); // Si Five est true et ManFive est false, on allume le voyant
  else digitalWrite(voyant_five, LOW); // Sinon, on éteint le voyant
  // sortie 6
  if(Six == true && ManSix == false) digitalWrite(voyant_six, HIGH); // Si Six est true et ManSix est false, on allume le voyant
  else digitalWrite(voyant_six, LOW); // Sinon, on éteint le voyant
  //sortie 7
  if(Seven == true && ManSeven == false) digitalWrite(voyant_seven, HIGH); // Si Seven est true et ManSeven est false, on allume le voyant
  else digitalWrite(voyant_seven, LOW); // Sinon, on éteint le voyant
  //sortie 8
  if(Eight == true && ManEight == false) digitalWrite(voyant_eight, HIGH); // Si Eight est true et ManEight est false, on allume le voyant
  else digitalWrite(voyant_eight, LOW); // Sinon, on éteint le voyant
}

//////////////////////////////////////////////////////////////////afficheur/////////////////////////////////////////////////////////////
void afficheur() { // Fonction afficheur
  if(One == true) disp1.setBrightness(7); // Si One est true, on met la luminosité de disp1 à 7
  else disp1.setBrightness(0); // Sinon, on met la luminosité de disp1 à 0
  if(Two == true) disp2.setBrightness(7); // Si Two est true, on met la luminosité de disp2 à 7
  else disp2.setBrightness(0); // Sinon, on met la luminosité de disp2 à 0
  if(Three== true) disp3.setBrightness(7); // Si Three est true, on met la luminosité de disp3 à 7
  else disp3.setBrightness(0); // Sinon, on met la luminosité de disp3 à 0
  if(Four == true) disp4.setBrightness(7); // Si Four est true, on met la luminosité de disp4 à 7
  else disp4.setBrightness(0); // Sinon, on met la luminosité de disp4 à 0
  if(Five == true) disp5.setBrightness(7); // Si Five est true, on met la luminosité de disp5 à 7
  else disp5.setBrightness(0); // Sinon, on met la luminosité de disp5 à 0
  if(Six == true) disp6.setBrightness(7); // Si Six est true, on met la luminosité de disp6 à 7
  else disp6.setBrightness(0); // Sinon, on met la luminosité de disp6 à 0
  if(Seven == true) disp7.setBrightness(7); // Si Seven est true, on met la luminosité de disp7 à 7
  else disp7.setBrightness(0); // Sinon, on met la luminosité de disp7 à 0
  if(Eight == true) disp8.setBrightness(7); // Si Eight est true, on met la luminosité de disp8 à 7
  else disp8.setBrightness(0); // Sinon, on met la luminosité de disp8 à 0
  
  disp1.showNumberDec(TIMER_OUT_ONE); // Affiche le timer de la sortie 1
  disp2.showNumberDec(TIMER_OUT_TWO); // Affiche le timer de la sortie 2
  disp3.showNumberDec(TIMER_OUT_THREE); // Affiche le timer de la sortie 3
  disp4.showNumberDec(TIMER_OUT_FOUR); // Affiche le timer de la sortie 4
  disp5.showNumberDec(TIMER_OUT_FIVE); // Affiche le timer de la sortie 5
  disp6.showNumberDec(TIMER_OUT_SIX); // Affiche le timer de la sortie 6
  disp7.showNumberDec(TIMER_OUT_SEVEN); // Affiche le timer de la sortie 7
  disp8.showNumberDec(TIMER_OUT_EIGHT); // Affiche le timer de la sortie 8
}

//////////////////////////////////////////////////////////////////displayMenu/////////////////////////////////////////////////////////////
void displayMenu() { // Fonction displayMenu
 lcd.clear(); // Efface l'écran LCD

  if (currentMenu == MENU_MAIN) { // Si on est dans le menu principal
    lcd.setCursor(0, 0); lcd.print("A: Launch"); // Affiche "A: Launch" en haut à gauche
    lcd.setCursor(0, 1); lcd.print("B: Options"); // Affiche "B: Options" en dessous
    lcd.setCursor(0, 2); lcd.print("C: Line Test"); // Affiche "C: Line Test" en dessous
  }
  else if (currentMenu == MENU_OPTIONS) { // Si on est dans le menu des options
    lcd.setCursor(0, 0); lcd.print("A: Set Output"); // Affiche "A: Set Output" en haut à gauche
    lcd.setCursor(0, 1); lcd.print("B: Set Timer"); // Affiche "B: Set Timer" en dessous
    lcd.setCursor(0, 3); lcd.print("D: Cancel"); // Affiche "D: Cancel" en bas à gauche
  }
  else if (currentMenu == MENU_LAUNCH) { // Si on est dans le menu de lancement
    lcd.setCursor(0, 0); lcd.print("A: Trigger by Table"); // Affiche "A: Trigger by Table" en haut à gauche
    lcd.setCursor(0, 1); lcd.print("B: Trigger by Radio"); // Affiche "B: Trigger by Radio" en dessous
    lcd.setCursor(0, 2); lcd.print("C: Trigger for Radio"); // Affiche "C: Manual Fire" en dessous
    lcd.setCursor(0, 3); lcd.print("D: Cancel"); // Affiche "D: Cancel" en bas à gauche
  }

}

//////////////////////////////////////////////////////////////////LaunchMenu/////////////////////////////////////////////////////////////
void LaunchMenu() { // Fonction LaunchMenu
  while (true) { // Boucle infinie pour rester dans le menu de lancement
    char key = keypad.getKey(); // Récupère la touche actuellement pressée sur le clavier
    if (key == 'A') { // Si on appuie sur A
      lcd.clear(); // Efface l'écran LCD
      lcd.setCursor(0, 0); // Positionne le curseur en haut à gauche
      lcd.print("Table Mode");// Affiche "Table Mode"
      lcd.setCursor(0, 1);// Positionne le curseur en dessous
      lcd.print("Ready for triggering");// Affiche "Ready for triggering"
      lcd.setCursor(0, 3);// Positionne le curseur en bas à gauche
      lcd.print("D: Cancel");// Affiche "D: Cancel"
      bool prevState = digitalRead(master_lauch);  // HIGH normalement (au repos avec pull-up)
      while (true) {// Boucle infinie pour rester dans le mode Table
        char innerKey = keypad.getKey();// Récupère la touche actuellement pressée sur le clavier
        bool currentState = digitalRead(master_lauch);// Lit l'état du bouton maître
        // FRONT DESCENDANT : HIGH -> LOW
        if (prevState == HIGH && currentState == LOW) {// Si l'état précédent était HIGH et l'état actuel est LOW
          triggersequence(); // Appelle la fonction triggersequence
          delay(1000); // Attente de 1 seconde pour éviter les rebonds
          currentMenu = MENU_LAUNCH; // Retourne au menu de lancement
          displayMenu(); // Affiche le menu de lancement
          break;// Sort de la boucle pour revenir au menu de lancement
        }
        prevState = currentState;// Met à jour l'état précédent avec l'état actuel

        if (innerKey == 'D') { // Si on appuie sur D
          currentMenu = MENU_LAUNCH;// Change le menu courant pour revenir au menu de lancement
          displayMenu();// Affiche le menu de lancement
          break;// Sort de la boucle pour revenir au menu de lancement
        }
        delay(20);  // anti-rebond simple
      }
    }
    else if (key == 'B') {// Si on appuie sur B
      lcd.clear(); // Efface l'écran LCD
      lcd.setCursor(0, 0); // Positionne le curseur en haut à gauche
      lcd.print("Radio Mode (IN)");// Affiche "Table Mode"
      lcd.setCursor(0, 1);// Positionne le curseur en dessous
      lcd.print("Ready for triggering");// Affiche "Ready for triggering"
      lcd.setCursor(0, 3);// Positionne le curseur en bas à gauche
      lcd.print("D: Cancel");// Affiche "D: Cancel"
      bool prevState = digitalRead(radio_in);  // HIGH normalement (au repos avec pull-up)
      while (true) {// Boucle infinie pour rester dans le mode Table
        char innerKey = keypad.getKey();// Récupère la touche actuellement pressée sur le clavier
        bool currentState = digitalRead(radio_in);// Lit l'état du bouton maître
        // FRONT DESCENDANT : HIGH -> LOW
        if (prevState == HIGH && currentState == LOW) {// Si l'état précédent était HIGH et l'état actuel est LOW
          triggersequence(); // Appelle la fonction triggersequence
          delay(1000); // Attente de 1 seconde pour éviter les rebonds
          currentMenu = MENU_LAUNCH; // Retourne au menu de lancement
          displayMenu(); // Affiche le menu de lancement
          break;// Sort de la boucle pour revenir au menu de lancement
        }
        prevState = currentState;// Met à jour l'état précédent avec l'état actuel

        if (innerKey == 'D') { // Si on appuie sur D
          currentMenu = MENU_LAUNCH;// Change le menu courant pour revenir au menu de lancement
          displayMenu();// Affiche le menu de lancement
          break;// Sort de la boucle pour revenir au menu de lancement
        }
        delay(20);  // anti-rebond simple
      }
    }
    else if (key == 'C') {// Si on appuie sur B
      lcd.clear(); // Efface l'écran LCD
      lcd.setCursor(0, 0); // Positionne le curseur en haut à gauche
      lcd.print("Radio Mode (EX)");// Affiche "Table Mode"
      lcd.setCursor(0, 1);// Positionne le curseur en dessous
      lcd.print("Ready for triggering");// Affiche "Ready for triggering"
      lcd.setCursor(0, 3);// Positionne le curseur en bas à gauche
      lcd.print("D: Cancel");// Affiche "D: Cancel"
      bool prevState = digitalRead(master_lauch);  // HIGH normalement (au repos avec pull-up)
      while (true) {// Boucle infinie pour rester dans le mode Table
        char innerKey = keypad.getKey();// Récupère la touche actuellement pressée sur le clavier
        bool currentState = digitalRead(master_lauch);// Lit l'état du bouton maître
        // FRONT DESCENDANT : HIGH -> LOW
        if (prevState == HIGH && currentState == LOW) {// Si l'état précédent était HIGH et l'état actuel est LOW
          digitalWrite(radio_out, HIGH); // Active la sortie radio
          delay(500); // Attente de 100 ms pour laisser le temps à la sortie de s'activer
          digitalWrite(radio_out, LOW); // Désactive la sortie radio
          delay(1000); // Attente de 1 seconde pour éviter les rebonds
          currentMenu = MENU_LAUNCH; // Retourne au menu de lancement
          displayMenu(); // Affiche le menu de lancement
          break;// Sort de la boucle pour revenir au menu de lancement
        }
        prevState = currentState;// Met à jour l'état précédent avec l'état actuel

        if (innerKey == 'D') { // Si on appuie sur D
          currentMenu = MENU_LAUNCH;// Change le menu courant pour revenir au menu de lancement
          displayMenu();// Affiche le menu de lancement
          break;// Sort de la boucle pour revenir au menu de lancement
        }
        delay(20);  // anti-rebond simple
      }
    }
    else if (key == 'D') {// Si on appuie sur D
      currentMenu = MENU_MAIN;// Change le menu courant pour revenir au menu principal
      displayMenu();// Affiche le menu principal
      return;// Retourne au menu principal
    }
  }
}

//////////////////////////////////////////////////////////////////triggersequence/////////////////////////////////////////////////////////////
void triggersequence() {// Fonction triggersequence
  lcd.clear();// Efface l'écran LCD
  lcd.setCursor(0, 0);// Positionne le curseur en haut à gauche
  lcd.print("Triggering...");// Affiche "Triggering..."

  const unsigned long pulseDuration = 500; // durée pendant laquelle la sortie reste HIGH
  unsigned long startTime = millis();// Enregistre le temps de début de la séquence
  
  unsigned long maxDelay = max(// Trouve le délai maximum parmi les timers définis
  max(TIMER_OUT_ONE, TIMER_OUT_TWO),
    max(
      max(TIMER_OUT_THREE, TIMER_OUT_FOUR),
      max(
        max(TIMER_OUT_FIVE, TIMER_OUT_SIX),
        max(TIMER_OUT_SEVEN, TIMER_OUT_EIGHT) 
      )
    )
  ); 
  unsigned long totalTime = maxDelay + pulseDuration;// Durée totale de la séquence, incluant le délai maximum et la durée du pulse
  while (millis() - startTime <= totalTime) {// Boucle jusqu'à ce que le temps écoulé dépasse la durée totale
    unsigned long elapsed = millis() - startTime;// Calcule le temps écoulé depuis le début de la séquence

    digitalWrite(out_one,   (One   && !ManOne   && elapsed >= TIMER_OUT_ONE   && elapsed < TIMER_OUT_ONE   + pulseDuration) ? HIGH : LOW);
    digitalWrite(out_two,   (Two   && !ManTwo   && elapsed >= TIMER_OUT_TWO   && elapsed < TIMER_OUT_TWO   + pulseDuration) ? HIGH : LOW);
    digitalWrite(out_three, (Three && !ManThree && elapsed >= TIMER_OUT_THREE && elapsed < TIMER_OUT_THREE + pulseDuration) ? HIGH : LOW);
    digitalWrite(out_four,  (Four  && !ManFour  && elapsed >= TIMER_OUT_FOUR  && elapsed < TIMER_OUT_FOUR  + pulseDuration) ? HIGH : LOW);
    digitalWrite(out_five,  (Five  && !ManFive  && elapsed >= TIMER_OUT_FIVE  && elapsed < TIMER_OUT_FIVE  + pulseDuration) ? HIGH : LOW);
    digitalWrite(out_six,   (Six   && !ManSix   && elapsed >= TIMER_OUT_SIX   && elapsed < TIMER_OUT_SIX   + pulseDuration) ? HIGH : LOW);
    digitalWrite(out_seven, (Seven && !ManSeven && elapsed >= TIMER_OUT_SEVEN && elapsed < TIMER_OUT_SEVEN + pulseDuration) ? HIGH : LOW);
    digitalWrite(out_eight, (Eight && !ManEight && elapsed >= TIMER_OUT_EIGHT && elapsed < TIMER_OUT_EIGHT + pulseDuration) ? HIGH : LOW);

    delay(10); // Limite la fréquence de boucle
  }
  // Sécurité : tout éteindre
  digitalWrite(out_one, LOW);
  digitalWrite(out_two, LOW);
  digitalWrite(out_three, LOW);
  digitalWrite(out_four, LOW);
  digitalWrite(out_five, LOW);
  digitalWrite(out_six, LOW);
  digitalWrite(out_seven, LOW);
  digitalWrite(out_eight, LOW);

  lcd.clear();// Efface l'écran LCD
  lcd.setCursor(0, 0);// Positionne le curseur en haut à gauche
  lcd.print("Done!");// Affiche "Done!"
  delay(1000);// Attente de 1 seconde pour que l'utilisateur puisse lire le message
  currentMenu = MENU_MAIN;// Change le menu courant pour revenir au menu principal
  displayMenu();// Affiche le menu principal
}

//////////////////////////////////////////////////////////////////testalllines/////////////////////////////////////////////////////////////
void testalllines() {// Fonction testalllines
  lcd.clear();// Efface l'écran LCD
  lcd.setCursor(0, 0);// Positionne le curseur en haut à gauche
  lcd.print("Testing Lines...");// Affiche "Testing Lines..."
          
  String linesOK = "";// Initialise une chaîne pour les lignes OK
  String linesFAIL = "";// Initialise une chaîne pour les lignes en échec

  for (int i = 0; i < 8; i++) {// Boucle pour tester les 8 lignes
    digitalWrite(test_out_oneout + i, HIGH);// Active la sortie correspondante
    delay(5);// Attente de 5 millisecondes pour stabiliser la sortie
    int inputVal = analogRead(test_out_onein + i);// Lit la valeur de l'entrée correspondante
    digitalWrite(test_out_oneout + i, LOW);// Désactive la sortie correspondante

    if (inputVal > 512) {// Si la valeur lue est supérieure à 512, la ligne est considérée comme OK
      linesOK += String(i + 1) + ",";// Ajoute le numéro de la ligne OK à la chaîne linesOK
    }
    else {// Si la valeur lue est inférieure ou égale à 512, la ligne est considérée comme en échec
      linesFAIL += String(i + 1) + ",";// Ajoute le numéro de la ligne en échec à la chaîne linesFAIL
    }
  }

  if (linesOK.length() > 0) linesOK.remove(linesOK.length() - 1);// Supprime la dernière virgule de la chaîne linesOK
  if (linesFAIL.length() > 0) linesFAIL.remove(linesFAIL.length() - 1);// Supprime la dernière virgule de la chaîne linesFAIL

  lcd.clear();// Efface l'écran LCD
  lcd.setCursor(0, 0);// Positionne le curseur en haut à gauche
  lcd.print("OK:" + linesOK);// Affiche les lignes OK

  lcd.setCursor(0, 1);// Positionne le curseur en dessous
  lcd.print("FAIL:" + linesFAIL);// Affiche les lignes en échec

  lcd.setCursor(0, 3);// Positionne le curseur en bas à gauche
  lcd.print("D: Back to Menu");// Affiche "D: Back to Menu"

  // Attendre que l'utilisateur appuie sur D
  while (true) {// Boucle infinie pour attendre l'appui sur D
    char waitKey = keypad.getKey();// Récupère la touche actuellement pressée sur le clavier
    if (waitKey == 'D') {// Si on appuie sur D
      displayMenu();// Appelle la fonction displayMenu pour revenir au menu principal
      break;// Sort de la boucle
    }
  }
}

//////////////////////////////////////////////////////////////////setOutput/////////////////////////////////////////////////////////////
void setOutput() {// Fonction setOutput
  afficheur();// Affiche l'état des sorties sur les afficheurs
  manuelfire();// Affiche l'état des boutons manuels
  voyant();// Affiche l'état des voyants
  lcd.clear();// Efface l'écran LCD
  lcd.setCursor(0, 0);// Positionne le curseur en haut à gauche
  lcd.print("set Output");// Affiche "set Output"

  int nbSorties = 0;// Nombre de sorties à activer

  while (true) {// Boucle infinie pour demander le nombre de sorties
    char key = keypad.getKey();// Récupère la touche actuellement pressée sur le clavier
    if (key >= '1' && key <= '8') {// Si la touche pressée est un chiffre entre 1 et 8
      nbSorties = key - '0';  // Convertit en chiffre
      break;// Sort de la boucle
    }
    else if (key == '0') {  //Gérer le cas où l'utilisateur appuie sur 0
      //Désactiver toutes les sorties directement
      One = Two = Three = Four = Five = Six = Seven = Eight = false;

      lcd.clear(); // Efface l'écran LCD
      lcd.setCursor(0, 0);// Positionne le curseur en haut à gauche
      lcd.print("Toutes sorties");// Affiche "Toutes sorties"
      lcd.setCursor(0, 1);// Positionne le curseur en dessous
      lcd.print("desactivees!");// Affiche "desactivées!"
      delay(1500);// Attente de 1,5 seconde pour que l'utilisateur puisse lire le message
      return;  //Quitter la fonction
    }
  }

  lcd.clear();// Efface l'écran LCD
  lcd.setCursor(0, 0);// Positionne le curseur en haut à gauche
  lcd.print("active output");// Affiche "active output"
  lcd.setCursor(0, 1);// Positionne le curseur en dessous
  for (int i = 1; i <= nbSorties; i++) {// Boucle pour afficher les sorties actives
    lcd.print(i);// Affiche le numéro de la sortie
    if (i != nbSorties) lcd.print(",");// Affiche une virgule si ce n'est pas la dernière sortie
  }

  lcd.setCursor(0, 2);// Positionne le curseur en dessous
  lcd.print("A: validate");// Affiche "A: validate"
  lcd.setCursor(0, 3);// Positionne le curseur en bas à gauche
  lcd.print("D: Cancel");// Affiche "D: Cancel"

  while (true) {// Boucle infinie pour attendre la confirmation ou l'annulation
    char confirmKey = keypad.getKey();// Récupère la touche actuellement pressée sur le clavier
    if (confirmKey == 'A') {// Si on appuie sur A
      // Réinitialiser toutes les sorties
      One = Two = Three = Four = Five = Six = Seven = Eight = false;

      // Activer selon le nombre
      if (nbSorties >= 1) One = true;
      if (nbSorties >= 2) Two = true;
      if (nbSorties >= 3) Three = true;
      if (nbSorties >= 4) Four = true;
      if (nbSorties >= 5) Five = true;
      if (nbSorties >= 6) Six = true;
      if (nbSorties >= 7) Seven = true;
      if (nbSorties >= 8) Eight = true;

      lcd.clear();// Efface l'écran LCD
      lcd.setCursor(0, 0);// Positionne le curseur en haut à gauche
      lcd.print("Output activated!");// Affiche "Output activated!"
      delay(500);// Attente de 0,5 seconde pour que l'utilisateur puisse lire le message
      afficheur();// Appelle la fonction afficheur pour mettre à jour les afficheurs
      delay(500);// Attente de 0,5 seconde pour que l'utilisateur puisse lire le message
      break;// Sort de la boucle pour revenir au menu principal
    }
    else if (confirmKey == 'D') { // Si on appuie sur D
      lcd.clear();// Efface l'écran LCD
      lcd.setCursor(0, 0);// Positionne le curseur en haut à gauche
      lcd.print("Cancel");// Affiche "Cancel"
      delay(1000);// Attente de 1 seconde pour que l'utilisateur puisse lire le message
      break;// Sort de la boucle pour revenir au menu principal
    }
  }
}

//////////////////////////////////////////////////////////////////setTimers/////////////////////////////////////////////////////////////
void setTimers() {// Fonction setTimers
  afficheur();// Affiche l'état des sorties sur les afficheurs
  lcd.clear();// Efface l'écran LCD
  lcd.setCursor(0, 0);// Positionne le curseur en haut à gauche
  lcd.print("active output:");// Affiche "active output:"

  String actives = "";// Initialise une chaîne pour les sorties actives
  if (One) actives += "1 ";
  if (Two) actives += "2 ";
  if (Three) actives += "3 ";
  if (Four) actives += "4 ";
  if (Five) actives += "5 ";
  if (Six) actives += "6 ";
  if (Seven) actives += "7 ";
  if (Eight) actives += "8 ";

  lcd.setCursor(0, 1);// Positionne le curseur en dessous
  lcd.print(actives); // Affiche les sorties actives

  lcd.setCursor(0, 2);// Positionne le curseur en dessous
  lcd.print("setTimers");// Affiche "setTimers"
  lcd.setCursor(0, 3);// Positionne le curseur en bas à gauche
  lcd.print("D: Cancel");// Affiche "D: Cancel"

  while (true) {// Boucle infinie pour attendre la sélection de la sortie
    char key = keypad.getKey();// Récupère la touche actuellement pressée sur le clavier
    
    if (key == 'D') {// Si on appuie sur D
      displayMenu();// Appelle la fonction displayMenu pour revenir au menu principal
      return;// Retourne au menu principal
    }

    if (key >= '1' && key <= '8') {// Si la touche pressée est un chiffre entre 1 et 8
      int sortie = key - '0';// Convertit le caractère en entier

      bool isActive = false;// Initialise une variable pour vérifier si la sortie est active
      switch (sortie) {// En fonction du numéro de la sortie
        case 1: isActive = One; break;
        case 2: isActive = Two; break;
        case 3: isActive = Three; break;
        case 4: isActive = Four; break;
        case 5: isActive = Five; break;
        case 6: isActive = Six; break;
        case 7: isActive = Seven; break;
        case 8: isActive = Eight; break;
      }

      if (!isActive) {  // Si la sortie n'est pas active
        lcd.clear();// Efface l'écran LCD
        lcd.setCursor(0, 0);// Positionne le curseur en haut à gauche
        lcd.print("active output");// Affiche "active output"
        delay(500);// Attente de 0,5 seconde pour que l'utilisateur puisse lire le message
        setTimers(); // Revenir à la sélection des timers
        return;// Retourne à la sélection des timers
      }

      lcd.clear();  // Efface l'écran LCD
      lcd.setCursor(0, 0);// Positionne le curseur en haut à gauche
      lcd.print("output ");// Affiche "output "
      lcd.print(sortie);// Affiche le numéro de la sortie sélectionnée
      lcd.print(": Timer?");// Affiche ": Timer?"
      lcd.setCursor(0, 1);// Positionne le curseur en dessous
      lcd.print("(ms, end by #)");// Affiche "(ms, end by #)"

      String timerStr = "";// Initialise une chaîne pour le timer
      while (true) {// Boucle infinie pour saisir le timer
        char tKey = keypad.getKey();// Récupère la touche actuellement pressée sur le clavier
        if (tKey >= '0' && tKey <= '9') {// Si la touche pressée est un chiffre entre 0 et 9
          if (timerStr.length() < 5) { // max 99999ms
            timerStr += tKey;// Ajoute le chiffre à la chaîne du timer
            lcd.setCursor(0, 2);// Positionne le curseur en dessous
            lcd.print("Valeur: " + timerStr + "   ");// Affiche la valeur du timer
          }
        }
        else if (tKey == '#') {// Si on appuie sur #
          int timerVal = timerStr.toInt();// Convertit la chaîne du timer en entier

          switch (sortie) {// En fonction du numéro de la sortie
            case 1: TIMER_OUT_ONE = timerVal; break;// Définit le timer pour la sortie 1
            case 2: TIMER_OUT_TWO = timerVal; break;// Définit le timer pour la sortie 2
            case 3: TIMER_OUT_THREE = timerVal; break;// Définit le timer pour la sortie 3
            case 4: TIMER_OUT_FOUR = timerVal; break;// Définit le timer pour la sortie 4
            case 5: TIMER_OUT_FIVE = timerVal; break;// Définit le timer pour la sortie 5
            case 6: TIMER_OUT_SIX = timerVal; break;// Définit le timer pour la sortie 6
            case 7: TIMER_OUT_SEVEN = timerVal; break;// Définit le timer pour la sortie 7
            case 8: TIMER_OUT_EIGHT = timerVal; break;// Définit le timer pour la sortie 8
            afficheur();// Met à jour l'affichage des timers
          }

          lcd.clear();// Efface l'écran LCD
          lcd.setCursor(0, 0);// Positionne le curseur en haut à gauche
          lcd.print("timer defined!");// Affiche "timer defined!"
          lcd.setCursor(0, 1);// Positionne le curseur en dessous
          lcd.print("output ");// Affiche "output "
          lcd.print(sortie);// Affiche le numéro de la sortie sélectionnée
          lcd.print(": ");// Affiche ": "
          lcd.print(timerVal);// Affiche la valeur du timer
          lcd.print("ms");// Affiche "ms"
          delay(1000);// Attente de 1 seconde pour que l'utilisateur puisse lire le message
          setTimers(); // Revenir à la sélection
          return;// Retourne à la sélection des timers
        }
      }
    }
  }
}

