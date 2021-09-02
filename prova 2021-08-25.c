const int led[] = {3, 6, 5};
const int bt = 7;
const int pwr = A0;


void setup()
{
	for(int i = 0; i < 3; i++)
		pinMode(led[i], OUTPUT);
	pinMode(pwr, INPUT);
	Serial.begin(9600);
}


void loop()
{
	Serial.println("1 - VERMELHO ");
	Serial.println("2 - VERDE ");
	Serial.println("3 - AZUL ");
	Serial.println("4 - RESET ");
	Serial.println("Digite a cor que deseja alterar: ");
	while(!Serial.available());
	
	int n_cor = Serial.parseInt();
	
	switch (n_cor)
	{
		case 1:
			Serial.println("Controlando o vermelho... \n");
			while(!digitalRead(bt))	
				analogWrite(led[0], analogRead(pwr)/4);
		break;
		case 2:
			Serial.println("Controlando o verde...");
			while(!digitalRead(bt))
				analogWrite(led[1], analogRead(pwr)/4);
		break;
		case 3:
			Serial.println("Controlando o azul... \n");
			while(!digitalRead(bt))
				analogWrite(led[2], analogRead(pwr)/4);
		break;
		case 4:
		Serial.println("Desligando todos... \n");
			for(int i = 0; i < 3; i++)
				digitalWrite(led[i], LOW);
		break;
		default:
			Serial.println("Número inválido! \n");
		break;
		
		
	}
	
	delay(10);
}