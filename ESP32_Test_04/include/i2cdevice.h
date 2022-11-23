#ifndef i2cdevice_h
#define i2cdevice_h

/**
 * @file i2cdevice.h
 * @author M.Busser (m.busser@bbstghs.de)
 * @brief This is a small library to combine bit oriented access to a PCF8575 port expander
 *        as well as byte oriented access.
 *        We only use the port expander as an output device, so all other functions like input
 *        or interrupts are not supported here.
 * 
 * @version 0.1
 * @date 2022-11-23
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "Arduino.h"
#include "Wire.h"

#define SDA 22
#define SCL 21

#define I2CDev_NrOfPorts 2


class I2CDevice
{
  public:
    /**
     * @brief Construct a new I2CDevice object
     * 
     * @param addr  The I2C-Adress of the device. For PCF8575, the valid adresses are 0x20..0x27 
     */
    I2CDevice( uint8_t addr );
    /**
     * @brief This function initializes the Wire library and join the I2C bus as a controller or a peripheral. 
     *        This function will call Wire.begin() only once, even if it is called multiple times.
     *        see:  https://www.arduino.cc/reference/en/language/functions/communication/wire/begin/
     */
    void begin();
    
    void begin( uint8_t sda, uint8_t scl );
    /**
     * @brief Inverts the bit pattern before sending it to the PCF8575, so writing a 1 to the device
     *        will light up an LED, connected to VCC
     * 
     * @param inv true = bit pattern will be inverted bevore sending
     *            false= no inversion 
     */
    void invertBeforeTransmission( bool inv );      
    /**
     * @brief Set a byte value to the internal buffer.
     *        A PCF8575 has 2 ports whitch means 2 bytes.
     * 
     * @param index [0|1]   Byte adress
     * @param value Value of the byte
     */
    void set( uint8_t index, uint8_t value );
    /**
     * @brief Reading a byte from the internal buffer
     * 
     * @param index [0|1]   Byte adress
     * @return * uint8_t Value of the byte
     */
    uint8_t get( uint8_t index );
    /**
     * @brief Transmitts the complete buffer to the device in one transmission.
     * 
     */
    void write( );


    //-----Bit oriented methods
    void pinMode(uint8_t pin, uint8_t mode);           
    void digitalWrite(uint8_t pin, uint8_t value);
    
   
  private:
    TwoWire *_wire;
    uint8_t _sda;
    uint8_t _scl;
    uint8_t _addr;
    uint8_t _ports[ I2CDev_NrOfPorts ]; 
    bool    _invert;

    static uint8_t I2CDev_Counter;

};



#endif