/*
 * production.h
 *
 *  Created on: Jan 23, 2019
 *      Author: Derek Ng
 */

#ifndef PRODUCTION_H_
#define PRODUCTION_H_
#include <stdio.h>
bool production(int argc, char* argv[]);
void numRowsAndCols(FILE* fp, int* numRowsInFile, int* numColsInFile);
void initializeArray(char* A, char* filename, int nRows, int nCols, int numRowsInFile, int numColsInFile);
void PlayOne (unsigned int nr, unsigned int nc, char* Old, char* New);
char getLetter(int row, int col, int nCols, char* Old);
int HowManyNeighbors(int row, int col, int nRows, int nCols, char* Old);
int generate(int gens,  int nRows,  int nCols,  char* old_p, char* new_p, char* other_p, char print, char pause);
bool anyX(char* arr, int nRows, int nCols);
bool sameContent(char* one_p, char* another_p, int nRows, int nCols);
void printThis(int nRows, int nCols, char* old_p);

#endif /* PRODUCTION_H_ */
