
CXX=$(CROSS_COMPILE)g++

all: writer

writer: writer.cpp Makefile
	$(CXX) -c writer.cpp 
	$(CXX) -o writer writer.o 

clean:
	rm -f *.o writer


	
