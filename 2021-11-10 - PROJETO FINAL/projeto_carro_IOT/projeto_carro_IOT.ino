//---------------------DHT---------------------------//
#include <DHT.h>
#define DHTPIN A5 //pino DHT
#define DHTTYPE DHT11 //tipo DHT
DHT dht(DHTPIN, DHTTYPE); //Instância dht (pino, tipo)
float dht_t=0;
//---------------------------------------------------//

//---------ESP8266------------//
#include <SoftwareSerial.h>
SoftwareSerial esp8266(4,5); 
#define bdrate 9600
#define DEBUG true
//----------------------------//


//---------------------ESP8266 SETUP (CONECTAR À INTERNET)----------------------//
void esp_setup(){
  esp8266.begin(bdrate);
  Serial.println("Desconectando de redes anteriores...");
  sendData("AT+CWQAP\r\n", 2000, DEBUG); //Desconecta de uma rede
  delay(1000);

  Serial.println("Resetando o ESP8266...");
  sendData("AT+RST\r\n", 2000, DEBUG); // reseta o esp
  delay(2000);

  Serial.println("Configurando o ESP8266 para modo Station...");
  sendData("AT+CWMODE=1\r\n", 1000, DEBUG); //modo station 
  delay(1000);

  Serial.println("Conectando o ESP8266 à rede wifi...");
  sendData("AT+CWJAP_CUR=\"SATC IOT\",\"IOT2021#\"\r\n", 2000, DEBUG);
  delay(5000);
  Serial.println("Pronto!");

  sendData("AT+CIFSR\r\n", 1000, DEBUG); //Mostra o ip
  delay(1000);
  
  sendData("AT+CIPMUX=1\r\n", 1000, DEBUG); //Múltiplas conexões
  delay(1000);
  
  sendData("AT+CIPSERVER=1,80\r\n", 1000, DEBUG); //conecta web server na porta 80

}
//-------------------------------------------------------------------------------//

//---------------------------LEITURA DO DHT--------------------------------------//
void dht_read(){
  dht_t = dht.readTemperature();
  if (isnan(dht_t)) { //Se não conseguiu ler algum deles
    Serial.println("Falha ao ler o sensor DHT!!!");
    delay(2000);
    return;
  }
  //Serial.println("Temperatura = " + String(dht_t) + "ºC");
  delay(2000);
}
//--------------------------------------------------------------------------------//
//-------------------------COMANDOS DO CARRO--------------------------------------//
#define ign 2
boolean ign_stt, ign_stt0;
boolean isIgn;

boolean ignicao(){
  ign_stt=digitalRead(ign);
  if(ign_stt && !ign_stt0){
    if(!isIgn){
      isIgn=true;
    }else{
      isIgn=false;
    }
  }
  ign_stt0=digitalRead(ign);
  return isIgn;
}

//--------------------------------------------------------------------------------//


//-----SETUP--------//
void setup() {
  Serial.begin(9600);
  dht.begin();
  esp_setup();
}
//-----------------//

//LOOP (FUNÇÕES SENDO EXECUTADAS)//
void loop() {
  if(ignicao()){
    dht_read();
    webserver();
    print("Ignição ligada");
  }else{
    print("Ignição desligada");
  }
}
//------------------------------//

//-----------------------------WEBSERVER--------------------------------------//
void webserver(){
  // Verifica se o ESP8266 esta enviando dados
  if (esp8266.available())
  {
    if (esp8266.find("+IPD,"))
    {
      delay(300);
      int connectionId = esp8266.read() - 48;
 
      String webpage = "<head><meta http-equiv=""refresh"" content=""3"">";
      webpage += "</head><h1><u>Exemplo: Web Server com ESP01.</u><h1>";
      webpage += "<h2>Leitura da Temperatura: ";
      webpage += dht_t;
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
 
      sendData(closeCommand, 2000, DEBUG);
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
//----------------------------------------------------------------------------//
