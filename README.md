# Track-N-Lock (Heat detection & tracking)

In this project, I have developed and implemented an algorithm to detect and track the highest temperature within a camera's field of vision - in this case a FLIR Lepton 2.0

I developed this project due to the lack of information and methods to track high temperature objects using this particular camera. Although FLIR Lepton 2.1 comes with a method to identify high temperatures, FLIR Lepton 2.0 does not, and therefore decided to write this program to try address this issue.

NOTE: In implementing this project I made use of two other repositories, namely one from Pure Engineering who developed the sample code for the FLIR Lepton and richardghirst who developed the ServoBlaster library. Their repositories can be found at the following links:

https://github.com/richardghirst

https://github.com/groupgets/LeptonModule

The code from Pure Engineering was used primarily as a starting point to write the program to detect high temperatures. In particular, the file LeptonThread.cpp has been heavily modified and I have extensively commented the code to understand the changes made.

The ServoBlaster library has been used in order to control a pan-tilt bracket that aids the camera in looking towards the correct direction to the highest temperature in its field of vision.


#---------------------------------- HOOKUP GUIDE ----------------------------------

In order to hook up the FLIR Lepton to the Raspberry Pi B+, refer to the sparkfun website:

https://learn.sparkfun.com/tutorials/flir-lepton-hookup-guide

NOTE: Be aware that the CS pin location from the raspberry pi used in the sparkfun guide differs from the CS pin in the RPI B+ because it counts with an expanded set of GPIO pins. Google the appropriate image to find the correct CS pin. 

#Raspberry Pi & FLIR Lepton--------------------------------------------------------

The hookup guide from Sparkfun should be anough to help you hookup the FLIR camera to the RPi.
The only thing that needs to be hooked up from here on are the two servos that will be used as part of the tracking and centering system to point to the highest temperature in the camera's field of vision.
At this point, it is a good idea to set up the pan-tilt if you have not and attach the camera to the front.
Refer to the the images inside the appropriate folder for guidance.

#ServoBlaster Library---------------------------------------------------------------

In this project, I used servo numbers 5 and 6 from the ServoBlaster library, which correspond to GPIO pins 23 and 24 from the RPi B+ respectively. These pins need to be attached to the servo's PWM signal pins. There is no particular reason why I chose to use these pins besides the fact that at the time I was writing this code, I was using the other pins for something else.

You can navigate to the "ServoBlaster" GitHub if you want to learn more about the ServoBlaster library, but the servo numbers and their corresponding GPIO pins are as follows:

Servo Number -> GPIO Number

0-->4

1-->17

2-->18

3-->21/27

4-->22

5-->23

6-->24

7-->25


Finally, navigate inside the PiBits>ServoBlaster>User. In this folder you will find the Servod.c program. Execute it using: sudo ./servod

This will enable the library, which will be later used by leptonThread.cpp

#Heat Detection Program---------------------------------------------------------------

Having hooked up all the components together and executed the ServoBlaster library, the only thing left to do is execute the camera's heat detection program.

Navigate inside the raspberrypi_thermalCam and type: qmake && make

This will create the executable file, which you can then run it.

#Heat Detection Algorithm--------------------------------------------------------------

As mentioned before, this project implements an algorithm to estimate the temperature in fahrenheit of the highest object within its field of vision. This algorithm was developed using a simple linear regression formula using the raw output of the camera as a baseline. Although the temperature was accurate for the room where I developed this algorithm, it might not be for the place where you are located. If you know how to run a regression, you can modify the formula accordingly within the source code.

You must also take into account that there are other factors that affect temperature such as the environmental and internal temperature of the camera, so even after running a regression with the raw outputs of the camera, the output might not be accurate when taken to other places. You need to think of other factors affecting temperature and add those sensors to your implementation if you want more accurate results.
