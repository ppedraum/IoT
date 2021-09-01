const int led[3] = {13, 12, 11};
const int ldr = A0;
int ldrRead = 0;

void setup(){

pinMode(led, OUTPUT);
pinMode(ldr, INPUT);

Serial.begin(9600);  
}

void loop(){

  ldrRead = analogRead(ldr);
  Serial.println(ldrRead);

  if(ldrRead >= 300 && ldrRead < 700 ){
    digitalWrite(led[0], HIGH);
    digitalWrite(led[1], LOW);
    digitalWrite(led[2], LOW);
  }
  else if (ldrRead >= 700 && ldrRead < 900){
    digitalWrite(led[0], LOW);
    digitalWrite(led[1], HIGH);
    digitalWrite(led[2], LOW);
  }
  else if (ldrRead >= 900){
    digitalWrite(led[0], LOW);
    digitalWrite(led[1], LOW);
    digitalWrite(led[2], HIGH);
  }
  
}
