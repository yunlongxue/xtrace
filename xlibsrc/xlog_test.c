#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include  <libgen.h>
#include "xlog.h"


void Usage(char *progname)
{
	/*
        fprintf(stderr, "Usage: %s  -list | -start | -stop\n", basename(progname));
	fprintf(stderr, "           [-i dbsid] [-u dbuser] [-p dbpaswd] [-n pnum] [-m pmax] [-c cpu_idle] [-w iowait] [-I interal]\n");
	fprintf(stderr, "    -list         list service detail information;\n");
	fprintf(stderr, "    -start        startup trans_service;\n");
	fprintf(stderr, "    -stop         shutdown trans_service;\n");
        fprintf(stderr, "    -i dbsid      specify the SID to access oracle;\n");
        fprintf(stderr, "    -u dbuser     specify the username to access oracle;\n");
        fprintf(stderr, "    -p dbpasswd   specify the password to access oracle;\n");
        fprintf(stderr, "    -n pnum       specify the num of processing process instance, default is 5;\n");
        fprintf(stderr, "    -m pmax       specify the the upper processes of OS, default is 500;\n");
        fprintf(stderr, "    -c cpu_ilde   specify the the low percent of CPU idle, default is 50;\n");
        fprintf(stderr, "    -w iowait     specify the the low percent of CPU iowait, default is 20;\n");
        fprintf(stderr, "    -I interal    specify the the interal of resource collecting, default is 10;\n");
        fprintf(stderr, "\n\n");
	*/
        fprintf(stderr, "Usage: %s  [-l logfile] [-d (trace | debug | info | warning | error | critical)]\n", basename(progname));
        fprintf(stderr, "    -l logfile	   Specify the logfile, default is NULL;\n");
        fprintf(stderr, "    -d * 	   Specify the log level, default is INFO;\n");
}


int main( int argc, char **argv )
{
	extern int	optind;
	extern char	*optarg;
	int	rt = 0;
	char	logfile[1024], *plogfile = NULL;
	char	logLevel[20];


	while ((rt = getopt(argc, argv, "i:u:p:n:m:c:w:l:I:s:t:h:d:")) != EOF)
	{
		switch(rt)
		{

		case 'l':
			strncpy( logfile, optarg, sizeof(logfile) );
			plogfile = logfile;
			break;

		case 'd':
			strcpy(  logLevel, optarg );
			break;
		default:
			Usage(argv[0]);
			exit(94);
		}
	}

	if( plogfile && strlen(plogfile) < 1 ) {
		Usage(argv[0]); 
		fprintf(stderr, "logfile must be specified!\n\n");
		exit(94);
	}
	xlog_init( "xlog_test", "debug" );
//	xloginit( logfile, logLevel );
	xlog( "log: %s\n", xlog_path() );

	errno = EIO;
	xperror( "perror" );
	xerror( "error \n" );
	xwarn( "warn \n" );
	xlog( "info \n" );
	xdebug( "debug \n" );
	xpdebug( "debug2" );

	return 0;
}
