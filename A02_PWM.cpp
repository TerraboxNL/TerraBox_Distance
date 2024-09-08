/*-------------------------------------------------------------------------------------------------


       /////// ////// //////  //////   /////     /////    ////  //    //
         //   //     //   // //   // //   //    //  //  //   // // //
        //   ////   //////  //////  ///////    /////   //   //   //
       //   //     //  //  // //   //   //    //   // //   //  // //
      //   ////// //   // //   // //   //    //////    ////  //   //

     
                 A R D U I N O   D I S T A N C E  S E N S O R S


                 (C) 2024, C. Hofman - cor.hofman@terrabox.nl

               <A02_PWM.cpp> - Library for GUI distance sensors.
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
#include "A02_PWM.h"

/*--------------------------------------------------------------------------------------------------
 *
 *  Create the Pulse Width Modulation variant for the A02 sensor.
 *  Because the request handling is the same as the UART Controlled version,
 *  this is inherited from the A02_UART_Controlled class.
 *  The only thing differing is the actual reading of the measurement.
 *  Instead of a serial message in the UART Controlled, a PWM value must be read.
 *  The wakeUp and the measurement request are exactly the same.
 *
 *------------------------------------------------------------------------------------------------*/
A02_PWM::A02_PWM(char * name, byte pwmRequestPin, byte pPwmPin) :
         A02_UART_Controlled(name, pwmRequestPin) {
	pwmPin = pPwmPin;

	//
	//  Assign the PWM input pin
	//
	pinMode(pwmPin, INPUT);
}

/*--------------------------------------------------------------------------------------------------
 *
 *  Read the PWM input for the distance.
 *
 *------------------------------------------------------------------------------------------------*/
int16_t A02_PWM::readDistance() {

  return pulseIn(pwmPin, HIGH);    // Just read the PWM input

}

