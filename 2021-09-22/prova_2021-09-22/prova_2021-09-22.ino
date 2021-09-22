#include <HCSR04.h>

const int dsp[7]={11, 10, A4, A3, A2, 12, 13};
const int pt=A5;
int contador=0;
const int echo=9, trig=8;
UltraSonicDistanceSensor distanceSensor(8, 9, 30);
int distance;
bool udsState, udsState_old;

int numero[10][7]{
  {1, 1, 1, 1, 1, 1, 0},
  {0, 1, 1, 0, 0, 0, 0},
  {1, 1, 0, 1, 1, 0, 1},
  {1, 1, 1, 1, 0, 0, 1},
  {0, 1, 1, 0, 0, 1, 1},
  {1, 0, 1, 1, 0, 1, 1},
  {1, 0, 1, 1, 1, 1, 1},
  {1, 1, 1, 0, 0, 0, 0},
  {1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 0, 0, 1, 1}
};

void setup(){
  for(int i=0; i<7;i++)
    pinMode(dsp[i], OUTPUT);
  pinMode(pt, OUTPUT);
  Serial.begin(9600);
}

void loop(){
  float distance=distanceSensor.measureDistanceCm(23.0);
  if(distance<30 && distance>2){
    udsState=1;
    digitalWrite(pt, HIGH);
    delay(50);
  }
  else{
    udsState=0;
    digitalWrite(pt, LOW);
    delay(50);
  }
  
  if(udsState && !udsState_old){
    contador++;
  if(contador>9){
    Serial.println("Número máximo atingido!");
    contador=9;
  }
  
  }
  
  if(distance<30 && distance>2){
    udsState_old=1;
    delay(50);
  }
  else{
    udsState_old=0;
    delay(50);
  }
  
  for(int i=0;i<7;i++){
    digitalWrite(dsp[i], numero[contador][i]);
  }
  
  
}
