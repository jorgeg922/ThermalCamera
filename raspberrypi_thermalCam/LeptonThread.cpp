//Author: Jorge G.
//Last updated: September 18, 2017

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "LeptonThread.h"
#include "Palettes.h"
#include "SPI.h"
#include "Lepton_I2C.h"
#include <QFile>
#include <QTextStream>
#include <QString>

#define PACKET_SIZE 164
#define PACKET_SIZE_UINT16 (PACKET_SIZE/2)
#define PACKETS_PER_FRAME 60
#define FRAME_SIZE_UINT16 (PACKET_SIZE_UINT16*PACKETS_PER_FRAME)
#define FPS 27;

using namespace std;
//bool runOperation;

LeptonThread::LeptonThread() : QThread()
{
}

LeptonThread::~LeptonThread() {
}

void LeptonThread::run()
{	
	SpiOpenPort(0); //open SpiPort to start reading packets
	int startingHorPulseInt = 150; //starting pulse set at 90 degrees at the beginning
	string startingHorPulse = convertToString(startingHorPulseInt); //convert the integer to string to be sent with sys command
	ServoBlaster(startingHorPulse, 1); //send the command by calling system

	int startingVerPulseInt = 100; //starting pulse for vertical movements
	string startingVerPulse = convertToString(startingVerPulseInt);
	ServoBlaster(startingVerPulse, 2);
	
	int horspeedServo = 0; //horspeedServo will be used to modify the speed at which the camera moves horizontally
	int verspeedServo = 0; //speed for vertical positioning
	double max_cal_temp = 0; //variable for the maximum calibrated temperature
	//double min_cal_temp = 0;//uncomment if using min temperature
	int frameCount = 0;//dummy to count the number of frames gone through
	
	while(true) { //main while loop
		//read data packets from lepton over SPI
		int resets = 0;
		for(int j=0;j<PACKETS_PER_FRAME;j++) {
			//if it's a drop packet, reset j to 0, set to -1 so he'll be at 0 again loop
			read(spi_cs0_fd, result+sizeof(uint8_t)*PACKET_SIZE*j, sizeof(uint8_t)*PACKET_SIZE);
			int packetNumber = result[j*PACKET_SIZE+1];
			if(packetNumber != j) {
				j = -1;
				resets += 1;
				usleep(1000);
				//Note: Selected 750 resets as an arbitrary limit, since there should never be 750 "null" packets between
				//two valid transmissions at the current poll rate
				//By polling faster, developers may easily exceed this count, and the down period between frames may then be flagged as a loss of sync
				if(resets == 750) {
					SpiClosePort(0);
					usleep(750000);
					SpiOpenPort(0);
				}
			}
		}
		
		frameBuffer = (uint16_t *)result;
		int row, column, maxValue_Col, maxValue_Row;
		//string newAngle, prevAngle; //not currently used
		uint16_t value;
		uint16_t minValue = 65535;
		uint16_t maxValue = 0;
		//loop to go through every pixel in one frame and get the max and min
		for(int i=0;i<FRAME_SIZE_UINT16;i++) {
			//skip the first 2 uint16_t's of every packet, they're 4 header bytes
			if(i % PACKET_SIZE_UINT16 < 2) {
				continue;
			}
			//flip the MSB and LSB at the last second
			int temp = result[i*2];
			result[i*2] = result[i*2+1];
			result[i*2+1] = temp;

			value = frameBuffer[i];

			if(value > maxValue) { //obtain the most current max value
				maxValue_Col = i% PACKET_SIZE_UINT16 - 2;//obtain column number
				maxValue_Row = i / PACKET_SIZE_UINT16; //obtain row number
				//speed for horizontal movement
				if(abs(maxValue_Col-40) >= 20){//if the max value is more than 20 pixels away from the middle
					horspeedServo = 14;//then the speed is set at 14 movements using ServoBlaster library
				}else{
					horspeedServo = 2;//if closer than 20 pixels, speed is set at 2.
				}
				//speed for vertical movement
				if(abs(maxValue_Row-30) >= 15){
					verspeedServo = 10; //for now (need angle calculation for vertical field of vision)
				}
				else{
					verspeedServo = 2;
				}
				maxValue = value; //maxValue for the current frame
			}
			if(value < minValue) {
				minValue = value; //minValue for the current frame (currently not using minValue for anything)
			}
			column = i % PACKET_SIZE_UINT16 - 2;
			row = i / PACKET_SIZE_UINT16 ;
		}
		if(frameCount == 81){ //at 81 frames, then move the camera. 81 frames = 3 seconds (27fps)			
			//qDebug()<<"Column Number";
			//qDebug()<<maxValue_Col;
			//qDebug()<<"row Number";
			//qDebug()<<maxValue_Row;
			if(maxValue_Col != 40 && maxValue_Col < 40){
				startingHorPulseInt += horspeedServo;
				if(startingHorPulseInt >= 50 && startingHorPulseInt <=249){
					startingHorPulse = convertToString(startingHorPulseInt);
					ServoBlaster(startingHorPulse, 1);
				}
			}else if(maxValue_Col != 40 && maxValue_Col > 40){
				startingHorPulseInt -= horspeedServo;
				if(startingHorPulseInt >= 50 && startingHorPulseInt <=249){
					startingHorPulse = convertToString(startingHorPulseInt);
					ServoBlaster(startingHorPulse, 1);
				}
			}

			if(maxValue_Row != 30 && maxValue_Row < 30){
				startingVerPulseInt -= verspeedServo;
				if(startingVerPulseInt >= 50 && startingVerPulseInt <= 249){
					startingVerPulse = convertToString(startingVerPulseInt);
					ServoBlaster(startingVerPulse, 2);
				}
			}else if(maxValue_Row != 30 && maxValue_Row > 30){
				startingVerPulseInt += verspeedServo;
				if(startingVerPulseInt >= 150 && startingVerPulseInt <= 249){
					startingVerPulse = convertToString(startingVerPulseInt);
					ServoBlaster(startingVerPulse, 2);
				}
			}
			//qDebug()<<"Horizontal:";
			//qDebug()<<startingHorPulseInt; // announcing the speed (can be commented out if needed)
			//qDebug()<<"Vertical:";
			//qDebug()<<startingVerPulseInt;
			
			
			double internal_tmp_k = LeptonAuxTemp(); //get the internal temperature of the camera in Kelvin
			
        	
        	//Formula to calculate the actual temperature of the maxValue. The formula was derived by performing
        	//a linear regression using actual temperature as the dependent variable and internal temperature
        	//and raw value of the maxValue from the camera as independent variables. While the results are accurate when
        	//used under room temperature, it fails to readjust in other environments. Need to sample more data and use ambient
        	//temperature as another variable. Also, might need to calculate distance of the pixel and add it as well.

        	max_cal_temp = ((110.78*internal_tmp_k)+(0.08*maxValue)-33745.3)+(0.2*((110.78*internal_tmp_k)+(0.08*maxValue)-33745.3));
        	
        	
      		if(max_cal_temp > 570){
        		tempWarning();
        		firePresence(true);
       		}else{
       			firePresence(false);
       		}
       		frameCount = 0; //reinitialize the frame count for next servo movement
		}	
		frameCount++;	
	}
	//finally, close SPI port just because
	SpiClosePort(0);
}
//bool LeptonThread::enable(){//enable method. Implement in main as if true then start the thread
//		runOperation = true;
//}
//bool LeptonThread::disable(){//disable method. Implement in main
//		runOperation = false;
//}
void LeptonThread::performFFC() {//perform a flat field correction
	lepton_perform_ffc();
}
double LeptonThread::LeptonAuxTemp(){
	int temp = aux_temperature();
	double aux_temp_f =  temp/100;//output does not provide decimal point so divide by 100
	return aux_temp_f;	
}
void LeptonThread::ServoBlaster(string mvDistance, int servoNumber){//servoBlaster is implemented by calling system and passing the argument
	 std::cout<<mvDistance;
	 if(servoNumber == 1){//horizontal servo							
	 system(("echo 6="+ mvDistance +" > /dev/servoblaster").c_str());
	 }
	 if(servoNumber == 2){//vertical servo
	 system(("echo 5="+ mvDistance +" > /dev/servoblaster").c_str());	
	 }
}
void  LeptonThread::showTemp(){//displays the FPA temperature (currently not being used)
	qDebug()<<lepton_temperature();
}
void LeptonThread::tempWarning(){//warning for temperature (needs revision)
	qDebug()<<"Detecting high temperature. Please verify.";
}
string LeptonThread::convertToString (int passedValue){ //converting int to string to later pass it over to ServoBlaster
	stringstream ss;
	ss << passedValue;
	std::string newString = ss.str();
	return newString;
}
bool LeptonThread::firePresence(bool fireSignal){
	if(fireSignal){
		return true;
	}else{
		return false;
	}
}
