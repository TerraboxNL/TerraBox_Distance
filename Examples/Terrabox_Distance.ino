#include <A02_UART_Auto.h>
#include <TaskScheduler.h>

//
//  Sketch monitors the distance measured...
//  Purpose is to show how to use the TerraBox_Distance library.
//
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
    //  The constructor creating the monitor
    //
    //  name      The name of the monitoring task
    //  cycleTime The interval time in ms
    //  pSensor   The sensor used to measure the distance
    //  
    DistanceMonitor(char* name, uint16_t cycleTime, DistanceSensor* pSensor) :
                 Task(name, cycleTime) {

      sensor = pSensor;

    }

    //
    //  Implementation of the monitoring logic. Printing the distance
    //
    void exec() {
  
       Serial.print("Distance: "); Serial.println(sensor->getDistance());

    }

};

A02_UART_Auto sensorAuto("A02 UART Auto sensor", 10000);
DistanceMonitor monitor("Distance monitor", 10000, &sensorAuto);

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
