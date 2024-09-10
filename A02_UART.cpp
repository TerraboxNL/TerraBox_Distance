/*-------------------------------------------------------------------------------------------------


       /////// ////// //////  //////   /////     /////    ////  //    //
         //   //     //   // //   // //   //    //  //  //   // // //
        //   ////   //////  //////  ///////    /////   //   //   //
       //   //     //  //  // //   //   //    //   // //   //  // //
      //   ////// //   // //   // //   //    //////    ////  //   //

     
                 A R D U I N O   D I S T A N C E  S E N S O R S


                 (C) 2024, C. Hofman - cor.hofman@terrabox.nl

                  <A02_UART.cpp> - Library for GUI widgets.
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

#define TRACE 0
/*------------------------------------------------------------------------------------------------
 *
 *  Default UART type using Serial1
 *
 *----------------------------------------------------------------------------------------------*/
A02_UART::A02_UART(char *name) :
          DistanceSensor(name) {
  serial = &Serial1;
}

/*------------------------------------------------------------------------------------------------
 *
 *  Default UART type using Serial1
 *
 *----------------------------------------------------------------------------------------------*/
A02_UART::A02_UART(char *name, uint32_t cycleTime) :
          DistanceSensor(name, cycleTime) {
  serial = &Serial1;
}

/*------------------------------------------------------------------------------------------------
 *
 *  Specifiable UART type
 *
 *    -----------------------
 *    Class            Tx  Rx
 *    -----------------------
 *    Serial            1   0
 *    Serial1          18  19 
 *    Serial2          16  17 
 *    Serial3          14  15 
 *    SoftwareSerial          
 *
 *----------------------------------------------------------------------------------------------*/
A02_UART::A02_UART(char *name, HardwareSerial* pSerial) :
          DistanceSensor(name) {
  serial = pSerial;
}

/*------------------------------------------------------------------------------------------------
 *
 *  Like A02_UART(name, pSerial) only with an additional cycleTime argument.
 *
 *----------------------------------------------------------------------------------------------*/
A02_UART::A02_UART(char *name, uint32_t cycleTime, HardwareSerial* pSerial) :
          DistanceSensor(name, cycleTime) {
  serial = pSerial;
}
/*-------------------------------------------------------------------------------------------------
 *
 *  Get a single distance measurement, representing the last measured distance.
 *  Or handle the resulting error code.
 *
 *-----------------------------------------------------------------------------------------------*/
void A02_UART::exec() {

  //
  //  Execute Distance Measurement task if needed
  //
  distance = registerDistance();

  //
  //  If the measurement was valid, the print it
  //
  if (distance > 0) {
 //   Serial.print(distance); Serial.println(F(" mm A02_UART::exec()"));
  }

  //
  //  Otherwise print a message telling us what was wrong.
  //
  else {
	Serial.print(F("A02_UART::exec(): "));
    switch (distance) {
      case -1:
        Serial.println(F("Distance: ERROR -- Sensor did not report the distance (in time)."));
        Serial.print(F("Distance: Information -- MEASUREMENT_TIMEOUT is set at: ")); Serial.print(MEASUREMENT_TIMEOUT); Serial.println(F(" ms."));
        break;

      case -2:
        Serial.println(F("Distance: ERROR -- Checksum error"));
        break;

      case -3:
        Serial.println(F("Distance: ERROR -- Corrupted message header. The fist byte did not contain 0xFF."));
        break;

      default:
        Serial.print(F("Distance: ERROR -- Unknown error code: ")); Serial.println(distance);
    }
  }
}

int16_t A02_UART::getDistance() {
  return distance;
}

/*-----------------------------------------------------------------------------
 *
 *  Flushes the input buffer as fast as possible
 *
 *---------------------------------------------------------------------------*/
