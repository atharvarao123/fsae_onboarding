#include <Arduino.h>
#include <stdlib.h>
#include <time.h>

#define RAND_MAX 110
#define RAND_MIN 1
#define SAMPLE_WINDOW 5
#define SENSOR_MIN 0
#define SENSOR_MAX 100


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

QueueHandle_t send_sensor;

void sensor(void* paramaters);
void can_task(void* parameters);

/*
Creates Two FreeRTOS tasks:
1. Sensor Task
2. Communication Protocol / CAN Frame

 Initializes Queue
*/

void setup() {
  Serial.begin(9600);
  send_sensor = xQueueCreate(5,sizeof(sensordata)); 
  xTaskCreate(sensor,"Sensor Task",2048,NULL,5,NULL);
  xTaskCreate(can_task,"Communication Protocol",2048,NULL,4,NULL);
  vTaskStartScheduler();
}


/*
Generates random throttle percentage (max 120 , min 0), applies processing and sends data through queue
*/
void sensor(void * parameters){
  
  int values[SAMPLE_WINDOW] = {0};
  int position = 0;
  int filtered_data = 0;
  int sum = 0;
  sensordata data;
  srand(time(NULL));

  while(1){
    int value_error = 0;
    
    uint16_t throttle_percentage = (rand() % RAND_MAX ) + RAND_MIN;

    if(throttle_percentage > SENSOR_MAX || throttle_percentage < SENSOR_MIN){
      value_error = 1;
    }

    // Moving Average Filter implementation (5 Samples)
    values[position] = throttle_percentage;
    for(int i = 0; i < SAMPLE_WINDOW; i++){
      sum+=values[i];
    }

    filtered_data = sum/SAMPLE_WINDOW;
    sum = 0;

    if(position == SAMPLE_WINDOW-1){
      position = 0;
      data.error = value_error;
      data.filtered_data = filtered_data;
      // Queue used here
      xQueueSend(send_sensor,&data,portMAX_DELAY);
    }
    else{
      position+=1;
    }
    vTaskDelay(500/portTICK_PERIOD_MS);
    value_error = 0;
  }
} 

/* Receives data from sensor task, and creates CAN frame with format:
1. ID 
2. Byte 1-0 -> filtered throttle percentage
3. Byte 2 -> error flag
*/
void can_task(void* paramaters){
    sensordata data;
    canformat can;
    while(1){
      xQueueReceive(send_sensor,&data,portMAX_DELAY);
      can.id = 0x111;
      can.msb = (data.filtered_data >> 8) & 0xFF;
      can.lsb = data.filtered_data & 0xFF;
      can.errorflag = data.error;

      uint16_t combined_value = (can.msb << 8) | can.lsb;
      
      Serial.print("Can[ID=0x");
      Serial.print(can.id,HEX);
      Serial.print("]: ");
      Serial.print(combined_value );
      Serial.print("%, ");
      if(can.errorflag == 1){
        Serial.println("ERROR");
      }
      else{
        Serial.println("OK");
      }
      vTaskDelay(1000/portTICK_PERIOD_MS);
    }
  }


void loop() {
  // put your main code here, to run repeatedly:
}

