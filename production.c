/*
 * production.c
 *
 *  Created on: Jan 23, 2019
 *      Author: Derek Ng
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "production.h"
#include <math.h>
#include <string.h>

/**
 * Main production code. Reads arguments from the command line.
 * @param argc Number of words on the command line
 * @param argv Array of pointers to character strings representing the words on the command line
 * @return true if the game was able to be played;
 *         false if the user entered bad input
 */
bool production(int argc, char* argv[]) {
	bool results = false; //returns true if everything seems ok
	bool done = false; //true if input is invalid
	int nRows = -1; //number of rows on the board, must be > 0
	int nCols = -1; //number of columns on the board, must be > 0
	int gens = -1; //number of generations to play, must be > 0
	char filename[100]; //filename
	for(int i = 0; i < 100; i++) { //initialize filename with nulls, fills 100 spots
		filename[i] = '\0';
	}
	char print = '\0'; //'y' if each generation should be printed, 'n' otherwise
	char pause = '\0'; //'y' if keystroke needed to go to next gen, 'n' otherwise

	if(argc < 5) { //not enough arguments
		printf("%d arguments provided. Four arguments required.\n", argc - 1);
		done = true;
	} else if(argc > 7) { //too many arguments
		printf("%d arguments provided. Six arguments maximum.\n", argc - 1);
		done = true;
	} else { //enough arguments provided
		char* ptr = 0;
		long arg_l = strtol(argv[1], &ptr, 10); //get the NR
		nRows = (int) arg_l;
		if(nRows < 1) { //NR argument is not valid
			printf("Usage: Rows should be greater than 0, received %d.\n", nRows);
			done = true;
		}

		arg_l = strtol(argv[2], &ptr, 10); //gets NC
		nCols = (int) arg_l;
		if(nCols < 1) { //NC argument is not valid
			printf("Usage: Columns should be greater than 0, received %d.\n", nCols);
			done = true;
		}

		arg_l = strtol(argv[3], &ptr, 10); //gets gens
		gens = (int) arg_l;
		if(gens < 1) { //gens argument is not valid
			printf("Usage: Generations should be greater than 0, received %d.\n", gens);
			done = true;
		}

		strcpy(filename, argv[4]); //gets name of inputfile

		if(argc > 5) { //gets print
			if(argv[5][0] == 'y' || argv[5][0] == 'n') { //checks if arg is a valid 'y' or 'n'
				print = argv[5][0];
			} else { //print not valid
				printf("Print argument should be a 'y' or 'n', received '%s'.\n", argv[5]);
				done = true;
			}
		} else { //print argument not specified by user, 'n' by default
			print = 'n';
		}

		if(argc > 6) { //gets pause
			if(argv[6][0] == 'y' || argv[6][0] == 'n') { //checks if arg is a valid 'y' or 'n'
				pause = argv[6][0];
			} else { //pause not valid
				printf("Pause argument should be a 'y' or 'n', received '%s'.\n", argv[6]);
				done = true;
			}
		} else { //pause argument not specified by user, 'n' by default
			pause = 'n';
		}

		FILE* fp = fopen(filename, "r"); //attempt to read file
		int numRows = 0; //number of rows in the file
		int numCols = 0; //number of columns in the file
		int* numRowsInFile = &numRows; //pointer to numRows
		int* numColsInFile = &numCols; //pointer to numCols

		if(fp == false) { //cannot read file
			done = true;
			printf("Cannot find the file: %s\n", filename);
		} else {
			//determines number of rows and columns in file and puts them in the pointers
			numRowsAndCols(fp, numRowsInFile, numColsInFile);
			if(nRows < *numRowsInFile) { //not enough rows on board
				done = true;
				printf("Number of rows on board (%d) is less than number of rows in file (%d).\n", *numRowsInFile, nRows);
			}
			if(nCols < *numColsInFile) { //not enough columns on board
				done = true;
				printf("Number of columns on board (%d) is less than number of columns in file (%d).\n", *numColsInFile, nCols);
			}
		}

		if(!done) {
			//print user's arguments
			printf("Number rows on board: \t\t%d\n", nRows);
			printf("Number columns on board: \t%d\n", nCols);
			printf("Max generations to be played: \t%d\n", gens);
			printf("File that was opened: \t\t%s\n",filename);
			if(print == 'y') {
				printf("Print each generation: \t\tYes\n");
			} else {
				printf("Print each generation: \t\tNo\n");
			}
			if(pause == 'y') {
				printf("Pause between generations: \tYes\n\n");
			} else {
				printf("Pause between generations: \tNo\n\n");
			}

			char A[nRows][nCols]; //array to hold a generation
			char B[nRows][nCols]; //array to hold another generation
			char C[nRows][nCols]; //array to hold another generation
			char* old_p = &A[0][0]; //points to current array
			char* new_p = &B[0][0]; //points to array generated from current array
			char* other_p = &C[0][0]; //points to array from previous generation

			//copies the data from the input file into array A
			initializeArray(old_p, filename, nRows, nCols, *numRowsInFile, *numColsInFile);
			if(print == 'y'){
				printf("Initial generation:\n");
				printThis(nRows, nCols, old_p); //prints initial generation
			}

			//plays the rest of the game
			generate(gens, nRows, nCols, old_p, new_p, other_p, print, pause);
		}
	}
	if(!done) { //no errors seemed to appear
		results = true;
	}
	return results;
}

