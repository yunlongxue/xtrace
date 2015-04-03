/* 
 * xueyunlong@gmail.com
 * 2011-06-15
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include  <libgen.h>
#include  <string>
#include "xlog.h"
#include "xclassFile.h"


void Usage(char *progname)
{
        fprintf(stderr, "Usage: %s  [-f file] [-d (trace | debug | info | warning | error | critical)]\n", basename(progname));
        fprintf(stderr, "    -l file	   Specify the file to be tested;\n");
        fprintf(stderr, "    -d * 	   Specify the log level, default is DEBUG;\n");
}


int main( int argc, char **argv )
{
	extern int	optind;
	extern char	*optarg;
	int	rt = 0;
	int	initloglevel = XLOG_DEBUG;
	char	logfile[1024], *plogfile = NULL;
	char	file[1024];


	while ((rt = getopt(argc, argv, "f:d:")) != EOF)
	{
		switch(rt)
		{
		case 'f':
			strncpy( file, optarg, sizeof(file) );
			break;

		case 'd':
			if ( !strcasecmp(optarg, "TRACE") )
				initloglevel = XLOG_TRACE;
			else if ( !strcasecmp(optarg, "DEBUG") )
				initloglevel = XLOG_DEBUG;
			else if ( !strcasecmp(optarg, "INFO") )
				initloglevel = XLOG_INFO;
			else if ( !strcasecmp(optarg, "WARN") )
				initloglevel = XLOG_WARN;
			else if ( !strcasecmp(optarg, "ERROR") )
				initloglevel = XLOG_ERROR;
			else {
				Usage(argv[0]); 
				fprintf(stderr, "Invalid option -%c%s\n\n", rt, optarg);
				exit(94);
			}
			break;
		default:
			Usage(argv[0]);
			exit(94);
		}
	}

	xloginit( plogfile, initloglevel );

	char	cpfile[1024];
	FILE *cpfp = NULL;
	xclassFile cfile;
	string str;
	str = file;
	xdebug( "file:[%s]\n", file );
	cfile.read_file( str );

	xlog( "\n\n-------------------------- content ------------------\n" );
	printf( "copy to new file $home/tmp/a \n" );
	sprintf( cpfile, "%s/tmp/a", getenv("HOME") );
	cpfp = fopen( cpfile, "w" );
	if ( cpfp == NULL )
	{
		xperror( "open error:%s", cpfile );
		return -1;
	}
	fwrite( cfile.content.c_str(), cfile.content.length(), 1, cpfp );
        fclose( cpfp ); cpfp = NULL;	
	xlog( "\n-------------------------- content ------------------\n" );

	xlog( "\n\n-------------------------- lines ------------------\n" );
	for ( int i = 0; i < cfile.lines.size(); i++ )
	{
		printf( "%d:%s", i+1, cfile.lines[i].c_str() );
	}
	xlog( "\n-------------------------- lines ------------------\n" );

	return 0;
}
