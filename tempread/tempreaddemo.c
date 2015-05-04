// g++ tempreaddemo.c tempread.cpp -o tempread -lm

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "tempread.h"


int main(int argc, char** argv)
{
	// Show banner
	printf( "tempread Temperature Sensor Reader Test Entry\n" );
	printf( "  Use -d [dev] to specify the device (ttyUSB0)\n" );
	printf( "  Use -u [uni] to specify the units (F)\n" );
	printf( "  Ctl-C to stop\n" );

	// Parse the command line parameters
	char* pszDev = 0;
	char* pszUnits = 0;
	for( int nLoop = 1; nLoop < argc; ++nLoop )
	{
		// Test for parameters we know
		if( strcmp( argv[ nLoop ], "-d" ) == 0 )
		{
			// Device found
			if( ++nLoop >= argc )
			{
				// Missing actual value
				printf( "Missing -d device parameter value\n" );
			}
			else
			{
				// Keep the device value
				pszDev = argv[ nLoop ];
			}
		}
		else if( strcmp( argv[ nLoop ], "-u" ) == 0 )
		{
			// Units found
			if( ++nLoop >= argc )
			{
				// Missing actual value
				printf( "Missing -u units parameter value\n" );
			}
			else
			{
				// Keep the units value
				pszUnits = argv[ nLoop ];
			}
		}
		else
		{
			// Unknown parameter
			printf( "Unknown parameter %s\n", argv[ nLoop ] );
		}
	}

	// Perform the test
	CTempRead readTest( pszDev, pszUnits );
	float flValue;
	bool  fSuccess;
	while( true )
	{
		// Read the temperature
		fSuccess = readTest.read( &flValue );
		if( fSuccess )
		{
			// Display the temperature
			printf( "The current temp is %.2f\n", flValue );
		}
		else
		{
			// Display the error message
			printf( "Error: %s\n", readTest.getLastError() );
		}

		// Wait 3 sec until the next test
		sleep( 3 );
	}
} // end main
