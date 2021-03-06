#include <HCSR04.h>

const int dspU[7]={11, 10, A4, A3, A2, 12, 13};
const int dspD[7]={7, 6, 5, 4, 3, 2, A1};
const int pt=A5;
int contador=0, unidade=0, dezena=0;
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
  for(int i=0; i<7;i++){
    pinMode(dspU[i], OUTPUT);
    pinMode(dspD[i], OUTPUT);
  }
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
  if(contador>99){
    Serial.println("Número máximo de itens atingido!");
    contador=99;
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
  unidade=contador%10;
  if(contador>9)
	dezena=contador/10;
	
  
  for(int i=0;i<7;i++){
    digitalWrite(dspU[i], numero[unidade][i]);
    digitalWrite(dspD[i], numero[dezena][i]);
  }
  
  
}