int led[7] = {
	9, 10, 13, 12, 11, 8, 7
};
int bte = 5, btd = 6;
bool bteState, btdState, bteState_old, btdState_old;
int contador = 0;

int numero[10][7] = 
{
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
	for(int i = 0; i < 7; i++){
		pinMode(led[i], OUTPUT);
	}
	pinMode(bte, INPUT_PULLUP);
	pinMode(btd, INPUT_PULLUP);
  
  	Serial.begin(9600);
	
}
void loop(){
	
	bteState = digitalRead(bte);
	btdState = digitalRead(btd);
	
	if(!bteState && bteState_old)
		if(contador>0)
			contador--;
	if(!btdState && btdState_old)
		if(contador<9)
			contador++;
	
	
	bteState_old = digitalRead(bte);
	btdState_old = digitalRead(btd);
	
	
	for(int i = 0; i<9; i++){
			digitalWrite(led[i], !numero[contador][i]);
	}
	
	Serial.println(contador);

	
	delay(10);
}