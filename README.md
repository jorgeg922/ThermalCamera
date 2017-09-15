# ThermalCamera FLIR LEPTON 2.0 Shuttered

Expanding upon the sample code provided by Sparkfun, this code will further implement
a simple tracking and detection system of high temperatures with a FLIR Lepton 2.0.

If not familiar with the sample code from Sparkfun, the files have also been included in this
folder, so you can get familiar with the original code. It can found inside "raspberrypi_video"

The modification to the original sample code can be found in "raspberrypi_ThermalCam". Most of the modifications will be found in LeptonThread.cpp and .h

Hookup Guide:

In order to hook up the FLIR Lepton to the Raspberry Pi B+, refer to the sparkfun website:

https://learn.sparkfun.com/tutorials/flir-lepton-hookup-guide

Note: Be aware that the CS pin from the raspberry pi used in the sparkfun guide differs from the CS pin in the RPI B+ because it counts with an expanded set of GPIO pins. Google the appropriate image to find the correct CS pin. If you do not connect the camera to the correct CS pin, the camera will show a red box if you run the video stream. If this happens, pick the other CS pin.

The hookup guide from sparkfun should be anough to help you hookup the FLIR camera to the RPI.
The only thing that needs to be hooked up from here on are the two servos that will be used as part of tracking and centering system to point to the highest temperature in the camera's field of vision.

In this code, I used servo numbers 5 and 6 from the ServoBlaster library, which correspond to GPIO pins 23 and 24 from the RPi respectively. There is no particular reason why I chose to use these pins besides the fact that at the time I was writing this code, I was using the other pins for something else.

You can google "ServoBlaster" and find the appropriate github if you want to learn more about the ServoBlaster library, but the servo numbers and GPIO pins are as follows:

Servo Number -> GPIO Number

0-->4

1-->17

2-->18

3-->21/27

4-->22

5-->23

6-->24

7-->25

Regardless of the pins you decide to use, wire these pins to the servos' PWM pins and power/ground them to the same source.

At this point, I am assuming you have already mounted the servos onto a pan tilt. If you have not done so, you must do it and attach the thermal camera to the front of the pan tilt. You can look at the images of how I did mine to use as a reference inside the images folder.

Now, if you are running the original sparkfun code, all you have to do is navigate into the appropriate folder and run the commands qmake && make. This will compile the code and give you an executable file. To run the executable run sudo ./raspberrypi_video

If you are running my code, you will need to navigate into the raspberrypi_libs folder first and run the servoBlaster library. Make sure to run it with the sudo command or it will not execute correctly. 

After executing the ServoBlaster library navigate to the raspberrypi_thermalCam folder and run the commands qmake && make. This will again produce an executable which you can run. If you followed the steps correctly, the servos will start automatically moving in an attempt to center the highest temperature within the its field of vision.

As mentioned before, the code also contains an algorithm to estimate the temperature in fahrenheit of the highest object within its field of vision. This algorithm was developed using a simple linear regression formula using the raw output of the camera as a baseline. Although the temperature was accurate for the room where I developed this algorithm, it might not be for the place where you are located. If you know how to run a regression, you can modify the formula accordingly within the source code.

You must also take into account that there are other factors that affect temperature such as the environmental, and internal temperature of the camera, so even after running a regression with the raw outputs of the camera, the output might not be accurate when taken to other places. You need to think of the factors affecting temperature and add those sensors to your implementation if you want more accurate results.
