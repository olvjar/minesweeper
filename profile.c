#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_PROFILES 10

struct recent_games{
		char mode;
		char outcome;
		char snapshot[100][100];
};

struct player{
	char name[20];
	int games_won_classic;
	int games_lost_classic;
	int games_won_custom;
	int games_lost_custom;
	struct recent_games recentgame[3];
};

typedef struct player profile;
typedef profile profileList[MAX_PROFILES];

int fileExists(char *filename) {
    FILE *file = fopen(filename, "r");
    if (file) {
        fclose(file);
        return 1;
    } else {
        return 0;
    }
}

void menuProfile(int *choice){
	printf("[1] SELECT profile\n[2] CREATE NEW profile\n[3] DELETE profile\n[4] RETURN to main menu\n\nSelection: ");
	scanf("%d", choice);
}

void selectProfile(profile *currentUser, profileList users){
	int i;
	printf("CURRENT USER: %s\n\n", currentUser->name);
	for(i = 0; i < MAX_PROFILES; i++){
		printf("%d. ", i + 1);
		if(strcmp(users[i].name, "") == 0){
		printf("< empty >\n");
		} else printf("%s\n", users[i].name);
	}
}

void newProfile(profile *currentUser, profileList *users){
	char filename[20];
    char path[] = "profiles/";
    FILE *user;

    printf("Provide profile name: ");
    scanf("%s", filename);
    strcat(filename, ".dat");
    strcat(path, filename);

	if()
    else if ((path) == 1) {
        printf("Profile already exists.\n");
        return;
    } else {
}

void changeProfile(profile *currentUser, profileList users){
	int choice, quit;
	
	while(!quit){
		menuProfile(&choice);
		switch(choice) {
            case 1:
                selectProfile(currentUser, users);
                break;
            case 2:
                //newProfile()
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
	
	strcpy(users[0].name, "molly");
	strcpy(users[1].name, "gem");
	strcpy(users[2].name, "forrest");
	
	
	changeProfile(&currentUser, users);
}
