
CFLAGS += -g -Wall -Werror

CC=$(CROSS_COMPILE)gcc

all: aesdsocket

writer: aesdsocket.cpp Makefile
	$(CC) $(CFLAGS) aesdsocket.c -o aesdsocket $(LDFLAGS) 

clean:
	rm -f aesdsocket


	
