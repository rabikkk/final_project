/* 
 * File:   Globals.h
 * Author: Bodo Forg    //adapted by Michael Schnorr
 *
 * Changed on 28. Mai 2015, 12:05
 */


#ifndef GLOBALS_H
#define	GLOBALS_H


/*global variables*/
extern unsigned short epsilon,Ta;	//Eingabe kann weg
extern unsigned char data[Pixel*2],data2[Pixelforth+2],firstrun,currentReadBlock;
extern unsigned char *p,*p2;
extern volatile char INT1occ;
extern char tempstring[150];
extern const char version[];
extern unsigned short DevID;			//Achtung: es wird immer mit der doppelten framerate gerechnet, damit diese ein short sein kann
extern volatile unsigned int VDD;
extern unsigned char Corr;
extern unsigned char giveOut,currentBuffer;

extern unsigned long MCLK;
extern unsigned int tFR;
extern unsigned char GetBlindFrame,BIAScurrent,CLKTRIM,BPATRIM,MBIT_TRIM,PU_TRIM,Resolution;//0x88;
extern float PTATGrad,PTATOff,PixCMin,PixCMax;
extern signed long PixC[Pixel];
extern signed short ThOff[Pixel];

extern unsigned short gradScale,SetBIASCalib,SetCLKCalib,SetBIASUser,SetCLKUser;
extern signed short ThGrad[Pixel];
extern unsigned short SetMBITCalib,SetMBITUser,SetBPACalib,SetBPAUser,SetPUCalib,SetPUUser;
extern unsigned char calib,SingleFrame;
extern unsigned short Voltage[2][Pixel+PTATamount+ELAMOUNT+2];
extern unsigned short TA;

extern unsigned short TableNumberSensor;
#ifdef ReadToFromTable
#ifdef EQUIADTABLE
	extern const unsigned int YADValues[NROFADELEMENTS];
#else
	extern const signed int YADValues[NROFADELEMENTS];
#endif
//extern volatile char epsilon;	//emission factor
extern const unsigned int XTATemps [NROFTAELEMENTS];
extern const unsigned int TempTable[NrOfTables][NROFADELEMENTS][NROFTAELEMENTS];
extern unsigned char TableToUse;
#endif

extern unsigned short eloffstack[ELAMOUNT][StackSize];
extern volatile unsigned char pelStack,useStack;

extern unsigned short GlobalGain;
extern char RefCal,pPTATStack;
extern unsigned short PTATStack[STACKSIZEPTAT];
extern char GetVddMeas,pVddSensorStack;
extern unsigned short VddSensorStack[STACKSIZEVDD],VddRef[2],PTATThermals[2];
extern unsigned char VddScaling;

extern signed short VddGrad[ELAMOUNT];
extern signed short VddOff[ELAMOUNT];
extern unsigned char VddScalingOff;

extern signed long sTABLEOFFSET;//Bodo: Neu eingeführt um Variablen Offset zu realisieren

#endif	/* GLOBALS_H */

