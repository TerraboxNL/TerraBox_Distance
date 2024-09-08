/*-------------------------------------------------------------------------------------------------


       /////// ////// //////  //////   /////     /////    ////  //    //
         //   //     //   // //   // //   //    //  //  //   // // //
        //   ////   //////  //////  ///////    /////   //   //   //
       //   //     //  //  // //   //   //    //   // //   //  // //
      //   ////// //   // //   // //   //    //////    ////  //   //

     
                 A R D U I N O   D I S T A N C E  S E N S O R S


                 (C) 2024, C. Hofman - cor.hofman@terrabox.nl

                <DistanceSensor.cpp> - Library for GUI widgets.
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
#include <DistanceSensor.h>
#include <TaskScheduler.h>

DistanceSensor::DistanceSensor(char* name) :
                Task(name) {
  // Does nothing
}

DistanceSensor::DistanceSensor(char* name, uint32_t cycleTime) :
                Task(name, cycleTime) {
  // Does nothing
}
/*
void DistanceSensor::begin() {
  // Does nothing
}

void DistanceSensor::end() {
  // Does nothing
}
*/

void DistanceSensor::exec() {

}

int16_t DistanceSensor::getDistance() {
  return distance;
}

void DistanceSensor::setDistance(uint16_t d) {
	distance = d;
}