/**
 * Calculates the number or rows and columns in the input file
 * @param fp Pointer to the input file
 * @param numRowsInFile Pointer to the variable that stores the number of rows in the input file
 * @param numColsInFile Pointer to the variable that stores the number of columns in the input file
 */
void numRowsAndCols(FILE* fp, int* numRowsInFile, int* numColsInFile) {
	char oRow[100]; //the text in a row of the input file
	bool doneReadingFile = false; //checks to see if the end of the file has been reached

	while(!doneReadingFile) { //exits loop when end of the file is reached
		oRow[0]='\0';
		fscanf(fp, "%s", oRow); //reads a line from the input file
		if(strlen(oRow)!=0) { //there was something there
			(*numRowsInFile)++;
			if(strlen(oRow) > *numColsInFile) { //update maximum width
				*numColsInFile = (int)strlen(oRow);
			}
		} else { //no more lines in the file
			doneReadingFile = true;
			fclose(fp);
		}
	}
}

/**
 * Creates the initial generation
 * @param A Array to be initialized
 * @param filename Pointer to filename of the input file
 * @param nRows Number of rows on the board
 * @param nCols Number of columns on the board
 * @param numRowsInFile Number of rows in the file
 * @param numColsInFile Number of columns in the file
 */
void initializeArray(char* A, char* filename, int nRows, int nCols, int numRowsInFile, int numColsInFile) {
	for(int row = 0; row < nRows; row++) { //fills every cell in the array with a 'o'
		for(int col = 0; col < nCols; col++) {
			*(A + (row * nCols) + col) = 'o';
		}
	}

	char oRow[100]; //the text in a row of the input file
	int startRow = (nRows - numRowsInFile) / 2; //vertically centers the input on the board
	int startCol = (nCols - numColsInFile) / 2; //horizontally centers the input on the board
	FILE* f = fopen(filename, "r"); //opens the input file
	for(int row = 0; row < numRowsInFile; row++) { //loops through array
		fscanf(f, "%s", oRow); //reads a line from the input file
		for(int col = 0; col < (int) strlen(oRow); col++) {
			if(oRow[col] == 'x') { //adds 'x' to the array of 'o's
				*(A + ((startRow + row) * nCols) + (startCol + col)) = 'x';
			}
		}
	}
}

/**
 * PlayOne carries out one generation
 * @param nr Number of rows in the petri dish
 * @param nc Number of columns in the petri dish
 * @param Old Location of the upper left of starting petri dish
 * @param New Location of the upper left of the ending petri dish
 */
