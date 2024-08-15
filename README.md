

       /////// ////// //////  //////   /////     /////    ////  //    //
         //   //     //   // //   // //   //    //  //  //   // // //
        //   ////   //////  //////  ///////    /////   //   //   //
       //   //     //  //  // //   //   //    //   // //   //  // //
      //   ////// //   // //   // //   //    //////    ////  //   //

     
                    A R D U I N O   S C H E D U L E R


                 (C) 2024, C. Hofman - cor.hofman@terrabox.nl

               <README.md> - Library for Distance sensors.
                    Created by Cor Hofman, 09 Aug 2024
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

Releases
========

Release v1.0.0-alpha:
A first release containing A02 ultra sound base distance sensor. 
Although it contains potential support for all the A02 versions of the sensor.
Only the UART Auto version has currently been tested and functions correctly

Abstract
========
In general this package is set up to contains Arduino support for distance sensors. The first supported sensor is of type A02. Specifically the UART Auto type. The other types UART Controlled and PWM versions have potential support, but it currently untested code.
To manage the periodical readout of the sensors the TerraBox_Scheduler Arduino package is used.
Basically every supported sensor is modelled as a Task, which enables it to be read periodically.


A02_UART_Auto
=============
The sensor type uses the sensor type which continuously measure the distance. That is the sensor produces a constant stream of measurements on its Tx pin. This is the white wire on the A02 its connector. The Rx pin is permanently held high as is recommended by the Product specification sheet of the sensor.

It inherits support for reading the transmitted measurement messages sent by the A02 Auto sensor.


A02_UART
========
The class A02_UART is the base class for all UART types sensors, being A02 Auto and A02 Controlled. The measurement message sent by both sensors version are equal. The way how to get the sensor to generate the measurements messages differs.

The A02 UART class is based on the generic DistanceSensor class.

DistanceSensor
==============
Since the DistanceSensor is a generic class for any distance sensor, the only thing it knows is a distance is to be measured somehow. For that the class assumes a registerDistance() method, which is invoked from the Task::exec() method and store the measurement in the distance attribute.
The getDistance() method support getting the last read distance.

The DistanceSensor, being the base class for all distance sensors, is based on the Task::Task(char* name, uint32_t cycleTime). See the TerraBox_Scheduler library README.md file for more details.
The essence is that a Task is periodically scheduled by the TaskScheduler and as a result the exec() methods of the DistanceSensor class is invoked periodically.  

A02_UART_Controlled
===================
This is, like he A02_UART_Auto a UART based sensor. However where the measurement message is the same as sent by the A02_UART_Auto, the method to trigger the measurement differs. The controlled version is basically silent and does not sent any messages without a request. As a consequence a request has to be sent to the sensor for it to generate a single measurement message. Further it will only react to a request if the sensor is awake. So some form of watchdog is needed to make sure the sensor is awake and can receive the measurement request. To do all that some orchestration of these activities is needed. The A02_Controlled therefor is based on the A02_UART to receive and process the measurement message. On top of that the A02_UART_Controlled class adds the logic to wake up the sensor, of needed and to request a measurement. After which is waits for the measurement message to arrive, which is processed and produces the measured distance in millimetres.

As said the A02_UART_Controlled is based upon the A02_UART class for its measurement message reading capabilities.

A02_PWM
=======
The third version of the Pulse Width Modulation version of the sensor. Like the A02_UART_Controlled sensor a request needs to be sent to the sensor in order for it to generate a PWM modulated distance value. The way in which to request the measurement is equal to that of the UART controlled version. Reading the PWM value is what differs from the UART Controlled version.

The A02_PWM is therefore based on the A02_UART_Controlled as it inherits the wakeup and request logic. Reading the PWM signal is standard Arduino functionality. So the measurement message part is can be overridden easily by a simple measurement message implementation. 

