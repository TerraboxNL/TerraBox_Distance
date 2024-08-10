/*-------------------------------------------------------------------------------------------------


       /////// ////// //////  //////   /////     /////    ////  //    //
         //   //     //   // //   // //   //    //  //  //   // // //
        //   ////   //////  //////  ///////    /////   //   //   //
       //   //     //  //  // //   //   //    //   // //   //  // //
      //   ////// //   // //   // //   //    //////    ////  //   //

     
                 A R D U I N O   D I S T A N C E  S E N S O R S


                 (C) 2024, C. Hofman - cor.hofman@terrabox.nl

                  <A02_UART.cpp> - Library for GUI widgets.
                     Created by Cor Hofman, 01 Aug 2024
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
#include <A02_UART.h>

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
    Serial.print(F("Distance: ")); Serial.print(distance); Serial.println(F(" mm."));
  }

  //
  //  Otherwise print a message telling us what was wrong.
  //
  else {
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
 *------------------------------------------------------------------------------------------------*/
int16_t A02_UART::readDistance() {      // Receives the requested distance measurement message.

  uint8_t hByte = 0x00;

  //
  // Wait inly a limited amount of time (i.e. MEASUREMENT_TIMEOUT)
  // If nothing arrives within the timeout, return an error code.
  //
  long now = millis();

  //
  // Wait while no message byte is available
  //
  while (!serial->available()) {

    //
    // If time passed is greater than MEASUREMENT_TIMEOUT, then return error code
    //
    if (millis() - now > MEASUREMENT_TIMEOUT)
      return -1;  // Indicate false measurement
  }

  //
  // Try to synchronize on header byte 0xff
  // but the number of bytes synchronise on is limited to MAX_BYTES_TO_READ
  // 
  int byteCount = 0;

  //
  // While permitted try to synchronise by recognising the 0xff header byte
  //
  while (1) {
    
    //
    // If a byte has arrived and it is 0xff, then break out of the loop
    if (serial->available()) {
      hByte = serial->read();
//      Serial.print("byte: 0x"); Serial.println(hByte, HEX);
      if (hByte == 0xff) {
        break;  // Break out of the surrounding while (1) loop
      }

      //
      // Otherwise increase the number of bytes used synchronising, but failing.
      //
      byteCount++;

      //
      // If number of bytes used to synchronise is greater than MAX_BYTE_TO_READ
      // return an error code for it.
      //
      if (byteCount >= MAX_BYTES_TO_READ) {
        // Serial.print("No header byte containing 0xff was found.");
        return -3;  // Time out occurred, while waiting for data bytes to arrive.
      }
    }
  }

  //
  // Create data buffer for the 4 message bytes.
  //
  unsigned char data_buffer[4] = {0};

  // Serial.println("Data available @2.");
  //
  // Insert header into array
  //
  data_buffer[0] = 0xff;

  //
  // Read remaining 3 characters of data and insert into array
  //
  for (int i = 1; i < 4; i++) {
    data_buffer[i] = serial->read();
  }

//  Serial.print("[0] 0x"); Serial.println(data_buffer[0], HEX);
//  Serial.print("[1] 0x"); Serial.println(data_buffer[1], HEX);
//  Serial.print("[2] 0x"); Serial.println(data_buffer[2], HEX);
//  Serial.print("[3] 0x"); Serial.println(data_buffer[3], HEX);

  //
  //Compute checksum
  //
  unsigned char CS = (data_buffer[0] + data_buffer[1] + data_buffer[2]) & 0x00FF;
//  Serial.print(" CS 0x"); Serial.println(CS, HEX);
//  Serial.println();

  //
  // If checksum is invalid return the error code for it.
  //
  if (data_buffer[3] != CS) {
    return -2;  // Used to many bytes to synchronise, which therefore failed.
  }

  // Serial.println("Data available @3.");
  //
  //  Decode the distance from the message using the high and low data bytes
  //
  int16_t d = (data_buffer[1] << 8) + data_buffer[2];

  //
  // Print to serial monitor
  //
  //Serial.print("distance: ");
  //Serial.print(distance);
  //Serial.println(" mm");

  //
  // Return the decoded distance
  //
  return d;

}
