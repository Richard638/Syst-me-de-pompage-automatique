#include <SoftwareSerial.h>

// Utilisation des ports série matériels sur Arduino Mega
#define SIM900_TX 18
#define SIM900_RX 19

// Seuils d'humidité (à ajuster selon vos besoins)
#define SOL_SEC 340
#define SOL_HUMIDE 290

#define POMPE 9

// Variables pour suivre l'état de l'appel et de la pompe
bool appelPasse = false;
bool pompeActive = false;

void setup() {
  // Initialisation du port série pour la communication avec le PC
  Serial.begin(9600);

  // Initialisation du port série pour la communication avec le module SIM900
  Serial1.begin(9600);

  // Initialisation de la broche du capteur d'humidité de sol
  pinMode(A0, INPUT);

  // Initialisation de la broche du relais
  pinMode(POMPE, OUTPUT);
  digitalWrite(POMPE, LOW); // Assurez-vous que le relais est désactivé au démarrage

  // Donne un peu de temps au module SIM900 pour démarrer
  delay(10000);

  // Test de la communication avec le module SIM900
  Serial1.println("AT");
  delay(1000);
  if (Serial1.available()) {
    Serial.println("Réponse du SIM900 : ");
    while (Serial1.available()) {
      Serial.write(Serial1.read());
    }
  } else {
    Serial.println("Pas de réponse du module SIM900");
  }
}

void loop() {
  // Lecture de la valeur du capteur d'humidité de sol
  int niveauHumidite = analogRead(A0);
  Serial.print("Niveau d'humidité du sol : ");
  Serial.println(niveauHumidite);

  // Si le sol est sec, passer un appel
  if (niveauHumidite > SOL_SEC && !appelPasse) {
    Serial.println("Le sol est sec. Passage d'un appel...");
    passerAppel("+221772966637"); // Remplacez par le numéro de téléphone à appeler
    appelPasse = true;
    delay(120000); // Attendre 2 minutes après l'appel
  }

  // Activation automatique de la pompe si le bouton "On" n'est pas appuyé
  if (appelPasse && !pompeActive) {
    Serial.println("Activation automatique de la pompe...");
    while (niveauHumidite > SOL_HUMIDE) {
      digitalWrite(POMPE, HIGH); // Activer la pompe
      delay(2000); // Attendre 2 secondes
      digitalWrite(POMPE, LOW); // Désactiver la pompe
      delay(2000); // Attendre 2 secondes avant de vérifier à nouveau
      niveauHumidite = analogRead(A0); // Vérifier à nouveau l'humidité du sol
    }
    pompeActive = true;
  }

  // Vérification si le sol est humide pour envoyer un message
  if (pompeActive) {
    niveauHumidite = analogRead(A0);
    if (niveauHumidite < SOL_HUMIDE && appelPasse) {
      Serial.println("Le sol est humide. Envoi d'un SMS...");
      envoyerSMS("+221772966637", "Le sol est humide ! Vous pouvez arrêter l'arrosage."); // Remplacez par le numéro et le message souhaités
      delay(120000); // Attendre 2 minutes après le SMS
      appelPasse = false;
      pompeActive = false;
    }
  }

  // Attendre un court instant avant la prochaine lecture
  delay(1000);
}

void passerAppel(const char* numero) {
  // Commande AT pour composer un numéro
  Serial1.print("ATD");
  Serial1.print(numero);
  Serial1.println(";");

  // Attendre une réponse du module
  delay(1000);

  if (Serial1.available()) {
    Serial.println("Réponse du module SIM900 : ");
    while (Serial1.available()) {
      Serial.write(Serial1.read());
    }
  } else {
    Serial.println("Aucune réponse après la commande ATD");
  }

  // Attendre un peu avant de raccrocher (10 secondes ici)
  delay(10000);

  // Commande AT pour raccrocher
  Serial1.println("ATH");

  // Attendre une réponse du module
  delay(1000);

  if (Serial1.available()) {
    Serial.println("Réponse du module SIM900 après ATH : ");
    while (Serial1.available()) {
      Serial.write(Serial1.read());
    }
  } else {
    Serial.println("Aucune réponse après la commande ATH");
  }
}

void envoyerSMS(const char* numero, const char* message) {
  // Commande AT pour mettre le module en mode texte
  Serial1.println("AT+CMGF=1");
  delay(100
