/*
 * tests.c
 *
 *  Created on: Jan 23, 2019
 *      Author: Derek Ng
 */
#include <stdbool.h>
#include "tests.h"
#include "production.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h> //for strncmp

/**
 * Main Test method
 * @return true if all tests passes;
 * 		   false otherwise
 */
bool tests(void) {
	bool results = false;
	puts("\nDOING TESTS\n");
	bool ok1 = testReadInput();
	if(ok1) puts("Found and read the test file.");
	bool ok2 = testMakeArrays();
	if(ok2) puts("Was able to allocate the arrays ok.");
	bool ok3 = testPlayOne();
	if(ok3) puts("playOne is ok.");
	bool ok4 = testNumRowsAndCols();
	if(ok4) puts("numRowsAndCols is ok.");
	bool ok5 = testInitializeArray();
	if(ok5) puts("initializeArray is ok.");
	bool ok6 = testPlayOne2();
	if(ok6) puts("PlayOne is ok.");
	bool ok7 = testGetLetter();
	if(ok7) puts("getLetter is ok.");
	bool ok8 = testHowManyNeighbors();
	if(ok8) puts("howManyNeighbors is ok.");
	bool ok9 = testGenerate();
	if(ok9) puts("generate is ok.");
	bool ok10 = testGenerate2();
	if(ok10) puts("generate2 is ok.");
	bool ok11 = testGenerate3();
	if(ok11) puts("generate3 is ok.");
	bool ok12 = testAnyX();
	if(ok12) puts("anyX is ok.");
	bool ok13 = testsSameContent();
	if(ok13) puts("sameContent is ok.");

	results = ok1&&ok2&&ok3&&ok4&&ok5&&ok6&&ok7&&ok8&&ok9&&ok10&&ok11&&ok12&&ok13;
	printf("tests returning %d.\n\n", results);
	puts("\nEND OF TESTING\n");
	puts("*****************************************\n");
	return results;
}

/**
 * Tests that input can be read
 * @return true if input can be read;
 * 		   false otherwise
 */
bool testReadInput(void) {
	//the plan is:
	//create a file
	//make sure it can be opened.
	//try a file that should not exist
	//check that we notice it does not exist

	bool results = false;
	bool ok1 = false;
	bool ok2 = false;
	bool ok3 = false;
	//
	//Here goes: create a file
	FILE* fp;
	fp = fopen("/tmp/LifeInput.txt", "w"); //we write it first for later reading
	if (fp != false) { //it opened, yay!
		//file containing a sequence of lines,
		//each consisting of a sequence of 'x' and 'o' characters,
		//indicating the occupied and unoccupied cells
		//of the initial configuration, e.g.
		//oxo
		//xox
		//xox
		//oxo
		int w1 = fprintf(fp, "oxo\n");//returns the number of chars excluding null
		int w2 = fprintf(fp, "xox\n");
		int w3 = fprintf(fp, "xox\n");
		int w4 = fprintf(fp, "oxo\n");
		if((w1 == w2) && (w2 == w3) && (w3 == w4) && (w4 == 4)) {
			ok1 = true;
		}
		fclose(fp);
	}
	fp = fopen("/tmp/LifeInput.txt", "r"); //we try to read it
	if (fp != false) { //it opened, yay!
		ok2 = true;
		//can we read the correct data?
		char oRow[4]; //test case, we know a string of 3 chars terminated by null
		//printf("First row is %s\n", oRow);
		if(0 == strncmp(oRow, "oxo", 4)) { //0 means match
			//got the right data
			ok3 = true;
		}
		fscanf(fp, "%s", oRow);
		//printf("Second row is %s\n", oRow);
		if(0 == strncmp(oRow, "xox", 4)) {
			//got the right data
			ok3 = true;
		}
		fscanf(fp, "%s", oRow);
		//printf("Third row is %s\n", oRow);
		if(0 == strncmp(oRow, "xox", 4)) {
			//got the right data
			ok3 = true;
		}
		fscanf(fp, "%s", oRow);
		//printf("Fourth row is %s\n", oRow);
		if(0 == strncmp(oRow, "oxo", 4)) {
			//got the right data
			ok3 = true;
		}
	}
	results = ok1 && ok2 && ok3;
	return results;
}

/**
 * Tests that an array can be copied
 * @return true if array is successfully copied;
 * 		   false otherwise
 */
