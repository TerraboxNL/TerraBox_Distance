/*-------------------------------------------------------------------------------------------------


       /////// ////// //////  //////   /////     /////    ////  //    //
         //   //     //   // //   // //   //    //  //  //   // // //
        //   ////   //////  //////  ///////    /////   //   //   //
       //   //     //  //  // //   //   //    //   // //   //  // //
      //   ////// //   // //   // //   //    //////    ////  //   //

     
                 A R D U I N O   D I S T A N C E  S E N S O R S


                 (C) 2024, C. Hofman - cor.hofman@terrabox.nl

                 <A02_UART_Auto.cpp> - Library for GUI widgets.
                     Created by Cor Hofman, 01-Aug-2026
                       Released into the public domain
                     as GitHub project: TerraBox_Scheduler
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
#include <A02_UART_Auto.h>

/*-------------------------------------------------------------------------------------------------
 *
 *  Create a sensor driver for A02 Auto mode.
 *
 *-----------------------------------------------------------------------------------------------*/
A02_UART_Auto::A02_UART_Auto(char *name) :
               A02_UART(name) {

	serial = &Serial1;

}

A02_UART_Auto::A02_UART_Auto(char *name, uint32_t cycleTime) :
               A02_UART(name, cycleTime) {

	serial = &Serial1;

}

void A02_UART_Auto::begin() {
//  Serial.println("Setting to 9600 Baud");
  serial->begin(9600);
  serial->write(1);
}

void A02_UART_Auto::exec() {
  distance = registerDistance();
}

void A02_UART_Auto::end() {
//  serial->end();
}

/*-------------------------------------------------------------------------------------------------
 *
 *  Read A02 sensor in UART Auto mode
 *
 *-----------------------------------------------------------------------------------------------*/
int16_t A02_UART_Auto::registerDistance() {
  //
  //  Initialize the serial interface
  //  avoiding getting polution of unneeded buffered messages
  //
//  serial->begin(9600);
//  serial->write(1);
//  delay(200);
  while (serial->available() > 7){
	  serial->read();
//	  Serial.println(serial->read(), HEX); // Eat away any buffered characters
  }

  //
  //  Read a single distance measurement
  //
  int16_t d = readDistance();

  //
  //  Close the Serial interface.
  //  This avoids getting buffered messages
  //
  // serial->end();

  //
  //  Return the distance read.
  //
  return d;
}
