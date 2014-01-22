// g++ tempread.cpp -o tempread -lm

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <math.h>
#include "tempread.h"

#include <bitset>
using namespace std;


CTempRead::CTempRead( const char* p_pcszDev, const char* p_pcszUnits )
{
	// Clear the initial error state
	m_pszErrMsg = 0;

	// Initialize the device
	m_pszDev = 0;
	if( !p_pcszDev )
	{
		// Default device if none specified
		setDev( "ttyUSB0" );
	}
	else if( !*p_pcszDev )
	{
		// Default device if empty specified
		setDev( "ttyUSB0" );
	}
	else
	{
		// Use the specified device
		setDev( p_pcszDev );
	}

	// Initialize the units
	if( !p_pcszUnits )
	{
		// Default Farenheit on no units
		m_fFaren = true;
	}
	else if( (*p_pcszUnits == 'C') || (*p_pcszUnits == 'c') )
	{
		// Celcius specified
		m_fFaren = false;
	}
	else
	{
		// Assume Farenheit
		m_fFaren = true;
	}
} // end CTempRead::CTempRead


CTempRead::~CTempRead( void )
{
	// Cleanup class data
	delete [] m_pszDev;
	delete [] m_pszErrMsg;
} // end CTempRead::~CTempRead


const char* CTempRead::getLastError( void )
{	return m_pszErrMsg ? m_pszErrMsg : "";
} // end CTempRead::getLastError


void CTempRead::setDev( const char* p_pcszDev )
{
	// Does the passed device include the full path?
	int nBuf;
	if( *p_pcszDev != '/' )
	{
		// Assume we have to prepend /dev/
		nBuf = strlen( p_pcszDev ) + 6;
		delete [] m_pszDev;
		m_pszDev = new char[ nBuf ];

		// Copy the name to the new buffer
		strcpy( m_pszDev, "/dev/" );
		strcat( m_pszDev, p_pcszDev );
	}
	else
	{	
		// Allocate a buffer for the passed device name
		nBuf = strlen( p_pcszDev ) + 1;
		delete [] m_pszDev;
		m_pszDev = new char[ nBuf ];

		// Copy the name to the new buffer
		strcpy( m_pszDev, p_pcszDev );
	}

} // end CTempRead::setDev


void CTempRead::setError( const char* p_pcszSet )
{
	// Allocate a buffer for the device name
	int nBuf = strlen( p_pcszSet ) + 1;
	delete [] m_pszErrMsg;
	m_pszErrMsg = new char[ nBuf ];

	// Copy the name to the new buffer
	strcpy( m_pszErrMsg, p_pcszSet );
} // end CTempRead::setError


bool CTempRead::read( float* p_pfTemp )
{
	// Open the configured device
	char szFormat[ 200 ];
	int nComPort = open( m_pszDev, O_RDWR | O_NONBLOCK | O_NOCTTY );
	if( nComPort < 0 )
	{
		// Failed to open the port
		sprintf( szFormat, "Failed to open %s (%d)", m_pszDev, errno );
		setError( szFormat );
		return false;
	}

	// Get the current port settings
	struct termios iosOld;
	if( tcgetattr( nComPort, &iosOld ) < 0 )
	{
		// Failed to get the port settings
		sprintf( szFormat, "Failed to get port settings for %s (%d)",
			m_pszDev, errno );
		setError( szFormat );
		close( nComPort );
		return false;
	}

	// Set the communications for this device
	// (Does this matter since we're not a real serial port?)
	struct termios iosNew = iosOld;
	cfsetispeed( &iosNew, B9600);
	cfsetospeed( &iosNew, B9600);
	cfmakeraw  ( &iosNew );
	if( tcsetattr( nComPort, TCSANOW, &iosNew ) < 0 )
	{
		// Failed to update the port settings
		sprintf( szFormat, "Failed to update port settings for %s (%d)",
			m_pszDev, errno );
		setError( szFormat );
		close( nComPort );
		return false;
	}

	// Execute the protocol to read the device
	bool fSuccess = initPort( nComPort );
	if( fSuccess )
	{
		// Get the data from the device
		char szData[18];
		int nLen = ::read( nComPort, szData, 18 );
		if( nLen != 18 )
		{
			// Read failed
			fSuccess = false;
		}
		else
		{
			// Parse the data
			*p_pfTemp = extractTemperature( szData );
		}
	}

	// Cleanup
	tcsetattr( nComPort, TCSANOW, &iosOld );
	close( nComPort );

	// Indicate success
	return fSuccess;
} // end CTempRead::read


