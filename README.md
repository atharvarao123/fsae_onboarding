# FSAE Onboarding Task

# Overview of what the Project Does

The project generates random noise (throttle pressure) with a maximum value of 120 and a minimum of 0. 
It then applies processing on the data, including out-of-range checks, and a moving average filter (with 5 samples). 
It sends the data (filtered sensor value, error flag for out of range), and  proceeds to create a CAN frame contaning the filtered value, id num and the error flag which is printed out.

# How to Compile and run the FreeRTOS project

1. Install PlatformIO on VSCode
  
2. Git Clone the project, and open the project folder in PlatformIO.
  
3. Connect ESP-32 Wrover ( Or your own available microcontroller and change the board in name in platformio.ini -> it should still work)
  
4. Build & Run the project

# CAN ID and Byte Layout

CAN ID: 0x111
Byte1: Most Significant Byte of the Sensor Value
Byte0: Least Significant Byte of the Sensor Value
Byte2: Error Code

Error Code:
1 - Error Detected  (if raw throttle percentage is over 100 or less than 0)
0 - No Error ( raw throttle percentage is contained within 0 and 100)


