#include <HCSR04.h>

const int trig=8, echo=7;
const int blue=10, red=11;

UltraSonicDistanceSensor distanceSensor(trig, echo);  // Initialize sensor that uses digital pins 13 and 12.

void setup () {
  pinMode(red, OUTPUT);
  pinMode(blue, OUTPUT);
    Serial.begin(9600);  // We initialize serial connection so that we could print values from sensor.
}

void loop () {
  float distancia = distanceSensor.measureDistanceCm();
  if(distancia<10){
    digitalWrite(red, HIGH);
    digitalWrite(blue, LOW);
  }
  else{
    digitalWrite(blue, HIGH);
    digitalWrite(red, LOW);
  }
}