void A02_UART::flushBuffer() {
  int16_t errNo  = 0;   // Success

  #if TRACE_TIMES
  long now       = 0;
  long elapsed   = 0;
  #endif


  #if TRACE
  Serial.println();
  #endif

  int16_t c = 0;

  //
  //  Flush the input buffer if there is buffered input...
  //
  if (Serial1.available()) {

    #if TRACE
    Serial.println(F("FLUSHING ...."));
    #endif
    #if TRACE_TIMES
    now = micros();
    #endif

    //
    //  Empty the buffer and extract the complete message from it
    //
    int x = 0;
    while ((c = Serial1.read()) >= 0 && Serial1.available()) {
      x++;
    }

    #if TRACE_TIMES
    elapsed = micros() - now;
    #endif
    #if TRACE_TIMES
    Serial.print(F("]]]] FLUSHED [[[[ in "));Serial.print(elapsed);
    Serial.print(F(" micros. "));
      #if TRACE
    Serial.print(x); Serial.println(F(" bytes"));
      #endif
    #elif TRACE
    Serial.print(F("]]]] FLUSHED [[[[ "));Serial.print(x); Serial.println(F(" bytes"));
    #endif
  }

}

/*-----------------------------------------------------------------------------
 *
 *  Flushes the input buffer and extracts the last complete message
 *
 *  data      A 4 byte data buffer, which returns the last received message
 *
 *---------------------------------------------------------------------------*/
int16_t A02_UART::readFromBuffer(uint8_t* data) {

  #if TRACE_TIMES
  long now       = 0;
  long elapsed   = 0;
  #endif


  #if TRACE
  Serial.println();
  #endif

  int16_t c = 0;

  #if TRACE
  Serial.println(F("== START ==>"));
  #endif

  //
  //  Flush the input buffer if there is buffered input...
  //
  if (Serial1.available()) {

    #if TRACE
    Serial.println(F("FLUSHING ...."));
    #endif
    #if TRACE_TIMES
    now = micros();
    #endif

    uint16_t x    = 0;
    int bufferIdx = 0;
    uint8_t msg1[4];
    uint8_t msg2[4];
    uint8_t* last      = msg1;
    uint8_t* current   = msg2;
    uint8_t* completed = nullptr;
    uint8_t* tmp       = nullptr;

    //
    //  Empty the buffer and extract the complete message from it
    //
    while ((c = Serial1.read()) >= 0 && Serial1.available()) {

 //     Serial.print(F("Read ["));Serial.print(bufferIdx);Serial.print(F("] 0x")); Serial.println((uint8_t)c, HEX);

      if (c == 0xff) {
        //
        // Only switch buffers if completed
        //
        if (bufferIdx == 4) {
          // completed = nullptr
          // current   = msg1
          // last      = msg2
          tmp       = current;   // tmp = msg1
          current   = last;      // current = msg2
          last      = tmp;       // last = msg1
          // completed = nullptr
          // current   = msg2
          // last      = msg1
        }
        bufferIdx = 0;         // Reset the buffer index

        //
        //  Clear current buffer
        //
        for (int i = 0; i < 4; i++)
          current[i] = 0;

        current[bufferIdx++] = (uint8_t)c;
      }
      else {
        current[bufferIdx++] = (uint8_t)c;

        if (bufferIdx == 4) {
          // completed = nullptr
          // current   = msg2
          // last      = msg1
          completed = current;  // completed = msg2
          // completed = msg2
          // current   = msg2
          // last      = msg1
        }
      }

      x++;
    }

    #if TRACE
    Serial.println(F("Completed message: "));
    for (int i = 0; i < 4; i++) {
      Serial.print(F("["));Serial.print(i);Serial.print(F("] 0x"));Serial.println(completed[i], HEX);
    }
    Serial.println();
    #endif

   #if TRACE_TIMES
    elapsed = micros() - now;
    #endif
    #if TRACE_TIMES
    Serial.print(F("]]]] FLUSHED [[[[ in "));Serial.print(elapsed);
    Serial.print(F(" micros. "));
      #if TRACE
    Serial.print(x); Serial.println(F(" bytes"));
      #endif
    #elif TRACE
    Serial.print(F("]]]] FLUSHED [[[[ "));Serial.print(x); Serial.println(F(" bytes"));
    #endif

    //
    //  If there is a completed message, then use that
    //
    if (completed) {

      //
      // Copy the message to the return buffer
      //
      #if TRACE
      Serial.println(F("Copied buffer:"));
      #endif

      for (int i = 0; i < 4; i++) {
        data[i] = completed[i];
        #if TRACE
         Serial.print(F("[")); Serial.print(i); Serial.print(F("] 0x")); Serial.println(data[i], HEX);
        #endif
     }

      #if TRACE
      Serial.println(F("<== END =="));
      #endif

      //
      //  Calculate the checksum.
      //  If not equal to the message checksum,
      //  then return a 02 error code.
      //
      uint8_t CS = (uint8_t)(data[0] + data[1] + data[2]) & 0x00FF;
      if (CS != completed[3])
        return -2;

      //
      //  Checksum is OK
      //  Calculate the distance and return that
      //
      return ((uint16_t)data[1] << 8) + (uint16_t)data[2];
    }
  }

  //
  // If no complete message was buffered, then return a zero.
  // I.e. nothing there.
  //
  return 0;
}


