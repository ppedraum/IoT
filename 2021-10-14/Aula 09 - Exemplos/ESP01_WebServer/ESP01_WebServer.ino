// Programa: Web Server com modulo ESP8266
// Alteracoes e adaptacoes: FILIPEFLOP
// Alterações Prof. Vagner Rodrigues
#include <SoftwareSerial.h>
 
/*
 * Adaptador      Arduino
 *    RX      ->   3(TX)
 *    TX      ->   2(RX)
 */
SoftwareSerial esp8266(4,5); 
  
#define DEBUG true
 
void setup()
{
  Serial.begin(9600);
  esp8266.begin(9600);
  
  //Desconecta de algum WiFi caso esteja conectado.
  sendData("AT+CWQAP\r\n", 2000, DEBUG);
  delay(1000);
  
  //Reseta
  sendData("AT+RST\r\n", 2000, DEBUG); // rst
  delay(2000);
  
  //Configura modo como station
  sendData("AT+CWMODE=1\r\n", 1000, DEBUG);
  delay(1000);
  
  // Conecta a rede wireless
  sendData("AT+CWJAP_CUR=\"SATC IOT\",\"IOT2021#\"\r\n", 2000, DEBUG);
  // SSID e senha devem estar entre aspas e por isso é necessário colocar a contra barra.
  delay(5000);
  
  // Mostra o endereco IP
  sendData("AT+CIFSR\r\n", 1000, DEBUG);
  delay(1000);
  
  /*Os próximos comandos são para configurar o ESP-01 como servidor*/
  // Configura para multiplas conexoes
  sendData("AT+CIPMUX=1\r\n", 1000, DEBUG);
  delay(1000);
  
  // Inicia o web server na porta 80
  sendData("AT+CIPSERVER=1,80\r\n", 1000, DEBUG);
}
 
void loop()
{
  // Verifica se o ESP8266 esta enviando dados
  if (esp8266.available())
  {
    if (esp8266.find("+IPD,"))
    {
      delay(300);
      int connectionId = esp8266.read() - 48;
 
      String webpage = "<head><meta http-equiv=""refresh"" content=""3"">";
      webpage += "</head><h1><u>Exemplo: Web Server com ESP01.</u><h1>";
      webpage += "<h2>Leitura digital do pino 4: ";
      int a = digitalRead(4);
      webpage += a;
      webpage += "</h2>";
 
      String cipSend = "AT+CIPSEND=";
      cipSend += connectionId;
      cipSend += ",";
      cipSend += webpage.length();
      cipSend += "\r\n";
 
      sendData(cipSend, 1000, DEBUG);
      sendData(webpage, 1000, DEBUG);
 
      String closeCommand = "AT+CIPCLOSE=";
      closeCommand += connectionId; // append connection id
      closeCommand += "\r\n";
 
      sendData(closeCommand, 3000, DEBUG);
    }
  }
}
 
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
    Serial.print(response);
  }
  return response;
}
