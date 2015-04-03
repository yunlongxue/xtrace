/*******************************************************************
 * xueyunlong@gmail.com
 * 2014-07-07
 * Hisory:
 * *****************************************************************/
#include <stdio.h>

int fun( )
{
	sleep( 2 );
	return 0;
}

int main( int argc, char **argv )
{
	fork(  );

	int i = 1, j = 0120;

	if ( 1 )
		fun( );
	
	fork(  );
	for ( i = 0; i < 3; i++ )  {
		j = 10 / ( i - 1 );
	}
	printf( "%d\n", j );

	return 0;
}