bool testMakeArrays(void) {
	bool results = false;
	bool ok1 = false;
	//we need to allocate and populate arrays
	//In this test case we set our arrays to 4 rows x3 columns
	int nRows = 4;
	int nCols = 4;
	char boardBefore[4][4] = {
			{'o','x','o','\0'},
			{'x','o','x','\0'},
			{'x','o','x','\0'},
			{'o','x','o','\0'}
	};

	char boardAfter[nRows][nCols];
	//here's the test
	for(int row = 0; row < nRows; row++) {
		for(int col = 0; col < nCols; col++) {
			boardAfter[row][col] = boardBefore[row][col];
		}
	}
	ok1 = true;
	for(int row = 0; row < nRows; row++) {
		for(int col = 0; col < nCols; col++) {
			if (boardAfter[row][col] != boardBefore[row][col]) {
				ok1 = false;
			}
		}
	}
	results = ok1;
	return results;
}

/**
 * Tests a still life pattern
 * @return true if final board is the same as the initial board;
 * 		   false otherwise
 */
bool testPlayOne(void) {
	bool results = false;
	bool ok1 = false;

	int nRows = 4;
	int nCols = 3;
	char boardBefore[4][3] = {
			{'o','x','o'},
			{'x','o','x'},
			{'x','o','x'},
			{'o','x','o'}
	};
	char correctBoardAfter[4][3] = {
			{'o','x','o'},
			{'x','o','x'},
			{'x','o','x'},
			{'o','x','o'}
	};

	char boardAfter[nRows][nCols];
	//here's the invocation
	PlayOne(nRows, nCols, (char*)boardBefore, (char*)boardAfter);
	//here's the check
	ok1 = true; //no errors found yet
	for(int row = 0; row < nRows; row++) {
		for(int col = 0; col < nCols; col++) {
			if(boardAfter[row][col] != correctBoardAfter[row][col]) { //error found
				ok1 = false;
			}
		}
	}
	results = ok1;
	return results;
}

/**
 * Tests numRowsAndCols method with acorn.txt
 * @return true if number rows and columns calculated correctly;
 * 		   false otherwise
 */
bool testNumRowsAndCols(void) {
	bool results = false;
	bool ok1 = false;
	bool ok2 = false;
	bool ok3 = false;

	char filename[] = "HW2_tests/acorn.txt";
	FILE* fp = fopen(filename, "r"); //attempt to read file
	if(fp != false) {
		ok1 = true; //can read file
	}
	int numRows = 0; //number of rows in the file
	int numCols = 0; //number of columns in the file
	int* numRowsInFile = &numRows; //pointer to numRows
	int* numColsInFile = &numCols; //pointer to numCols

	numRowsAndCols(fp, numRowsInFile, numColsInFile); //invoking method

	if(*numRowsInFile == 3) {
		ok2 = true; //correct number of rows in file
	}
	if(*numColsInFile == 8) {
		ok3 = true; //correct number of columns in file
	}
	results = ok1 && ok2 && ok3;
	return results;
}

/**
 * Tests initializeArray method with acorn.txt
 * @return true if number rows and columns calculated correctly;
 * 		   false otherwise
 */
bool testInitializeArray(void) {
	bool results = false;
	bool ok1 = false;
	bool ok2 = true;

	int nRows = 4; //number of rows on the board
	int nCols = 9; //number of columns on the board
	int numRowsInFile = 3; //number of rows in the file
	int numColsInFile = 8; //number of columns in the file
	char A[nRows][nCols]; //initial game board
	char* Aptr = &A[0][0]; //pointer to the game board
	char filename[] = "HW2_tests/acorn.txt";
	FILE* f = fopen(filename, "r"); //attempt to read file
	if(f != false) {
		ok1 = true; //can read file
	}

	initializeArray(Aptr, filename, nRows, nCols, numRowsInFile, numColsInFile); //invoking method

	char correctBoard[4][9] = {
		{'o','o','x','o','o','o','o','o','o'},
		{'o','o','o','o','x','o','o','o','o'},
		{'o','x','x','o','o','x','x','x','o'},
		{'o','o','o','o','o','o','o','o','o'}
	};

	//compares correct board to board generated by code
	for(int row = 0; row < nRows; row++) {
		for(int col = 0; col < nCols; col++) {
			if(A[row][col] != correctBoard[row][col]) { //error found
				ok2 = false;
			}
		}
	}
	results = ok1 && ok2;
	return results;
}

