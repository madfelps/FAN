#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include <string.h>
#include <time.h>


void rectangle(int y1, int x1, int y2, int x2)
{
    mvhline(y1, x1, 0, x2-x1);
    mvhline(y2, x1, 0, x2-x1);
    mvvline(y1, x1, 0, y2-y1);
    mvvline(y1, x2, 0, y2-y1);
    mvaddch(y1, x1, ACS_ULCORNER);
    mvaddch(y2, x1, ACS_LLCORNER);
    mvaddch(y1, x2, ACS_URCORNER);
    mvaddch(y2, x2, ACS_LRCORNER);
}

void DescreveSensor(char string [][100]){
	strcpy(string[0], "Temperature of Gate Driver Board C: ");
	strcpy(string[1], "Temperature of IGBT Module Phase A C: ");
	strcpy(string[2], "Temperature of IGBT Module Phase B C: ");
	strcpy(string[3], "Temperature of IGBT Module Phase C C: ");
	strcpy(string[4], "Angle: ");
	strcpy(string[5], "Speed: ");
	strcpy(string[6], "Torque Command: ");
	strcpy(string[7], "Torque Feedback: ");

}

void GeraAleatoriosTransformaString(int aleatorio[8], char numero[][10]){
	aleatorio[0] = rand()%100;
	sprintf(numero[0], "%i", aleatorio[0]);
	aleatorio[1] = rand()%100;
	sprintf(numero[1], "%i", aleatorio[1]);
	aleatorio[2] = rand()%100;
	sprintf(numero[2], "%i", aleatorio[2]);
	aleatorio[3] = rand()%100;
	sprintf(numero[3], "%i", aleatorio[3]);
	aleatorio[4] = rand()%100;
	sprintf(numero[4], "%i", aleatorio[4]);
	aleatorio[5] = rand()%100;
	sprintf(numero[5], "%i", aleatorio[5]);
	aleatorio[6] = rand()%100;
	sprintf(numero[6], "%i", aleatorio[6]);
	aleatorio[7] = rand()%100;
	sprintf(numero[7], "%i", aleatorio[7]);	
}

void ConcatenaTodasStrings(char numero[][10], char string[][100]){
	strcat(string[0], numero[0]);
	strcat(string[1], numero[1]);
	strcat(string[2], numero[2]);
	strcat(string[3], numero[3]);
	strcat(string[4], numero[4]);
	strcat(string[5], numero[5]);
	strcat(string[6], numero[6]);
	strcat(string[7], numero[7]);
}

void EscreveNoArquivoTexto(FILE* arq, char string[][100]){
	fprintf(arq, "%s\n", string[0]);
	fprintf(arq, "%s\n", string[1]);
	fprintf(arq, "%s\n", string[2]);
	fprintf(arq, "%s\n", string[3]);
	fprintf(arq, "%s\n", string[4]);
	fprintf(arq, "%s\n", string[5]);
	fprintf(arq, "%s\n", string[6]);
	fprintf(arq, "%s\n", string[7]);

	fprintf(arq, "%s\n", string[8]);

	fprintf(arq, "%s\n", "-----------------------------------------------------------------------");
}


int main(){

	char TempoEmString[10];

	clock_t GuardaIntervaloTempo;
	clock_t Temporaria;

	FILE* arq;
	arq = fopen("log.txt", "a");

	char numero[9][10];
	char string[9][100];


	/*
	string[0][0] = "Temperature of Gate Driver Board C: ";

	string[1][0] = "Temperature of IGBT Module Phase A C: ";

	string[2][0] = "Temperature of IGBT Module Phase B C: ";

	string[3][0] = "Temperature of IGBT Module Phase C C: ";

	string[4][0] = "Angle: ";

	string[5][0] = "Speed: ";

	string[6][0] = "Torque Command: ";

	string[7][0] = "Torque Feedback: ";

	*/


	srand(time(NULL));
	int aleatorio[8];
	int* PonteiroAleatorio;


	initscr(); 	

	rectangle(9, 4, 14, 50);
	rectangle(9, 55, 15, 85);

	int contador = 0;
	GuardaIntervaloTempo = clock();
	Temporaria = GuardaIntervaloTempo;	

	while(contador != 5){

		DescreveSensor(string);
		GeraAleatoriosTransformaString(aleatorio, numero);
		move(10, 5);
		printw("Temperature of Gate Driver Board C: %i\n", aleatorio[0]);
		move(11, 5);
		printw("Temperature of IGBT Module Phase A C: %i\n", aleatorio[1]);
		move(12, 5);
		printw("Temperature of IGBT Module Phase B C: %i\n", aleatorio[2]);
		move(13, 5);
		printw("Temperature of IGBT Module Phase C C: %i\n", aleatorio[3]);
		move(10, 60);
		printw("Angle: %i\n", aleatorio[4]);
		move(11, 60);
		printw("Speed: %i\n", aleatorio[5]);
		move(12, 60);
		printw("Torque Command: %i\n", aleatorio[6]);
		move(13, 60);
		printw("Torque Feedback: %i\n", aleatorio[7]);	
		move(15, 5);
		sleep(1);	
		refresh();
		contador++;
		ConcatenaTodasStrings(numero, string);

		Temporaria = clock() - GuardaIntervaloTempo;
		strcpy(string[8], "Instante da geração dos dados (em segundos): ");
		sprintf(TempoEmString, "%lf", (double) Temporaria*10000/CLOCKS_PER_SEC);
		strcat(string[8], TempoEmString);


		EscreveNoArquivoTexto(arq, string);


	}

	getch();
	endwin();
	fclose(arq);

	return 0;
}