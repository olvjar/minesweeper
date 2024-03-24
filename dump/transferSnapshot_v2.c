void transferSnapshot(char destFile[], char sourceFile[]){
	FILE *fsource;
	FILE *fdest;
	char n;
	
	fsource = fopen(sourceFile, "r");
	fdest = fopen(destFile, "w");
	
	n = fgetc(fsource);
	while (n != EOF){
		fputc(n, fdest);
		n = fgetc(fsource);
	}
	
	printf("copy success");
	
	fclose(fsource);
	fclose(fdest);
}