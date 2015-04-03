/* 
 * Author:	XUEYL	xueyunlong@gmail.com
 * Date:	2011-06-15
 *
 * Version History:
 * 2011-06-15	V1.0
 * 
 */


#ifndef _XLIB_XCLASSFILE_H_
#define _XLIB_XCLASSFILE_H_

#include <vector>
#include <string>
using namespace std;

#define	XFCLOSE( fp ) \
	do  { \
		if ( fp )  { \
			fclose( fp ); \
			fp = NULL; \
		} \
	} while( 0 )

class xclassFile {

public:
	xclassFile();


	/* Read a file, and store it's content in lines.
	   output: 
	   return value: 
	   	0: when success
		!= 0: when failed
	*/
	int string_to_file( vector<string> &str, const string file);
	int	file_to_string( const string file, vector<string> *src  );
	int	file_to_string( FILE *fp, vector<string> *src  );

	static int print( FILE *fp );
	/*
	int	file_to_string( FILE *fp, vector<string> lines & );
	int	file_to_string( string file, vector<string> lines & );
	int	file_to_string( int fd, vector<string> lines & );
	int sting_to_file( FILE *fp, vector<string> lines & );
	int sting_to_file( string file, vector<string> lines & );
	int sting_to_file( int fd, vector<string> lines & );
	int append_lines_into_file( FILE *fp, vector<string> lines & );
	int append_lines_into_file( string file, vector<string> lines & );
	int append_lines_into_file( int fd, vector<string> lines & );
	*/
};

#endif
