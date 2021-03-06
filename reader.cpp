#include "reader.h"

int main ()
{
	pthread_t rowReaderThread, colReaderThread, calcThread, printThread;
	int rc[NUM_THREADS];
	int i = 0;

	// Semaphoes ^__^
	sem_init(&row_semaphore, 0, 1);
	sem_init(&col_semaphore, 0, 1);
	sem_init(&calc_semaphore, 0, BUFF_SIZE);

	sem_wait(&row_semaphore);

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
	
	//	Making the main func to wait for the threads to end 
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
			sem_wait(&row_semaphore);
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
		for (int r = 0; r < NUM_ROWS; ++r)
		{
			sem_wait(&col_semaphore);
			row_ready = false;
			sem_post(&row_semaphore);	
			for (int i = 0; i < NUM_COLS; i++)
			{
		  		if(i != 0)
		  		{
					sem_wait(&col_semaphore);
		  		}
	  			my_file.seekg(0);

				for (int j = 0; j < NUM_ROWS; ++j)
				{
					if (!getline(my_file, line))
					{
						cout << "error\n";
						pthread_exit(NULL);
					}
					col_parser(line, j, i);
				}
				col_ready = true;
			}
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
				// cout << "col[" << r << "]: " << atoi(word.c_str()) << endl;
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
				{
					// cout << "col[" << r << "]: " << atoi(word.c_str()) << endl;
					col[r] = atoi(word.c_str());
				}
			}
		}
	}
}


void *calcMatrix(void *param)
{
	cout << "calcMatrix\n";
	int value;
	for (int i = 0; i < NUM_COLS*NUM_ROWS; i)
	{	
		if(!col_ready || !row_ready)
			continue;

		value = 0;
		for (int j = 0; j < NUM_COLS; ++j)
			value += row[j] * col[j];

		col_ready = false;
		sem_post(&col_semaphore);
		sem_wait(&calc_semaphore);
		
		calculated[calc_start_idx % 10] = value;
		calc_start_idx++;
		i++;
		// sem_post(&calc_semaphore);
	}
	cout << "calcMatrix Ended!\n";
	pthread_exit(NULL);
}

void *printMatrix(void *param)
{
	cout << "printMatrix\n";
	ofstream my_file(C_MATRIX_PATH);
	int idx = 0;
	for (int i = 0; i < NUM_ROWS; i)
	{
		// cout << "i: " << i << endl;
		if (idx == NUM_COLS)
		{
			for (int j = 0; j < NUM_COLS; ++j)
				my_file << out[j] << ' ';
			my_file << endl;
			idx = 0;
			i++;
		}
		if(is_sth_calculated(calc_start_idx, calc_end_idx))
		{
			// cout << "gg\n";
			out[idx] = calculated[(calc_end_idx+1) % 10];
			calc_end_idx++;
			idx++;
			sem_post(&calc_semaphore);
		}
	}
	my_file.close();
	cout << "printMatrix Ended!\n";
	pthread_exit(NULL);	
}

bool is_sth_calculated(int calc_start_idx, int calc_end_idx)
{
	if (calc_end_idx - calc_start_idx != 9)
		return true;
	return false;
}