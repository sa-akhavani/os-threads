CC=g++

all: reader.out

reader.out: reader.o
	$(CC) reader.o -lpthread -o reader.out
reader.o: reader.cpp reader.h
	$(CC) -c reader.cpp -lpthread

clean:
	rm *.o reader.out