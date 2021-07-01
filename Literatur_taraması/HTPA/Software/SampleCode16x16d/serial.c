#include "Globals.h"



/************************************************************/
/*	int InitPorts(void)										*/
/*															*/
/*		Initialize Ports									*/
/*															*/
/*		Inputparameters:	none							*/
/*		Outputparameters:	none							*/
/************************************************************/
void InitPorts(){
}



/////bestimmt To aus den Messwerten (Streaming)/////////////////
/********************************************************************
 * Function:        void StartStreaming(char Temps,char Stream)
 *
 * Description:     This function starts the streaming of the Objecttemperature
 *                  main sequence can be seen in the interrupt
 *
 * Dependencies:    Temps: Give out calculated temperatures
 *					Stream: keep on streaming 
 *******************************************************************/
void StartStreaming(char Temps,char Stream){
unsigned int k,i,j;
unsigned char outbuffer;
TAsicSFR I2Creg;
unsigned long PTATLong,Vddlong;
signed long VoltageLong,PowerGradScale,NewDeltaVdd;
signed short VoltageSigned,DeltaVdd;
signed long long deltaPixC,PixCComp;
unsigned long long Ta4pow,To4pow;
unsigned long aveloff;
unsigned long long DividerVdd,DividerVdd2;
signed long long VoltageLongLong;



firstrun=1;
currentReadBlock=0;

pelStack=0;
useStack=0;

//Benutze hier einen 32Bit Timer um die Ausleserate mit der framerate zu synchroniseren
T3CONbits.TON = 0;				// Stop any 16-bit Timer3 operation
T2CONbits.TON = 0;				// Stop any 16/32-bit Timer2 operation
T2CONbits.T32 = 1;				// Enable 32-bit Timer mode
T2CONbits.TCKPS = 0x00;			// Select 1:1 Prescaler (=000)
T2CONbits.TCS = 0;				// Select internal instruction cycle clock
T2CONbits.TGATE = 0;			// Disable Gated Timer mode
TMR3 = 0x00;					// Clear 32-bit Timer (msw)
TMR2 = 0x00; 					// Clear 32-bit Timer (lsw)
//take a little bit lower value and then check the status flag in the int
#ifdef GroundI2C
	PR3 = (unsigned int)((unsigned int)FCY*((float)tFR*1.4)/1000.0);		// Load 32-bit period value (msw)
	PR2 = (unsigned int)((unsigned int)FCY*((float)tFR*1.4)/1000.0); 		// Load 32-bit period value (lsw)
#endif
// IPC3bits.T3IP = 0x04;			// Set Timer3 Interrupt Priority Level
INT1occ=0;
calib=0;
giveOut=0;

//enable I²C in case it was switched off before and set all registers
SensorWakeup(1-Temps);



	//fill the stack (PTAT) first
	I2Creg.adr=0x1A;
	I2Creg.read=0x00;
	I2Creg.Config=0x01;
	I2Creg.sfrdata=0x09;		//always start with block 0
	WriteConf(I2Creg);
	do{
		p=data;
		I2Creg.adr=0x1A;
		I2Creg.read=0x00;
		I2Creg.Config=0x02;
		Read_RAM(data,I2Creg,0x01);
	}while((data[0]&0x01)!=0x01);
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
	
	PTATStack[0]=(*(unsigned char*)(p++))<<8;
	PTATStack[0]|=(*(unsigned char*)(p++)&0x00FF);
	PTATStack[1]=(*(unsigned char*)(p2++))<<8;
	PTATStack[1]|=(*(unsigned char*)(p2++)&0x00FF);
	for(i=2;i<STACKSIZEPTAT;i+=2){
		PTATStack[i]=PTATStack[0];
		PTATStack[i+1]=PTATStack[1];
	}
	pPTATStack=2;

	//PTAT stack is full, now do the same for the Vdd stack
	I2Creg.adr=0x1A;
	I2Creg.read=0x00;
	I2Creg.Config=0x01;
	I2Creg.sfrdata=0x0D;		//always start with block 0
	WriteConf(I2Creg);
	do{
		p=data;
		I2Creg.adr=0x1A;
		I2Creg.read=0x00;
		I2Creg.Config=0x02;
		Read_RAM(data,I2Creg,0x01);
	}while((data[0]&0x01)!=0x01);
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

	VddSensorStack[0]=(*(unsigned char*)(p++))<<8;
	VddSensorStack[0]|=(*(unsigned char*)(p++)&0x00FF);
	VddSensorStack[1]=(*(unsigned char*)(p2++))<<8;
	VddSensorStack[1]|=(*(unsigned char*)(p2++)&0x00FF);
	for(i=2;i<STACKSIZEVDD;i+=2){
		VddSensorStack[i]=VddSensorStack[0];
		VddSensorStack[i+1]=VddSensorStack[1];
	}
	pVddSensorStack=2;

	I2Creg.adr=0x1A;
	I2Creg.read=0x00;
	I2Creg.Config=0x01;

	I2Creg.sfrdata=0x0B;		//always start with block 0 and el. Offset
	GetBlindFrame=1;

	WriteConf(I2Creg);
	
	if(!Stream){
		SingleFrame=1;
	}
	else{
		SingleFrame=0;
	}
	

do{
	if(giveOut){
		outbuffer=currentBuffer^0x01;
			for(k=0;k<ELAMOUNTHALF;k++){
				if(useStack){
					aveloff=eloffstack[k][0];
					for(j=1;j<StackSize;j++){
						aveloff+=eloffstack[k][j];
					}
					Voltage[outbuffer][ELOFFSET+k]=(unsigned short)(aveloff/(unsigned long)StackSize);
				}
				else{
					aveloff=0;
					for(j=0;j<pelStack;j++){
						aveloff+=eloffstack[k][j];
					}
					Voltage[outbuffer][ELOFFSET+k]=(unsigned short)(aveloff/(unsigned long)j);
				}
			}
	
			for(k=ELAMOUNTHALF;k<ELAMOUNT;k++){
				if(useStack){
					aveloff=eloffstack[k][0];
					for(j=1;j<StackSize;j++){
						aveloff+=eloffstack[k][j];
					}
					Voltage[outbuffer][ELOFFSET+k]=(unsigned short)(aveloff/(unsigned long)StackSize);
				}
				else{
					aveloff=0;
					for(j=0;j<pelStack;j++){
						aveloff+=eloffstack[k][j];
					}
					Voltage[outbuffer][ELOFFSET+k]=(unsigned short)(aveloff/(unsigned long)j);
				}
			}
		Vddlong=(unsigned long)VddSensorStack[0];
		for(k=1;k<STACKSIZEVDD;k++){
			Vddlong+=(unsigned long)VddSensorStack[k];
		}
		Vddlong/=(unsigned long)STACKSIZEVDD;
		Voltage[outbuffer][VDDADDRESS]=(unsigned short)Vddlong;	//this overwrites the Vdd of the PCB with the Vdd of the sensor
		if(Temps||Corr){		//calculate Temperatures
			//first Ta
			PTATLong=0;
			for(k=0;k<(unsigned short)STACKSIZEPTAT;k++){
				PTATLong+=(unsigned long)PTATStack[k];
			}
			TA=(unsigned short)((float)(PTATLong<<(16-Resolution))/(float)STACKSIZEPTAT*PTATGrad+PTATOff+0.5);		
			PTATLong/=(unsigned long)STACKSIZEPTAT;		//is needed later	
			Voltage[outbuffer][VDDADDRESS+1]=TA;
			PowerGradScale=(signed long)pow(2.0,(float)gradScale);

				//DeltaVdd=(signed short)((signed long)Vddlong-(signed long)VddRef);
				NewDeltaVdd=(signed long)PTATLong;
				NewDeltaVdd-=(signed long)PTATThermals[0];
				NewDeltaVdd*=((signed long)VddRef[1]-(signed long)VddRef[0]);
				NewDeltaVdd/=((signed long)PTATThermals[1]-(signed long)PTATThermals[0]);
				NewDeltaVdd=(signed long)Vddlong-(signed long)VddRef[0]-NewDeltaVdd;
				DeltaVdd=(signed short)NewDeltaVdd;
				DividerVdd=(unsigned long long)(pow(2.0,(float)VddScaling));
				DividerVdd2=(unsigned long long)(pow(2.0,(float)VddScalingOff));

			for(k=0;k<(unsigned short)Pixel/2;k++){		//subtract the el. and thermal Offsets
				Voltage[outbuffer][k]=Voltage[outbuffer][k]<<(16-Resolution);
				VoltageLong=(signed long)((signed short)ThGrad[k]);
				VoltageLong*=(signed long)PTATLong;
				VoltageLong/=PowerGradScale;
				VoltageLong+=(signed long)ThOff[k];
				VoltageLong+=(signed long)(Voltage[outbuffer][k%ELAMOUNTHALF+ELOFFSET]<<(16-Resolution));
				VoltageLongLong=(signed long long)VddGrad[k%ELAMOUNTHALF];
				VoltageLongLong*=(signed long long)PTATLong;
				VoltageLongLong/=(signed long long)DividerVdd;
				VoltageLongLong+=(signed long long)VddOff[k%ELAMOUNTHALF];
				VoltageLongLong*=(signed long long)DeltaVdd;
				VoltageLongLong/=(signed long long)DividerVdd2;
				VoltageLong+=(signed long)VoltageLongLong;
				VoltageSigned=(signed short)Voltage[outbuffer][k]-(signed short)VoltageLong;
				if(Corr){
					//simply add an offset
					VoltageSigned+=(signed short)CorrOffset;
					Voltage[outbuffer][k]=(unsigned short)VoltageSigned;
				}
				else{
					//calc. Temperatures
					Voltage[outbuffer][k]=(unsigned short)calcTO(TA,VoltageSigned,PixC[k],k);
				}
			}
			for(i=ZEILE/2;i<ZEILE;i++){
				for(k=0;k<SPALTE;k++){		//subtract the thermal Offsets
					Voltage[outbuffer][k+i*SPALTE]=Voltage[outbuffer][k+i*SPALTE]<<(16-Resolution);
					VoltageLong=(signed long)((signed short)ThGrad[k+(ZEILE/2+ZEILE-1-i)*SPALTE]);
					VoltageLong*=(signed long)PTATLong;
					VoltageLong/=PowerGradScale;
					VoltageLong+=(signed long)ThOff[k+(ZEILE/2+ZEILE-1-i)*SPALTE];
					VoltageLong+=(signed long)(Voltage[outbuffer][(k+i*SPALTE)%ELAMOUNTHALF+ELAMOUNTHALF+ELOFFSET]<<(16-Resolution));
					VoltageLongLong=(signed long long)VddGrad[k+(3-i%4)*SPALTE+ELAMOUNTHALF];
					VoltageLongLong*=(signed long long)PTATLong;
					VoltageLongLong/=(signed long long)DividerVdd;
					VoltageLongLong+=(signed long long)VddOff[k+(3-i%4)*SPALTE+ELAMOUNTHALF];
					VoltageLongLong*=(signed long long)DeltaVdd;
					VoltageLongLong/=(signed long long)DividerVdd2;						VoltageLong+=(signed long)VoltageLongLong;
					VoltageSigned=(signed short)Voltage[outbuffer][k+i*SPALTE]-(signed short)VoltageLong;
					if(Corr){
						VoltageSigned+=(signed short)CorrOffset;
						Voltage[outbuffer][k+i*SPALTE]=(unsigned short)VoltageSigned;
					}
					else{
						//calc. Temperatures
							Voltage[outbuffer][k+i*SPALTE]=(unsigned short)calcTO(TA,VoltageSigned,PixC[k+(ZEILE/2+ZEILE-1-i)*SPALTE],1);
					}
				}
			}
			//now give out the data		
		}

		UDPPutArray((BYTE*)Voltage[outbuffer],DATALength);  

	}
}while((!INT1occ)&&(SingleFrame<3));

return;
}


