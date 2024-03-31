#include<stdio.h>
#include<string.h>
#include<stdlib.h>

void transferSnapshot(char destFile[], char sourceFile[]){
	FILE *fsource;
	FILE *fdest;
	char mode[21];
	char outcome[51];
	int rows, cols, i ,j;
	char val[100][100];
	
	fsource = fopen(sourceFile, "r");
	fdest = fopen(destFile, "w");
	
	fscanf(fsource, "%s", outcome);
	fscanf(fsource, "%s", mode);
	fscanf(fsource, "%d", &rows);
	fscanf(fsource, "%d", &cols);

	fprintf(fdest, "%s\n", outcome);
	fprintf(fdest, "%s ", mode);
	fprintf(fdest, "%d ", rows);
	fprintf(fdest, "%d\n", cols);

	for(i=0;i<rows;i++){
		for(j=0;j<cols;j++){
			fscanf(fsource, " %c", &val[i][j]);
		}
	}
	
	for(i=0;i<rows;i++){
		for(j=0;j<cols;j++){
			fprintf(fdest, "%c ", val[i][j]);
		}
		fprintf(fdest, "\n");
	}
	
	printf("copy success");
	
	fclose(fsource);
	fclose(fdest);
}

int main()
{

	char SS0[101] = "snapshot0.txt";
	char SS1[101] = "snapshot1.txt";
	char SS2[101] = "snapshot2.txt";
	char fnew[101] = "new.txt";
	
	transferSnapshot(SS2, SS1);
	transferSnapshot(SS1, SS0);
	transferSnapshot(SS0, fnew);
	
	return 0;
}
