/*-------------------------------------------------------------------------------------------------


       /////// ////// //////  //////   /////     /////    ////  //    //
         //   //     //   // //   // //   //    //  //  //   // // //
        //   ////   //////  //////  ///////    /////   //   //   //
       //   //     //  //  // //   //   //    //   // //   //  // //
      //   ////// //   // //   // //   //    //////    ////  //   //

     
                 A R D U I N O   D I S T A N C E  S E N S O R S


                 (C) 2024, C. Hofman - cor.hofman@terrabox.nl

                 <DistanceSensor.h> - Library for GUI widgets.
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
#include <TaskScheduler.h>

#ifndef DISTANCE_SENSOR_H_
#define DISTANCE_SENSOR_H_

/*=================================================================================================
 *  D i s t a n c e  S e n s o r - Base class for distance sensors
 *===============================================================================================*/
class DistanceSensor : public Task {

  public:
	int16_t distance = 0;                // Has distance after invoking exec().
	                                     // > 0 = valid distance
	                                     // < 0 = sensor type depended error code

	uint32_t lastDistanceTS   = 0;       // The time stamp of the valid distance read
	uint32_t lastErrorTS      = 0;       // The time stamp of last error occurrence
    int16_t  lastDistance     = 0;       // The last valid distance read
    int16_t  lastErrCode      = 0;       // The last error code

    DistanceSensor(char* name);
    DistanceSensor(char* name, uint32_t cycleTime);
//    virtual ~DistanceSensor();

    virtual void begin();                // Initialisation needed before starting the task
    virtual void end();                  // Called if the task needs to be ended
    virtual void exec();                 //

    virtual int16_t getDistance();       // Returns the distance after exec has run.
    virtual int16_t registerDistance();  // Handles protocol and Returns distance.
                                         // >0 is distance,
                                         // <0 is error.
    virtual void    setDistance(uint16_t d);        // Assigns the distance measured
};

#endif

