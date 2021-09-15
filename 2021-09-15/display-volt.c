const int ledFrc[7] = { 4, A0, A3, A2, A1, 3, 2};
const int ledInt[7] = {9, 10, 13, 12, 11, 8, 7};
const int pt=A5;
const int vtm=A4;
float volts=0;
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
  	for(int i=0; i<7; i++){
		pinMode(ledFrc[i], OUTPUT);
		pinMode(ledInt[i], OUTPUT);
  	}
  	pinMode(pt, OUTPUT);
  	pinMode(vtm, INPUT);
	Serial.begin(9600);
}
void loop(){
	volts = (5.0/1023)*analogRead(vtm); // 5/1023=0 MAS 5.0/1023!=0 (se ligar no valor float)
	for(int i=0; i<7; i++){
		digitalWrite(ledInt[i], numero[(int)volts][i]);
		digitalWrite(ledFrc[i], numero[((int)(volts*10))%10][i]);
	}
	digitalWrite(pt, HIGH);
	delay(10);
}