/********************************************************************
 * Function:        void ReadCalibData(unsigned char print)
 *
 * Description:     This function reads the content of the EEPROM
 *
 * Dependencies:    print: -> statement for printing via UART/UDP
 *******************************************************************/
//read back all calibration from the sensor EEPROM and store them in the global variables
void ReadCalibData(unsigned char print){
TAsicSFR I2Creg;
unsigned short i,PixCRaw;
float common[2];
char eecpy[8];


	p=data;
	Read_EEPROM(data,0x00,0x50,I2Creg);
	memcpy((char*)&PixCMin,(char*)&data[0],4);
	memcpy((char*)&PixCMax,(char*)&data[(unsigned short)AdrPixCMax*2],4);
	memcpy((char*)&gradScale,(char*)&data[(unsigned short)AdrGradScale*2],2);
	memcpy((char*)&GlobalGain,(char*)&data[(unsigned short)AdrGlobalGain*2],2);
	if((GlobalGain==0x0000)||(GlobalGain==0xFFFF)){
		GlobalGain=10000;
		memcpy(eecpy,(char*)&GlobalGain,sizeof(short));
		HighDensPageWrite((unsigned short)AdrGlobalGain,eecpy,sizeof(short));
	}
	memcpy((char*)&TableNumberSensor,(char*)&data[(unsigned short)AdrTableNumber*2],2);

	sTABLEOFFSET=TABLEOFFSET;

	memcpy((char*)&epsilon,(char*)&data[(unsigned short)AdrEpsilon*2],2);

	memcpy((char*)&SetBIASCalib,(char*)&data[(unsigned short)AdrBIASPixC*2],2);
	memcpy((char*)&SetCLKCalib,(char*)&data[(unsigned short)AdrCLKPixC*2],2);
	memcpy((char*)&SetMBITCalib,(char*)&data[(unsigned short)AdrMBITPixC*2],2);
	memcpy((char*)&SetBPACalib,(char*)&data[(unsigned short)AdrBPAPixC*2],2);
	memcpy((char*)&SetPUCalib,(char*)&data[(unsigned short)AdrPUPixC*2],2);

	memcpy((char*)&SetBIASUser,(char*)&data[(unsigned short)AdrBIASUser*2],2);
	memcpy((char*)&SetCLKUser,(char*)&data[(unsigned short)AdrCLKUser*2],2);
	memcpy((char*)&SetMBITUser,(char*)&data[(unsigned short)AdrMBITUser*2],2);
	memcpy((char*)&SetBPAUser,(char*)&data[(unsigned short)AdrBPAUser*2],2);
	memcpy((char*)&SetPUUser,(char*)&data[(unsigned short)AdrPUUser*2],2);

	memcpy((char*)&PTATGrad,(char*)&data[(unsigned short)AdrPTATGrad*2],4);
	memcpy((char*)&PTATOff,(char*)&data[(unsigned short)AdrPTATGrad*2+4],4);
	memcpy((char*)&DevID,(char*)&data[(unsigned short)AdrDevID*2],4);

	//read all settings for clock etc. from the EEPROM, if not set, use the default values!
	if((SetMBITUser<0x09)||(SetMBITUser>0x3C)){		//EEPROM empty or setting out of spec -> set to default
		SetMBITUser=(unsigned char)MBITTRIMDefault;
		eecpy[0]=SetMBITUser;
		HighDensPageWrite((unsigned short)AdrMBITUser,eecpy,sizeof(char));
	}
	if((!SetBIASUser)||(SetBIASUser>0x1F)){		//EEPROM empty or setting out of spec -> set to default
		SetBIASUser=(unsigned char)BIAScurrentDefault;
		eecpy[0]=SetBIASUser;
		HighDensPageWrite((unsigned short)AdrBIASUser,eecpy,sizeof(char));
	}

	if(SetCLKUser>0x3F){		//EEPROM empty or setting out of spec -> set to default
		SetCLKUser=(unsigned char)CLKTRIMDefault;
		eecpy[0]=SetCLKUser;
		HighDensPageWrite((unsigned short)AdrCLKUser,eecpy,sizeof(char));
	}


	if((!SetBPAUser)||(SetBPAUser>0x1F)){		//EEPROM empty or setting out of spec -> set to default
		SetBPAUser=(unsigned char)BPATRIMDefault;
		eecpy[0]=SetBPAUser;
		HighDensPageWrite((unsigned short)AdrBPAUser,eecpy,sizeof(char));
	}
	if(!((SetPUUser==0x11)||(SetPUUser==0x22)||(SetPUUser==0x44)||(SetPUUser==0x88))){		//EEPROM empty or setting out of spec -> set to default
		SetPUUser=(unsigned char)PUTRIMDefault;
		eecpy[0]=SetPUUser;
		HighDensPageWrite((unsigned short)AdrPUUser,eecpy,sizeof(char));
	}

	//read Thermals now
	p=data;
	Read_EEPROM(data,AdrTh1,0x100,I2Creg);
	memcpy((char*)&ThGrad,(char*)&data,Pixel*2);
	p=data;
	Read_EEPROM(data,AdrTh2,0x100,I2Creg);
	memcpy((char*)&ThOff,(char*)&data,Pixel*2);
	//and PixC if already done
	p=data;
	Read_EEPROM(data,AdrPixC,0x100,I2Creg);
		for(i=0;i<Pixel;i++){
			memcpy((char*)&PixCRaw,(char*)&data[i*2],2);
			PixC[i]=(unsigned long)((((float)PixCRaw/65535.0)*(PixCMax-PixCMin)+(float)PixCMin)*(float)epsilon/100.0*(float)GlobalGain/10000.0+0.5);
		}	

	
	for(i=0;i<(unsigned short)ELAMOUNT;i+=4){
		//scaled to 12 bit, so merge 4 values at a time due to the 16 bit EEPROM
		HighDensSequentialRead((unsigned short)AdrVddCompValues2+i*3/4,eecpy,sizeof(char)*6);
		VddGrad[i]=(signed short)((((eecpy[1]&0x0F)<<8)+eecpy[0])&0xFFF);
		VddGrad[i]-=0x800;
		VddGrad[i+1]=(signed short)(((eecpy[2]<<8)+(eecpy[1]&0xF0)>>4)&0xFFF);
		VddGrad[i+1]-=0x800;
		VddGrad[i+2]=(signed short)((((eecpy[4]&0x0F)<<8)+eecpy[3])&0xFFF);
		VddGrad[i+2]-=0x800;
		VddGrad[i+3]=(signed short)(((eecpy[5]<<8)+(eecpy[4]&0xF0)>>4)&0xFFF);
		VddGrad[i+3]-=0x800;
		HighDensSequentialRead((unsigned short)AdrVddCompValues+i*3/4,eecpy,sizeof(char)*6);
		VddOff[i]=(signed short)((((eecpy[1]&0x0F)<<8)+eecpy[0])&0xFFF);
		VddOff[i]-=0x800;
		VddOff[i+1]=(signed short)(((eecpy[2]<<8)+(eecpy[1]&0xF0)>>4)&0xFFF);
		VddOff[i+1]-=0x800;
		VddOff[i+2]=(signed short)((((eecpy[4]&0x0F)<<8)+eecpy[3])&0xFFF);
		VddOff[i+2]-=0x800;
		VddOff[i+3]=(signed short)(((eecpy[5]<<8)+(eecpy[4]&0xF0)>>4)&0xFFF);
		VddOff[i+3]-=0x800;	
	}
	HighDensSequentialRead((unsigned short)AdrVddScaling,eecpy,sizeof(char));
	VddScaling=(unsigned short)eecpy[0];	
	HighDensSequentialRead((unsigned short)AdrVddScalingOff,eecpy,sizeof(char));
	VddScalingOff=(unsigned short)eecpy[0];
	HighDensSequentialRead((unsigned short)AdrVddMeasTh1,eecpy,sizeof(char)*4);
	memcpy((char*)&VddRef,eecpy,sizeof(short)*2);
	HighDensSequentialRead((unsigned short)AdrPTATTh1,eecpy,sizeof(char)*4);
	memcpy((char*)&PTATThermals,eecpy,sizeof(short)*2);	
	
return;
}


