/*
	Pisca-pisca 1 - 2021-09-01
*/

const int led[] = {
	2, 3, 4, 5, 6, 7, 8, 9, 10, 11
};


void setup()
{
	for (int i = 0; i < 10; i++){
		pinMode(led[i], OUTPUT);
	}
	Serial.begin(9600);
	
}

void loop()
{
	
	for(int i = 0; i < 10; i++){
		digitalWrite(led[i], HIGH);
		delay(500);
		
	}
	for(int i = 0; i < 10; i++){
		digitalWrite(led[i], LOW);
	}
	delay(500);
	
	
	
	delay(10);
}