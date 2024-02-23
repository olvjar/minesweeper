/*
	Description: <describe what this program does briefly >
	Author: GRINO, Mary Eunice E. and TAMONDONG, Mariel M.
	Section: S17B
	Last Modified: February 07, 2024
	cknowledgments: <list of references used in the making of this project >
*/

#include<stdio.h>
#include<string.h>

typedef char string20[21];

void play()
{
	int gameSelect;
	int start = 0;
	
	do {
	printf("Game type selection\n[1] CLASSIC or [2] CUSTOM");
	scanf(" %d", &gameSelect);
	
	switch (gameSelect)
	{
		case 1: 
			playClassic();
			start = 1;
			break;
		case 2:
			playCustom();
			start = 1;
			break;
		default:
			printf("Invalid input. Try again.");
	}
	} while(!start);
}

void playClassic()
{
	int classicSelect;
	int start = 0;
	
	do {
	printf("Difficulty\n[1] EASY or [2] DIFFICULT");
	scanf(" %d", &gameSelect);
	
	switch (gameSelect)
	{
		case 1: 
			classicEasy();
			start = 1;
			break;
		case 2:
			classicDifficult();
			start = 1;
			break;
		default:
			printf("Invalid input. Try again.");
	}
	} while(!start);
}

void classicEasy()
{
	
}

void classicDifficult()
{
	
}

void playCustom()
{
	
}

void changeProfile(string20 profile[])
{
	int profileChoice, i;
	
	printf("Profile selection\n[1] CHANGE/SELECT\n[2] NEW\n[3] DELETE\n");
	scanf(" %d", &profileChoice);
	
	switch (profileChoice)
	{
		case 1: //displays existing profiles
			for(i=0;i<10; i++)
				printf("Profile %d: %s", i+1, profile[i]);
			break;
		case 2: //new, name input
			
			break;
		case 3:
			
			break;
	}
}

int main(){
	int menuSelect;
	int start = 0;
	string20 profile[10] = {"Empty", "Empty","Empty","Empty","Empty","Empty","Empty","Empty","Empty","Empty"};
	
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
			changeProfile(profile);
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

	return 0;		
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
