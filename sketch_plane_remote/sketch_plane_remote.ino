

#include <RF24.h> //http://tmrh20.github.io/RF24/

uint8_t send(const uint8_t length, const uint8_t data[]);


// Configurer vos radio nRF24L01+ sur le bus SPI et mettre  CE sur D7 et CSN sur D8
RF24 radio(7, 8);

// Le nom des "pipes" de communication, un en lecture, un en écriture
const uint8_t addresses[][6] = {"1Node","2Node"};
#define MY_ADDRESS_INDEX 0
#define TARGET_ADDRESS_INDEX 1

#define PAYLOAD_SIZE 32
uint8_t payload[PAYLOAD_SIZE];

void setup() {
  //Utilise le serial pour communiquer avec le PC lors du dev
  Serial.begin(115200);
  
  // Configure la radio
  radio.setPayloadSize(PAYLOAD_SIZE);
  radio.begin();
  radio.setPALevel(RF24_PA_LOW);  //LOW pour le dev. Voir RF24_PA_MAX pour test en plein air
  
  radio.openWritingPipe((const uint8_t*) addresses + MY_ADDRESS_INDEX); // Adresse pour la telecommande
  radio.openReadingPipe(1, (const uint8_t*) addresses + TARGET_ADDRESS_INDEX); // Addresse de la partie controle
  
  radio.startListening(); //Se met en écoute
}

void loop() {
  
  for(uint8_t i=0;i<180;i++) {
    payload[0] = i;
    sendPayload();
    delay(100);
  }
  for(uint8_t i=180;i>0;i--) {
    payload[0] = i;
    sendPayload();
    delay(100);
  }
  
}

//Envoi des données par radio.
// Taille des données : 32 bytes
// Return <= 0 si erreur
uint8_t sendPayload()
{
  radio.stopListening();   // On arrête d'écouter pour qu'on puisse émettre

  if (!radio.write( payload, sizeof(uint8_t)*PAYLOAD_SIZE )) {
    Serial.println(F("erreur d'envoi de la taille"));  //Remonte l'erreur
    return 0;
  }
  Serial.println(payload[0]);
  Serial.println(F("---"));

  radio.startListening(); // On se remet en mode écoute
  return PAYLOAD_SIZE;
}