/**
 * Tests PlayOne
 * @return true if one generation completed correctly;
 * 		   false otherwise
 */
bool testPlayOne2(void) {
	bool results = false;
	bool ok = true;

	int nRows = 6; //number of rows on the board
	int nCols = 11; //number of columns on the board
	char A[6][11] = {
		{'o','o','o','o','o','o','o','o','o','o','o'},
		{'o','o','o','o','x','x','x','o','o','o','o'},
		{'o','o','x','o','o','x','o','o','x','o','o'},
		{'o','x','o','o','o','o','o','o','o','x','o'},
		{'o','o','x','o','o','o','o','o','x','o','o'},
		{'o','o','o','o','o','o','o','o','o','o','o'}
	}; //initial game board
	char* Aptr = &A[0][0]; //pointer to initial game board
	char B[nRows][nCols]; //array where next generation will be stored
	char* Bptr = &B[0][0]; //pointer to next generation game board

	PlayOne(nRows, nCols, Aptr, Bptr); //invokes method

	char correctBoard[6][11] = {
		{'o','o','o','o','o','x','o','o','o','o','o'},
		{'o','o','o','o','x','x','x','o','o','o','o'},
		{'o','o','o','o','x','x','x','o','o','o','o'},
		{'o','x','x','o','o','o','o','o','x','x','o'},
		{'o','o','o','o','o','o','o','o','o','o','o'},
		{'o','o','o','o','o','o','o','o','o','o','o'}
	};

	//compares correct board to code-generated board
	for(int row = 0; row < nRows; row++) {
		for(int col = 0; col < nCols; col++) {
			if(B[row][col] != correctBoard[row][col]) { //error found
				ok = false;
			}
		}
	}
	results = ok;
	return results;
}

/**
 * Tests getLetter
 * @return true if gets correct letter;
 * 		   false otherwise
 */
bool testGetLetter(void) {
	bool results = false;
	bool ok1 = false;
	bool ok2 = false;

	int nCols = 5; //number of columns in game board

	char A[4][5] = {
		{'o','o','o','o','o'},
		{'o','x','o','o','x'},
		{'o','o','x','o','o'},
		{'o','x','o','o','o'}
	}; //initial game board
	char* Aptr = &A[0][0]; //pointer to the initial game board

	char x = getLetter(0, 0, nCols, Aptr); //invokes method
	char y = getLetter(2, 2, nCols, Aptr); //invokes method

	if(x == 'o') {
		ok1 = true; //correct char returned
	}
	if(y == 'x') {
		ok2 = true; //correct char returned
	}

	results = ok1 && ok2;
	return results;
}

/**
 * Tests howManyNeighbors
 * @return true if gets correct number of neighbors;
 * 		   false otherwise
 */
bool testHowManyNeighbors(void) {
	bool results = false;
	bool ok1 = false;
	bool ok2 = false;
	bool ok3 = false;
	bool ok4 = false;
	bool ok5 = false;
	bool ok6 = false;
	bool ok7 = false;
	bool ok8 = false;
	bool ok9 = false;

	int nRows = 4;
	int nCols = 5;

	char A[4][5] = {
		{'o','o','o','o','o'},
		{'o','x','o','o','x'},
		{'o','x','x','o','o'},
		{'o','x','x','o','o'}
	}; //initial game board
	char* Aptr = &A[0][0]; //pointer to initial game board

	//tests different cells in the game board above
	int b = HowManyNeighbors(0, 0, nRows, nCols, Aptr);
	int c = HowManyNeighbors(0, 2, nRows, nCols, Aptr);
	int d = HowManyNeighbors(0, 4, nRows, nCols, Aptr);
	int e = HowManyNeighbors(1, 1, nRows, nCols, Aptr);
	int f = HowManyNeighbors(1, 4, nRows, nCols, Aptr);
	int g = HowManyNeighbors(2, 0, nRows, nCols, Aptr);
	int h = HowManyNeighbors(2, 1, nRows, nCols, Aptr);
	int i = HowManyNeighbors(3, 0, nRows, nCols, Aptr);
	int j = HowManyNeighbors(3, 4, nRows, nCols, Aptr);

	//ok is true if correct number of neighbors returned
	if(b == 1) {
		ok1 = true;
	}
	if(c == 1) {
		ok2 = true;
	}
	if(d == 1) {
		ok3 = true;
	}
	if(e == 2) {
		ok4 = true;
	}
	if(f == 0) {
		ok5 = true;
	}
	if(g == 3) {
		ok6 = true;
	}
	if(h == 4) {
		ok7 = true;
	}
	if(i == 2) {
		ok8 = true;
	}
	if(j == 0) {
		ok9 = true;
	}

	results = ok1 && ok2 && ok3 && ok4 && ok5 && ok6 && ok7 && ok8 && ok9;
	return results;
}

