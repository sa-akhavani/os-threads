#include "reader.h"

int main () {
	pthread_t rowReaderThread, colReaderThread, calcThread, printThread;
	int rc[NUM_THREADS];
	int i = 0;

	// Semaphoes ^__^
	// sem_init(&row_sem, 0, 1);
	// sem_init(&col_sem, 0, 1);
	// sem_init(&calc_sem, 0, 10);

	// sem_wait(&row_sem);	


	// Creating Threads
	rc[0] = pthread_create(&rowReaderThread, NULL, readRow, (void*)i);
	rc[1] = pthread_create(&colReaderThread, NULL, readCol, (void*)i);
	rc[2] = pthread_create(&calcThread, NULL, calcMatrix, (void*)i);
	rc[3] = pthread_create(&printThread, NULL, printMatrix, (void*)i);
	if (rc[0] || rc[1] || rc[2] || rc[3])
	{
		cout << "Error:unable to create thread!" << endl;
		exit(-1);
	}

	pthread_exit(NULL);
	return 0;
}   

void *readRow(void* param)
{
	cout << "readRow\n";
	ifstream my_file(A_MATRIX_PATH);
	string line;

	if (my_file.is_open())
	{
		for (int i = 0; i < NUM_ROWS; ++i)
		{
			// sem_wait(&row_sem);
			if (!getline(my_file, line))
				break;
			row_parser(line);
			row_ready = true;
		}
		my_file.close();
	}
	
	cout << "readRow Ended!\n";
	pthread_exit(NULL);
}

void row_parser(string line)
{
	int idx = 0;		
	string word = "";
	for (int i = 0; i < line.length(); ++i)
	{
		if(line[i] == ' ')
		{
			row[idx] = atoi(word.c_str());
			idx++;
			word = "";
		}
		else
		{
			word += line[i];
			if (i == line.length() - 1 && line[i] != ' ')
			{
				row[idx] = atoi(word.c_str());
			}
		}
	}
}


void *readCol(void *param)
{
	cout << "readCol\n";
	ifstream my_file(B_MATRIX_PATH);
	string line;

	if (my_file.is_open())
	{
		for (int i = 0; i < NUM_COLS; ++i)
		{
			my_file.seekg(0);
			for (int j = 0; j < NUM_ROWS; ++j)
			{
				getline(my_file, line);
				col_parser(line, j, i);
			}
			col_ready = true;
		}
		my_file.close();
	}
	
	cout << "readCol Ended!\n";
	pthread_exit(NULL);
}

void col_parser(string line, int r, int c)
{
	int idx = 0;		
	string word = "";
	for (int i = 0; i < line.length(); ++i)
	{
		if(line[i] == ' ' && i != 0)
		{
			if (idx == c)
			{
				col[r] = atoi(word.c_str());
				break;
			}

			idx++;
			word = "";
		}
		else if (line[i] == ' ' && i == 0)
			continue;
		else
		{
			word += line[i];
			if (i == line.length() - 1 && line[i] != ' ')
			{
				if (idx == c)
					col[r] = atoi(word.c_str());
			}
		}
	}
}


void *calcMatrix(void *param)
{
	cout << "calcMatrix\n";
	int value;
	for (int i = 0; i < NUM_COLS*NUM_ROWS; ++i)
	{
		if(!col_ready || !row_ready)
			continue;
		value = calc_entry();
	}

	cout << "calcMatrix Ended!\n";
	pthread_exit(NULL);
}

int calc_entry()
{
	int sum = 0;
	for (int i = 0; i < NUM_COLS; ++i)
		sum += row[i] * col[i];
	return sum;
}

void *printMatrix(void *param)
{

}