/*******************************************************************
 * xueyunlong@gmail.com
 * Hisory:
 * 2011-06-28 V0.1  xueyunlong@gmail.com
 * 2011-10-10 V0.2  xueyunlong@gmail.com
 * 2013-02-10 V0.3  xueyunlong@gmail.com
 * 2014-06-10 V0.4  xueyunlong@gmail.com
 * 2014-08-01 V1.0  xueyunlong@gmail.com
 * *****************************************************************/
#define APP_V1

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "xlog.h"
#include "xgreen.h"
#include "xfoot/xfoot.h"
#include "xtrace.h"

#include <string>
using namespace std;



#if defined( APP_V1 )

const char *helpMessage = 
R"(  
Usage: xtrace <-o|-c|-h> src.cpp

Example:
    xtrace -o src.cpp
        Open xtrace for src.cpp. 
        Then Compile the src.cpp and run to show the xtrace information.
    xtrace -c src.cpp
        Close xtrace for src.cpp.

XTRACE OUTPUT FORMAT:                                                     
    XTRACE | $pid | $location   | $run_count| $total_time| $foot_time 
    Example: 
    XTRACE |11287 |src.cpp-116-main|2|5s|0s-0us| 
        XTRACE:    The xtrace information flag.
        11287:     The process id, 
        src.cpp-116-main: The program's running  location
            The program runing location, it's now run the file of src.cpp,
            the line is 116, the function is main.
        2:  The location's executed time.   
            for example: in a loop statement, first run is 1,second run is 2;
        5s: The total time used since the program started. rounds the time
            to seconds.
        1s-2us:  The time used since last xtrace; 
            the time is 1 second and 2 usecond.
            rounds the time to usecond. 1 second = 1000000 useconds

Reference:
	https://github.com/yunlongxue/xtrace
    
)";

#else


const char *helpMessage = 
R"(  
Usage: xtrace <-o|-c|-l|-h> src.cpp

Example:
    xtrace -l src.cpp 
        List Functions in the src.cpp
    xtrace -o src.cpp
        Open xtrace for src.cpp. 
        Then Compile the src.cpp and run to show the xtrace information.
    xtrace -c src.cpp
        Close xtrace for src.cpp.

XTRACE OUTPUT FORMAT:                                                     
    XTRACE | $pid | $location   | $run_count| $total_time| $foot_time 
    Example: 
    XTRACE |11287 |src.cpp-116-main|2|5s|0s-0us| 
        XTRACE:    The xtrace information flag.
        11287:     The process id, 
        src.cpp-116-main: The program's running  location
            The program runing location, it's now run the file of src.cpp,
            the line is 116, the function is main.
        2:  The location's executed time.   
            for example, in a loop statement, first run is 1, 
            second run is 2;
        5s: The total time used since the program started. rounds the time
            to seconds.
        1s-2us:  The time used since last xtrace; 
            the time is 1 second and 2 usecond.
            rounds the time to usecond. 1 second = 1000000 useconds

Reference:
	https://github.com/yunlongxue/xtrace
    
)";


#endif


void usage(char *progname)
{
	printf( "%s", helpMessage );
}

int main( int argc, char **argv )
{
	int		rt = 0;
	char	*fpath;
	string	cmd;	
	xtrace	xt;

	if ( argc != 2 && argc != 3 ) {
		xerror( "Arguments error\n" );
		usage( argv[0] );
		return -1;
	}
	
	cmd = argv[1];
	if ( cmd == "-h" ) {
		usage( argv[0] );
		return 0;		
	}

	struct stat stbuf;
	fpath = argv[2];
	if ( fpath == NULL )  {
		xerror( "Miss input file \n" );
		return -1;
	}
	if ( lstat( fpath, &stbuf ) == -1 )  {
			xperror( "Failed:%s\n", fpath );
			return -1;
	}

	if ( cmd == "-l" ) {
		if (!( S_ISREG( stbuf.st_mode ) 
			||	S_ISLNK( stbuf.st_mode )
			 ))  {
			xerror( "Not a file:%s\n", fpath );
			return -1;
		}
		if (!( ( stbuf.st_mode & S_IRUSR )  
			))  {
			xerror( "Not readable file:%s\n", fpath );
			return -1;
		}
	}
	if ( cmd == "-o" || cmd == "-c" ) {
		if (!( S_ISREG( stbuf.st_mode ) 
			 ))  {
			xerror( "Not a regular file:%s\n", fpath );
			return -1;
		}

		if (!( ( stbuf.st_mode & S_IRUSR )  
			   && ( stbuf.st_mode & S_IWUSR )  
			))  {
			xerror( "Permission failed file:%s\n", fpath );
			return -1;
		}
	}

	char	*tmpdir = NULL;
	if ( tmpdir == NULL )  do{
		tmpdir = getenv("TMP");

		if ( tmpdir == NULL )
			break;
		if ( stat( tmpdir, &stbuf ) == -1 )  {
			tmpdir = NULL;
			break;
		}
		if (!( S_ISDIR( stbuf.st_mode ) 
			 ))  {
			tmpdir = NULL;
			break;
		}
	} while( 0 );

	if ( tmpdir == NULL )  do{
		tmpdir = getenv("TEMP");

		if ( tmpdir == NULL )
			break;
		if ( stat( tmpdir, &stbuf ) == -1 )  {
			tmpdir = NULL;
			break;
		}
		if (!( S_ISDIR( stbuf.st_mode ) 
			 ))  {
			tmpdir = NULL;
			break;
		}
	}while( 0 );

	if ( tmpdir == NULL )  {
		#if defined( OS_LINUX )
			tmpdir = "/tmp";
		#elif defined( OS_WINDOWS )
			xerror( "environment variable TEMP not set.\n" );
			return -1;
		#else
			#error  NO default tmp dir.
		#endif
	}
	setTmpdir( tmpdir );
	xdebug( "tmp dir: %s\n", tmpdir );

	if ( cmd == "-o" ) {
		xlog( "Open xtrace for: %s \n", argv[2] );
		rt = xt.open_xtrace( argv[2] );
		if ( rt != 0 )  {
			xerror( "Failed:%d\n", rt );
		}
		return rt;
	} else if ( cmd == "-c" ) {
		xlog( "Close xtrace for: %s\n", argv[2] );
		return xt.close_xtrace( argv[2] );
	} else if ( cmd == "-l" ) {
		return xt.listFunctions( argv[2] );
	} else {
		xerror( "Arguments error\n" );
		usage( argv[0] );
		return -1;
	}

	xlog( "Done.\n" );
	return 0;
}

