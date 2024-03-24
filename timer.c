#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "interface.c"

void delay(int time){
	clock_t delay;
	delay = clock() + time;
	while(delay > clock());
}

void gameClock(int gameOn){
	time_t timeStart, timeEnd;
	int timeElapsed, hours, minutes, seconds;
	
	time(&timeStart);
	printf("TIME: ");
	while(gameOn == 1){
		time(&timeEnd);
        timeElapsed = difftime(timeEnd, timeStart);
        
        hours = timeElapsed / 3600;
        minutes = (timeElapsed % 3600) / 60;
        seconds = timeElapsed % 60;
        
		printf("%02d:%02d:%02d", hours, minutes, seconds);
		iClear(6, 0, 20, 0);
		delay(300);
	}
}

int main(){
	int n = 1;
	
	gameClock(n);
}
