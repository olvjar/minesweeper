#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PROFILES 10
#define USER_PATH "profiles/"
#define USER_DIR "profiles/profile_list.txt"
#define GAME_PATH "snapshots/"
#define CLASSIC_EASY "classic_easy"
#define CLASSIC_DIFFICULT "classic_difficult"

struct recent_games {
    char path[100];
    char outcome[50];
    char mode[50];
    int time;
    int rows;
    int cols;
    char snapshot[100][100];
};

struct player {
    char name[21];
    int games_won_classic;
    int games_lost_classic;
    int games_won_custom;
    int games_lost_custom;
    struct recent_games recentgame[3];
};

typedef struct player profile;
typedef profile profileList[MAX_PROFILES];

struct scoreboard {
    char user[21];
    int time;
};
typedef struct scoreboard leaderboard[3];

void sortLeaderboard(leaderboard ranking, int count) {
    int i, j, low;
    int temp_time;
    char temp_user[21];

    for (i = 0; i < count - 1; i++) {
        low = i;
        for (j = i + 1; j < count; j++) {
            if (ranking[low].time > ranking[j].time) {
                low = j;
            }
        }

        if (i != low) {
            // time swap
            temp_time = ranking[i].time;
            ranking[i].time = ranking[low].time;
            ranking[low].time = temp_time;

            // user swap
            strcpy(temp_user, ranking[i].user);
            strcpy(ranking[i].user, ranking[low].user);
            strcpy(ranking[low].user, temp_user);
        }
    }
}

void printLeaderboard(leaderboard easyRanking, leaderboard difficultRanking) {
    int easyHours[3], easyMinutes[3], easySeconds[3];
    int diffHours[3], diffMinutes[3], diffSeconds[3];
    int easyTime[3];
    int difficultTime[3];
    int i;

    for (i = 0; i < 3; i++) {
        easyTime[i] = easyRanking[i].time;
        difficultTime[i] = difficultRanking[i].time;
    }

    for (i = 0; i < 3; i++) {
        easyHours[i] = easyTime[i] / 3600;
        easyMinutes[i] = (easyTime[i] % 3600) / 60;
        easySeconds[i] = easyTime[i] % 60;
        diffHours[i] = difficultTime[i] / 3600;
        diffMinutes[i] = (difficultTime[i] % 3600) / 60;
        diffSeconds[i] = difficultTime[i] % 60;
    }
    
    printf("|   EASY**********************LEADERBOARD   |\n");
    printf("|   [#1 %-20s -  %02d:%02d:%02d]   |\n", easyRanking[0].user, easyHours[0], easyMinutes[0], easySeconds[0]);
    printf("|   [#2 %-20s -  %02d:%02d:%02d]   |\n", easyRanking[1].user, easyHours[1], easyMinutes[1], easySeconds[1]);
    printf("|   [#3 %-20s -  %02d:%02d:%02d]   |\n", easyRanking[2].user, easyHours[2], easyMinutes[2], easySeconds[2]);
    printf("|   DIFFICULT*****************LEADERBOARD   |\n");
    printf("|   [#1 %-20s -  %02d:%02d:%02d]   |\n", difficultRanking[0].user, diffHours[0], diffMinutes[0], diffSeconds[0]);
    printf("|   [#2 %-20s -  %02d:%02d:%02d]   |\n", difficultRanking[1].user, diffHours[1], diffMinutes[1], diffSeconds[1]);
    printf("|   [#3 %-20s -  %02d:%02d:%02d]   |\n", difficultRanking[2].user, diffHours[2], diffMinutes[2], diffSeconds[2]);

}

