//Programa: Eco para comandos AT com ESP8266
//Prof. Vagner Rodrigues

// Obs: Para o correto funcionamento é necessario que o baud rate do ESP
// tenha sido modificado para 9600. Em 115200 pode haver problemas de comunicação.
// Para mudar o baud rate verifique o exemplo "ESP01_ATComands" 

#include <SoftwareSerial.h>

#define DEBUG true

/*
 * Adaptador      Arduino
 *    RX      ->   3(TX)
 *    TX      ->   2(RX)
 */
SoftwareSerial esp8266(2, 3);
 
void setup() {

  // Inicializa ambas as portas seriais
  Serial.begin(9600);
  esp8266.begin(9600);
}
 
void loop() {
  // Verifica se tem mensagem do ESP e manda para a Serial
  if (esp8266.available()) {
    int inByte = esp8266.read();
    Serial.write(inByte);    
  }
  
 
  // Analogamente, verifica se tem sinal na Serial, e manda para o ESP
  if (Serial.available()) {
    int inByte = Serial.read();
    esp8266.write(inByte);
  }
}
