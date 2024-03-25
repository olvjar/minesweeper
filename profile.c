#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_PROFILES 10

#define USER_PATH "profiles/"
#define USER_DIR "profiles/profile_list.txt"
#define GAME_PATH "snapshots/"

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
typedef profile profileList[MAX_PROFILES];

void menuProfile(profile currentUser, int *choice){
	printf("CURRENT USER: %s\n\n[1] SELECT profile\n[2] CREATE NEW profile\n[3] DELETE profile\n[4] RETURN to main menu\n\nSELECTION: ", currentUser.name);
	scanf("%d", choice);
}

int fileExists(char *filename) {
    FILE *file = fopen(filename, "r");
    if (file) {
        fclose(file);
        return 1;
    } else {
        return 0;
    }
}

int checkCapital(char name[]){
	int i;
	
	for(i = 0; i<strlen(name); i++){
		if(name[i] >= 'A' && name [i] <= 'Z'){
		} else return 0;
	}
	return 1;
}

void sortProfiles(profileList users){
	int i, j, num;
	int low;
	char temp[21];
	FILE *dir;
	
	// read list
	dir = fopen(USER_DIR, "r");
	fscanf(dir, " %d", &num);
    for(i = 0; i < num; i++){
		fscanf(dir, "%s", users[i].name);
	}
	fclose(dir);
	
	//sort algorithm
	for (i = 0; i < num-1; i++){
		low = i;
		for (j = i+1; j < num; j++){
			if(strcmp(users[low].name, users[j].name) > 0){ // when low is higher
				low = j;
			}
		}
		
		if (i != low){
			strcpy(temp, users[i].name);
			strcpy(users[i].name, users[low].name);
			strcpy(users[low].name, temp);
		}
	}

	// update list
    dir = fopen(USER_DIR, "w");
    fprintf(dir, "%d\n", num);
    for(i = 0; i < num; i++){
		fprintf(dir, "%s\n", users[i].name);
	}
    fclose(dir);
}

void checkProfiles(profileList users){
	int i, numFiles;
	FILE *dir;

	sortProfiles(users);
	dir = fopen(USER_DIR, "r");
	fscanf(dir, "%d", &numFiles);

	for(i = 0; i < numFiles; i++){
		fscanf(dir, "%s", users[i].name);
		printf("%d. ", i + 1);

		if(strcmp(users[i].name, "") == 0){
			printf("< empty >\n");
		} else printf("%s\n", users[i].name);
	}

	fclose(dir);
}

void selectProfile(profile *currentUser, profileList *users){
	char name[21];
	char filename[21];
	char path[] = USER_PATH;
	//FILE *user;

	printf("[PROFILE SELECTION]\nCURRENT USER: %s\n", currentUser->name);
	checkProfiles(*users);

	printf("\nSELECT PROFILE TO USE: ");
	scanf("%s", name);
	strcpy(filename, name);
	strcat(filename, ".txt");
	strcat(path, filename);
	
	if(fileExists(path) == 0) {
	    printf("\nProfile does not exist. Try again.\n\n");
	} else {
	    strcpy(currentUser->name, name);
	    //viewStatistics(currentUser);
	}
}


