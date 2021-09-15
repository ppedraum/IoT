const int ledUn[7] = { 4, A0, A3, A2, A1, 3, 2};
const int ledDz[7] = {9, 10, 13, 12, 11, 8, 7};
const int bte=5, btd=6;
int bteS, btdS, bteS_old, btdS_old;
int contador=0;
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
		pinMode(ledUn[i], OUTPUT);
		pinMode(ledDz[i], OUTPUT);
  	}
	pinMode(bte, INPUT_PULLUP);
	pinMode(btd, INPUT_PULLUP);
	Serial.begin(9600);
}
void loop(){
	bteS = digitalRead(bte);
	btdS = digitalRead(btd);
	if(!bteS && bteS_old){
		if(contador>0)
			contador--;
	}
	if(!btdS && btdS_old){
		if(contador<99)
			contador++;
	}
	bteS_old = digitalRead(bte);
	btdS_old = digitalRead(btd);
  	int un = contador%10;
  	int dez = contador/10;
	for(int i=0; i<7; i++){
		digitalWrite(ledUn[i], numero[un][i]);
		digitalWrite(ledDz[i], numero[dez][i]);

	}
	delay(10);
}