/*
	Contador de 10 leds - 2021-09-01
*/

const int led[] = {
	2, 3, 4, 5, 6, 7, 8, 9, 10, 11
};
const int bt[] = {12, 13};
int estadoBt[2];
int estadoBt_old[2];
int posLed;


void setup()
{
	for (int i = 0; i < 10; i++){
		pinMode(led[i], OUTPUT);
		if(i < 2)
			pinMode(bt[i], INPUT);
	}
	Serial.begin(9600);
	
}

void loop()
{
	/*
	if(digitalRead(bt[0]) && digitalRead(bt[1])){
		for (int i = 0; i < 10; i++)
			digitalWrite(led[i], HIGH);
	}else{
		for (int i = 0; i < 10; i++)
			digitalWrite(led[i], LOW);
	
	}
	*/
	
	estadoBt[0] = digitalRead(bt[0]);
	estadoBt[1] = digitalRead(bt[1]);
	
	if(estadoBt[0] && !estadoBt_old[0]){
		posLed--;
		if(posLed < 0)
			posLed = 0;
	}
	if(estadoBt[1] && !estadoBt_old[1]){
		posLed++;
		if(posLed > 10)
			posLed = 10;
	}
	
	estadoBt_old[0] = digitalRead(bt[0]);
	estadoBt_old[1] = digitalRead(bt[1]);
	
	Serial.println(posLed);
	
	
	for(int i = 0; i < 10; i++){
		if(i < posLed)
			digitalWrite(led[i], HIGH);
		else{
			digitalWrite(led[i], LOW);
		}
	
	}
	
	
	
	
	
	delay(10);
}