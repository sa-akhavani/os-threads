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


void row_parser(ifstream &my_file)
{
	string line;
	while(getline(my_file, line))
	{
		int arr[NUM_COLS];
		int idx = 0;		
		string word = "";
		for (int i = 0; i < line.length(); ++i)
		{
			if(line[i] == ' ')
			{
				arr[idx] = atoi(word.c_str());
				idx++;
				word = "";
			}
			else
			{
				word += line[i];
				if (i == line.length() - 1 && line[i] != ' ')
				{
					arr[idx] = atoi(word.c_str());
				}
			}
		}
		for (int i = 0; i < NUM_COLS; ++i)
		{
			cout << arr[i] << endl;
		}	
	}
}

void *readRow(void *arr)
{
	int *my_row;
	my_row = (int *) arr;
	for (int i = 0; i < NUM_COLS; ++i)
		cout << my_row[i] << endl;
}

int main () {
	ifstream A_file(A_MATRIX_PATH);
	row_parser(A_file);
	A_file.close();
	// pthread_t rowReaderThread;
	// int rc;
	// int i[NUM_COLS];
	// i[0] = 1;
	// i[1] = 0;
	// i[2] = 1;
	// i[3] = 1;
	// i[4] = 1;

	// rc = pthread_create(&rowReaderThread, NULL, readRow, (void *)i);
	// if (rc)
	// {
	// 	cout << "Error:unable to create thread," << rc << endl;
	// 	exit(-1);
	// }	
 //   pthread_exit(NULL);
}   