/**
 * Tests generate, terminates with all dead
 * @return true if a generations are generated correctly and game stops properly;
 * 		   false otherwise
 */
bool testGenerate(void) {
	bool results = false;
	bool ok1 = false;
	bool ok2 = true;

	int gens = 10;
	int nRows = 4;
	int nCols = 5;
	char A[4][5] = {
		{'o','o','o','o','o'},
		{'o','x','o','o','x'},
		{'o','o','x','o','o'},
		{'o','x','o','o','o'}
	}; //initial game board
	char* Aptr = &A[0][0]; //pointer to initial game board
	char B[4][5] = {
		{'o','o','o','o','o'},
		{'o','o','o','o','o'},
		{'o','o','o','o','o'},
		{'o','o','o','o','o'}
	}; //holds other generations
	char* Bptr = &B[0][0];
	char C[4][5] = {
		{'o','o','o','o','o'},
		{'o','o','o','o','o'},
		{'o','o','o','o','o'},
		{'o','o','o','o','o'}
	}; //holds other generations
	char* Cptr = &C[0][0];
	char print = 'n';
	char pause = 'n';

	int genPlayed = generate(gens, nRows, nCols, Aptr, Bptr, Cptr, print, pause); //invokes method

	if(genPlayed == 2) {
		ok1 = true; //generations played is 2
	}

	char correctBoard[4][5] = {
		{'o','o','o','o','o'},
		{'o','o','o','o','o'},
		{'o','o','o','o','o'},
		{'o','o','o','o','o'}
	};

	//compares correct board to code-generated board
	for(int row = 0; row < nRows; row++) {
		for(int col = 0; col < nCols; col++) {
			if(C[row][col] != correctBoard[row][col]) { //error found
				ok2 = false;
			}
		}
	}
	results = ok1 && ok2;
	return results;
}

/**
 * Tests generate method with blinker, terminates with repetition
 * @return true if a generations are generated correctly and game stops properly;
 * 		   false otherwise
 */
bool testGenerate2(void) {
	bool results = false;
	bool ok1 = false;
	bool ok2 = true;
	bool ok3 = true;

	int gens = 10;
	int nRows = 3;
	int nCols = 3;
	char A[3][3] = {
		{'o','o','o'},
		{'x','x','x'},
		{'o','o','o'}
	}; //initial game board
	char* Aptr = &A[0][0]; //pointer to initial game board
	char B[3][3] = {
		{'o','o','o'},
		{'o','o','o'},
		{'o','o','o'}
	}; //holds other generations
	char* Bptr = &B[0][0];
	char C[3][3] = {
		{'o','o','o'},
		{'o','o','o'},
		{'o','o','o'}
	}; //holds other generations
	char* Cptr = &C[0][0];
	char print = 'n';
	char pause = 'n';

	int genPlayed = generate(gens, nRows, nCols, Aptr, Bptr, Cptr, print, pause); //invokes method

	if(genPlayed == 2) {
		ok1 = true; //generations played should be two
	}

	char correctBoard[3][3] = {
		{'o','o','o'},
		{'x','x','x'},
		{'o','o','o'}
	}; //two generations ago

	char correctBoardPrevGen[3][3] = {
		{'o','x','o'},
		{'o','x','o'},
		{'o','x','o'}
	}; //previous generation

	//compares correct board to code-generated board for two generations ago
	for(int row = 0; row < nRows; row++) {
		for(int col = 0; col < nCols; col++) {
			if(C[row][col] != correctBoard[row][col]) { //error found
				ok2 = false;
			}
		}
	}

	//compares correct board to code-generated board for previous generation
	for(int row = 0; row < nRows; row++) {
		for(int col = 0; col < nCols; col++) {
			if(B[row][col] != correctBoardPrevGen[row][col]) { //error found
				ok3 = false;
			}
		}
	}

	results = ok1 && ok2 && ok3;
	return results;
}

