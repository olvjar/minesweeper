    fprintf(stdout, "%d %d\n", customLevel->rows, customLevel->cols);
    	for (int i = 0; i < customLevel->rows; i++) {
        	for (int j = 0; j < customLevel->cols; j++) {
            fprintf(stdout, " %c ", customLevel->board[i][j]);
        	}
        printf("\n");

        // 15 cols max
        // 10 rows max

// PROFILE


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
	    printf("%s\n%s\n", currentUser->name, currentUser->player_path);
	    
	    user = fopen(path, "rb");
		fread(currentUser->name, sizeof(currentUser), 1, user);
	    //fread(currentUser->player_path, sizeof(currentUser->player_path), 1, user);
	    //fread(&currentUser->games_won_classic, sizeof(currentUser->games_won_classic), 1, user);
	    //fread(&currentUser->games_lost_classic, sizeof(currentUser->games_lost_classic), 1, user);
	    //fread(&currentUser->games_won_custom, sizeof(currentUser->games_won_custom), 1, user);
	    //fread(&currentUser->games_lost_custom, sizeof(currentUser->games_lost_custom), 1, user);
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