void makeLeaderboard(leaderboard easyRanking, leaderboard difficultRanking, profileList users) {
    FILE *user, *dir, *recentgames;
    int i, j, b, c, numFiles;
    int easyCount = 0, difficultCount = 0; // valid entries
    char path[100];
    struct scoreboard tempEasy[MAX_PROFILES * 3];
    struct scoreboard tempDifficult[MAX_PROFILES * 3];

    // initialize clean leaderboard
    for (i = 0; i < MAX_PROFILES * 3; i++) {
        if (i < 3) {
            easyRanking[i].time = 0;
            difficultRanking[i].time = 0;
            easyRanking[i].user[0] = '\0';
            difficultRanking[i].user[0] = '\0';
        }
        tempEasy[i].time = 0;
        tempDifficult[i].time = 0;
        tempEasy[i].user[0] = '\0';
        tempDifficult[i].user[0] = '\0';
    }

    dir = fopen(USER_DIR, "r");
    if (dir == NULL) {
        printf("Error opening profile directory.\n");
        return;
    }
    fscanf(dir, "%d", &numFiles);

    for (i = 0; i < numFiles; i++) {
        fscanf(dir, "%s", users[i].name);

        if (strcmp(users[i].name, "") == 0) {
            return;
        } else {
            strcpy(path, USER_PATH);
            strcat(path, users[i].name);
            strcat(path, ".txt");
            user = fopen(path, "r");
            if (user == NULL) {
                printf("Error opening user file %s.\n", path);
                return;
            }
            fscanf(user, "%s", users[i].name);
            fscanf(user, "%d\n%d\n%d\n%d", &users[i].games_won_classic, &users[i].games_lost_classic, &users[i].games_won_custom, &users[i].games_lost_custom);
            for (j = 0; j < 3; j++) {
                fscanf(user, "%s\n", users[i].recentgame[j].path);
            }
            fclose(user);
        }
    }
    fclose(dir);

    // check all scanned profiles existing recent games
    for (i = 0; i < numFiles; i++) { // go through all profiles
        for (j = 0; j < 3; j++) { // go through recent games
            recentgames = fopen(users[i].recentgame[j].path, "r");
            if (recentgames == NULL) {
                printf("Error opening recent game file %s.\n", users[i].recentgame[j].path);
                return;
            }

            fscanf(recentgames, "%s", users[i].recentgame[j].outcome);
            fscanf(recentgames, "%d", &users[i].recentgame[j].time);
            fscanf(recentgames, "%s", users[i].recentgame[j].mode);
            fscanf(recentgames, "%d %d", &users[i].recentgame[j].rows, &users[i].recentgame[j].cols);

            for (b = 0; b < users[i].recentgame[j].rows; b++) {
                for (c = 0; c < users[i].recentgame[j].cols; c++) {
                    fscanf(recentgames, " %c", &users[i].recentgame[j].snapshot[b][c]);
                }
            }
            fclose(recentgames);

            // write temp leaderboards
            if (strcmp(users[i].recentgame[j].outcome, "WON") == 0) {
                if (strcmp(users[i].recentgame[j].mode, CLASSIC_EASY) == 0) {
                    strcpy(tempEasy[easyCount].user, users[i].name);
                    tempEasy[easyCount].time = users[i].recentgame[j].time;
                    easyCount++;
                } else if (strcmp(users[i].recentgame[j].mode, CLASSIC_DIFFICULT) == 0) {
                    strcpy(tempDifficult[difficultCount].user, users[i].name);
                    tempDifficult[difficultCount].time = users[i].recentgame[j].time;
                    difficultCount++;
                }
            }
        }
    }

    // set the temporary leaderboards
    sortLeaderboard(tempEasy, easyCount); 
    sortLeaderboard(tempDifficult, difficultCount);

    // fill remaining rankings if not enough valid entries
    if (easyCount > 0) {
        for (i = 0; i < 3 && i < easyCount; i++) {
            easyRanking[i] = tempEasy[i];
        }
        for (; i < 3; i++) {
            easyRanking[i] = tempEasy[0]; // use the best time for remaining ranks
        }
    }

    if (difficultCount > 0) {
        for (i = 0; i < 3 && i < difficultCount; i++) {
            difficultRanking[i] = tempDifficult[i];
        }
        for (; i < 3; i++) {
            difficultRanking[i] = tempDifficult[0];
        }
    }

    // print leaderboard
    printLeaderboard(easyRanking, difficultRanking);
}

int main() {
    leaderboard easyRanking;
    leaderboard difficultRanking;
    profileList users;

    makeLeaderboard(easyRanking, difficultRanking, users);
    return 0; // Added return statement
}
