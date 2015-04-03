/*******************************************************************
 * xueyunlong@gmail.com
 * 2012-12-17
 * Hisory:
 * 2014-07-10 v0.2 Use regex
 * 2014-07-15 v0.3 Use lex & yacc
 * *****************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <libgen.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "xcodes.h"
#include "xlog.h"
#include "xclassFile.h"
#include "xgreen.h"
#include "xfoot/xfoot.h"
#include "xtrace.h"
using namespace std;

extern "C" { 
	int	xs0SetFlag( int *flag );
	int xs0( FILE *input, FILE *output ); 
	int xs1( FILE *input, FILE *output ); 
	int xs2( FILE *input, FILE *output ); 
	int xs4( FILE *input, FILE *output ); 
	int xs5( FILE *input, FILE *output ); 
	int xs6( FILE *input, FILE *output ); 
	int xs10( FILE *input, FILE *output ); 
	void xs30SetSrcfp( FILE *fp );
	int xs30( FILE *input, FILE *output ); 
	int	xs110SetFlag( int *flag );
	int xs110( FILE *input, FILE *output ); 
	int xs120( FILE *input, FILE *output ); 
	int xs400( FILE *input, FILE *output ); 
	int xs402( FILE *input, FILE *output ); 
	void xs405SetSrcfp( FILE *fp );
	int xs405( FILE *input, FILE *output ); 
	int xs500( FILE *input, FILE *output ); 
	int xs510( FILE *input, FILE *output ); 
	int xs550( FILE *input, FILE *output ); 
}

/*
 * do the xs0, xs1, ... to the file of inf;
 * after finished, the parsered content is stored in the inf,
 * the outf is only a tmp file
 */
typedef int (*xs)( FILE *input, FILE *output ); 
int xparser( xs fun, FILE *inf, FILE *outf )
{
	int rt = 0; 

	if( ftruncate( fileno(outf), 0 ) != 0 ) 
		return -1; 
	rt = xs500( inf, outf ); 
	if ( rt != 0 )  { 
		xdebug( "%s Failed\n", "xs510" );
		return -1; 
	} 
	fflush( inf ); fseek( inf, 0, SEEK_SET ); 
	fflush( outf ); fseek( outf, 0, SEEK_SET ); 

	if( ftruncate( fileno(inf), 0 ) != 0 ) 
		return -1; 
	rt = fun( outf, inf ); 
	if ( rt != 0 )  { 
		xdebug( "xparser Failed. return code: %d \n", rt );
		return rt; 
	} 
	fflush( inf ); fseek( inf, 0, SEEK_SET ); 
	fflush( outf ); fseek( outf, 0, SEEK_SET ); 
	return 0;
}

