#include <stdarg.h>

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
long uts_ms = 0;
float uts_cm = 0;

#define led_a 9
#define led_b 10

#define led_c 11
#define led_d 12

unsigned long pre_millis=0;


void uts_routine(){ 
  //Faz a rotina do HCSR04 (medir a distância) 
  
  uts_ms=uts.timing();
  uts_cm=uts.convert(uts_ms, Ultrasonic::CM);
  uts_m = uts_cm/100;
  if(uts_cm<20 && uts_cm>=10){
    blinkw(2000, buzz);
  }else if(uts_cm<10 && uts_cm>=5){
    blinkw(1000, buzz);
    //Buzzer mais alto
  }else if(uts_cm<5){
    blinkw(500, buzz);
  }else{
    digitalWrite(buzz, LOW);
  }
}
//---------------------------------------------------//


//-----------------CARRO------------------------------//
#define bt_marcha 4
int bt_stt=0, bt_stt0=0;
int vles[]={0,0,0}; //Vou nem falar nada disso
//2=ignição - 1=Marcha - 2=Faróis

//Então, aqui eu meio que caguei tudo e fiz um monte de arrays para diferentes estados de cada input
//Não necessáriamente está certo fazer desse jeito mas funciona. Isso que é importante.

String marcha_desc[]={"Marcha Normal", "Marcha Re", "Marcha Morto"};
String carro_stt[]={"Ligado", "Desligado"};
String farol_desc[]={"Ligados", "Desligados"};
//vles[0] ---> 0=Normal - 1=Ré - 2=pisca alerta
//vles[1] ---> 0=Desl - 1=Lig
String re_stt="";
#define buzz 8


void ver_marcha(){ 
  //Verifica em qual marcha está
  
  bt_action_press(bt_marcha, 2, 0, vles[1]);

  switch(vles[1]){
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
      while(vles[0]==2){
        blinkw(500, led_c);
        blinkw(500, led_d);
      }
    break;
    if(vles[1]==1){
      re_stt="Ativo";
    }else{
      re_stt="Inativo";
    }
  }
}

void ver_lights(){ 
  //Verifica as luzes do farol
  
  bt_action_press(bt_marcha, 1, 0, vles[2]);
  switch (vles[2]){
    case 0:
      digitalWrite(led_c, LOW);
      digitalWrite(led_d, LOW);
    break;
    case 1:
      digitalWrite(led_c, HIGH);
      digitalWrite(led_d, HIGH);
    break;
  }
}

void com_routines(){ 
  //Agrega todas as rotinas do carro que possuem funções definidas ou apenas uma variável
  
  ver_lights();
  ver_marcha();

  temp_oleo = dht.readTemperature();
}

//----------------------------------------------------//


//---------------FUNÇÕES PRONTAS PARA USO-------------//
void blinkw(int interval, int port){ 
  //Faz um blink com algum dispositivo de saída digital
  
  unsigned long cur_millis = millis();
  if(cur_millis-pre_millis>=interval/2 && cur_millis-pre_millis<interval){
    digitalWrite(port, HIGH);
    Serial.println("blink");
  }
  else{
    pre_millis=cur_millis;
    digitalWrite(port, LOW);
    Serial.println("no blink");
  }
}

void bt_action_press(int bt, int interval, int interval0, int counter){ 
  //Verifica se um botão foi pressionado e quantos estados se somam ao contador
  //Inclui último valor
  
  bt_stt=digitalRead(bt);

  if(bt_stt && !bt_stt0){
    counter++;
    if(counter>interval)
      bt=interval0;
  }
  
  bt_stt0=digitalRead(bt);
}
//----------------------------------------------------//



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



//-----SETUP--------//
void setup() {
  Serial.begin(9600);
  dht.begin();
  esp_setup();

}
//-----------------//

//LOOP (FUNÇÕES SENDO EXECUTADAS)//
void loop() {
  webserver(); //mantém o webserver rodando
  com_routines(); //verifica as diferentes rotinas do carro e seus sensores
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

      //Aqui é onde se cria a parte HTML do webserver, basicamente vai somando o código por linhas à
      //Var webpage e depois upa tudo com o comando sendData()
      
      String webpage = "<head><meta http-equiv=""refresh"" content=""3"">";
      webpage+= "<title>Carro</title></head>";
      webpage+= "<body><h1>Carrinho Topzera</h1>";
      webpage+= "Estado atual do carro<br><br>";
      webpage+= "<h2>INFORMACOES GERAIS</h2>";
      webpage+= "<h3>ESTADO DO CARRO: </h3>";
      webpage+= carro_stt[vles[0]];
      webpage+= "<br>";
      webpage+= "Velocidade do carro: <br>";
      webpage+= "Faróis:";
      webpage+= farol_desc[vles[2]];
      webpage+= "<br>";
      webpage+= "Marcha:";
      webpage+= marcha_desc[vles[1]];
      webpage+= "<br>";
      webpage+= "Temperatura do Oleo:";
      webpage+= temp_oleo;
      webpage+= "<br>";
      webpage+= "<h2>Sensor de re</h2>";
      webpage+= "Estado:";
      webpage+= re_stt;
      webpage+="<br>";
      webpage+= "Distancia a um objeto proximo: ";
      webpage+= uts_m;
      webpage+="m<br>";
      webpage += "</body>";

      //Prepara o ESP8266 pra receber tal número de caracteres ou números, acho eu
      String cipSend = "AT+CIPSEND=";
      cipSend += connectionId;
      cipSend += ",";
      cipSend += webpage.length();
      cipSend += "\r\n";
 
      sendData(cipSend, 1000, DEBUG);
      sendData(webpage, 1000, DEBUG);
 
      String closeCommand = "AT+CIPCLOSE="; //Fecha a transmissão de dados
      closeCommand += connectionId; // append connection id
      closeCommand += "\r\n";
 
      sendData(closeCommand, 2000, DEBUG);
    }
  }
}

String sendData(String command, const int timeout, boolean debug)
//Ok, esse comando aparentemente é muito importante mas eu não faço muita
//Noção de como funciona, mas basicamente ele envia comandos pra o ESP8266 com o print
//E também printa na Serial caso o debug esteja ativado
//(Na real é exatamente isso que faz, escrevi enquanto lia o código KKKKK

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
