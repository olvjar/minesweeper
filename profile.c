#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_PROFILES 10

#define USER_PATH "profiles/"
#define USER_DIR "profiles/profile_list.txt"

struct recent_games{
		char mode;
		char outcome[100];
		char snapshot[100][100];
};

struct player{
	char name[21];
	char player_path[100];
	int games_won_classic;
	int games_lost_classic;
	int games_won_custom;
	int games_lost_custom;
	struct recent_games recentgame[3];
};

typedef struct player profile;
typedef profile profileList[MAX_PROFILES];

void menuProfile(int *choice){
	printf("[1] SELECT profile\n[2] CREATE NEW profile\n[3] DELETE profile\n[4] RETURN to main menu\n\nSelection: ");
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

void checkProfiles(profileList *users){
	int i, numFiles;
	FILE *dir;

	dir = fopen(USER_DIR, "r");
	fscanf(dir, "%d", &numFiles);

	for(i = 0; i < numFiles; i++){
		fscanf(dir, "%s", users[i]->name);
		printf("%d. ", i + 1);

		if(strcmp(users[i]->name, "") == 0){
			printf("< empty >\n");
		} else printf("%s\n", users[i]->name);
	}

	fclose(dir);
}

void viewStatistics(profile *currentUser){
		//int i;
		
		printf("Name: %s\n", currentUser->name);
    	printf("Path: %s\n", currentUser->player_path);
    	printf("Classic games - Won: %d Lost: %d\n", currentUser->games_won_classic, currentUser->games_lost_classic);
    	printf("Custom games - Won: %d Lost: %d\n", currentUser->games_won_custom, currentUser->games_lost_custom);
		printf("Recent Games:\n");
		/*
    	for(int i = 0; i < 3; i++) {
        	printf("Path %d: %s\n", i, currentUser->recentgame[i]->path);
    	}
    	*/
}

void selectProfile(profile *currentUser, profileList *users){
	char name[21];
	char filename[21];
	char path[] = USER_PATH;
	FILE *user;

	printf("CURRENT USER: %s\n\n", currentUser->name);
	checkProfiles(users);

	printf("SELECT PROFILE TO USE: ");
	scanf("%s", name);
	strcpy(filename, name);
	strcat(filename, ".dat");
	strcat(path, filename);
	
	if(fileExists(path) == 0) {
	    printf("Profile does not exist. Try again.\n");
	} else {
	    strcpy(currentUser->name, name);
	    strcpy(currentUser->player_path, path);
	    printf("TEST\n%s\n%s\n\n\nOUTPUT:\n", currentUser->name, currentUser->player_path);
	    
	    user = fopen(path, "rb+");
		fread(currentUser->name, sizeof(char), 1, user);
		fread(currentUser->player_path, sizeof(char), 1, user);
		fseek(user, size, SEEK_SET);
		//fread(&currentUser->games_won_classic, sizeof(int), 1, user);
	    fclose(user);
	    viewStatistics(currentUser);
	}
}

/*
void newProfile(profileList *users){
	char name[21];
	char filename[21];
    char path[] = USER_PATH;
    int num, i;
    FILE *user;
    FILE *dir;

	printf("EXISTING USER PROFILES:\n");
	checkProfiles(users);

    printf("Provide profile name (3 to 20 characters): ");
	scanf("%s", name);
	strcat(path, name);
	strcat(path, "/");
	strcpy(filename, name);
	strcat(filename, ".dat");
	strcat(path, filename);

    if(strlen(name) < 3 || strlen(name) > 20){
    	printf("mali.\n");
	}else if(fileExists(path) == 1) {
        printf("Profile already exists.\n");
    }else {
        printf("User profile %s will be created.\n", name);

		dir = fopen(USER_DIR, "r");
    	fscanf(dir, "%d", &num);
    	for(i = 0; i < num; i++){
			fscanf(dir, "%s", users[i]->name);
		}
		fclose(dir);

		// write list
    	dir = fopen(USER_DIR, "w");
    	fprintf(dir, "%d\n", num + 1);
    	for(i = 0; i < num; i++){
			fprintf(dir, "%s\n", users[i]->name);
		}
		fprintf(dir, "%s\n", name);
    	fclose(dir);
    	
    	user = fopen(path, "wb+");
    	fwrite(users[0], 7, sizeof(users[0]), user);
    	fclose(user);
    	
	}
}
*/

void changeProfile(profile *currentUser, profileList *users){
	int choice, quit;

	while(!quit){
		menuProfile(&choice);
		switch(choice) {
            case 1:
                selectProfile(currentUser, users);
                break;
            case 2:
                //newProfile(users);
                break;
            case 3:
            	//deleteProfile()
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
	strcpy(currentUser.name, "gem");
	profileList users;
	changeProfile(&currentUser, &users);
}
