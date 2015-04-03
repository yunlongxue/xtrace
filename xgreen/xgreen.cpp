/*******************************************************************
 * xueyunlong@gmail.com
 * 2014-09-01
 * *****************************************************************/

#include <string>
using namespace std;

#include "../xlibsrc/xlog.h"
#include "xgreen.h"


static	string	xtmpdir;

const	char*	getTmpdir()
{
	return	xtmpdir.c_str( );

}


void	setTmpdir( const char *tmpdir )
{
	xtmpdir = tmpdir;
}


