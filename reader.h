#ifndef READER_H
#define READER_H

#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <fstream>
#include <string>

using namespace std;

// Num of threads
#define NUM_THREADS 	4
// Matrix Size
#define NUM_ROWS		5
#define NUM_COLS		5
// Input Files
#define A_MATRIX_PATH	"A"
#define B_MATRIX_PATH	"B"

// Global Variables
int row[NUM_ROWS];
int col[NUM_COLS];
int calculed[NUM_COLS];

bool row_ready = false;
bool col_ready = false;

// Thread Functions
void *readRow(void *param);
void *readCol(void *param);
void *calcMatrix(void *param);
void *printMatrix(void *param);

// Functions
void row_parser(string line);
void col_parser(string line, int i, int j);
int calc_entry()
#endif