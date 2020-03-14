#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>

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


int main(){
	initscr(); 
	srand(time(NULL));
	int aleatorio1, aleatorio2, aleatorio3, aleatorio4, aleatorio5, aleatorio6, aleatorio7, aleatorio8;
	while(1){
		aleatorio1 = rand()%100;
		aleatorio2 = rand()%100;
		aleatorio3 = rand()%100;
		aleatorio4 = rand()%100;
		aleatorio5 = rand()%100;
		aleatorio6 = rand()%100;
		aleatorio7 = rand()%100;
		aleatorio8 = rand()%100;
		rectangle(9, 4, 14, 50);
		rectangle(9, 55, 14, 85);
		//mvhline(14, 50, 9, 50);
		//mvhline(y1, x1, 0, x2-x1);
		move(10, 5);
		printw("Temperature of Gate Driver Board C: %i\n", aleatorio1);
		move(11, 5);
		printw("Temperature of IGBT Module Phase A C: %i", aleatorio2);
		move(12, 5);
		printw("Temperature of IGBT Module Phase B C: %i", aleatorio3);
		move(13, 5);
		printw("Temperature of IGBT Module Phase C C: %i", aleatorio4);
		move(10, 60);
		printw("Angle: %i", aleatorio5);
		move(11, 60);
		printw("Speed: %i", aleatorio6);
		move(12, 60);
		printw("Torque Command: %i", aleatorio7);
		move(13, 60);
		printw("Torque Feedback: %i", aleatorio8);	
		move(15, 5);	
		refresh();
		sleep(1);
	}
	getch();
	endwin();
	return 0;
}