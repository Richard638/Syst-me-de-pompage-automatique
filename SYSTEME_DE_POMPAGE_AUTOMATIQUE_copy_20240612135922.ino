                                      // SYSTEME DE POMPAGE AUTOMATIQUE                                                    

const int analogInPin = A0; // Broche du capteur analogique
const int buzzer = 5;       // Broche du buzzer
const int relais_Pompe = 4; // Broche du relais de la pompe

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Initialisation de l'écran LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  pinMode(buzzer, OUTPUT);       // Configurer le buzzer comme sortie
  pinMode(relais_Pompe, OUTPUT); // Configurer le relais de la pompe comme sortie
  Serial.begin(9600);            // Initialiser la communication série
  lcd.init();                    // Initialiser l'écran LCD
  lcd.backlight();               // Activer le rétroéclairage de l'écran LCD
}

void loop() {
  int sensorValue = analogRead(analogInPin); // Lire la valeur du capteur
  Serial.print("sensor = ");
  Serial.println(sensorValue); // Afficher la valeur du capteur sur le moniteur série

  if (sensorValue >= 0 && sensorValue <= 600) {
    // Niveau d'eau bas
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("NIVEAU: bas ##");
    lcd.setCursor(0, 1);
    lcd.print("POMPE: Run!!!");
    digitalWrite(relais_Pompe, HIGH); // Activer la pompe
    tone(buzzer, 300, 500);           // Activer le buzzer
  }  else if (sensorValue >= 610 && sensorValue <= 650) {
    // Niveau d'eau haut
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("NIVEAU: haut ##");
    lcd.setCursor(0, 1);
    lcd.print("POMPE: Stop!!!");
    digitalWrite(relais_Pompe, LOW); // Désactiver la pompe
    noTone(buzzer);                  // Désactiver le buzzer
  } else {
    // Valeur du capteur en dehors des plages définies
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("POMPAGE D'EAU");
    lcd.setCursor(2, 1);
    lcd.print("AUTOMATIQUE");
    digitalWrite(relais_Pompe, LOW); // Désactiver la pompe par précaution
    noTone(buzzer);                  // Désactiver le buzzer
  }

  delay(1000); // Attendre 1 seconde avant la prochaine lecture
}


//Le système décrit utilise un capteur pour surveiller le niveau de l'eau.
//Ce capteur est connecté à une carte Arduino. 
//L'Arduino vérifie constamment le niveau d'eau et affiche ces informations sur un petit écran. 
//En fonction du niveau d'eau détecté, le système décide si la pompe doit fonctionner ou non. 
//Si le niveau est bas ou moyen, la pompe se met en marche et un petit buzzer émetun son pour alerter l'utilisateur. 
//Si le niveau est haut, la pompe s'arrête et le buzzer se tait.
