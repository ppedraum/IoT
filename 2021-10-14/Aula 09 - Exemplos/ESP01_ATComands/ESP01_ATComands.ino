//Programa: Testes de Comandos AT com ESP8266
//Prof. Vagner Rodrigues
#include <SoftwareSerial.h>
 
/*
 * Adaptador      Arduino
 *    RX      ->   3(TX)
 *    TX      ->   2(RX)
 */
SoftwareSerial esp8266(2,3); 
  
#define DEBUG true
 
void setup()
{
  Serial.begin(9600);
  //O baud rate padrão do ESP8266 é 115200. Vamos considerar que já foi modificado para 9600.
  
  esp8266.begin(57600);
  //Caso necessário, descomente a linha abaixo para modificar o baud rate do ESP para 9600.
  //sendData("AT+UART_DEF=9600,8,1,0,0\r\n", 2000, DEBUG);
  //Grave o programa e observe no monitor serial.
  
  Serial.println("Envia AT e espera \"OK\"");
  delay(500);
  sendData("AT\r\n", 2000, DEBUG);
  delay(500);
  Serial.println("Verifica a versão do firwmare");
  delay(500);
  sendData("AT+GMR\r\n", 2000, DEBUG); // rst
  Serial.println("Fim do teste");
}
 
void loop() {}
 
String sendData(String command, const int timeout, boolean debug)
{
  // Envio dos comandos AT para o modulo
  String response = "";
  esp8266.print(command);
  long int time = millis();
  while ( (time + timeout) > millis())
  {
    while (esp8266.available())
    {
      // The esp has data so display its output to the serial window
      char c = esp8266.read(); // read the next character.
      response += c;
    }
  }
  if (debug)
  {
    Serial.println(response);
  }
  return response;
}
