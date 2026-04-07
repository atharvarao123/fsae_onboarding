#ifndef MAIN_H
#define MAIN_H 

//Standard Libraries 
#include <Arduino.h>
#include <stdlib.h>
#include <time.h>


#define RAND_MAX 110
#define RAND_MIN 1
#define SAMPLE_WINDOW 5
#define SENSOR_MIN 0
#define SENSOR_MAX 100

// Format for sending sensor data and format for CAN

typedef struct sensordata{
  int filtered_data;
  int error;
} sensordata;

typedef struct canformat{
    int id;
    byte msb;
    byte lsb;
    byte errorflag;
} canformat;

/*
Generates Random Noise for throttle percentage, applies moving average filter and sends data through queue
*/
void sensor(void* paramaters);

/*
Creates CAN frame, and prints filtered data to serial monitor
*/
void can_task(void* parameters);


#endif MAIN_H