bool CTempRead::writePort( int p_nComPort, const char* p_pcszData )
{
	char szFormat[ 200 ];

	// Write the port
	int nWant = strlen( p_pcszData );
	int nRes  = write( p_nComPort, p_pcszData, nWant );
	if( nRes < 0 )
	{
		// Write failed
		sprintf( szFormat, "Init device write failed (%d)", errno );
		setError( szFormat );
		return false;
	}
	if( nRes < 1 )
	{
		// Write truncated
		sprintf( szFormat, "Init device write len %d truncated to %d",
			nWant, nRes );
		setError( szFormat );
		return false;
	}

	// Success
	return true;
} // end CTempRead::writePort


bool CTempRead::discardPort( int p_nComPort )
{
	// Read off junk data
	int nRes;
	char szDump[ 100 ];
	do
	{
		// Allow data time to appear
		usleep( 150000 );
	} while( (nRes = ::read( p_nComPort, szDump, 100 )) > 0 );

	// Did we succeed?
	if( (nRes < 0) && (errno != EAGAIN) )
	{
		// Failed to read
		sprintf( szDump, "Failed to discard device data (%d)", errno );
		setError( szDump );
		return false;
	}

	// Success
	return true;
} // end CTempRead::discardPort


bool CTempRead::initPort( int p_nComPort )
{
	// Write the protocol
	bool fSuccess = writePort( p_nComPort, "X" );
	if( fSuccess )
	{
		// Discard result
		fSuccess = discardPort( p_nComPort );
	}
	if( fSuccess )
	{
		// Next part of protocol
		fSuccess = writePort( p_nComPort, "P" );
	}
	if( fSuccess )
	{
		// Discard result
		fSuccess = discardPort( p_nComPort );
	}
	if( fSuccess )
	{
		// Request a reading
		fSuccess = writePort( p_nComPort, "R" );
	}
	if( fSuccess )
	{
		// Wait for results to appear before we return,
		// and allow the read to process
		usleep( 150000 );
	}

	// Indicate status
	return fSuccess;
} // end CTempRead::initPort

       
float CTempRead::extractTemperature( const char* p_pcszData )
{
	int temperatureIndex =
		((p_pcszData[15] == 0) && 
		 (p_pcszData[16] == 0) &&
		 (p_pcszData[17] == 0)) ? 0 : 9;
	
	// Use bitset for simplicity.
	bitset<8> set((unsigned int) p_pcszData[temperatureIndex]);
	int temporary = (int) set.to_ulong() | ((int)p_pcszData[temperatureIndex + 1] << 8);

	bool isnegative = false;

	// Check to see if the temperature is in the 
	// negative range.
	if ((temporary & 0x8000) == 0x8000)
	{
		temporary &= 0x07ff;
		isnegative = true;
		temporary = 0x800 - temporary;
	}

	temporary &= 0x07ff;
	
	float celcius = (float)temporary / 16.0f;

	// Make negative if necessary.
	if (isnegative)
	{ 
		celcius *= -1;
	}

	// Initialize the return value with the parsed reading
	float val;
	if (m_fFaren)
	{
		// Convert from C to F 
		val = (celcius * 9.0f / 5.0f) + 32.0f;
	}
	else
	{
		// Use C
		val = celcius;
	}

	// Round temperature to two places
	return (float)(floor(((double)val * 100.0) + 0.5) / 100.0); 
} // end CTempRead::extractTemperature
