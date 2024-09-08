/*-------------------------------------------------------------------------------------------------


       /////// ////// //////  //////   /////     /////    ////  //    //
         //   //     //   // //   // //   //    //  //  //   // // //
        //   ////   //////  //////  ///////    /////   //   //   //
       //   //     //  //  // //   //   //    //   // //   //  // //
      //   ////// //   // //   // //   //    //////    ////  //   //

     
                 A R D U I N O   D I S T A N C E  S E N S O R S


                 (C) 2024, C. Hofman - cor.hofman@terrabox.nl

                   <A02_UART.h> - Library for GUI widgets.
                                 24 Aug 2024
                       Released into the public domain
               as GitHub project: TerraboxNL/TerraBox_Scheduler
                   under the GNU General public license V3.0
                          
      This program is free software: you can redistribute it and/or modify
      it under the terms of the GNU General Public License as published by
      the Free Software Foundation, either version 3 of the License, or
      (at your option) any later version.

      This program is distributed in the hope that it will be useful,
      but WITHOUT ANY WARRANTY; without even the implied warranty of
      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
      GNU General Public License for more details.

      You should have received a copy of the GNU General Public License
      along with this program.  If not, see <https://www.gnu.org/licenses/>.

 *---------------------------------------------------------------------------*
 *
 *  C H A N G E  L O G :
 *  ==========================================================================
 *  P0001 - Initial release 
 *  ==========================================================================
 *
 *--------------------------------------------------------------------------*/
#include <Arduino.h>
#include <DistanceSensor.h>

#ifndef A02_UART_H_
#define A02_UART_H_


/*==================================================================================================
 *
 *    A02  UART  version
 *
 *================================================================================================*/
#define MEASUREMENT_TIMEOUT         900      // A02_UART normally delivers a message between 100-500 ms.
#define MAX_BYTES_TO_READ           12        // Max bytes to read in finding a 0xff header byte


/*=================================================================================================
 *  A 0 2  U A R T  - Base class for UART based distance sensors
 *===============================================================================================*/
class A02_UART : public DistanceSensor {
  protected:
    HardwareSerial* serial;

  public:
    long maxElapsed = 0;
    long minElapsed = 999999;

    A02_UART(char *name, HardwareSerial* serial); // Create A02 driver using a specific Serial device
    A02_UART(char *name, uint32_t cycleTime, HardwareSerial* serial); // Create A02 driver using a specific Serial device
    A02_UART(char *name);                         // Create A02 driver using Serial1
    A02_UART(char *name, uint32_t cycleTime);     // Create A02 driver using Serial1
//    virtual ~A02_UART();

//    virtual void begin();
    void exec();
//    virtual void end();

    void            flushBuffer();                  // Flushes the input buffer
    int16_t         readFromBuffer(uint8_t* data);  // Extracts the last buffered message
    int16_t         readFromSensor(uint8_t* data);  // Extracts 1st incoming message from the snesor
    virtual int16_t registerDistance();             // Handles protocol and returns distance.
                                                    // >0 is distance,
                                                    // <0 is error.
    virtual int16_t readDistance();                 // Receives and decodes the distance message.
                                                    // >0 implies ok. The number represents the distance
                                                    // <0 implies some error.
    virtual int16_t getDistance();                  // Returns the distance after exec has run.
};

#endif

