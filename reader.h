#ifndef READER_H
#define READER_H

#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <fstream>
#include <string>
#include <semaphore.h>

using namespace std;

// Num of threads
#define NUM_THREADS 	4
// Matrix Size
#define NUM_ROWS		5
#define NUM_COLS		5
// Input Files
#define A_MATRIX_PATH	"matrixA"
#define B_MATRIX_PATH	"matrixB"
#define C_MATRIX_PATH	"matrixC"
// Write and Calc Buffer
#define BUFF_SIZE		10


// Global Variables
int row[NUM_ROWS];
int col[NUM_COLS];
int calculated[BUFF_SIZE];
int calc_start_idx = 0;
int calc_end_idx = 9;
int out[NUM_COLS];

bool row_ready = false;
bool col_ready = false;


// Semaphores
sem_t row_semaphore, col_semaphore, calc_semaphore;

// Thread Functions
void *readRow(void *param);
void *readCol(void *param);
void *calcMatrix(void *param);
void *printMatrix(void *param);

// Functions
void row_parser(string line);
void col_parser(string line, int i, int j);
int calc_entry();
bool is_sth_calculated(int calc_start_idx, int calc_end_idx);
#endif