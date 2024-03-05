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

#include "gameProper.c"

// PREPROCESSOR DIRECTIVES
struct level{
	int rows;
	int cols;
	int mines;
	int gameBoard[100][100];
	char board[100][100];
};

typedef struct level game;

struct recent_games{
		char mode;
		char outcome;
		char snapshot[100][100];
};
typedef struct player{
	char name[20];
	int games_won_classic;
	int games_lost_classic;
	int games_won_custom;
	int games_lost_custom;
	struct recent_games recentgame;
};

typedef struct player profile;

// FUNCTIONS
void play(profile user, game level, game customLevel)
{
	int gameSelect;
	int validChoice = 0;

	do {
	printf("Game type selection\n[1] CLASSIC or [2] CUSTOM");
	scanf(" %d", &gameSelect);

	switch (gameSelect)
	{
		case 1:
			playClassic(level);
			validChoice = 1;
			break;
		case 2:
			playCustom(&customLevel);
			validChoice = 1;
			break;
		default:
			printf("Invalid input. Try again.");
	}
	} while(!validChoice);
}

void playClassic(game level)
{
	int gameSelect;
	int start = 0;
	
	do {
	printf("Difficulty\n[1] EASY or [2] DIFFICULT");
	scanf(" %d", &gameSelect);
	
	switch (gameSelect)
	{
		case 1: 
			level.rows = level.cols = 8;
			level.mines = 10;
			gameProper(level);
			start = 1;
			break;
		case 2:
			level.rows = 15;
			level.cols = 10;
			level.mines = 10;
			gameProper(level);
			start = 1;
			break;
		default:
			printf("Invalid input. Try again.");
	}
	} while(!start);
}

void playCustom(game *customLevel){
	char filename[20];
    char path[] = "levels/";
	FILE *chosenLevel;

	printf("Choose a level to play:");
	prinft("");
}

// GAME PROPER
int main(){
	// initalize variables
	profile user;
	game customLevel;
	game level;

	// start
	int menuSelect;
	int start = 0;

    do {
	printf("Main Menu\n[1] PLAY\t[2] LEVEL EDITOR\n[3] CHANGE PROFILE\t[4] VIEW STATISTICS \n[0] QUIT");
	scanf(" %d", &menuSelect);

	switch (menuSelect)
	{
		case 1:
			//play(game);
			start = 1;
			break;
		case 2:
			//levelEditor(&customLevel);
			start = 1;
			break;
		case 3:
			//changeProfile(&user);
			start = 1;
			break;
		case 4:
			//viewStats(&user);
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
