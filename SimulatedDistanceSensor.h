/*-------------------------------------------------------------------------------------------------


       /////// ////// //////  //////   /////     /////    ////  //    //
         //   //     //   // //   // //   //    //  //  //   // // //
        //   ////   //////  //////  ///////    /////   //   //   //
       //   //     //  //  // //   //   //    //   // //   //  // //
      //   ////// //   // //   // //   //    //////    ////  //   //

     
                 A R D U I N O   D I S T A N C E  S E N S O R S


                 (C) 2024, C. Hofman - cor.hofman@terrabox.nl

          <SimulatedDistanceSensor.h> - Library for Distance sensors.
                                 11 Aug 2024
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
#include <DistanceSensor.h>

#ifndef SIMULATED_DISTANCE_SENSOR_H_
#define SIMULATED_DISTANCE_SENSOR_H_


#define SIMU_TYPE_UP          1
#define SIMU_TYPE_DOWN        2
#define SIMU_TYPE_RANDOM      3

class SimulatedDistanceSensor : public DistanceSensor {
  private:
    uint16_t simulation;          // Use one of the SIMU_TYPE_* defines
    int16_t animationLevel = 0;   // The simulated sensor value
    int16_t simulatedLevel = 0;   // The simulated sensor value
    uint16_t min;                 // Minimum sensor value
    uint16_t max;                 // Maximum sensor value

  public:
	SimulatedDistanceSensor(char* name, uint16_t type, uint16_t min, uint16_t max);
	SimulatedDistanceSensor(char* name, uint32_t cycleTime, uint16_t type, uint16_t min, uint16_t max);

	virtual void begin();
	virtual void end();

	virtual int16_t registerDistance();

	virtual void SimulatedDistanceSensor::setInitialTarget();
    virtual void SimulatedDistanceSensor::guardTarget();
	virtual void animate();

};

#endif