void newProfile(profile *currentUser, profileList users){
	char name[21];
	char filename[21];
    char path[] = USER_PATH;
    char gamePath[] = GAME_PATH;
    int num, i;
    FILE *user;
    FILE *dir;

	printf("[PROFILE CREATION]\nEXISTING USER PROFILES:\n");
	checkProfiles(users);
	
	dir = fopen(USER_DIR, "r");
    fscanf(dir, "%d", &num);
    fclose(dir);
    
    if(num > 9){
		printf("\nMax number of profiles reached. (10 profiles)\n");
		return;
	}

    printf("\nName must be:\n[1] 3 to 20 characters\n[2] Uppercase letters only\n\nProvide profile name: ");
	scanf("%s", name);
	strcpy(filename, name);
	strcat(filename, ".txt");
	strcat(path, filename);

    if(strlen(name) > 20){
    	printf("Name is over 20 characters.\n");
    	return;
	}else if(strlen(name) < 3){
		printf("Name is less than 3 characters.\n");
    	return;
	}else if(fileExists(path) != 0) {
        printf("Profile already exists.\n");
	}else if(!(checkCapital(name))){
		printf("Name is not all uppercase letters\n.");
	}else{
        printf("\nUser profile [%s] created.\n\n", name);

		// read list of profiles
		dir = fopen(USER_DIR, "r");
    	fscanf(dir, " %d", &num);
    	for(i = 0; i < num; i++){
			fscanf(dir, "%s", users[i].name);
		}
		fclose(dir);

		// write list
    	dir = fopen(USER_DIR, "w");
    	fprintf(dir, "%d\n", num + 1);
    	for(i = 0; i < num; i++){
			fprintf(dir, "%s\n", users[i].name);
		}
		fprintf(dir, "%s\n", name);
    	fclose(dir);
    	

    	user = fopen(path, "w");
		// user details
		fprintf(user, "%s\n", name);
		// game details
		fprintf(user, "%d\n", 0);
		fprintf(user, "%d\n", 0);
		fprintf(user, "%d\n", 0);
		fprintf(user, "%d\n", 0);
		// recent games
		for(i = 0; i < 3; i++){
			fprintf(user, "%s%s_snapshot%d.txt\n", GAME_PATH, name, i);
		}
    	fclose(user);
    	// make recent games files
    	for(i = 0; i < 3; i++){
    		strcpy(gamePath, GAME_PATH);
			strcat(gamePath, name);
			if (i == 0){
				strcat(gamePath, "_snapshot0.txt");
			}
			if (i == 1){
				strcat(gamePath, "_snapshot1.txt");
			}
			if (i == 2){
				strcat(gamePath, "_snapshot2.txt");
			}
			FILE *recent = fopen(gamePath, "w");
			fclose(recent); 
		}
    	strcpy(currentUser->name, name);
	}
}

void deleteProfile(profile *currentUser, profileList users){
	char name[21];
	char filename[21];
    char path[] = USER_PATH;
    char gamePath[] = GAME_PATH;
    int num, i;
    FILE *dir;

	printf("[PROFILE DELETION]\nEXISTING USER PROFILES:\n");
	checkProfiles(users);

	printf("\nSELECT PROFILE TO DELETE: ");
	scanf(" %s", name);
	strcpy(filename, name);
	strcat(filename, ".txt");
	strcat(path, filename);

	if(fileExists(path) == 0) {
        printf("\nUser does not exist. Try again.\n\n");
        return;
    } else {
		// read list
	    dir = fopen(USER_DIR, "r");
	    fscanf(dir, " %d", &num);
	    for(i = 0; i < num; i++){
			fscanf(dir, "%s", users[i].name);
		}
		fclose(dir);

		// update list
	    dir = fopen(USER_DIR, "w");
	    fprintf(dir, "%d\n", num - 1);
	    for(i = 0; i < num; i++){
	    	if(strcmp(users[i].name, name) == 0){
	    		fprintf(dir, "%s", "");
			} else fprintf(dir, "%s\n", users[i].name);
		}
	    fclose(dir);
	    
	    // switch current user to none if currentUser == name
		if (strcmp(currentUser->name, name) == 0){
			strcpy(currentUser->name, "");
		}
	
	    // delete user snapshots | WORKING
		for(i = 0; i < 3; i++){
			strcpy(gamePath, GAME_PATH);
			strcat(gamePath, name);
			if (i == 0){
				strcat(gamePath, "_snapshot0.txt");
			}
			if (i == 1){
				strcat(gamePath, "_snapshot1.txt");
			}
			if (i == 2){
				strcat(gamePath, "_snapshot2.txt");
			}
			remove(gamePath);
		}
	    
		// delete user file | WORKING
		strcpy(filename, name);
		strcat(filename, ".txt");
		strcpy(path, USER_PATH);
		strcat(path, filename);
		remove(path);
		
		printf("\nProfile [%s] deleted successfully.\n\n", name);
	}
}


void changeProfile(profile *currentUser, profileList *users){
	int choice, quit;

	while(!quit){
		menuProfile(*currentUser, &choice);
		switch(choice) {
            case 1:
                selectProfile(currentUser, users); // WORKING
                break;
            case 2:
                newProfile(currentUser, *users); // WORKING
                break;
            case 3:
            	deleteProfile(currentUser, *users); // WORKING
                break;
            case 4:
            	printf("Returned to main menu.\n");
				quit = 1;
				break;
			default:
				printf("Invalid selection. Please choose again.\n");
		}
	}
	
}

int main(){
	profile currentUser;
	profileList users;
	changeProfile(&currentUser, &users);
}
