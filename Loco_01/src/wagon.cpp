#include <wagon.h>



/**
 * @brief Initialize the array of structure.
 *        First parameter is the bus adress of the wagon. If there is actually no
 *        wagon, the address 0x00 can be used.
 *        Second parameter in a decimal representation of the 2-letter country code
 *        used to adress the wagon.
 * 
 *        Keep in mind:  0x3c is assigned to the display in the loco
 */

#ifdef TRAIN_GERMANY
  TWagon allWagons[ NR_OF_WAGONS ] = { 
        {0x10, convertCountryCode( countries[L1] )},
        {0x00, convertCountryCode( countries[IT] )}, 
        {0x00, convertCountryCode( countries[NO] )}, 
        {0x00, convertCountryCode( countries[IR] )}, 
        {0x00, convertCountryCode( countries[HR] )}, 
        {0x00, convertCountryCode( countries[LT] )}, 
        {0x31, convertCountryCode( countries[DE] )}, 
        {0x32, convertCountryCode( countries[DK] )}, 
        {0x00, convertCountryCode( countries[PT] )}, 
        {0x00, convertCountryCode( countries[CH] )}, 
        {0x00, convertCountryCode( countries[X1] )}, 
        {0x00, convertCountryCode( countries[X2] )}, 
        {0x00, convertCountryCode( countries[NL] )}, 
        {0x00, convertCountryCode( countries[TR] )}, 
        {0x00, convertCountryCode( countries[AT] )}, 
        {0x00, convertCountryCode( countries[PL] )}, 
        {0x00, convertCountryCode( countries[FR] )}, 
        {0x00, convertCountryCode( countries[SK] )}, 
        {0x00, convertCountryCode( countries[SI] )}, 
        {0x20, convertCountryCode( countries[BE] )}, 
        {0x22, convertCountryCode( countries[SE] )}, 
        {0x24, convertCountryCode( countries[RO] )}, 
        {0x00, convertCountryCode( countries[X3] )}, 
        {0x00, convertCountryCode( countries[X4] )}, 
        {0x00, convertCountryCode( countries[LX] )}
      };
#endif

#ifdef TRAIN_NORWAY
  TWagon allWagons[ NR_OF_WAGONS ] = { 
        {0x00, convertCountryCode( countries[L1] )},
        {0x00, convertCountryCode( countries[IT] )}, 
        {0x00, convertCountryCode( countries[NO] )}, 
        {0x00, convertCountryCode( countries[IR] )}, 
        {0x00, convertCountryCode( countries[HR] )}, 
        {0x00, convertCountryCode( countries[LT] )}, 
        {0x00, convertCountryCode( countries[DE] )}, 
        {0x00, convertCountryCode( countries[DK] )}, 
        {0x00, convertCountryCode( countries[PT] )}, 
        {0x00, convertCountryCode( countries[CH] )}, 
        {0x00, convertCountryCode( countries[X1] )}, 
        {0x00, convertCountryCode( countries[X2] )}, 
        {0x00, convertCountryCode( countries[NL] )}, 
        {0x00, convertCountryCode( countries[TR] )}, 
        {0x00, convertCountryCode( countries[AT] )}, 
        {0x00, convertCountryCode( countries[PL] )}, 
        {0x00, convertCountryCode( countries[FR] )}, 
        {0x00, convertCountryCode( countries[SK] )}, 
        {0x00, convertCountryCode( countries[SI] )}, 
        {0x00, convertCountryCode( countries[BE] )}, 
        {0x00, convertCountryCode( countries[SE] )}, 
        {0x00, convertCountryCode( countries[RO] )}, 
        {0x00, convertCountryCode( countries[X3] )}, 
        {0x00, convertCountryCode( countries[X4] )}, 
        {0x00, convertCountryCode( countries[LX] )}
      };
#endif


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