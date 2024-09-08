/*-------------------------------------------------------------------------------------------------


       /////// ////// //////  //////   /////     /////    ////  //    //
         //   //     //   // //   // //   //    //  //  //   // // //
        //   ////   //////  //////  ///////    /////   //   //   //
       //   //     //  //  // //   //   //    //   // //   //  // //
      //   ////// //   // //   // //   //    //////    ////  //   //

     
                 A R D U I N O   D I S T A N C E  S E N S O R S


                 (C) 2024, C. Hofman - cor.hofman@terrabox.nl

               <A02_UART_Auto.h> - Library for GUI widgets.
                                 01 Aug 2024
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
#include <A02_UART.h>

#ifndef A02_UART_AUTO_H_
#define A02_UART_AUTO_H_

/*=================================================================================================
 *  A 0 2  U A R T  A u t o - Base class for UART Auto based distance sensors
 *                            UART implies a serial protocol.
 *                            Auto implies distance messages are send at regular intervals
 *===============================================================================================*/
class A02_UART_Auto : public A02_UART {

  public:
	A02_UART_Auto(char *name);
	A02_UART_Auto(char *name, uint32_t cycleTime);
//	virtual ~A02_UART_Auto();

    virtual void begin();                // Initialises protocol for the UART send only protocol
//    virtual void exec();                 // Kick off a single measurement every x ms.
    virtual void end();

    virtual int16_t registerDistance();  // Delegates protocol, monitors wake up needs
                                         // and returns distance or an error code.
                                         // >0 is distance,
                                         // <0 is error code.
//    virtual int16_t readDistance();      // Receives and decodes the distance message.
                                         // >0 implies ok. The number represents the distance
                                         // <0 implies some error.
};

#endif