///////////////////////////////////////////////////////////////////////////////////


/********************************************************************
 * Function:        void InitCLKTRIM(unsigned char user)
 *
 * Description:     
 *
 * Dependencies:    user:	0-> Setting during Calibration
 *							1-> Setting from user
 *******************************************************************/
void InitCLKTRIM(unsigned char user){
	TAsicSFR I2Creg;

	I2Creg.adr=0x1A;	
	I2Creg.read=0x00;
	I2Creg.Config=0x06;		//CLK_TRIM
	if(user){
		I2Creg.sfrdata=SetCLKUser;
		MCLK=(unsigned long)(190476*(unsigned long)SetCLKUser+1000000);		//0=1MHz, 63=13MHz
	}
	else{	
		I2Creg.sfrdata=SetCLKCalib;
		MCLK=(unsigned long)(190476*(unsigned long)SetCLKCalib+1000000);		//0=1MHz, 63=13MHz
	}
	WriteConf(I2Creg);
	tFR=(unsigned int)((pow(2.0,(float)Resolution-4.0)+4.0)*32.0*1000.0/(float)MCLK);					//Time for one frame in ms
	if(tFR<0x9)
		tFR=0x9;	//Michael: tbd, is reduced due to el. Offset...
	return;
}

/********************************************************************
 * Function:        void InitBIASTRIM(unsigned char user)
 *
 * Description:     
 *
 * Dependencies:    user:	0-> Setting during Calibration
 *							1-> Setting from user
 *******************************************************************/
