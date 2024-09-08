/*-------------------------------------------------------------------------------------------------


       /////// ////// //////  //////   /////     /////    ////  //    //
         //   //     //   // //   // //   //    //  //  //   // // //
        //   ////   //////  //////  ///////    /////   //   //   //
       //   //     //  //  // //   //   //    //   // //   //  // //
      //   ////// //   // //   // //   //    //////    ////  //   //

     
                 A R D U I N O   D I S T A N C E  S E N S O R S


                 (C) 2024, C. Hofman - cor.hofman@terrabox.nl

               A02_UART_Controlled.h> - Library for GUI widgets.
                                 01 Aug 2024
                       Released into the public domain
                as GitHub project: TerraboxNL/TerraBox_Distance
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
#include <A02_UART.h>

#ifndef A02_UART_CONTROLLED_H_
#define A02_UART_CONTROLLED_H_

/*==================================================================================================
 *
 *    A02  UART  CONTROLLED version
 *
 *================================================================================================*/
//#define A02_UART_CONTROLLED 1      // Implemented by the A02_UART_CONTROLLED class
//#define A02_UART_AUTO       2      // Implemented by the A02_UART_AUTO class
//#define A02_UART_PWM        3      // No class needed just read a PWM enabled input

#define WAKE_UP_CALL_DURATION       2100     // Have wake up all of WAKE_UP_CALL_DURATION ms.


/*=================================================================================================
 *  A 0 2  U A R T  D i s t a n c e - Base class for UART controlled based distance sensors
 *                                    UART implies a serial protocol.
 *                                    Controlled implies the presence of request/reply mechanism
 *===============================================================================================*/
class A02_UART_Controlled : protected A02_UART {
  private:
    int     needWakeUpCall = 1;          // Needs to be 1 to force an initial wake up call.
    long    cycleTime      = CYCLE_TIME; // Needs to set a CYCLE_TIME. Cycle time may get adapted.
    int     failureCount   = 0;          // As name suggests, it counts protocol failures

  protected:
    byte    requestPin     = -1;         // If positive requestPin is used as Arduino Tx pin

  public:
    A02_UART_Controlled(char *name);                 // Uses Serial1 as default stream
    A02_UART_Controlled(char *name, HardwareSerial* serial);   // Specifies the stream to use
    A02_UART_Controlled(char *name, byte requestPin);  // Uses a specific pin as request

    virtual void begin();                // Intialises protocol for the UART request/reply protocol
    virtual void end();
    virtual void exec();                 // Kicks off a single measurement every x ms.

    virtual int16_t registerDistance();  // Delegates protocol, monitors wake up needs
                                         // and returns distance or an error code.
                                         // >0 is distance,
                                         // <0 is error code.
    virtual void wakeUpCall();           // Protocol: Issues a wake up request
    virtual void requestDistance();      // Protocol: Send a "get measurement" request
    virtual int  measureDistance();      // Orchestrates protocol: wakeUp, Request, Receive
    virtual int  readDistance();         // Receives and decodes the distance message.
                                         // >0 implies ok. The number represents the distance
                                         // <0 implies some error.
};

#endif