/*--------------------------------------------------------------------------------------------------
 *
 *  Low level A02 distance message reader of UART type sensor.
 *
 *  byte: 0        1           2          3
 *     .------+-----------+----------+----------.
 *     | 0xff | high byte | low byte | checksum |
 *     `------+-----------+----------+----------'
 *
 *  byte 0: Header byte, which is always 0xff. Used to recognise the start of message
 *  byte 1: High byte of 16 bit distance in mm.
 *  byte 2: Low byte of 16 bit distance in mm.
 *  byte 3: checksum = (byte 0 + byte 1 + byte 2) & 0x00ff
 *
 *  Return codes:
 *  >0   A valid distance extracted from a measurement message.
 *
 *  <0   No valid measurement message was received, causes are:
 *
 *  -1   Failed to read a distance message within MEASUREMENT_TIMEOUT ms
 *  -2   Corrupted message. Calculated checksum did not match the message checksum.
 *  -3   Read message bytes but a header byte was not found after reading MAX_BYTES_TO_READ
 *
 *  Note 1: The field lastErrCode contains the most recent error code
 *  Note 2: The field lastDistance contains the most recent distance read.
 *
 *------------------------------------------------------------------------------------------------*/
int16_t A02_UART::readDistance() {      // Receives the requested distance measurement message.

	  uint8_t data[4];
	  int16_t dist = 0;

	  long now = millis();

	  //
	  //  First try the buffered messages for distance measurment...
	  //
	  distance  = readFromBuffer(data);
	  #if TRACE
	    Serial.print(F("Distance from buffer message: ")); Serial.println(distance);
	  #endif

	  //
	  //  If no message there, then continue receiving frok the sensor...
	  //  Since the buffer has been emptied we now read what is coming
	  //  from the sensor.
	  //  To be absolutely sure about that readFromSensor() flushes
	  //  the buffer once more without processing what is in the buffer.
	  //
	  if (dist <= 0) {
	    dist = readFromSensor(data);
	  }

	  long elapsed = millis() - now;
	  if (elapsed > maxElapsed)
	    maxElapsed = elapsed;

	  if (elapsed < minElapsed)
	    minElapsed = elapsed;

      #if TRACE
	  Serial.print(F("Distance measurement elapsed time: ")); Serial.print(elapsed); Serial.print(F(" ms, return code: "));Serial.println(distance);
      #endif

	  #if SUMMARY_TIMES
	  Serial.print(F("Elapsed time (min - max): ")); Serial.print(elapsed);Serial.print(F(" ("));Serial.print(minElepased); Serial.print(F(" - "));Serial.print(maxElapsed);Serial.println(F(" )"));
	  Serial.println();
	  #endif

	  //
	  //  Register the last error code or distance
	  //  This can be used to return the last read distance
	  //  in case of an error code
	  //
	  if (dist < 0) {
		  lastErrCode    = dist;
		  lastErrorTS    = millis();
	  }
	  else {
		  lastDistance   = dist;
		  lastDistanceTS = millis();
	  }

	  return dist;
}

