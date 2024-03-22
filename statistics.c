#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "interface.c"

#define MAX_PROFILES 10

#define USER_PATH "profiles/"
#define USER_DIR "profiles/profile_list.txt"
#define GAME_PATH "snapshots/"

// PREPROCESSOR DIRECTIVES
struct level {
    int rows;
    int cols;
    int mines;
	char mode[30];
    int gameBoard[100][100];
    char board[100][100];
};

struct fileInfo{
	char filename[20];
	char path[100];
};

typedef struct level game;
typedef struct fileInfo file;
typedef file customLevelList[100];
typedef game recentGame;


struct recent_games{
	char path[100];
	char outcome[11];
	char mode[50];
    int rows;
    int cols;
	char snapshot[100][100];
};

struct player{
	char name[21];
	int games_won_classic;
	int games_lost_classic;
	int games_won_custom;
	int games_lost_custom;
	struct recent_games recentgame[3];
};

typedef struct player profile;

void viewStatistics(profile *currentUser){
    int i, j, k;
    FILE *recentgames;
		
	printf("Name: %s\n", currentUser->name);
    printf("Classic games - Won: %d Lost: %d\n", currentUser->games_won_classic, currentUser->games_lost_classic);
    printf("Custom games - Won: %d Lost: %d\n", currentUser->games_won_custom, currentUser->games_lost_custom);
	printf("Recent Games:\n");
    
    for(i = 0; i < 3; i++) {
	    recentgames = fopen(currentUser->recentgame[i].path, "r");
	    
		fscanf(recentgames, "%s", currentUser->recentgame[i].outcome);
	    fscanf(recentgames, "%s", currentUser->recentgame[i].mode);
	    fscanf(recentgames, "%d %d", &currentUser->recentgame[i].rows, &currentUser->recentgame[i].cols);
	    
	  	for (int b = 0; b < currentUser->recentgame[i].rows; b++) {
	       for (int c = 0; c < currentUser->recentgame[i].cols; c++) {
	            fscanf(recentgames, " %c", &currentUser->recentgame[i].snapshot[b][c]);
	        }
	    }
	    
	    fclose(recentgames);
	        
	    printf("GAME %s\n", currentUser->recentgame[i].outcome);
	    printf("%s\n", currentUser->recentgame[i].mode);
	    printf("%d %d\n", currentUser->recentgame[i].rows, currentUser->recentgame[i].cols);
	    for (j = 0; j < currentUser->recentgame[i].rows; j++) {
	        for (k = 0; k < currentUser->recentgame[i].cols; k++) {
	            printf(" %c ", currentUser->recentgame[i].snapshot[j][k]);
	        }
	    printf("\n");
	    }
    }
}

int main(){
    profile currentUser;
    FILE *user;

    user = fopen("profiles/molly.txt", "r");
    fscanf(user, "%s", currentUser.name);
    fscanf(user, "%d\n%d\n%d\n%d", &currentUser.games_won_classic, &currentUser.games_lost_classic, &currentUser.games_won_custom, &currentUser.games_lost_custom);
 	for(int i = 0; i < 3; i++){
   		fscanf(user, "%s\n", currentUser.recentgame[i].path);
	}

	viewStatistics(&currentUser);
}
