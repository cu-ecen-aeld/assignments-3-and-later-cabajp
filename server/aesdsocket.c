#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <limits.h>

/* defines and globals */

#define FILE_NAME "/var/tmp/aesdsocketdata"
#define BUFFER_SIZE 20000

void createServer(void); 
void waitForData(void);
int sendfile(int socket);
static void signal_handler (int signo);

char *buffer; 
FILE* outfile;
int sockfd; 
struct addrinfo hints, *res;

int main(int argc, char** argv )
{
    pid_t p;	
    int daemon = 0;	
    int i;
    
    printf(">>> starting with arg %d\n", argc); 
    
    for(i=0; i<argc; i++)
    {
    	printf(">>> starting with arg %s\n", argv[i]); 
    } 
    
  
    if ( argc == 2 && !strcmp(argv[1], "-d") )
    {
        printf("starting with  %s\n", argv[1]); 
        daemon = 1;
    }	


    /* open file for output */	    
    outfile = fopen(FILE_NAME, "w+");
    if (outfile == NULL) 
    {
        syslog(LOG_ERR, "File %s, could not be opened", FILE_NAME);
        closelog();
        exit (EXIT_FAILURE);
    }	


    openlog("aesdsocket", 0, LOG_USER);	
 
    /* register handler for SIGINT or SIGTERM */
    if (signal (SIGINT, signal_handler) == SIG_ERR) 
    {
        syslog(LOG_ERR, "Cannot handle SIGINT!\n"); 
        exit (EXIT_FAILURE);
    }
    
    if (signal (SIGTERM, signal_handler) == SIG_ERR) 
    {
        syslog(LOG_ERR, "Cannot handle SIGINT!\n"); 
        exit (EXIT_FAILURE);
    }

    buffer = malloc(BUFFER_SIZE);	
    if(buffer == NULL)
    {
        syslog(LOG_ERR, "Cannot allocate buffer\n"); 
        exit (EXIT_FAILURE);
    }	

    createServer();

    // ----------------------------------------------
    
    if(daemon)
    {
	p = fork();
        if(p<0)
        {
            syslog(LOG_ERR, "Cannot fork\n"); 
            exit (EXIT_FAILURE);
        }
        else if ( p == 0)
	    waitForData();	
    }
    else
	waitForData();
    
    return 0;
}

/* handler for SIGINT */
void signal_handler (int signo)
{
    /*
    * Technically, you shouldn't use printf() in a
    * signal handler, but it isn't the end of the
    * world. I'll discuss why in the section
    * "Reentrancy."
    */
    
    printf("Caught signal, exiting!\n");
    syslog(LOG_INFO,"Caught signal, exiting!\n");
    shutdown(sockfd, SHUT_WR);
    close(sockfd);
    fclose(outfile);	
    free(buffer);
    closelog();	
    exit (EXIT_SUCCESS);
}

void createServer(void)
{
	// first, load up address structs with getaddrinfo():
    	memset(&hints, 0, sizeof hints);
    	hints.ai_family = AF_UNSPEC;  // use IPv4 or IPv6, whichever
    	hints.ai_socktype = SOCK_STREAM;
    	hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

    	getaddrinfo(NULL, "9000", &hints, &res);

    	// make a socket:
    	sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    	
    	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0)
            printf("setsockopt(SO_REUSEADDR) failed\n");
    	
     	// bind it to the port we passed in to getaddrinfo():
        if(bind(sockfd, res->ai_addr, res->ai_addrlen) == -1)
        {
		printf("Bind Err\n");	
        }
        
    	if(listen(sockfd, 1) == -1)
    	{
    		printf("Listen Err\n");	
    	}
    	
        printf("listening\n");	
        
        freeaddrinfo(res);
}

void waitForData(void)
{
    struct sockaddr_storage their_addr;
    socklen_t addr_size;
    int new_fd;		

    while(1)
    {
   	
    	// now accept an incoming connection:
    	addr_size = sizeof their_addr;
    	new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);

    	// convert the IP to a string and print it:
    	// inet_ntop(their_addr->ai_family, addr, ipstr, sizeof ipstr);
    	//  syslog(LOG_INFO, "Accepted connection from %s\n", ipstr);	
    	printf("accepted\n");	
    	syslog(LOG_INFO, "Accepted connection\n");	

    	int received = 0;
    	
    	do 
  	{
	    received += recv(new_fd, &buffer[received], BUFFER_SIZE - received, 0);	  		  	
  	} while(buffer[received-1] != '\n');	
    	
    	printf("%d\n",received);
    	printf(">>> %s",buffer);	
    	fputs (buffer,outfile);
    
    	sendfile(new_fd);
    	
    	close(new_fd);
     }
}

int sendfile(int socket)
{
    // possition to start    	
    rewind(outfile);
    
    // read line by line
    while(fgets (buffer, BUFFER_SIZE, outfile))
    {
        printf("<<< %s",buffer);	
        send(socket, buffer, strlen(buffer), 0);
    }
    
    return 0;
}

