#include <Ultrasonic.h>
#define trigger 4
#define echo 5
Ultrasonic uts(trigger, echo);
//
#define bt 3
int bt_stt=0, bt_stt0=0, bt_vle=0;
String bt_desc={"Marcha Normal", "Marcha Ré", "Marcha Morto"};
//bt_vle ---> 0=Normal - 1=Ré - 2=Morto
//
#define buzz 2

//

void setup()
{
  Serial.begin(9600);
  pinMode(bt, INPUT);
  pinMode(buzz, OUTPUT);
}
//--------------------------------------------------//
void uts_measure(){
  long uts_ms=uts.timing();
  float uts_cm=uts.convert(uts_ms, Ultrasonic::CM);
  return uts_cm;
}
void uts_routine(){
  if(uts_measure()>20){
    //buzzer
  }else if(uts_measure()>10){
    //Buzzer mais alto
  }else{
    
  }
}
//--------------------------------------------------//
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
      //Desliga leds da ré
    break;
    case 1:
      //Liga leds da ré
      //Inicia rotina do sensor ultrassonico
    break;
    case 2:
      //Pisca alerta
    break;
  }
}
//--------------------------------------------------//

//--------------------------------------------------//

void loop()
{
  
}
