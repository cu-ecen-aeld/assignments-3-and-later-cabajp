#include <iostream>
#include <fstream>
#include <stdio.h>
#include <unistd.h>
#include <syslog.h>

using namespace std;
 
int main(int argc, char** argv )
{
    openlog(argv[0], 0, LOG_USER);	

    if ( argc != 3 )
    {
	syslog(LOG_ERR, "wrong number of input arguments");	
        closelog();	
	return 1;
    }
 	
    try 
    {
	ofstream file;
	file.open (argv[1]);
    	file << argv[2];
    	file.close();
    	syslog(LOG_DEBUG, "Writing %s to %s", argv[2], argv[1]);	
    }
    catch (...)
    {
    	syslog(LOG_ERR, "File %s, could not be written", argv[1]);	
        closelog();	
	return 1;
    }  	
 
    closelog();	
    return 0;
}
