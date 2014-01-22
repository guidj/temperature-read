#ifndef __TEMPREAD_H__
#define __TEMPREAD_H__


/**
 * @class CTempRead
 * @brief Manages communications with the USB temprature sensor.
 */
class CTempRead
{
	public:
		// PUBLIC CLASS INTERFACES

		/**
		 * @brief Instantiantes an instance for a device, with the specified
		 *		temperature units.
		 *
		 * If the device is null or empty, the default "ttyUSB0" will be used.
		 * If the units are null or invalid Farenheit will be used by default.
		 *
		 * @param[in] p_pcszDev Device name
		 * @param[in] p_pcszUnits Calculation units ("C" or "F")
		 */
		CTempRead( const char* p_pcszDev, const char* p_pcszUnits );

		/**
		 * @brief Releases our resources on destruction.
		 */
		~CTempRead( void );

		/**
		 * @brief Gets a description of the last error.
		 *
		 * Do not store this pointer, as it will change when the next
		 * error in generated.
		 *
		 * @return Pointer to the error message buffere
		 */
		const char* getLastError( void );

		/**
		 * @brief  Reads the temperature from the device.
		 * @param  p_pfRead[out] Receives temperature on success
		 * @return Success flag
		 */
		bool read( float* p_pfRead );

	protected:
		// PROTECTED CLASS IMPLEMENTATION METHODS

		/**
		 * @brief Keeps the specified device name.
		 * @param[in] p_pcszDev Device name to keep
		 */
		void setDev( const char* p_pcszDev );

		/**
		 * @brief Sets the current error message.
		 * @param[in] p_pcszSet Message to set
		 */
		void setError( const char* p_pcszSet );

		/**
		 * @brief Writes out the passed string to the port.
		 * @param[in] p_nComPort Port to write
		 * @param[in] p_pcszData Data to write to port
		 * @return Success flag
		 */
		bool writePort( int p_nComPort, const char* p_pcszData );

		/**
		 * @brief Discards incoming data on the passed port.
		 * @param[in] p_nComPort Port to discard
		 * @return Success flag
		 */
		bool discardPort( int p_nComPort );

		/**
		 * @brief Executes initialization protocol for the device.
		 * @param[in] p_nComPort Device port
		 * @return Success flag
		 */
		bool initPort( int p_nComPort );

		/**
		 * @brief Extracts the temperature from the passed device byte stream.
		 *
		 * The temperature is returned in the configured units for the class.
		 *
		 * @param[in] p_pcszData Stream from which to extract the temperature
		 * @return Extracted temperature in configured units
		 */
		float extractTemperature( const char* p_pcszData );

	protected:
		// PROTECTED CLASS IMPLEMENTATION DATA

		/**
		 * @var   m_pszDev
		 * @brief Device name of temperature sensor. 
		 */
		char* m_pszDev;

		/**
		 * @var   m_fFaren
		 * @brief Units in Farenheit flag. 
		 */
		bool m_fFaren;

		/**
		 * @var   m_pszErrMsg
		 * @brief Describes the last error.
		 */
		char* m_pszErrMsg;

}; // end class CTempRead


#endif // __TEMPREAD_H__
