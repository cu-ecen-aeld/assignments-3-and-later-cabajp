#include <stdio.h>
#include <unistd.h>
#include <syslog.h>

using namespace std;
 
int main(int argc, char** argv )
{
    openlog("writer", 0, LOG_USER);	

    if ( argc != 3 )
    {
	syslog(LOG_ERR, "wrong number of input arguments");	
        closelog();	
	return 1;
    }


#if 0 	
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
#endif 
    
    FILE* outfile;
    
    outfile = fopen(argv[1], "w+");
    if (outfile == NULL) {
        syslog(LOG_ERR, "File %s, could not be written", argv[1]);
        closelog();
        return 1;
    }	
    
    fprintf(outfile, "%s",argv[2]);
    syslog(LOG_DEBUG, "Writing %s to %s", argv[2], argv[1]);	
 
    fclose(outfile);	
    closelog();	
    return 0;
}
