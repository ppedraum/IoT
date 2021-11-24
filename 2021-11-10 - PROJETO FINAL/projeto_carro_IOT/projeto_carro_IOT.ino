//---------------------DHT---------------------------//
#include <DHT.h>
#define DHTPIN A5 //pino DHT
#define DHTTYPE DHT11 //tipo DHT
DHT dht(DHTPIN, DHTTYPE); //Instância dht (pino, tipo)
float dht_t=0;
//---------------------------------------------------//
//-----------------HCSR04----------------------------//
#include <Ultrasonic.h>
#define trigger 6
#define echo 7
Ultrasonic uts(trigger, echo);

#define led_a 9
#define led_b 10

unsigned long pre_millis=0;

//
#define bt 4
int bt_stt=0, bt_stt0=0, bt_vle=0;
String bt_desc[]={"Marcha Normal", "Marcha Re", "Marcha Morto"};
//bt_vle ---> 0=Normal - 1=Ré - 2=Morto
String re_stt="";
//
#define buzz 8

//

void bzz_blink(int interval){
  unsigned long cur_millis = millis();
  if(cur_millis-pre_millis>=interval/2 && cur_millis-pre_millis<interval){
    digitalWrite(buzz, HIGH);
    Serial.println("buzz");
  }
  else if(cur_millis-pre_millis>=interval){
    pre_millis=cur_millis;
    digitalWrite(buzz, LOW);
    Serial.println("no buzz");
  }
}

long uts_measure(){
  long uts_ms=uts.timing();
  float uts_cm=uts.convert(uts_ms, Ultrasonic::CM);
  return uts_cm;
}
void uts_routine(){
  long uts_ms=uts.timing();
  float uts_cm=uts.convert(uts_ms, Ultrasonic::CM);
  
  if(uts_cm<20 && uts_cm>=10){
    bzz_blink(2000);
  }else if(uts_cm<10){
    bzz_blink(1000);
    //Buzzer mais alto
  }else if(uts_cm<5){
    bzz_blink(500);
  }else{
    digitalWrite(buzz, LOW);
  }
  
}
void bt_read(){
  bt_stt=digitalRead(bt);

  if(bt_stt && !bt_stt0){
    bt_vle++;
    if(bt_vle>2)
      bt_vle=0;
  }
  
  bt_stt0=digitalRead(bt);

  switch(bt_vle){
    case 0:
      digitalWrite(led_a, LOW);
      digitalWrite(led_b, LOW);
    break;
    case 1:
      digitalWrite(led_a, HIGH);
      digitalWrite(led_b, HIGH);
      uts_routine();
    break;
    case 2:
      digitalWrite(led_a, LOW);
      digitalWrite(led_b, LOW);
    break;

    if(bt_vle==1){
      re_stt="Ativo";
    }else{
      re_stt="Inativo";
    }
  }
}
//--------------------------------------------------//

//---------------------------------------------------//


//---------ESP8266------------//
#include <SoftwareSerial.h>
SoftwareSerial esp8266(2,3); 
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
//--------------------------------------------------------------------------------//
//-------------------------COMANDOS DO CARRO--------------------------------------//

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
  webserver();
  bt_read();
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
      webpage+= "<title>Carro</title></head>";
      webpage+= "<body><h1>Carrinho Topzera</h1>";
      webpage+= "Estado atual do carro<br><br>";
      webpage+= "<h2>INFORMACOES GERAIS</h2>";
      webpage+= "Velocidade do carro: <br>";
      webpage+= "Marcha:";
      webpage+= bt_desc[bt_vle];
      webpage+= "<br>";
      webpage+= "Temperatura do Oleo:";
      webpage+= dht.readTemperature();
      webpage+= "<br>";
      webpage+= "Luminosidade da area: <br>";
      webpage+= "<h2>Sensor de re</h2>";
      //webpage+= "Estado: "+ re_stt +" <br>";
      //webpage+= "Distancia a um objeto proximo: "+ String(uts_measure()/100) +"m</body>";
      webpage += "</body>";
      
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
