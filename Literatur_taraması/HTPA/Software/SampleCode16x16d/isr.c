#include "Globals.h"

//////////////////////////////////
/*        ISR´s               	*/
////////////////////////////////


}

/********************************************************************
 * Function:        void T3_Interrupt_ISR(void)
 *
 * Description:     This function reads the Voltage of all IR Pixel and the PTAT
 *                  and directly writes the new calculated values into the output register (no IC1 Interrupt needed)
 *
 * Dependencies:    
 *******************************************************************/
void __ISR(_TIMER_3_VECTOR, ipl5)T3_Interrupt_ISR(void){
unsigned short k,i,LineVoltage;
static unsigned char LastReadBlock,ReadBlindFrame=0,VddSampledBefore=0,VddSampledNow=0;
TAsicSFR I2Creg;
static unsigned short FramesBeforeEl=0;

/*	Order of Voltage[][i]:
*	0 to Pixel-1 (1023):						Pixelvoltage
*	Pixel(1024) to Pixel+63 (1087):				el. Offset
*	1088:										VDD
*	1089:										TAmb
*	PTATSTARTADSRESS (1090) to DATALength-1:	PTAT
*/

	do{
		p=data;
		I2Creg.adr=0x1A;
		I2Creg.read=0x00;
		I2Creg.Config=0x02;
		Read_RAM(data,I2Creg,0x01);
	}while((data[0]&0x01)!=0x01);
#endif
	//Now readout the sensor either active or blind block
	I2Creg.adr=0x1A;
	I2Creg.read=0x00;
	I2Creg.Config=0x0A;
	p=data;
	Read_RAM(data,I2Creg,Pixelforth+2);

	I2Creg.adr=0x1A;
	I2Creg.read=0x00;
	I2Creg.Config=0x0B;
	p2=data2;
	Read_RAM(data2,I2Creg,Pixelforth+2);

	//and start now already the sensor again to shorten the calculation and UDPPutArray time
	LastReadBlock=currentReadBlock;
	currentReadBlock++;			//get the next block
#ifndef IgnoreElOffset
	if(GetBlindFrame){
		ReadBlindFrame=1;
		GetBlindFrame=0;
		currentReadBlock=0;
	}
#endif
	if(currentReadBlock>1){
		currentReadBlock=0;
		#ifndef IgnoreElOffset
			FramesBeforeEl++;
			if(FramesBeforeEl>(unsigned short)GetElEveryFrameX){
				GetBlindFrame=1;		//get a blind frame now
				FramesBeforeEl=0;
			}
		#endif
	}
	
	I2Creg.adr=0x1A;
	I2Creg.read=0x00;
	I2Creg.Config=0x01;

	if(!GetBlindFrame){
		if(!GetVddMeas){
			I2Creg.sfrdata=(currentReadBlock<<4)|0x09;
			VddSampledNow=0;
		}	
		else{
			I2Creg.sfrdata=(currentReadBlock<<4)|0x0D;		//start Vdd-Meas next
			VddSampledNow=1;
		}
	}
	else{
		I2Creg.sfrdata=0x0B;			//blind frame
	}

	WriteConf(I2Creg);
	TMR3 = 0x00;					// Clear 32-bit Timer (msw)
	TMR2 = 0x00; 					// Clear 32-bit Timer (lsw)
#ifdef GroundI2C
//ground the I²C and turn it off
	I2C1CON = 0x1200;
	TRISAbits.TRISA14=0;
	TRISAbits.TRISA15=0;
	LATAbits.LATA14=0;
	LATAbits.LATA15=0;
#endif


#ifndef IgnoreElOffset
	if(!ReadBlindFrame){
#endif
		if(VddSampledBefore){
			VddSensorStack[pVddSensorStack]=(*(unsigned char*)(p++))<<8;
			VddSensorStack[pVddSensorStack]|=(*(unsigned char*)(p++)&0x00FF);
			pVddSensorStack++;
		}
		else{
			PTATStack[pPTATStack]=(*(unsigned char*)(p++))<<8;
			PTATStack[pPTATStack]|=(*(unsigned char*)(p++)&0x00FF);
			Voltage[currentBuffer][PTATSTARTADSRESS+LastReadBlock*2]=PTATStack[pPTATStack];
			pPTATStack++;
		}
		for(i=0;i<4;i++){
			//Line=LastReadBlock*64+i*256;		//Sensor Rev. 0
		//	Line=LastReadBlock*(unsigned short)Pixelforth+i*SPALTE*2;		//Sensor Rev. 1
			LineVoltage=LastReadBlock*PixelEighth+i*SPALTE;
			for(k=0;k<SPALTE;k++){
				Voltage[currentBuffer][k+LineVoltage]=(*(unsigned char*)(p++))<<8;
				//Voltage[currentBuffer][k+LineVoltage]=Voltage[currentBuffer][k+LineVoltage]<<8;
				Voltage[currentBuffer][k+LineVoltage]|=(*(unsigned char*)(p++)&0x00FF);
				//Voltage[currentBuffer][k+LineVoltage]=Voltage[currentBuffer][k+LineVoltage]<<(16-Resolution);
			}
		}	
	
		//now get the second block
		if(VddSampledBefore){
			VddSensorStack[pVddSensorStack]=(*(unsigned char*)(p2++))<<8;
			VddSensorStack[pVddSensorStack]|=(*(unsigned char*)(p2++)&0x00FF);
			pVddSensorStack++;
			if(pVddSensorStack==STACKSIZEVDD)
				pVddSensorStack=0;
		}
		else{
			PTATStack[pPTATStack]=(*(unsigned char*)(p2++))<<8;
			PTATStack[pPTATStack]|=(*(unsigned char*)(p2++)&0x00FF);
			Voltage[currentBuffer][PTATSTARTADSRESS+LastReadBlock*2+1]=PTATStack[pPTATStack];
			pPTATStack++;
			if(pPTATStack==STACKSIZEPTAT)
				pPTATStack=0;
		}
		VddSampledBefore=VddSampledNow;
		if(pPTATStack==STACKSIZEPTAT)
			pPTATStack=0;
		for(i=0;i<4;i++){
			//Line=LastReadBlock*64+i*256+1024;			//Sensor Rev. 0
//			Line=(3-LastReadBlock)*(unsigned short)Pixelforth+(3-i)*64+(unsigned short)Pixel;	//Sensor Rev. 1
			LineVoltage=(1-LastReadBlock)*PixelEighth+(3-i)*SPALTE+Pixel/2;
			for(k=0;k<SPALTE;k++){
				Voltage[currentBuffer][k+LineVoltage]=(*(unsigned char*)(p2++))<<8;
				//Voltage[currentBuffer][k+LineVoltage]=Voltage[currentBuffer][k+LineVoltage]<<8;
				Voltage[currentBuffer][k+LineVoltage]|=(*(unsigned char*)(p2++)&0x00FF);
				//Voltage[currentBuffer][k+LineVoltage]=Voltage[currentBuffer][k+LineVoltage]<<(16-Resolution);
			}
		}
#ifndef IgnoreElOffset
	}
	else{
		ReadBlindFrame=0;
		currentReadBlock=0;		//not needed, is set before to 0
		//first PTAT?
		p+=2;
		for(k=0;k<ELAMOUNTHALF;k++){
			eloffstack[k][pelStack]=(*(unsigned char*)(p++))<<8;
			//eloffstack[k][pelStack]=eloffstack[k][pelStack]<<8;
			eloffstack[k][pelStack]|=(*(unsigned char*)(p++)&0x00FF);	// Es gibt 128 Blind Pixel je Hälfte!!!
		}
		//first PTAT?
		p2+=2;
		for(i=0;i<4;i++){
			LineVoltage=(3-i)*SPALTE+ELAMOUNTHALF;
			for(k=0;k<SPALTE;k++){
				eloffstack[k+LineVoltage][pelStack]=(*(unsigned char*)(p2++))<<8;
				//eloffstack[k+LineVoltage][pelStack]=eloffstack[k+LineVoltage][pelStack]<<8;
				eloffstack[k+LineVoltage][pelStack]|=(*(unsigned char*)(p2++)&0x00FF);	// Es gibt 128 Blind Pixel je Hälfte!!!
			}
		}
//			for(k=ELAMOUNTHALF;k<ELAMOUNT;k++){
//				eloffstack[k][pelStack]=*(unsigned char*)(p2++);
//				eloffstack[k][pelStack]=eloffstack[k][pelStack]<<8;
//				eloffstack[k][pelStack]+=*(unsigned char*)(p2++);	// Es gibt 128 Blind Pixel je Hälfte!!!
//			}
		pelStack++;
		if(pelStack==StackSize){
			pelStack=0;
			useStack=1;
		}
	}
#endif
	if(LastReadBlock==1){		//now push the data out
//		GetVDD(0);
//		Voltage[currentBuffer][VDDADDRESS]=(unsigned short)VDD;
		if(!GetBlindFrame){
			GetVddMeas+=1;
			if(GetVddMeas>2)
				GetVddMeas=0;
		}
		currentBuffer^=0x01;
		giveOut=1;	
		if(SingleFrame){
			SingleFrame++;
		}
	}

	IFS0bits.T3IF=0;


return;
}


