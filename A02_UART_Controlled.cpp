/*-------------------------------------------------------------------------------------------------


       /////// ////// //////  //////   /////     /////    ////  //    //
         //   //     //   // //   // //   //    //  //  //   // // //
        //   ////   //////  //////  ///////    /////   //   //   //
       //   //     //  //  // //   //   //    //   // //   //  // //
      //   ////// //   // //   // //   //    //////    ////  //   //

     
                 A R D U I N O   D I S T A N C E  S E N S O R S


                 (C) 2024, C. Hofman - cor.hofman@terrabox.nl

             <A02_UART_Controlled.cpp> - Library for GUI widgets.
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
#include <A02_UART_Controlled.h>

/*-------------------------------------------------------------------------------------------------
 *
 *  Specify some other already initialized (!) Serial device to be used.
 *  NOTE: The stream passed must be initialized!!!.
 *        I.e. for Serial, Serial1, etc. the begin() must already be executed !!
 *
 *  serial      The address of the serial device to use. Serial,
 *              Serial1 .. Serial3, SoftwareSerial. Note it must be
 *              initialized at 9600 Baud for A02 sensors.
 *
 *-----------------------------------------------------------------------------------------------*/
A02_UART_Controlled::A02_UART_Controlled(char *name, HardwareSerial* serial) :
                     A02_UART (name, serial) {
  // Does nothing further
}

/*-------------------------------------------------------------------------------------------------
 *
 *  Use Serial1 to handle request/reply
 *
 *  pRequestPin      The Digital Output pin to use for requests
 *
 *-----------------------------------------------------------------------------------------------*/
A02_UART_Controlled::A02_UART_Controlled(char *name) :
                     A02_UART (name) {
  // Does nothing further
}

/*-------------------------------------------------------------------------------------------------
 *
 *  Use dedicated output pin, instead of Tx pin to send requests
 *
 *  pRequestPin      The Digital Output pin to use for requests
 *
 *-----------------------------------------------------------------------------------------------*/
A02_UART_Controlled::A02_UART_Controlled(char *name, byte pRequestPin) :
                     A02_UART (name) {

  requestPin = pRequestPin;

}

/*-------------------------------------------------------------------------------------------------
 *
 *  Initialize the handling of the A02 distance sensor.
 *
 *-----------------------------------------------------------------------------------------------*/
void A02_UART_Controlled::begin() {
    //
    //  Initialize the Serial port connected to the A02, which always operates at 9600 baud
	//
    serial->begin(9600);

    //
    // Make Arduino Tx / A02 Rx pin high, for now permanently until triggered
    // either for a wakeup or a request trigger
    //
    if (requestPin > 0) {
      digitalWrite(requestPin, HIGH);
    }
    else {
      serial->write(1);
    }

    setCycleTime(CYCLE_TIME);
}


/*--------------------------------------------------------------------------------------------------
 *
 *  Issue a wake up call to the distance sensor.
 *
 *------------------------------------------------------------------------------------------------*/
void A02_UART_Controlled::wakeUpCall() {
    Serial.println(F("Wake up"));

    //
    // Make Arduino Tx / A02 Rx pin low
    //
    if (requestPin > 0) {
      digitalWrite(requestPin, LOW);
    }
    else {
      serial->write((unsigned char)0);
    }
 
    //
    // Have a low/high/low pulse of at least WAKE_UP_CALL_DURATION ms.
    //
    delay(WAKE_UP_CALL_DURATION);  

    //
    // Make Arduino Tx / A02 Rx pin high
    //
    if (requestPin > 0) {
      digitalWrite(requestPin, HIGH);
    }
    else {
      serial->write(1);
    }
}

/*-------------------------------------------------------------------------------------------------
 *
 *  Ask the sensor to report the current distance.
 *
 *-----------------------------------------------------------------------------------------------*/
void A02_UART_Controlled::requestDistance() {
    Serial.println(F("Request"));

    //
    // Make Arduino Tx / A02 Rx pin low
    //
    if (requestPin > 0) {
      digitalWrite(requestPin, LOW);
    }
    else {
      serial->write((unsigned char)0);
    }
 

    delay(80); // Delay at least 70 ms, 80 ms to be safe

    //
    // Make Arduino Tx / A02 Rx pin high
    //
    if (requestPin > 0) {
      digitalWrite(requestPin, HIGH);
    }
    else {
      serial->write(1);
    }
}

/*-------------------------------------------------------------------------------------------------
 *
 *  Return the measured distance. A return < 0 implies no distance was successfully measured.
 *
 *-----------------------------------------------------------------------------------------------*/
int A02_UART_Controlled::measureDistance() {

  //
  // If a wakeup call is needed, then give one
  //
  Serial.print(F("needWakeUpCall: ")); Serial.println(needWakeUpCall);
  if (needWakeUpCall) {
    wakeUpCall();
  }

  //
  //  Request a single distance measurement
  //
  requestDistance();

  //
  //  Read the measured distance and return it
  //  This calls the inherited method from A02_UART
  //
  return readDistance();
}

/*--------------------------------------------------------------------------------------------------
 *
 *  Return the measured distance. If measuring did not succeed a value < 0 is returned.
 *
 *------------------------------------------------------------------------------------------------*/
int16_t A02_UART_Controlled::registerDistance() {

  long now = millis();  // Remember when we started

  //
  //  Measure the distance
  //
  int distance = measureDistance();

  //
  //  Check no errors occurred
  //  A negative distance implies an error
  //
  if (distance > 0) {
    needWakeUpCall = 0;   
  }
  else {
    Serial.print(F("FailureCount: ")); Serial.println(failureCount);
    if (failureCount++ > 10) {
      needWakeUpCall = 1;
      failureCount = 0;
    }
  }

  //
  //  The cycle time only applies to measuring the distance
  //  So only adjust it if only the distance was measured.
  //  Exclude the situation where a wake up call was made.
  //
  int aDelay = 0;
  if (! needWakeUpCall) {
    aDelay = getCycleTime() - (int)(millis()-now);
    if ((aDelay = getCycleTime() - (millis()-now)) > 1) {
      delay(aDelay);  // Delay for the rest of the cycleTime.
    }
    else {
      Serial.println();
      Serial.print(F("Cycle time ")); Serial.print(cycleTime); 
      Serial.println(F("ms is too fast !!!"));
      setCycleTime(getCycleTime() + 100);  // Increase the cycle time
      Serial.print(F("New cycle time ")); Serial.println(cycleTime);
    }
  }

  return distance;
}