SimulatedDistanceSensor
=======================
This sensor does not support is not an actual physical sensor, but it is a simple simulation of a sensor. It merely generates values. Either random or in a sequence up or down.

It inherits from the base class DistanceSensor. Like all other sensors. So once you'v got the proper sensor you simply replace this simulated sensor by a physical one and its corresponding supporting class version.

The simulation has two constructors:

``` C++
SimulatedDistanceSensor(char* name, uint16_t type, uint16_t min, uint16_t max);
SimulatedDistanceSensor(char* name, uint32_t cycleTime, uint16_t type, uint16_t min, uint16_t max);
```
- name		The task name
- cycleTime	The interval at which measurements are read from the sensor
- type		Specifies the behaviour by using the constants SIMU_TYPE_UP, SIMU_TYPE_DOWN or SIMU_TYPE_RANDOM
- min		The minimum value it generates
- max		The maximum value it generates

  
Typical Arduino sketch
======================
The sketch can be found in the Arduino library under the name Scheduler_Example.ino in the folder named Scheduler_Example as well.

``` cpp
#include <A02_UART_Auto.h>
#include <TaskScheduler.h>

//
//  Sketch monitors the distance measured...
//  Sensor used: A02_UART_Auto
//
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
    //  Implementation of the monitoring logic. Printing the distance
    //
    void exec() {
  
       Serial.print("Distance: "); Serial.println(sensor->getDistance());

    }

};

A02_UART_Auto sensorAuto("A02 UART Auto sensor");
DistanceMonitor monitor("Distance monitor", &sensorAuto);

void setup()
{
  Serial.begin(115200);
  while(!Serial);

  //
  //  Since cycle times for both tasks are equal (5000 ms)
  //  Run the sensor last, makes it the first task to execute
  //  After which the monitor will run...
  //  Processing the sensor measured distance by printing it.
  //
  scheduler.run(&monitor);     // Monitor run() first -> second in the TaskList
  scheduler.run(&sensorAuto);  // Sensor run() second -> first in the TaskList
}

//
//  Execute the tasks
//  1) Read a measurement
//  2) Monitor the measurement
//
void loop()
{

  //
  //  Read the distance measurement
  //  and have the monitor print it
  //
  scheduler.schedule();

}
```

Example Output
==============
Note that the times for the Hello world message shifts with 20 ms after the monitor has been invoked for the first time. Also note that the interval for Hello te execute is 5000 ms. Theinterval for the monitor is 15000 ms. So at the third 5000 ms interval the monitor and Hello tasks compete in getting scheduled at that interval first. Because the last run() Task is the first in the scheduling list it has higher priority. So the monitor runs first. Running the monitor takes 20 ms. So the Hello task is delayed in running that 20 ms and runs after 5020 ms instead of 5000.

The scheduler uses the last actual start time of a Task + its interval times specified, all following cycles for the Hello task will actually be 5000 ms again. So the scheduler adapts the Task start times in such a way that the interval times are properly maintained.

``` txt
<SerialMonitorOutput>
   :
   :
01:56:21.652 -> Distance: 1722
01:56:26.715 -> Distance: 1724
01:56:31.771 -> Distance: 1724
01:56:36.795 -> Distance: 71
01:56:41.831 -> Distance: 77
01:56:46.884 -> Distance: 255
01:56:51.938 -> Distance: 403
01:56:56.984 -> Distance: 713
01:57:02.011 -> Distance: 572
01:57:07.088 -> Distance: 352
01:57:12.130 -> Distance: 276
01:57:17.147 -> Distance: 150
01:57:22.194 -> Distance: 150
01:57:27.255 -> Distance: 27
01:57:32.283 -> Distance: 97
01:57:37.320 -> Distance: 35
01:57:42.387 -> Distance: 38
01:57:47.424 -> Distance: 38
01:57:52.469 -> Distance: 147
01:57:57.499 -> Distance: 1722
   :
   :
</SerialMonitorOutput>
```