void InitBIASTRIM(unsigned char user){
	TAsicSFR I2Creg;

	if(user)
		I2Creg.sfrdata=SetBIASUser;
	else	
		I2Creg.sfrdata=SetBIASCalib;
	I2Creg.adr=0x1A;	
	I2Creg.read=0x00;
	I2Creg.Config=0x04;		//BIAS_TRIM_LEFT
	WriteConf(I2Creg);
	
	DelayMs(5);
	
	I2Creg.adr=0x1A;	
	I2Creg.read=0x00;
	I2Creg.Config=0x05;		//BIAS_TRIM_RIGHT
	WriteConf(I2Creg);
	return;
}

/********************************************************************
 * Function:        void InitMBITTRIM(unsigned char user)
 *
 * Description:     
 *
 * Dependencies:    user:	0-> Setting during Calibration
 *							1-> Setting from user
 *******************************************************************/
void InitMBITTRIM(unsigned char user){
	TAsicSFR I2Creg;

	I2Creg.adr=0x1A;	
	I2Creg.read=0x00;
	I2Creg.Config=0x03;		//MBIT_TRIM
	if(user){
		I2Creg.sfrdata=SetMBITUser;
		Resolution=(SetMBITUser&0x0F)+4;
	}
	else{	
		I2Creg.sfrdata=SetMBITCalib;
		Resolution=(SetMBITCalib&0x0F)+4;
	}
	WriteConf(I2Creg);
	return;
}

