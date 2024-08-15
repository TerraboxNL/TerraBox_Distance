/*-------------------------------------------------------------------------------------------------


       /////// ////// //////  //////   /////     /////    ////  //    //
         //   //     //   // //   // //   //    //  //  //   // // //
        //   ////   //////  //////  ///////    /////   //   //   //
       //   //     //  //  // //   //   //    //   // //   //  // //
      //   ////// //   // //   // //   //    //////    ////  //   //

     
                 A R D U I N O   D I S T A N C E  S E N S O R S


                 (C) 2024, C. Hofman - cor.hofman@terrabox.nl

            <DistanceSimulator.cpp> - Library for distance sensors.
                    Created by Cor Hofman, 11 Aug 2024
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
#include <SimulatedDistanceSensor.h>

SimulatedDistanceSensor::SimulatedDistanceSensor(char* name, uint32_t cycleTime, uint16_t pType, uint16_t pMin, uint16_t pMax) :
		                 DistanceSensor(name, cycleTime) {

	simulation = pType;
    min        = pMin;
    max        = pMax;

}

SimulatedDistanceSensor::SimulatedDistanceSensor(char* name, uint16_t pType, uint16_t pMin, uint16_t pMax) :
                         DistanceSensor(name) {

	simulation = pType;
    min        = pMin;
    max        = pMax;

}

void SimulatedDistanceSensor::begin() {
    randomSeed(analogRead(0));
	setInitialTarget();
}

void SimulatedDistanceSensor::end() {

}

void SimulatedDistanceSensor::setInitialTarget() {
	//
    //  Set new animation target in animationLevel
	//  simulatedLevel contains the current level.
    //
    switch (simulation) {
          case SIMU_TYPE_UP:
            if (++simulatedLevel > max)
            {
              animationLevel = min;
            }
            break;
          case SIMU_TYPE_DOWN:
            if (--simulatedLevel < min)
            {
              animationLevel = max;
            }
            break;
          case SIMU_TYPE_RANDOM:
        	if (simulatedLevel == animationLevel)
               animationLevel = random(min,max);
            break;
     }
}

int16_t SimulatedDistanceSensor::registerDistance() {
	guardTarget();
	animate();
    return simulatedLevel;
}

void SimulatedDistanceSensor::guardTarget() {
	//
    //  Set new animation target in animationLevel
	//  simulatedLevel contains the current level.
    //
    switch (simulation) {
          case SIMU_TYPE_UP:
            if (++simulatedLevel > max)
            {
              animationLevel = min;
            }
            break;
          case SIMU_TYPE_DOWN:
            if (--simulatedLevel < min)
            {
              animationLevel = max;
            }
            break;
          case SIMU_TYPE_RANDOM:
            simulatedLevel = random(min,max);
            break;
     }
}

/*-----------------------------------------------------------------------------
 *
 *  Animates moderate change of sensor level
 *
 ----------------------------------------------------------------------------*/
void SimulatedDistanceSensor::animate() {

    if (animationLevel > simulatedLevel) {
        simulatedLevel++;
    }
    else if (animationLevel < simulatedLevel) {
        simulatedLevel--;
    }

}


