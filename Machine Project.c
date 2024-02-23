/*
	Description: <describe what this program does briefly >
	Author: GRINO, Mary Eunice E. and TAMONDONG, Mariel M.
	Section: S17B
	Last Modified: February 07, 2024
	cknowledgments: <list of references used in the making of this project >
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// PREPROCESSOR DIRECTIVES

// FUNCTIONS

int main(){
	int menuSelect;
	int start = 0;

    do {
	printf("Main Menu\n[1] PLAY\t[2] LEVEL EDITOR\n[3] CHANGE PROFILE\t[4] VIEW STATISTICS \n[0] QUIT");
	scanf(" %d", &menuSelect);
	
	switch (menuSelect)
	{
		case 1: 
			play();
			start = 1;
			break;
		case 2:
			levelEditor();
			start = 1;
			break;
		case 3:
			changeProfile();
			start = 1;
			break;
		case 4:
			statistics();
			start = 1;
			break;
		default:
			printf("Invalid input. Try again.");
	}
	} while(!start);
}

/*
	This is to certify that this project is my own work, based on my personal
	efforts in studying and applying the concepts learned. I have constructed
	the functions and their respective algorithms and corresponding code by
	myself. The program was run, tested, and debugged by my own efforts.
	I further certify that I have not copied in part or whole or otherwise
	plagiarized the work of other students and/or persons.
	
	GRINO, MARY EUNICE E., DLSU ID# 12325872
	TAMONDONG, MARIEL M., DLSU ID# 12308323
*/
