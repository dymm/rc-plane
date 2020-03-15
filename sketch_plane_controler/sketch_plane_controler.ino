

#include <Servo.h>
#include <RF24.h> //http://tmrh20.github.io/RF24/

uint8_t send(const uint8_t length, const uint8_t data[]);
uint8_t receive(const uint8_t maxLength, uint8_t out[]);

Servo servo;

// Configurer vos radio nRF24L01+ sur le bus SPI et mettre  CE sur D7 et CSN sur D8
RF24 radio(7, 8);

// Le nom des "pipes" de communication, un en lecture, un en écriture
const uint8_t addresses[][6] = {"1Node","2Node"};
#define MY_ADDRESS_INDEX 1
#define TARGET_ADDRESS_INDEX 0

#define PAYLOAD_SIZE 32
uint8_t payload[PAYLOAD_SIZE];

void setup() {
  servo.attach(9);
  
  // Configure la radio
  radio.setPayloadSize(PAYLOAD_SIZE);
  radio.begin();
  radio.setPALevel(RF24_PA_LOW);  //LOW pour le dev. Voir RF24_PA_MAX pour test en plein air
  
  radio.openWritingPipe((const uint8_t*) addresses + MY_ADDRESS_INDEX); // Adresse pour la telecommande
  radio.openReadingPipe(1, (const uint8_t*) addresses + TARGET_ADDRESS_INDEX); // Addresse de la partie controle
  
  radio.startListening(); //Se met en écoute

    
  //Utilise le serial pour communiquer avec le PC lors du dev
  Serial.begin(115200);
}

void loop() {

  uint8_t length = receive();
  if(length>0) {
    servo.write((int)payload[0]);
  }
}

uint8_t receive() {
  uint8_t length = 0;
  if(radio.available()) {
    radio.read( payload,  PAYLOAD_SIZE);
    length = PAYLOAD_SIZE;
  }

  if(length>0) {
    Serial.print(F("Received "));
    Serial.println(length);
    Serial.println(payload[0]);
  }
  return length;
}
