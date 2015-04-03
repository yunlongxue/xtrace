/* ********************************************************
 * Author: xueyunlong@gmal.com
 * Date:   2008-10-19
 * Usage:
 * 		xdebug( fmt, ... ): printf  message only when DEBUG
 * 			flag defined in the Makefile, and flag the message
 * 			as "WARN"	
 * 		xpdebug(fmt, ... ): the same as xdebug, but also print 
 * 			additional system error message as like perror( ). 
 *
 * 		xlog( fmt, ... ), xwarn( fmt, ... ), xerror( fmt, ... ):
 * 			print message and flage the message to LOG or WARN
 * 			or ERROR.
 * 		xplog, xpwarn, xperror:
 * 			the same as xlog, xwarn, xerrora; but also print                                                             
 * 			additional system error message as like perror(  ).
 * 			
 * *******************************************************/


#ifndef _XLOG_H_20081019_
#define _XLOG_H_20081019_

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <strings.h>
#include <stdarg.h>
#include <libgen.h>
#include <errno.h>


/*
__PRETTY_FUNCTION__ is processed in compiling time
#if defined(__PRETTY_FUNCTION__)
	#define __X_FUNCTION__   __PRETTY_FUNCTION__
#elif defined(__FUNCTION__)
	#define __X_FUNCTION__   __FUNCTION__
#elif defined( __func__ )
	#define __X_FUNCTION__   __func__
#else
	#define __X_FUNCTION__   ""
#endif 
*/
#define __X_FUNCTION__   __func__


#ifdef __cplusplus
extern "C"  {
#endif



/* Define for log level */
#define XLOG_DEBUG	0
#define XLOG_PDEBUG	1
#define XLOG_INFO	2
#define XLOG_PINFO	3
#define XLOG_WARN	4
#define XLOG_PWARN	5
#define XLOG_ERROR	6
#define XLOG_PERROR	7


#if defined(DEBUG)
	#define xdebug( ... ) \
		do  { \
			_xlog( __FILE__, __LINE__, __X_FUNCTION__, XLOG_DEBUG, __VA_ARGS__ ); \
		} while( 0 )

	#define xpdebug( ... ) \
		do  { \
			_xlog( __FILE__, __LINE__, __X_FUNCTION__, XLOG_PDEBUG, __VA_ARGS__ ); \
		} while( 0 )

#else
	#define xdebug( ... )   do {} while( 0 )
	#define xpdebug( ... )   do {} while( 0 )
#endif 


#define xlog( ... ) \
	do  { \
		_xlog( __FILE__, __LINE__, __X_FUNCTION__, XLOG_INFO, __VA_ARGS__ ); \
	} while( 0 )

#define xplog( ... ) \
	do  { \
		_xlog( __FILE__, __LINE__, __X_FUNCTION__, XLOG_PINFO, __VA_ARGS__ ); \
	} while( 0 )



#define xwarn( ... ) \
	do  { \
		_xlog( __FILE__, __LINE__, __X_FUNCTION__, XLOG_WARN, __VA_ARGS__ ); \
	} while( 0 )

#define xpwarn( ... ) \
	do  { \
		_xlog( __FILE__, __LINE__, __X_FUNCTION__, XLOG_PWARN, __VA_ARGS__ ); \
	} while( 0 )



#define xerror( ... ) \
	do  { \
		_xlog( __FILE__, __LINE__, __X_FUNCTION__, XLOG_ERROR, __VA_ARGS__ ); \
	} while( 0 )

#define xperror( ... ) \
	do  { \
		_xlog( __FILE__, __LINE__, __X_FUNCTION__, XLOG_PERROR, __VA_ARGS__ ); \
	} while( 0 )



int xlog_init( const char *logfile, const char *level );
int xlog_uninit();
const char* xlog_path();
void _xlog( const char *srcfile, const int srcline, const char *srcfunc, const int level, const char *fmt, ... );


#ifdef __cplusplus
}
#endif


#endif
