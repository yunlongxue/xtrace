/* 
 * Author:	XUEYL	xueyunlong@gmail.com
 * Date:	2012-12-15
 * 
 */


#ifndef _XTRACE_XTRACE_H_
#define _XTRACE_XTRACE_H_

#include <vector>
#include <string>
using namespace std;


class xtrace {
private:
	vector<string> src;

public:
	xtrace(){};

	/* Enable xtrace for the c++ file */
	int open_xtrace( const string cppfile );

	/* Disable xtrace for the c++ file */
	int	close_xtrace( const string cppfile );

	/* list Functions in the c++ file */
	int listFunctions( const string cppfile );

};

#endif
