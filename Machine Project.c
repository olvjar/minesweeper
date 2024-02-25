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
typedef struct level{
	int row;
	int col;
	int mines;
	int board[][100];
};

typedef struct player{
	char name[20];
	int games_won_classic;
	int games_lost_classic;
	int games_won_custom;
	int games_lost_custom;
	struct recent_games{
		char mode;
		char outcome;
		char snapshot[][100];
	};
} profile;

// FUNCTIONS
void viewStats(profile *user){
	FILE user = *fopen("~/profiles/gem.txt", "r");
	
	// check file if it exists
	if (user == NULL) {
        printf("Error opening file.\n");
        return;
    }
	else{
		printf("%s", "/profiles/gem.txt");
	}
}

// GAME PROPER
int main(){
	int menuSelect;
	int start = 0;
	profile user;

    do {
	printf("Main Menu\n[1] PLAY\t[2] LEVEL EDITOR\n[3] CHANGE PROFILE\t[4] VIEW STATISTICS \n[0] QUIT");
	scanf(" %d", &menuSelect);
	
	switch (menuSelect)
	{
		case 1: 
			//play();
			start = 1;
			break;
		case 2:
			//levelEditor();
			start = 1;
			break;
		case 3:
			//changeProfile(profile);
			start = 1;
			break;
		case 4:
			viewStats(&user);
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
