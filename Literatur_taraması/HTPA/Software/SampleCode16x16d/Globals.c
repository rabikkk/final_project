#include "Globals.h"

/*global variables*/
unsigned short epsilon,Ta;	//Eingabe kann weg
unsigned char data[Pixel*2],data2[Pixelforth+2],firstrun,currentReadBlock=0;
unsigned char *p,*p2;
volatile char INT1occ=0;
char tempstring[150];
const char version[]={"HTPA16x16dR1 2017/08/29 v.0.01 Heimann Sensor GmbH; written by M. Schnorr\r\n"};
unsigned short DevID;
volatile unsigned int VDD;
unsigned char Corr=0;
unsigned char giveOut,currentBuffer=0;

unsigned long MCLK;
unsigned int tFR;
unsigned char GetBlindFrame=0,BIAScurrent=0x0C,CLKTRIM=0x15,BPATRIM=0x0C,MBIT_TRIM=0x0C,PU_TRIM=0x88,Resolution;//0x88;
float PTATGrad,PTATOff,PixCMin,PixCMax;
signed long PixC[Pixel];
signed short ThOff[Pixel];

unsigned short gradScale,SetBIASCalib,SetCLKCalib,SetBIASUser,SetCLKUser;
signed short ThGrad[Pixel];
unsigned short SetMBITCalib,SetMBITUser,SetBPACalib,SetBPAUser,SetPUCalib,SetPUUser;

unsigned char calib,SingleFrame;
unsigned short Voltage[2][Pixel+PTATamount+ELAMOUNT+2];		//+2 for VDD and TA
unsigned short TA;
unsigned short TableNumberSensor;

unsigned short eloffstack[ELAMOUNT][StackSize];
volatile unsigned char pelStack,useStack;

#ifdef Multitable
	unsigned char TableToUse;
#endif
unsigned short GlobalGain;
char RefCal,pPTATStack;
unsigned short PTATStack[STACKSIZEPTAT];
char GetVddMeas=0,pVddSensorStack;
unsigned short VddSensorStack[STACKSIZEVDD],VddRef[2],PTATThermals[2];
unsigned char VddScaling;

signed short VddGrad[ELAMOUNT];
signed short VddOff[ELAMOUNT];
unsigned char VddScalingOff;

signed long sTABLEOFFSET;//Bodo: Neu eingeführt um Variablen Offset zu realisieren