void PlayOne(unsigned int nr, unsigned int nc, char* Old, char* New) {
	int nRows = nr; //number of rows on the board
	int nCols = nc; //number of columns on the board
	//loop over all locations in the new dish
	//figure out whether occupied or not
	for(int row = 0; row < nRows; row++) { //loops through array
		for(int col = 0; col < nCols; col++) {
			bool occupied = true;
			occupied = (getLetter(row, col, nCols, Old) == 'x'); //occupied if 'x'

			if(occupied
					&& ((HowManyNeighbors(row, col, nRows, nCols, Old) < 2) //less than 2 or over 3 neighbors
							|| (HowManyNeighbors(row, col, nRows, nCols, Old) > 3))) { //Death rule
				*(New + (row * nCols) + col) = 'o';
			} else if(occupied) { //Survival rule
				*(New + (row * nCols) + col) = 'x';
			} else if(HowManyNeighbors(row, col, nRows, nCols, Old) == 3) { //Birth rule
				*(New + (row * nCols) + col) = 'x';
			} else {
				*(New + (row * nCols) + col) = 'o';
			}
		}
	}
}

/**
 * Gets a letter from an array
 * @param row Row to retrieve the letter from
 * @param col Column to retrieve the letter from
 * @param nCols Number of columns in the array
 * @param Old Pointer to the array
 * @return a letter
 */
char getLetter(int row, int col, int nCols, char* Old) {
	return *(Old + (row * nCols) + col);
}

/**
 * Determines the number of neighbors (x's) surrounding a cell
 * @param row Row that cell is located in
 * @param col Column that cell is located in
 * @param nRows Number of rows in the array
 * @param nCols Number of columns in the array
 * @param Old Pointer to the array
 * @return number of neighbors (x's) surrounding a cell
 */
int HowManyNeighbors(int row, int col, int nRows, int nCols, char* Old) {
	int howManyN = 0;
	//there could be as many as 8 neighbors
	//cells on an edge or corner have fewer neighbors
	//we will search for neighbors clockwise from NorthWest

	if(row > 0) { //looks north
		if(col > 0) {
			if(getLetter(row - 1, col - 1, nCols, Old) == 'x') { //NW
				howManyN++;
			}
		}
		if(getLetter(row - 1, col, nCols, Old) == 'x') { //N
			howManyN++;
		}
		if(col < (nCols - 1)) {
			if(getLetter(row - 1, col + 1, nCols, Old) == 'x') { //NE
				howManyN++;
			}
		}
	}

	if(col > 0)	{ //looks west
		if(getLetter(row, col - 1, nCols, Old) == 'x') { //W
			howManyN++;
		}
	}

	if(col < (nCols - 1)) { //looks east
		if(getLetter(row, col + 1, nCols, Old) == 'x') { //E
			howManyN++;
		}
	}

	if(row < nRows - 1)	{ //looks south
		if(col > 0) {
			if(getLetter(row + 1, col - 1, nCols, Old) == 'x') { //SW
				howManyN++;
			}
		}
		if(getLetter(row + 1, col, nCols, Old) == 'x') { //S
			howManyN++;
		}
		if(col < (nCols - 1)) {
			if(getLetter(row + 1, col + 1, nCols, Old) == 'x') { //SE
				howManyN++;
			}
		}
	}
	return howManyN;
}

/**
 * Generates a new generation & checks for stopping conditions
 * @param gens Maximum generations to play game
 * @param nRows Number of rows in the arrays
 * @param nCols Number of columns in the arrays
 * @param old_p Pointer to the previous generation
 * @param new_p Pointer to the current generation
 * @param other_p Pointer to two generations ago
 * @param print Determines if each generation should be printed: 'y' for yes, 'n' for no
 * @param pause Determines if there should be a pause between generations: 'y' for yes, 'n' for no
 * @return Number of generations played
 */
