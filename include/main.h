#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>

/*
 *   PREPROCESSOR DIRECTIVES 
 */

#define CLEARSCREEN system("cls")

// CONSTANTS
#define MAX_PROFILES 10
#define HIDDEN 10
#define FLAG 100

#define CLASSIC_EASY "CLASSIC-EASY"
#define CLASSIC_DIFFICULT "CLASSIC-DIFFICULT"
#define CUSTOM "CUSTOM"

// PATHS
#define LVL_PATH "data/levels/"
#define LVL_DIR "data/levels/level_list.txt"
#define USER_PATH "data/profiles/"
#define USER_DIR "data/profiles/profile_list.txt"
#define GAME_PATH "data/snapshots/"

/*
 *    STRUCTS
 */

// GAME PROPER
struct level {
	int rows;
    	int cols;
	int mines;
	char mode[30];
	int gameBoard[10][15];
	char board[10][15];
};

struct fileInfo{
	char filename[21];
	char path[100];
};

typedef struct level game;
typedef struct fileInfo file;
typedef file customLevelList[20];
typedef game recentGame;

// PLAYER STATS
struct recent_games{
	char path[100];
	char outcome[50];
	char mode[50];
	int time;
	int rows;
	int cols;
	char snapshot[10][15];
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
typedef profile profileList[MAX_PROFILES];


// LEADERBOARD
struct scoreboard{
		char user[21];
		int time;
};

typedef struct scoreboard leaderboard[3];

#endif