/********************************************************************
 * Function:        void InitBPATRIM(unsigned char user)
 *
 * Description:     
 *
 * Dependencies:    user:	0-> Setting during Calibration
 *							1-> Setting from user
 *******************************************************************/
void InitBPATRIM(unsigned char user){
	TAsicSFR I2Creg;

	if(user)
		I2Creg.sfrdata=SetBPAUser;
	else	
		I2Creg.sfrdata=SetBPACalib;
	I2Creg.adr=0x1A;	
	I2Creg.read=0x00;
	I2Creg.Config=0x07;		//BIAS_TRIM_LEFT
	WriteConf(I2Creg);
	
	DelayMs(5);
	
	I2Creg.adr=0x1A;	
	I2Creg.read=0x00;
	I2Creg.Config=0x08;		//BIAS_TRIM_RIGHT
	WriteConf(I2Creg);
	return;
}

/********************************************************************
 * Function:        void InitPUTRIM(unsigned char user)
 *
 * Description:     
 *
 * Dependencies:    user:	0-> Setting during Calibration
 *							1-> Setting from user
 *******************************************************************/
void InitPUTRIM(unsigned char user){
	TAsicSFR I2Creg;

	I2Creg.adr=0x1A;	
	I2Creg.read=0x00;
	I2Creg.Config=0x09;		//PU_TRIM SDA & SCL
	if(user)
		I2Creg.sfrdata=SetPUUser;
	else	
		I2Creg.sfrdata=SetPUCalib;
	WriteConf(I2Creg);
	return;
}