int generate(int gens, int nRows, int nCols, char* old_p, char* new_p, char* other_p, char print, char pause) {
	int g = 0; //number of generations played
	bool done = false; //true if game ended
	bool firstTime = true; //first generation

	//loops until all organisms are dead, repetition occurs, or max number of generations reached
	for(int gensDone = 0; !done && (gensDone < gens); gensDone++) {
		if(!done) { //play one generation
			PlayOne(nRows, nCols, old_p, new_p);
		}
		g++;

		if(!anyX(new_p, nRows, nCols)) { //all organisms are dead
			done = true;
			printf("Generation %d (final configuration):\n", g);
			printThis(nRows, nCols, new_p);
			printf("All organisms dead after %d generations.\n", g);
		}
		if(sameContent(old_p, new_p, nRows, nCols)) { //repeats from last generation
			done = true;
			printf("Generation %d (final configuration):\n", g);
			printThis(nRows, nCols, new_p);
			printf("Pattern repeated in one generation (Generations %d & %d).\n", g-1, g);
		}
		if(firstTime) { //only checks 2nd generation repeat after two loops of game
			firstTime = false;
		} else {
			if(sameContent(new_p, other_p, nRows, nCols)) { //repeats from two generations ago
				done = true;
				printf("Generation %d (final configuration):\n", g);
				printThis(nRows, nCols, other_p);
				printf("Pattern repeated after two generations (Generations %d & %d).\n", g-2, g);
			}
		}
		if(!done && (gensDone == gens - 1)) { //maximum generations reached
			done = true;
			printf("Generation %d (final configuration):\n", g);
			printThis(nRows, nCols, new_p);
			printf("%d generations reached.\n", gens);
		}
		if(!done) { //ending conditions not met, game continues
			if(print == 'y') { //prints each generation
				printf("Generation %d:\n", g);
				printThis(nRows, nCols, new_p);
			}
			if(pause == 'y') { //pauses between each generation
				puts("\nPress enter to continue.");
				getc(stdin); //just waits for user input
			}
			char* temp = other_p;
			other_p = old_p; //1 generation ago become 2 generations ago
			old_p = new_p; //current generation becomes 1 generation ago
			new_p = temp; //2 generations assigned to new generation pointer to be overwritten
		}
	}
	return g;
}

/**
 * Determines if any organisms are still alive
 * @param arr Array of organisms
 * @param nRows Number of rows in array
 * @param nCols Number of columns in array
 * @return true if organisms (x's) are present;
 * 		   false if no organisms (x's) are present
 */
bool anyX(char* arr, int nRows, int nCols) {
	bool any = false; //true if an 'x' is present in an array
	for(int row = 0; !any && (row < nRows); row++) { //stops if 'x' found or nRows reached
		for(int col = 0; !any && (col < nCols); col++) { //stops if 'x' found or nCols reached
			if(*(arr + row * nCols + col) == 'x') {
				any = true;
			}
		}
	}
	return any;
}

/**
 * Determines if two arrays are identical
 * @param one_p Pointer to one array
 * @param another_p Pointer to a second array
 * @param nRows Number of rows in the array
 * @param nCols Number of columns in the array
 * @return true if both arrays are identical;
 * 		   false if there is a difference in the arrays
 */
bool sameContent(char* one_p, char* another_p, int nRows, int nCols) {
	bool same = true; //for all the comparisons that have been made so far
	for(int row = 0; same && (row < nRows); row++) { //stops if two values aren't the same or if nRows reached
		for(int col = 0; same && (col < nCols); col++) { //stops if two values aren't the same or if nCols reached
			if(*(one_p + row * nCols + col) != *(another_p + row * nCols + col)) {
				same = false;
			}
		}
	}
	return same;
}

/**
 * Prints the x's in the array
 * @param nRows Number of rows in the array
 * @param nCols Number of columns in the array
 * @param old_p Pointer to the array to be printed
 */
void printThis(int nRows, int nCols, char* old_p) {
	for(int row = 0; row < nRows; row++) { //loops through array
		for(int col = 0; col < nCols; col++) {
			if(*(old_p + row * nCols + col) == 'x') { //prints if 'x'
				printf("x");
			} else { //doesn't print 'o'
				printf(" ");
			}
		}
		printf("\n");
	}
	printf("\n");
}
