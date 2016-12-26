#include <iostream>
#include <cstdlib>
#include <pthread.h>

using namespace std;

// Num of threads
#define NUM_THREADS 	4
// Matrix Size
#define NUM_ROWS		5
#define NUM_COLS		5

void *readRow(void *arr)
{
	int *my_row;
	my_row = (int *) arr;
	for (int i = 0; i < NUM_COLS; ++i)
		cout << my_row[i] << endl;
}

int main () {
	pthread_t rowReaderThread;
	int rc;
	int i[NUM_COLS];
	i[0] = 1;
	i[1] = 0;
	i[2] = 1;
	i[3] = 1;
	i[4] = 1;

	rc = pthread_create(&rowReaderThread, NULL, readRow, (void *)i);
	if (rc)
	{
		cout << "Error:unable to create thread," << rc << endl;
		exit(-1);
	}	
   pthread_exit(NULL);
}   