/**
 * Tests generate with acorn, terminates with max generations reached
 * @return true if a generations are generated correctly and game stops properly;
 * 		   false otherwise
 */
bool testGenerate3(void) {
	bool results = false;
	bool ok1 = false;
	bool ok2 = true;

	int gens = 5;
	int nRows = 3;
	int nCols = 8;
	char A[3][8] = {
		{'o','o','x','o','o','o','o','o'},
		{'o','o','o','o','x','o','o','o'},
		{'o','x','x','o','o','x','x','x'}
	}; //Initial game board
	char* Aptr = &A[0][0]; //pointer to initial game board
	char B[3][8] = {
		{'o','o','o','o','o','o','o','o'},
		{'o','o','o','o','o','o','o','o'},
		{'o','o','o','o','o','o','o','o'}
	}; //holds future generations
	char* Bptr = &B[0][0];
	char C[3][8] = {
		{'o','o','o','o','o','o','o','o'},
		{'o','o','o','o','o','o','o','o'},
		{'o','o','o','o','o','o','o','o'}
	}; //holds future generations
	char* Cptr = &C[0][0];
	char print = 'n';
	char pause = 'n';

	int genPlayed = generate(gens, nRows, nCols, Aptr, Bptr, Cptr, print, pause); //invokes method

	if(genPlayed == 5) {
		ok1 = true; //generations played should be five
	}

	char correctBoard[3][8] = {
		{'o','o','x','o','x','x','o','o'},
		{'o','o','x','o','o','o','o','o'},
		{'o','o','o','x','o','o','o','o'}
	};

	//compares correct board to code-generated board
	for(int row = 0; row < nRows; row++) {
		for(int col = 0; col < nCols; col++) {
			if(C[row][col] != correctBoard[row][col]) { //error found
				ok2 = false;
			}
		}
	}
	results = ok1 && ok2;
	return results;
}

/**
 * Tests anyX
 * @return true if correctly identifies if there are x's in array;
 * 		   false otherwise
 */
bool testAnyX(void) {
	bool results = false;
	bool ok1 = false;
	bool ok2 = false;

	int nRows = 4;
	int nCols = 5;

	char A[4][5] = {
		{'o','o','o','o','o'},
		{'o','x','o','o','x'},
		{'o','o','x','o','o'},
		{'o','x','o','o','o'}
	}; //game board with x's
	char* Aptr = &A[0][0];

	char B[4][5] = {
		{'o','o','o','o','o'},
		{'o','o','o','o','o'},
		{'o','o','o','o','o'},
		{'o','o','o','o','o'}
	}; //game board with no x's
	char* Bptr = &B[0][0];

	if(anyX(Aptr, nRows, nCols)) {
		ok1 = true; //detects that there are x's
	}
	if(!anyX(Bptr, nRows, nCols)) {
		ok2 = true; //detects that there aren't x's
	}

	results = ok1 && ok2;
	return results;
}

/**
 * Tests sameContent
 * @return true if correctly identifies if two arrays are identical in content;
 * 		   false otherwise
 */
bool testsSameContent(void) {
	bool results = false;
	bool ok1 = false;
	bool ok2 = false;

	int nRows = 4;
	int nCols = 5;

	char A[4][5] = {
		{'o','o','o','o','o'},
		{'o','x','o','o','x'},
		{'o','o','x','o','o'},
		{'o','x','o','o','o'}
	}; //game board
	char* Aptr = &A[0][0];

	char B[4][5] = {
		{'o','o','o','o','o'},
		{'o','x','o','o','x'},
		{'o','o','x','o','o'},
		{'o','x','o','o','o'}
	}; //same game board as A
	char* Bptr = &B[0][0];

	if(sameContent(Aptr, Bptr, nRows, nCols)) {
		ok1 = true; //detects that A and B are the same
	}

	char C[4][5] = {
		{'o','o','o','o','o'},
		{'o','x','o','o','x'},
		{'o','o','x','o','o'},
		{'o','x','o','o','o'}
	}; //game board
	char* Cptr = &C[0][0];

	char D[4][5] = {
		{'o','o','o','o','o'},
		{'o','x','o','o','x'},
		{'o','o','o','o','o'},
		{'o','x','o','o','o'}
	}; //different game board from C
	char* Dptr = &D[0][0];

	if(!sameContent(Cptr, Dptr, nRows, nCols)) {
		ok2 = true; //detects that C is different from D
	}

	results = ok1 && ok2;
	return results;
}