#define	XPARSER( fun, inf, outf ) \
	do{ \
		int rt; \
		xdebug( "%s begin\n", #fun ); \
		rt = xparser( fun, inf, outf ); \
		if ( rt != 0 )  { \
			xdebug( "%s Failed \n", #fun ); \
			fclose( inf ); inf = NULL;	\
			fclose( outf ); outf = NULL;	\
			return rt; \
		} \
	}while( 0 );

/*
 * Return value:
 * 	succ: 0
 * 	failed: -1
 * Notice:
 * After copy finished, both in and out are at the end of the file.
 * If want to use the file, should make a fseek set to the begin 
 * of the file.
 */
int copy( FILE *in, FILE *out )
{	
	char  buffer[4096];
    size_t  n;

    while ((n = fread(buffer, sizeof(char), sizeof(buffer), in)) > 0)
    {
		if ( ferror(in) ) { 
			xperror( "copy error\n" ); 
			return -1;
		}
        if (fwrite(buffer, sizeof(char), n, out) != n)  {
            xperror("write failed\n");
			return -1;
		}
    }
	fflush( in ); fseek( in, 0, SEEK_SET );
	fflush( out ); fseek( out, 0, SEEK_SET );
	return 0;
}

/*
 * Return: 
 * 0: successful
 * 1: failed.
 *
 * the copy function have the following Problem:
 * When error happend, disk full, for example, it will lead to
 * to src file losts.
 *
 * 1. rename out file to out file.bk
 * 2. copy in to out file
 * 3. if copy success, delete out_file.bk
 * 4. if copy failed, rename out_file.bk to out_file 
 *
 * bug: when the file is a Read only file, it will also be changed.
 */
int safeCopy( FILE *infp, const char *out )
{
	int rt = 0;
	FILE	*outfp;
	string	str = out; str +=  ".xtrace.bk";

	rt = rename ( out, str.c_str()  );
	if ( rt != 0 )  {
		xperror( "can't back up source code file\n" );
		return -1;
	}

	outfp = fopen( out, "w" );
	if ( outfp == NULL )  {
		xperror( "Open file failed: %s\n", out );
		return -2;
	}
	rt = copy( infp, outfp );
	if ( rt != 0 )  {
		xdebug( "copy infp to outfp file failed\n" );
		XFCLOSE( outfp );
		rt = rename ( str.c_str(), out  );
		if ( rt != 0 )  {
			xperror( "Restore file failed,the source code keeped in:%s\n", str.c_str() );
			return -3;
		}
		return -4;
	}

	//copy successful
	//close outfp, delete backup file
	XFCLOSE( outfp );
	unlink( str.c_str() );

	return 0;
}



// Read file into src
int ReadFile( const string file, string &src )
{
	FILE *fp;
	char *buffer;
	unsigned int i, size;

	fp = fopen( file.c_str( ), "r" );
	if ( fp == NULL )  {
		xperror( "Open file failed: %s\n", file.c_str() );
		return -1;
	}
	fseek( fp, 0, SEEK_END );
	size = ftell( fp );
	rewind( fp );
	buffer = (char*)malloc( sizeof(char) * size );
    if ( buffer == NULL) { 
		 xperror( "Memory error" ); 
		 return -1;
	}
	
	i = fread( buffer, 1, size, fp );
	if ( i != size ) { 
		xperror( "Reading file error: %s", file.c_str() );
		free( buffer );
		return -1;
	}
	src.append( buffer, size );
	fclose( fp ); fp = NULL;
	free( buffer ); buffer = NULL;

	return 0;
}

// Write src to file
int WriteFile( const string file, const string &src )
{
	FILE *fp;
	unsigned int size;

	fp = fopen( file.c_str( ), "w" );
	if ( fp == NULL )  {
		xperror( "Open file failed: %s", file.c_str() );
		return -1;
	}
	size = fwrite( src.c_str(), sizeof(char), src.length(), fp );
	if( size != src.length() ) {
		xperror( "Writing to file failed: %s", file.c_str() );
		return -1;
	}
	fclose( fp ); fp = NULL;

	return 0;
}


/*
 * Return:
 * 0: success
 * 1: the src file already have the xtrace flag
 * -1: error happend
 */ 
int xtrace::open_xtrace( const string cppFile )
{
	xdebug( "xtrace::open_xtrace begin\n" );

	int		rt = 0;
	FILE	*srcfp = NULL;
	FILE	*tmpa = NULL, *tmpb = NULL;

	char buf[512];
	snprintf( buf, sizeof( buf ), "%s/%s", 
		getTmpdir(), XFOOT_HEADER );
	rt = WriteFile( buf, XFOOT ); 
	if ( rt != 0 )  return rt;
	*buf = 0x00;

	tmpa = tmpfile();
	tmpb = tmpfile();
	if ( tmpa == NULL || tmpb == NULL )  {
		xdebug( "Failed to create tmp file\n" );
		XFCLOSE( srcfp ); XFCLOSE( tmpa ); XFCLOSE( tmpb );
		return -1;
	}

	srcfp = fopen( cppFile.c_str( ), "r" );
	if ( srcfp == NULL )  {
		xperror( "Open file failed: %s\n", cppFile.c_str() );
		XFCLOSE( srcfp ); XFCLOSE( tmpa ); XFCLOSE( tmpb );
		return -1;
	}


	rt = copy( srcfp, tmpa );
	if ( rt != 0 )  {
		xdebug( "copy tmpa to src file failed\n" );
		XFCLOSE( srcfp ); XFCLOSE( tmpa ); XFCLOSE( tmpb );
		return -1;
	}
	int	xfootFlag = 0;
	xs0SetFlag( &xfootFlag );
	XPARSER( xs0, tmpa, tmpb );
	if ( xfootFlag != 0 )  {
		XFCLOSE( srcfp ); XFCLOSE( tmpa ); XFCLOSE( tmpb );
		xdebug( "%s\n", X2_KEY_EXIST  );
		return X1_KEY_EXIST;
	}

	XPARSER( xs1, tmpa, tmpb );
	XPARSER( xs2, tmpa, tmpb );
	XPARSER( xs4, tmpa, tmpb );
	XPARSER( xs5, tmpa, tmpb );
	XPARSER( xs6, tmpa, tmpb );
	XPARSER( xs10, tmpa, tmpb );
	xs30SetSrcfp( srcfp );
	XPARSER( xs30, tmpa, tmpb );
	rt = safeCopy( tmpa, cppFile.c_str() );
	if ( rt != 0 )  {
		xdebug( "copy tmpa to srcfp file failed\n" );
		XFCLOSE( srcfp ); XFCLOSE( tmpa ); XFCLOSE( tmpb );
		return -1;
	}

	XFCLOSE( srcfp ); XFCLOSE( tmpa ); XFCLOSE( tmpb );
	return 0;
}


int xtrace::close_xtrace( const string cppFile )
{
	xdebug( "xtrace::close_xtrace begin\n" );

	int		rt = 0;
	FILE	*srcfp = NULL;
	FILE	*tmpa = NULL, *tmpb = NULL;

	tmpa = tmpfile();
	tmpb = tmpfile();
	if ( tmpa == NULL || tmpb == NULL )  {
		xdebug( "Failed to create tmp file\n" );
		XFCLOSE( srcfp ); XFCLOSE( tmpa ); XFCLOSE( tmpb );
		return -1;
	}

	srcfp = fopen( cppFile.c_str( ), "r" );
	if ( srcfp == NULL )  {
		xperror( "Open file failed: %s\n", cppFile.c_str() );
		XFCLOSE( srcfp ); XFCLOSE( tmpa ); XFCLOSE( tmpb );
		return -1;
	}
	int	xfootFlag = 0;
	xs110SetFlag( &xfootFlag );
	rt = xs110( srcfp, tmpa );
	if ( rt != 0 )  { 
		xdebug( "%s Failed \n", "xs120" );
		XFCLOSE( srcfp ); XFCLOSE( tmpa ); XFCLOSE( tmpb );
		return -1;
	}
	if ( xfootFlag == 0 )  {
		XFCLOSE( srcfp ); XFCLOSE( tmpa ); XFCLOSE( tmpb );
		xdebug( "%s\n", X2_NOT_XTRACE_FILE  );
		return X1_NOT_XTRACE_FILE;
	}
	fflush( tmpa ); fseek( tmpa, 0, SEEK_SET ); 

	rt = xs120( tmpa, tmpb );
	if ( rt != 0 )  { 
		xdebug( "%s Failed \n", "xs120" );
		XFCLOSE( srcfp ); XFCLOSE( tmpa ); XFCLOSE( tmpb );
		return -1;
	}
	XFCLOSE( srcfp );
	fflush( tmpb ); fseek( tmpb, 0, SEEK_SET ); 

	rt = safeCopy( tmpb, cppFile.c_str() );
	if ( rt != 0 )  {
		xdebug( "copy tmp to srcfp file failed\n" );
		XFCLOSE( srcfp ); XFCLOSE( tmpa ); XFCLOSE( tmpb );
		return -1;
	}

	XFCLOSE( srcfp ); XFCLOSE( tmpa ); XFCLOSE( tmpb );
	return 0;
}

/*
 * Return:
 * 0: success
 * -1: error happend
 */ 
int xtrace::listFunctions( const string cppFile )
{
	xdebug( "xtrace::listFunctions begin\n" );

	int		rt ;
	FILE	*srcfp;
	FILE	*tmpa = NULL, *tmpb = NULL;

	tmpa = tmpfile();
	tmpb = tmpfile();
	if ( tmpa == NULL || tmpb == NULL )  {
		xdebug( "Failed to create tmp file\n" );
		return -1;
	}

	srcfp = fopen( cppFile.c_str( ), "r" );
	if ( srcfp == NULL )  {
		xperror( "Open file failed: %s\n", cppFile.c_str() );
		return -1;
	}

	//in the XPARSER, do the swap( srcfp, tmpa )
	//So need do swap again to keep srcfp from changed.
	XPARSER( xs1, srcfp, tmpa );
	std::swap( srcfp, tmpa );
	XPARSER( xs2, tmpa, tmpb );
	//XPARSER( xs3, tmpa, tmpb );
	XPARSER( xs400, tmpa, tmpb );
	XPARSER( xs402, tmpa, tmpb );
#if 0
copy(tmpa, stdout);
return -1;
#endif
	xs405SetSrcfp( srcfp );
	XPARSER( xs405, tmpa, stdout );
	fclose( srcfp ); srcfp = NULL;
	fclose( tmpa ); tmpa = NULL;
	fclose( tmpb ); tmpb = NULL;

	return 0;
}


