

       /////// ////// //////  //////   /////     /////    ////  //    //
         //   //     //   // //   // //   //    //  //  //   // // //
        //   ////   //////  //////  ///////    /////   //   //   //
       //   //     //  //  // //   //   //    //   // //   //  // //
      //   ////// //   // //   // //   //    //////    ////  //   //

     
                A R D U I N O   D I S T A N C E  S E N S O R S


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
Only the UART Auto version has currently been tested and functions correctly.

Release V1.0.0-beta:
A first release containing A02 ultra sound base distance sensor. 
Although it contains potential support for all the A02 versions of the sensor.
Only the UART Auto version has currently been tested and functions correctly.


Abstract
========
This package is set up to support distance sensors for the Arduino platform. The first supported sensor is of type A02. Specifically the UART Auto type. The other types UART Controlled and PWM versions have potential support, but it is untested code.

DistanceSensor
==============
The DistanceSensor is a generic class modelling a range of distance sensors, the only thing it knows is that a distance is to be measured somehow and made available. For that the class assumes a registerDistance() function obtaining a distance from a sensor. Secondly it provide the getDistance() function which returns the distance measured to whatever caller object.

Managing sensor data
====================
To manage the periodical readout of sensors the TerraBox_Scheduler package is used. The DistanceSensor inherits from the Task class, enabling time based scheduling. As a result the sensor measurement is read periodically. Since the DistanceSensor is the Base class for a lot of distance sensors this is an enabler for managing any sensor based on it. 
The specified time interval determines how often sensor is accessed for a distance measurement. The resulting measurement can then be used multiple times by one or more processing tasks. The sensor read out interval can be controlled independently from other processing tasks. Modelling processing logic as a task as well provides an increased level of control. Because tasks can be postponed at will decreasing the CPU load. Creating different Tasks also enables you to separate concerns in your coding by using different tasks. For instance separating signal sensing, signal processing and GUI control. Which is a well known practice.
At the end of this README.md document a rudimentary example is provided using two tasks. The sensor task and a visualisation task with which the measured distance is can visually be monitored.

For more details about Tasks see the TerraBox_Scheduler library README.md file.

Sensor DYP-A02 V2.0
===================
The A02 sensor type uses ultra sound to measure a distances expressed in millimetres. It exists in a water tight version. The area four version available. Switch, RS485, UART Automatic, UART Controlled and UART PWM. Note that the different behaviours of the types mentioned is not configurable. You have to order the specific type you need as it is configured by the manufacturer.
It can operate on DC power from 3.3.V up to 5.0V

The only type currently supported by this library is the Automatic type.
Other types might be added later on.

class A02_UART
==============
The class A02_UART is the base class for all A02_UART type sensors, being A02 Auto, A02 Controlled and A02_PWM. The measurement messages sent by the Auto and Controlled sensors are identical. The PWM sensor uses Pulse Width Modulation to transmit the measurements. The way how to request the sensor to generate the measurements messages differs. The Auto sensor uses a digital enable/disable signal. The other sensors share the use of an identical request message.

All A02 sensors are based on the A02_UART class based. The A02_UART class, on its turn, is based on the generic DistanceSensor class.


class A02_UART_Auto
===================
This automatic A02 sensor type measures the distance continuously and produces a constant stream of distance measurement messages. The A02 its Rx line is permanently held high. As is recommended by the Product specification sheet of the sensor.
It inherits support for reading the transmitted measurement messages from the A02 UART class.

How to connect, for instance to Serial1 on an Arduino Mega2560:
- The Arduino pin TX1/pin 18 can be connected to A02 RX line (white wire). Connecting it to any Arduino Digital Output will work as well.
- The Arduino pin RX1/pin 19 is connected to A02 TX line (yellow wire).
- The Arduino +5 V is connected to the A02 red wire.
- The Arduino GND is connected to the A02 black wire.
 

A02_UART_Controlled
===================
The controlled versions similar to the automatic one with respect to the measurement message. The method to trigger the measurement differs. The controlled version is basically silent and does not sent any messages without a request. The sensor also has a sleep mode. Minimising power consumption. So it will only react to a request if the sensor is awake. It needs some form of watchdog to make sure the sensor is awake and will react on a measurement request. To do all that some orchestration of these activities is needed on top of the capability of receiving a measurement message. The A02_Controlled therefor inherits the A02_UART its capability to receive and interpret the measurement message. On top of that the A02_UART_Controlled class adds the logic to wake up the sensor, if needed, and to request a single measurement. After which is waits for the measurement message to arrive, which is then processed and produces the measured distance.

Connect it to any Serial, for instance Serial1 on an Arduino Mega2560:
- The Arduino pin TX1/pin 18 is be connected to A02 RX line (white wire).
- The Arduino pin RX1/pin 19 is connected to A02 TX line (yellow wire).
- The Arduino +5 V is connected to the A02 red wire.
- The Arduino GND is connected to the A02 black wire.


A02_PWM
=======
The third version of the Pulse Width Modulation version of the sensor. Like the A02_UART_Controlled sensor needs a request to be sent to the sensor in order for it to generate a PWM modulated distance value. The way in which to request the measurement is equal to that of the UART controlled version. Reading the PWM value is what differs from the UART Controlled version.

The A02_PWM is therefore based on the A02_UART_Controlled as it inherits the wakeup and request logic. Reading the PWM signal is standard Arduino functionality. So the measurement message part is be overridden easily by a simple measurement message implementation. 

Connect it to any Serial and PWM enabled pin, for instance Serial1 on an Arduino Mega2560:
- The Arduino pin TX1/pin 18 is be connected to A02 RX line (white wire).
- The Arduino pin DI/PWM/pin 10 is connected to A02 TX line (yellow wire).
- The Arduino +5 V is connected to the A02 red wire.
- The Arduino GND is connected to the A02 black wire.

A02_Switch
==========
Not supported.

This type will output dedicated signal levels on both the TX and RX lines, depending on a factory configured threshold distance. The factory default is 1.5 metres (i.e. 1500mm).

The TX and RX levels:
-TX == low  & RX == high if less than the threshold value.
-TX == High & RX == low  if more than the threshold value.

How to connect:
- The the A02 TX line (yellow wire) must be connected to a Digital Input. 
- The the A02 RX line (white wire) must be connected to a Digital Input. 
- The Arduino +5 V is connected to the A02 red wire.
- The Arduino GND is connected to the A02 black wire.


SimulatedDistanceSensor
=======================
This sensor does not support an actual physical sensor, but it is a simple simulation of a sensor. It merely generates artificial measurements. Either random or in an alternating up and down sequence.

It inherits from the base class DistanceSensor. It can therefore easily be used to test the application. Without the need for a physical sensor. Once the proper sensor has arrived in the mail  simply connect the physical sensor and replace the simulated sensor class by the one supporting the hardware sensor of choice.

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
```

Example Output
==============
As can be seen, the distance is measured every 5 seconds by the sensor and printed by the monitor.

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