/*-----------------------------------------------------------------------------
 *
 *  Reads the first incoming message directly from the sensor.
 *
 *  data      4 byte data buffer in which message is returned
 *
 *  Return:   > 0 the distance measured
 *            < 0 Error code
 *
 *---------------------------------------------------------------------------*/
int16_t A02_UART::readFromSensor(uint8_t* data) {
  int16_t errNo  = 0;   // Success

  #if TRACE
  Serial.println(F("== START ==>"));
  #endif

  //
  //  Flush the buffer contents first, ignoring all previous messages...
  //
  flushBuffer();

  //
  //  Start receiving the latest message...
  //
  #if TRACE_TIMES
  now = millis();
  #endif

  //
  // Wait for new message to arrive of which the first byte is 0xff
  // Since Serial1.read() return -1 int16_t value
  // USE a 16BIT variable with Serial.read(), NOT 8 BIT
  // because the int -1 return value and byte 0x00ff are indistinguishable.
  // Since we need to detect a 0xff byte as start of message header marker
  // a -1 (noting to read) will actually be interpreted as the header marker.
  //
  int16_t c;
  long now      = millis();
  int byteCount = 0;
  while ((c = Serial1.read()) != 0xff) {
    if (c >= 0) {
      //
      // Otherwise increase the number of bytes used.
	  // Worst case 0xff should be spotted at its latest as the 7th byte read.
      //
      byteCount++;

      //
      // If number of bytes used to synchronise is greater than MAX_BYTE_TO_READ
      // return an error code for it.
      //
      if (byteCount >= MAX_BYTES_TO_READ) {
        return -3;  // Time out occurred, while waiting for data bytes to arrive.
      }
    }

	//
	// If time passed is greater than MEASUREMENT_TIMEOUT, then return error code
	//
	if (byteCount == 0 && millis() - now > MEASUREMENT_TIMEOUT)
	  return -1;  // Indicate false measurement

  }

  #if TRACE_TIMES
  elapsed = millis()-now;
  Serial.print(F("Waiting time till 0xff arrives (ms):")); Serial.println(elapsed);
  Serial.println();
  #if TRACE
  Serial.print(F("[0] 0x")); Serial.println(c, HEX);
  #endif
  #elif TRACE
  Serial.println();
  Serial.print(F"[0] 0x")); Serial.println(c, HEX);
  #endif

  //
  //  Store the 0xff message header
  //
  data[0] = (uint8_t)c;

  //
  //  Keep reading until we have a complete message
  //
  while (1) {

    //
    //  Read the remaining 3 message bytes to get
    //  the message body and messahe checksum.
    //
    for (int x = 1; x < 4; x++) {
      //
      //  Wait for the next byte to read
      //
      while ((c = Serial1.read()) == -1) ;

      //
      //  If this is a 0xff, then the message is incomplete, so ABORT.
      //
      if (c == 0xff) {
        #ifdef TRACE
        Serial.print(F("["));Serial.print(x);Serial.print(F("] 0x")); Serial.print((uint8_t)c, HEX); Serial.println(F(", **** ABORT ****  Incomplete message, resync..."));
        #endif
        break;  // incomplete message
      }

      //
      //  Store the data body byte just read
      //
      #if TRACE
      Serial.print(F("[")); Serial.print(x); Serial.print(F("] 0x")); Serial.println(c, HEX);
      #endif
      data[x] = (uint8_t)c;

    } // Message complete

    uint8_t CS = (uint8_t)(data[0] + data[1] + data[2]) & 0x00FF;
    if (CS != data[3])
      errNo = -2;  // So error -2, which is invalid checksum

    break;
  }

  #if TRACE
  Serial.println(F("<== END =="));
  #endif

  if (errNo < 0)
    return errNo;

  return ((uint16_t)data[1] << 8) + (uint16_t)data[2];
}
