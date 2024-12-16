
CC=$(CROSS_COMPILE)g++

all: writer

writer: writer.cpp Makefile
	$(CC) -c writer.cpp 
	$(CC) -o writer writer.o 

clean:
	rm -f *.o writer


	
