#include <wagon.h>



/**
 * @brief Initialize the array of structure.
 *        First parameter is the bus adress of the wagon
 *        Second parameter in a decimal representation of the 2-letter country code
 *        used to adress the wagon.
 */
TWagon allWagons[ NR_OF_WAGONS ] = { {0x20, convertCountryCode( countries[cDK] )}, 
                                     {0x21, convertCountryCode( countries[cDE] )}, 
                                     {0x22, convertCountryCode( countries[cIT] )}, 
                                     {0x23, convertCountryCode( countries[cHR] )}, 
                                     {0x24, convertCountryCode( countries[cLT] )}, 
                                     {0x25, convertCountryCode( countries[cIR] )}
                                    };


/**
 * @brief This method converts a given 2-letter country-code into a
 *        decimal representation by the follwowing formula:
 *        a = ASCII-value of the first letter
 *        b = ASCII-value of the second letter
 * 
 * @param code Is a string with at least 2 characters in length
 * @return uint16_t  = a * 256 + b
 */
uint16_t convertCountryCode( const char *code ) {    
  uint16_t a,b;
  if ( strlen( code ) >= 2 ) {
    a = code[0];
    b = code[1];
    return ( (a << 8) | b );
  } else return (0);
}


void wagon_setup() {
  for (uint8_t i = 0; i < NR_OF_WAGONS; i++) {
    allWagons[ i ].begin();
  }   
}

void wagon_test() {
  for (uint8_t i = 0; i<NR_OF_WAGONS; i++) {
    Serial.println(  allWagons[ i ].wagon );
  }
}