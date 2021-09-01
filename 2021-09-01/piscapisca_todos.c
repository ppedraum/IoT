
const int led[] = {
	2, 3, 4, 5, 6, 7, 8, 9, 10, 11
};
const int bt = 12;
int estadoBt;
int estadoBt_old;
int posLed;
unsigned long lT = 0, now = 0;

void setup()
{
	for (int i = 0; i < 10; i++){
		pinMode(led[i], OUTPUT);
	}
  	pinMode(bt, INPUT);
	Serial.begin(9600);
  	
}

void loop()
{
	now = millis();
	estadoBt = digitalRead(bt);
	
	if(estadoBt && !estadoBt_old){
		posLed++;
		if(posLed < 3)
			posLed = 0;
	}
	
	estadoBt_old = digitalRead(bt);
	
	Serial.println(posLed);
	
	
	switch posLed
	{
		case 0:
			for(int i = 0; i < 10; i++){
				if(now - lT == 500){
					digitalWrite(led[i], HIGH);
					lT = now;
				}
			}
			for(int i = 0; i < 10; i++)
				digitalWrite(led[i], LOW);

			while(now - lT < 500);
			lT = now;
				
			
		break;
		case 1:
		
		break;
	}
	
	
	
	lT = 0;
	
	delay(10);
}