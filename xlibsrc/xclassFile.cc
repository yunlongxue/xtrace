/* 
 * Author:	XUEYL	xueyunlong@gmail.com
 * Date:	2011-06-15
 *
 * Version History:
 * 2011-06-15	V1.0
 * 
 */

#include "xlog.h"
#include "xclassFile.h"
#include <fstream>

xclassFile::xclassFile ()
{
}


int	xclassFile::file_to_string( const string file, vector<string> *src  )
{
	xdebug( "file: [%s] \n", file.c_str() );
	ifstream f;
	string	buf;

	f.open( file );
	if( !f ) {
		xerror( "Open file failed:%s\n", file.c_str() );
		return -1;
	}

	while ( f.good() ) {
		std::getline( f, buf );
		src->push_back( buf );
	}
	f.close();

	return 0;
}

int	xclassFile::file_to_string( FILE *fp, vector<string> *src  )
{
	xdebug( "file fp: [%p] \n", fp );
	char	buf[4096];

	if ( fp == NULL )
	{
		xerror( "input file is null" );
		return -1;
	}
	while ( fgets( buf, sizeof(buf), fp ) )
	{
		src->push_back( buf );
	}
	
	xdebug( "xclassFile::file_to_string Finished.  \n" );	
	return 0;
}

int xclassFile::string_to_file( vector<string> &str, const string file)
{
	xdebug( "file: [%s] \n", file.c_str() );
	ofstream f;
	string	buf;

	f.open( file );
	if( !f ) { 
		xerror( "Open file failed:%s\n", file.c_str() );
		return -1;
	}

	for( auto it = str.begin(); it != str.end(); it++ ) {
		f<<*it;
	}
	f.close();
	return 0;
}



int xclassFile::print( FILE *fp )
{
	xdebug( "file fp: [%p] \n", fp );
	char	buf[4096];

	if ( fp == NULL )
	{
		xerror( "input file is null" );
		return -1;
	}
	while ( fgets( buf, sizeof(buf), fp ) )
	{
		printf( "%s", buf );
	}
	
	xdebug( "xclassFile::print Finished.  \n" );	
	return 0;		
}

/*



int	xclassFile::file_to_string( string file, vector<string> lines & )
{
	char	buf[4096];
	FILE	*fp = NULL;

	xdebug( "file: [%s] \n", file.c_str() );

	fp = fopen( file.c_str(), "r" );
	if ( fp == NULL )
	{
		xperror( file.c_str() );
		return -1;
	}
	while ( fgets( buf, sizeof(buf), fp ) )
	{
		lines.push_back( buf );
	}
	
	return 0;
}

int	xclassFile::file_to_string( int fd, vector<string> lines & )
{
	char	buf[4096];
	FILE	*fp = NULL;

	xdebug( "file id: %d \n", fd );

	fp = fdopen( fd, "r" );
	if ( fp == NULL )
	{
		xperror( "file id: [%d]", fd );
		return -1;
	}
	while ( fgets( buf, sizeof(buf), fp ) )
	{
		lines.push_back( buf );
	}
	
	return 0;
}


int sting_to_file( int fd, vector<string> lines & );


*/
