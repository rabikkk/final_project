#include "Globals.h"

#ifdef PCBR2
	#pragma config FPLLIDIV = DIV_2		//PLL Input Div
#else
	#pragma config FPLLIDIV = DIV_1		//PLL Input Div
#endif
#pragma config FPLLMUL = MUL_20		//PLL Multi
#pragma config UPLLIDIV=DIV_1		//USB PLL input div
#pragma config UPLLEN=OFF   		//USB-PLL-Input-Enable
#pragma config FPLLODIV = DIV_1		//System PLL Output Div
#ifdef PCBR2
	#pragma config FNOSC = FRCPLL		//Osc Selection
	#pragma config POSCMOD = OFF			//Primary Osc Config 
#else
	#pragma config FNOSC = PRIPLL		//Osc Selection
	#pragma config POSCMOD = EC			//Primary Osc Config 
#endif
#pragma config FSOSCEN=OFF			//Secondary Osc Enable
#pragma config IESO=OFF				//Internal /External Switch over
#pragma config OSCIOFNC=OFF			//CLK0 Output Active on OSCO pin
#pragma config FPBDIV = DIV_1  	 	//Peripheral Clock Div
#pragma config FCKSM=CSDCMD			//CLock Switching an Monitor selection //set in code (initports)
#pragma config WDTPS=PS32768		//WDT postscaler... set in code, if needed //obviously driven with 1ms
#pragma config FWDTEN = OFF   		//WDT Enable
#pragma config ICESEL=ICS_PGx2		//ICE/ICD Comm Channel select
#pragma config CP=ON				//Code Protection
//#pragma config BWP=ON				//Boot Flash writeable ON=protected


///////////////////////////////////////////////////////////////
/*				Sizes of Variables for PIC32					
Datatype 		Bytes			Datatype	Bytes
int				4				long long	8
long			4				char		1
double 			8				float 		4				 
short			2											 */
///////////////////////////////////////////////////////////////


/******************************************************/
/*********************MAIN PROGRAM*********************/
/******************************************************/
int main(void){


//read all calibration data from the sensor
ReadCalibData(0);

SensorWakeup(1);		//user setting

while(1){
	StartStreaming(1,1);
		}
}
