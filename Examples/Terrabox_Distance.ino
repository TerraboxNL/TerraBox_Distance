/*-------------------------------------------------------------------------------------------------


       /////// ////// //////  //////   /////     /////    ////  //    //
         //   //     //   // //   // //   //    //  //  //   // // //
        //   ////   //////  //////  ///////    /////   //   //   //
       //   //     //  //  // //   //   //    //   // //   //  // //
      //   ////// //   // //   // //   //    //////    ////  //   //


                 A R D U I N O   D I S T A N C E  S E N S O R S


                 (C) 2024, C. Hofman - cor.hofman@terrabox.nl

               <Terrabox)Distance.ino> - Library for LevelIndicators.
                                 10 Sep 2024
                       Released into the public domain
              as GitHub project: TerraboxNL/TerraBox_LevelIndicator
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
#include <A02_UART_Auto.h>
#include <TaskScheduler.h>

//==========================================================================
//
//  Sketch monitors the distance measured...
//  Sensor used: A02_UART_Auto
//
//==========================================================================

class DistanceMonitor : public Task {

  private:
    DistanceSensor* sensor;      // The sensor used to measure the distance

  public:
    //
    //  The constructor creating the monitor
    //
    //  name      The name of the monitoring task
    //  pSensor   The sensor used to measure the distance
    //  
    DistanceMonitor(char* name, DistanceSensor* pSensor) :
                 Task(name) {

      sensor = pSensor;

    }

    //
    //  Implementation of the monitoring logic.
    //  In this case only printing the distance
    //
    void exec() {
  
       Serial.print("Distance: "); Serial.print(sensor->getDistance());Serial.println(" mm");

    }

};

//
//  Your sensor of choice.
//
#if 0
SimulatedDistanceSensor sensor("Simulated test sensor", SIMU_TYPE_RANDOM, 0 - 100);
#else
A02_UART_Auto sensor("A02 UART Auto sensor");          // Create the A02 Auto sensor
#endif

//
//  The monitor displaying the measured value
//
DistanceMonitor monitor("Distance monitor", &sensor);  // Create using the sensor read out

void setup()
{
  Serial.begin(115200);
  while(!Serial);

  //
  //  The cycle times for both tasks are equal (5000 ms)
  //  invoking run() for the sensor last, makes it the first task to execute
  //  After which the monitor will run...
  //  Processing the sensor measured distance by printing it.
  //
  scheduler.run(&monitor);     // Make monitor runnable first, which makes it second in the TaskList
  scheduler.run(&sensor);      // Make sensor runnable second, which make it first in the TaskList
}

//
//  Schedule the tasks
//  1) Read a measurement from the sensor
//  2) Process the measurement by the monitor
//
void loop()
{

  //
  //  Read the distance measurement
  //  and have the monitor print it
  //
  scheduler.schedule();

}
