#include "Lepton_I2C.h"

#include "leptonSDKEmb32PUB/LEPTON_SDK.h"
#include "leptonSDKEmb32PUB/LEPTON_SYS.h"
#include "leptonSDKEmb32PUB/LEPTON_Types.h"
//My addition to this code
#include <iostream>
using namespace std;
#include  "leptonSDKEmb32PUB/LEPTON_AGC.h"
#include <sstream>
//End my addition to the code
bool _connected;

LEP_CAMERA_PORT_DESC_T _port;
//MY addition to the code 
LEP_SYS_FPA_TEMPERATURE_KELVIN_T fpa_temp_kelvin;
LEP_RESULT result;
LEP_SYS_AUX_TEMPERATURE_KELVIN_T aux_temp_kelvin;
LEP_RESULT aux_result;
//End of addition

int lepton_connect() {
	LEP_OpenPort(1, LEP_CCI_TWI, 400, &_port);
	_connected = true;
	return 0;
}

void lepton_perform_ffc() {
	if(!_connected) {
		lepton_connect();
	}
	LEP_RunSysFFCNormalization(&_port);
}

//presumably more commands could go here if desired
//Function to get the (FPA) internal temperature of the camera in Kelvin
int lepton_temperature(){
	if(!_connected){
		lepton_connect();
	}
	result = (LEP_GetSysFpaTemperatureKelvin(&_port, &fpa_temp_kelvin));
	return( fpa_temp_kelvin);
}
//end of function
//Function to get (AUX) internal temperature
int aux_temperature(){
	if(!_connected){
		lepton_connect();
	}
	aux_result = (LEP_GetSysAuxTemperatureKelvin(&_port, &aux_temp_kelvin));
	return( aux_temp_kelvin);
}
//end of function
