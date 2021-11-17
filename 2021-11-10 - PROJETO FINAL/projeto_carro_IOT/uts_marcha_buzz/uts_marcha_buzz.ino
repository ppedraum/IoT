#include <Ultrasonic.h>
#define trigger 6
#define echo 7
Ultrasonic uts(trigger, echo);

#define led_a 9
#define led_b 10

unsigned long pre_millis=0;
//
#define bt 2
int bt_stt=0, bt_stt0=0, bt_vle=0;
String bt_desc[]={"Marcha Normal", "Marcha Ré", "Marcha Morto"};
//bt_vle ---> 0=Normal - 1=Ré - 2=Morto
//
#define buzz 8

//

void setup()
{
  Serial.begin(9600);
  pinMode(bt, INPUT);
  pinMode(buzz, OUTPUT);
  pinMode(led_a, OUTPUT);
  pinMode(led_b, OUTPUT);
}
//--------------------------------------------------//
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
  return uts_ms;
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

    Serial.println(bt_vle);
  }
}
//--------------------------------------------------//

//--------------------------------------------------//

void loop()
{
  bt_read();
  
}