/********************************************************************
 * Function:        void SensorSleep(void)
 *
 * Description:     
 *
 * Dependencies:    none
 *******************************************************************/
void SensorSleep(void){
	TAsicSFR I2Creg;

	//set the Wake-up bit to 0, do this twice to make it work properly
	I2Creg.adr=0x1A;
	I2Creg.read=0x00;
	I2Creg.Config=0x00;
	I2Creg.sfrdata=0x01;
	WriteConf(I2Creg);

	I2Creg.adr=0x1A;
	I2Creg.read=0x00;
	I2Creg.Config=0x00;
	I2Creg.sfrdata=0x01;
	WriteConf(I2Creg);
	//set the SCL and SDA pins to high and disable the I²C Interface
	I2C1CON = 0x1200;
	TRISAbits.TRISA14=0;
	TRISAbits.TRISA15=0;
	LATAbits.LATA14=1;
	LATAbits.LATA15=1;
return;
}

/********************************************************************
 * Function:        void SensorWakeup(unsigned char user)
 *
 * Description:     
 *
 * Dependencies:    none
 *******************************************************************/
void SensorWakeup(unsigned char user){
	TAsicSFR I2Creg;

	TRISAbits.TRISA14=0;
	TRISAbits.TRISA15=0;
	LATAbits.LATA14=1;
	LATAbits.LATA15=1;
	Delay10us(50);
	InitI2C(0);
	I2Creg.adr=0x1A;
	I2Creg.read=0x00;
	I2Creg.Config=0x01;
	I2Creg.sfrdata=0x01;
	WriteConf(I2Creg);
	//write all Settings again to the sensor
	DelayMs(10);		//give the sensor some time to wake-up
	InitMBITTRIM(user);
	DelayMs(5);
	InitBIASTRIM(user);
	DelayMs(5);
	InitBPATRIM(user);
	DelayMs(5);
	InitPUTRIM(user);
	DelayMs(5);
	InitCLKTRIM(user);
	DelayMs(5);
return;
}



