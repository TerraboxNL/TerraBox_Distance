/*-------------------------------------------------------------------------------------------------


       /////// ////// //////  //////   /////     /////    ////  //    //
         //   //     //   // //   // //   //    //  //  //   // // //
        //   ////   //////  //////  ///////    /////   //   //   //
       //   //     //  //  // //   //   //    //   // //   //  // //
      //   ////// //   // //   // //   //    //////    ////  //   //

     
                 A R D U I N O   D I S T A N C E  S E N S O R S


                 (C) 2024, C. Hofman - cor.hofman@terrabox.nl

                   <A02_PWM.h> - Library for GUI widgets.
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
#include <A02_UART_Controlled.h>

#ifndef A02_PWM_H_
#define A02_PWM_H_

/*==================================================================================================
 *
 *    A02  PWM version
 *
 *================================================================================================*/
#define PWM_REQUEST_PIN              25  // Arduino Tx / A02 Rx pin
#define PWM_INPUT_PIN                45  // Arduino Rx / A02 Tx pin

/*=================================================================================================
 *  A 0 2  P W M - Base class for UART Controlled PWM controlled distance sensors
 *                 PWM implies that the reply is a PWM encoded distance.
 *===============================================================================================*/
class A02_PWM : protected A02_UART_Controlled {
  private:
    byte pwmPin;
    
  public:
    A02_PWM(char *name, byte requestPin, byte pPwmPin);
    virtual void begin();
    virtual void end();
    virtual void exec();

    //
    //  Inherited methods from A02_UART_Controlled
    //  Since the same trigger protocol with wakeUp and Request triggers are used.
    //
    virtual void    wakeUpCall();        // Issues a wake up sequence
    virtual void    requestDistance();   // Send a request to receive a measurement
    virtual int16_t registerDistance();  // Top-level method, monitors wakeUp needs and returns distance
    virtual int16_t measureDistance();   // Handles the protocol: wakeUp, Request, Receive

    //
    //  Override from A02_UART
    //
    int16_t readDistance();               // Receives the PWM coded measurement and decodes it.
};

#endif

