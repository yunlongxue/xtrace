/* --------------------------- 2003-10-20 -------------------------------- 
 * FUNCTION: Log utils.
 * Author: XUEYL xueyunlong@gmail.com
 * Create Date: 2003-10-20
 * Version history:
 * 	V1.0: 2003-10-20
 * 	V2.0: 2006-09-01 Use c99 instead of marco.
 * 	V3.0: 2008-10-20 Add level control.
 * 	V3.1: 2012-11-21 Add delete xtrace functionn
 ------------------------------------------------------------------------- */

#include "xlog.h"

static FILE *_xlog_fp = NULL; /* log file point */
static char _xlog_path[512];  /* log file path  */

#if defined(DEBUG)
static unsigned int _xlog_level = XLOG_DEBUG; /* initial xlog level to level */
#else
static unsigned int _xlog_level = XLOG_INFO; /* initial xlog level to level */
#endif

static char _xlog_level_str[][80] =  {
	"DEBUG",
	"DEBUG",
	"INFO",
	"INFO",
	"WARN",
	"WARN",
	"ERROR",
	"ERROR",
};



/* --------------------------- 2008-12-10 --------------------------------                                                 
 * make_dir: Make directory recursvely.     
 *   fullpath: The full path of dir which will be recursivly make. example:                                                
 *      /home/d1/d2/ -- Make d1 and d2 directory.                                                                          
 *      /home/d1/d2 -- Make d1 directory, MakeDir treat d2 as a file name.                                                 
 * Note: the directory created would be #DIR_MODE                                                                          
 ------------------------------------------------------------------------- */                                              
static int make_dir( const char *fullpath )
{                                                                                                                          
    char    *p, buf[1024];                                                                                            
                                                                                                                           
    if ( !fullpath )  {                                                                                                    
        fprintf( stderr, "path can't be void. The path is void*" );
        return -1;                                                                                                         
    }                                                                                                                      
    *buf = 0x00;
    strncpy( buf, fullpath, 1024 );                                                                                   
    buf[1024-1] = 0x00;                                                                                              
                                                                                                                           
    p = buf + 1;                                                                                                           
    while ( (p = strchr(p, '/')) != NULL ) {                                                                               
        *p = 0x00;                                                                                                         
        if ( mkdir(buf, S_IXUSR | S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IXGRP ) != 0 && errno != EEXIST ) {
            fprintf( stderr, "Make dir failed: %s \n", buf );
			perror( "" );
            return -1;                                                                                                     
        }                                                                                                                  
        *p++ = '/';                                                                                                        
    }                                                                                                                      
                                                                                                                           
    return 0;                                                                                                              
} 





/* --------------------------- 2008-12-10 -------------------------------- 
 * xloginit: Rnitialize log enviroment.
 * logfile: Log destination ,xloginit set log file as $HOME/$yyyymmdd/$logfile. 
 * 	logfile could be NULL or NOT NULL.
 * 	If logfile is NULL, xlog print log to screen.
 * 	If logfile is NOT NULL, xlog print log to screen and $logfile.
 * loglevelinit: Determine which log should be log.
 * 	In xlog function, if loglevel >= loglevelinit, then it will be loged, otherwise the message will be ignored. 
 ------------------------------------------------------------------------- */
int xlog_init( const char *logfile, const char *level )
{
	char	buf[4096];
	time_t	clock;
	struct tm *ptm;

#if 0
	 printf ( " 1 level:%d\n", level == NULL   );
	if (  !level )
		 {
			 printf ( "level:%s\n", level );
				 return 0;
		 }
	 printf ( " 2 level:%s\n", level );
#endif

	if ( level == NULL )
		 _xlog_level = XLOG_INFO; 
	else if ( !strcasecmp( level, "DEBUG" )  )
		_xlog_level = XLOG_DEBUG;
	else if ( !strcasecmp( level, "INFO" )  )
		_xlog_level = XLOG_INFO;
	else if ( !strcasecmp( level, "WARN" )  )
		_xlog_level = XLOG_WARN;
	else if ( !strcasecmp( level, "ERROR" )  )
		_xlog_level = XLOG_ERROR;
	else  {
		fprintf ( stderr, "Set log level to 'INFO' as default" );
		_xlog_level = XLOG_INFO;
	}

	if( logfile == NULL )
		return 0;

	time( &clock );
	ptm = localtime( &clock );
	snprintf( buf, sizeof(buf), "%s/log/%04d%02d%02d/",
		getenv("HOME"), ptm->tm_year+1900, ptm->tm_mon+1, ptm->tm_mday );
	make_dir( buf );
	strcat( buf, logfile );
	_xlog_fp = fopen( buf, "a" );
	if( !_xlog_fp )  {
		printf( "Open log[%s] failed: %s \n", buf, strerror(errno) );
		return 0;
	}
	fflush( _xlog_fp );
	*_xlog_path = 0x00;
	strcpy( _xlog_path, buf );

	return 0;
}

const char* xlog_path()
{
	return _xlog_fp == NULL ? NULL : _xlog_path;
}

int xlog_uninit()
{
	if ( _xlog_fp != NULL )
		fclose( _xlog_fp );
	_xlog_fp = NULL;
	return 0;
}

void _xlog( const char *srcfile, const int srcline, const char *srcfunc, 
	const int level, const char *fmt, ... )
{
	static va_list	ap;
	static char	buf[4096];
	static int	errno_save = 0;

	errno_save = errno;
	if( level < _xlog_level )
		return;

	*buf = 0x00;

#ifdef DEBUG
	static time_t	clock;
	static struct tm *ptm;
	time( &clock );
	ptm = localtime( &clock );

	if( _xlog_level <= XLOG_PDEBUG )
		snprintf( buf, sizeof(buf), "|%04d%02d%02d %02d:%02d:%02d|%d|%s|%s-%d-%s| ",
			ptm->tm_year+1900, ptm->tm_mon + 1, ptm->tm_mday,
			ptm->tm_hour, ptm->tm_min, ptm->tm_sec,
			getpid(), _xlog_level_str[level],
			srcfile, srcline, srcfunc );
	else
		snprintf( buf, sizeof(buf), "|%04d%02d%02d %02d:%02d:%02d|%d|%s| ",
			ptm->tm_year+1900, ptm->tm_mon+1, ptm->tm_mday,
			ptm->tm_hour, ptm->tm_min, ptm->tm_sec,
			getpid(), _xlog_level_str[level] );
#endif

	va_start( ap, fmt );
	vsnprintf( buf + strlen(buf), sizeof(buf) - strlen(buf), fmt, ap );
	va_end( ap );
	if( level == XLOG_PERROR || level == XLOG_PWARN
			|| level == XLOG_PDEBUG || level == XLOG_PINFO )
		snprintf( buf + strlen(buf), sizeof(buf) - strlen(buf),
			"%d: %s\n",
		   	errno_save, strerror(errno_save) );
	buf[sizeof(buf) - 1] = 0x00;

	if( level == XLOG_ERROR || level == XLOG_PERROR )
		fprintf( stderr, "%s", buf );
	else
		fprintf( stdout, "%s", buf );
	if( _xlog_fp ) {
		fprintf( _xlog_fp, "%s", buf );
		fflush( _xlog_fp );
	}

	errno = errno_save;
	return;
}

