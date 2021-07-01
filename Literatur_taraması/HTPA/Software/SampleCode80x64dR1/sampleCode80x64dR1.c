unsigned char data[1282],data2[1282];
unsigned char *p,*p2;
unsigned char giveOut,currentBuffer=0,test[2][(unsigned short)DATALength*2];	//DataLength=1292
unsigned short gradScale,TA,Voltage[2][Pixel+PTATamount+ELAMOUNT+2];			//Pixel=5120, PTATamount=8, ELAMOUNT=1280 (not needed anymore)
unsigned short SetMBITCalib,SetMBITUser,SetBPACalib,SetBPAUser,SetPUCalib,SetPUUser,Resolution;
unsigned short SetBIASCalib,SetCLKCalib,SetBIASUser,SetCLKUser,DevID;
signed char ThGrad[Pixel];
signed short ThOff[Pixel]
unsigned long PixC[Pixel];
unsigned short PTATStack[STACKSIZEPTAT],VddSensorStack[STACKSIZEVDD],pVddSensorStack,VddRef[2],,PTATThermals[2];
signed short VddGrad[ELAMOUNT];
signed short VddOff[ELAMOUNT];
unsigned char VddScaling,VddScalingOff;
unsigned short GlobalGain;
char GlobalOffset;
char GetVddMeas,RefCal,pPTATStack,pVddSensorStack;


int main(void){

//now wake up the sensor
I2Creg.adr=0x1A;
I2Creg.read=0x00;
I2Creg.Config=0x01;
I2Creg.sfrdata=0x01;
WriteConf(I2Creg);

DelayMs(5);		//Give the module some time to wake up

//read the calibration data from the sensor
ReadCalibData();

InitMBITTRIM(1);

DelayMs(5);

InitBIASTRIM(1);

DelayMs(5);

InitBPATRIM(1);

DelayMs(5);

InitPUTRIM(1);

DelayMs(5);

InitCLKTRIM(1);		//Resolution (MBIT) has to be set before!!!

do{
	//Readout and start the sensor.
	ReadSensor via e.g. Timer3
	//if temperature is required do the following (all done in the StartStreaming function)
	//1. determine ambient temperature via PTAT
	//2. substract the thermal offset for each pixel
	//3. determine the object temperature from the look-up table
	StartStreaming(1,1);
}while(1);
}

/********************************************************************
 * Function:        void T3_Interrupt_ISR(void)
 *
 * Description:     This function reads the Voltage of all IR Pixel and the PTAT
 *
 * Dependencies:    
 *******************************************************************/
void __ISR(_TIMER_3_VECTOR, ipl5)T3_Interrupt_ISR(void){
unsigned short k,i,LineVoltage;
static unsigned char LastReadBlock;
TAsicSFR I2Creg;

/*	Order of Voltage[][i]:
*	0 to Pixel-1 (5119):						Pixelvoltage
*	Pixel(5119) to Pixel+1280 (6399):			el. Offset
*	6400:										VDD
*	6401:										TAmb
*	PTATSTARTADSRESS (6402) to DATALength-1:	PTAT
*/

	//Wait until conversion is finished
	do{
		p=data;
		I2Creg.Config=0x02;
		Read_RAM(data,I2Creg,0x01);
	}while((data[0]&0x01)!=0x01);

	//Now readout the sensor either active or blind block
	I2Creg.Config=0x0A;
	p=data;
	Read_RAM(data,I2Creg,Pixelforth+2);

	I2Creg.Config=0x0B;
	p2=data2;
	Read_RAM(data2,I2Creg,Pixelforth+2);

	//and start now already the sensor again to shorten the calculation and UDPPutArray time
	LastReadBlock=currentReadBlock;
	currentReadBlock++;			//get the next block
	if(GetBlindFrame){
		if(FirstFrame){
			ReadBlindFrame=1;
			GetBlindFrame=1;
			currentReadBlock=0;
		}
		else{
			ReadBlindFrame=1;
			GetBlindFrame=0;
			currentReadBlock=0;
		}
	}

	if(currentReadBlock>3){
		currentReadBlock=0;
		FramesBeforeEl++;
		if(FramesBeforeEl>(unsigned short)GetElEveryFrameX){
			GetBlindFrame=1;		//get a blind frame now
			FramesBeforeEl=0;
		}
	}
	
	I2Creg.Config=0x01;

	if(!GetBlindFrame){
		if(SENSORREV1){
			if(!GetVddMeas){
				I2Creg.sfrdata=(currentReadBlock<<4)|0x09;
				VddSampledNow=0;
			}	
			else{
				I2Creg.sfrdata=(currentReadBlock<<4)|0x0D;		//start Vdd-Meas next
				VddSampledNow=1;
			}
		}
		else
			I2Creg.sfrdata=(currentReadBlock<<4)|0x09;
	}
	else{
		I2Creg.sfrdata=0x0B;			//blind frame
	}

	WriteConf(I2Creg);
	TMR3 = 0x00;					// Clear 32-bit Timer (msw)
	TMR2 = 0x00; 					// Clear 32-bit Timer (lsw)

	if(!ReadBlindFrame){
		//now push the data into the stack
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
		for(i=0;i<8;i++){
			LineVoltage=LastReadBlock*PixelEighth+i*COLUMN;
			for(k=0;k<COLUMN;k++){
				Voltage[currentBuffer][k+LineVoltage]=(*(unsigned char*)(p++))<<8;
				Voltage[currentBuffer][k+LineVoltage]|=(*(unsigned char*)(p++)&0x00FF);
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
		for(i=0;i<8;i++){
			LineVoltage=(3-LastReadBlock)*PixelEighth+(7-i)*COLUMN+Pixel/2;
			for(k=0;k<COLUMN;k++){
				Voltage[currentBuffer][k+LineVoltage]=(*(unsigned char*)(p2++))<<8;
				Voltage[currentBuffer][k+LineVoltage]|=(*(unsigned char*)(p2++)&0x00FF);
			}
		}
	}
	else{
		if(FirstFrame){
			FirstFrame--;
		}
		else{
			ReadBlindFrame=0;
			currentReadBlock=0;		//not needed, is set before to 0
			//first PTAT, ignore it here
			p+=2;
			for(k=0;k<ELAMOUNTHALF;k++){
				eloffstack[k][pelStack]=(*(unsigned char*)(p++))<<8;
				eloffstack[k][pelStack]|=(*(unsigned char*)(p++)&0x00FF);	// Es gibt 128 Blind Pixel je Hälfte!!!
			}
			//first PTAT, ignore it here
			p2+=2;
			for(i=0;i<8;i++){
				LineVoltage=(7-i)*COLUMN+ELAMOUNTHALF;
				for(k=0;k<COLUMN;k++){
					eloffstack[k+LineVoltage][pelStack]=(*(unsigned char*)(p2++))<<8;
					eloffstack[k+LineVoltage][pelStack]|=(*(unsigned char*)(p2++)&0x00FF);	// Es gibt 128 Blind Pixel je Hälfte!!!
				}
			}
			pelStack++;
			if(pelStack==StackSize){
				pelStack=0;
				useStack=1;
			}
		}
	}

	if(LastReadBlock==3){		//now push the data out
		GetVDD(0);
		Voltage[currentBuffer][VDDADDRESS]=(unsigned short)VDD/DIVIDEFACTORVDD;	//scale it back to 1 mV resolution
		currentBuffer^=0x01;
		giveOut=1;	
		if(!GetBlindFrame){
			GetVddMeas+=1;
			if(GetVddMeas>2)
				GetVddMeas=0;
		}
		if(SingleFrame){
			SingleFrame++;
		}
	}

	IFS0bits.T3IF=0;


return;
}
/********************************************************************
 * Function:        void StartStreaming(void)
 *
 * Description:     This function starts the streaming of the sensor
 *                  main sequence can be seen in the interrupt
 *
 * Dependencies:    
 *******************************************************************/
void StartStreaming(char Temps,char Stream){
unsigned int k,i;
unsigned char outbuffer;
TAsicSFR I2Creg;
unsigned long PTATLong;
signed long VoltageLong,PowerGradScale,NewDeltaVdd;
signed short VoltageSigned,DeltaVdd;
unsigned long aveloff,Vddlong;
signed long long VoltageLongLong;
unsigned long long DividerVdd,DividerVdd2;

firstrun=1;
currentReadBlock=0;
//Use a 32Bite Timer to synchronize with the framerate of the sensor
T3CONbits.TON = 0;				// Stop any 16-bit Timer3 operation
T2CONbits.TON = 0;				// Stop any 16/32-bit Timer2 operation
T2CONbits.T32 = 1;				// Enable 32-bit Timer mode
T2CONbits.TCKPS = 0x00;			// Select 1:1 Prescaler (=000)
T2CONbits.TCS = 0;				// Select internal instruction cycle clock
T2CONbits.TGATE = 0;			// Disable Gated Timer mode
TMR3 = 0x00;					// Clear 32-bit Timer (msw)
TMR2 = 0x00; 					// Clear 32-bit Timer (lsw)
//take a little bit lower value and then check the status flag in the int
PR3 = (unsigned int)((unsigned int)FCY*((float)tFR*0.9)/1000.0);		// Load 32-bit period value (msw)
PR2 = (unsigned int)((unsigned int)FCY*((float)tFR*0.9)/1000.0); 		// Load 32-bit period value (lsw)
// IPC3bits.T3IP = 0x04;			// Set Timer3 Interrupt Priority Level
IFS0bits.T3IF = 0;				// Clear Timer3 Interrupt Flag
IEC0bits.T3IE = 1;				// Enable Timer3 interrupt
T2CONbits.TON = 1;				// Start 32-bit Timer
giveOut=0;

//fill the PTATStack and VddSensorStack somewhere first, otherwise the first calculated data will be wrong

I2Creg.Config=0x01;
I2Creg.sfrdata=0x0B;		//always start with block 0 and el. Offset
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
		//el. Buffer has to be filled in the interrupt from time to time, set GetElEveryFrameX to a good value
		for(k=0;k<PixelEighth;k++){
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

		for(k=PixelEighth;k<Pixelforth;k++){
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
		if(Temps){		//calculate Temperatures
			//first Ta
			PTATLong=0;
				for(k=0;k<(unsigned short)STACKSIZEPTAT;k++){
					PTATLong+=(unsigned long)PTATStack[k];
				}
			TA=(unsigned short)((float)(PTATLong<<(16-Resolution))/(float)STACKSIZEPTAT*PTATGrad+PTATOff+0.5);	
			PTATLong/=(unsigned long)STACKSIZEPTAT;		//is needed later for TWOVDD	
			Voltage[outbuffer][VDDADDRESS+1]=TA;
			PowerGradScale=(signed long)pow(2.0,(float)gradScale);
			//now do the average of Vdd from the sensor if the sensor can measure Vdd by itself
			Vddlong=(unsigned long)VddSensorStack[0];
			for(k=1;k<STACKSIZEVDD;k++){
				Vddlong+=(unsigned long)VddSensorStack[k];
			}
			Vddlong/=(unsigned long)STACKSIZEVDD;
			Voltage[outbuffer][VDDADDRESS]=(unsigned short)Vddlong;	//this overwrites the Vdd of the PCB with the Vdd of the sensor
			NewDeltaVdd=(signed long)PTATLong;
			NewDeltaVdd-=(signed long)PTATThermals[0];
			NewDeltaVdd*=((signed long)VddRef[1]-(signed long)VddRef[0]);
			NewDeltaVdd/=((signed long)PTATThermals[1]-(signed long)PTATThermals[0]);
			NewDeltaVdd=(signed long)Vddlong-(signed long)VddRef[0]-NewDeltaVdd;
			DeltaVdd=(signed short)NewDeltaVdd;
			DividerVdd=(unsigned long long)(pow(2.0,(float)VddScaling));
			DividerVdd2=(unsigned long long)(pow(2.0,(float)VddScalingOff));			
			for(k=0;k<(unsigned short)Pixel/2;k++){		
				//substract the thermal Offsets
				VoltageLong=(signed long)((signed short)ThGrad[k]);
				VoltageLong*=(signed long)PTATLong;			//this is changed compared to older ones. Use PTAT as the reference!
				VoltageLong/=PowerGradScale;
				VoltageLong+=(signed long)ThOff[k];
				//el. Offset
				VoltageLong+=(signed long)(Voltage[outbuffer][k%ELAMOUNTHALF+ELOFFSET]<<(16-Resolution));
				//VDD-Comp
				VoltageLongLong=(signed long long)VddGrad[k%ELAMOUNTHALF];
				VoltageLongLong*=(signed long long)PTATLong;
				VoltageLongLong/=(signed long long)DividerVdd;
				VoltageLongLong+=(signed long long)VddOff[k%ELAMOUNTHALF];
				VoltageLongLong*=(signed long long)DeltaVdd;
				VoltageLongLong/=(signed long long)DividerVdd2;
				VoltageLong+=(signed long)VoltageLongLong;				
				//all 3 Offsets (Thermal, electrical and Vdd) are calculated, now substract them
				VoltageSigned=(signed short)Voltage[outbuffer][k]-(signed short)VoltageLong;
				//calc. Temperatures			
				Voltage[outbuffer][k]=(unsigned short)calcTO(TA,VoltageSigned,PixC[k],k);
			}
			for(i=ROW/2;i<ROW;i++){
				for(k=0;k<COLUMN;k++){		//substract the thermal Offsets
					VoltageLong=(signed long)((signed short)ThGrad[k+(ROW/2+ROW-1-i)*COLUMN]);
					VoltageLong*=(signed long)PTATLong;
					VoltageLong/=PowerGradScale;
					VoltageLong+=(signed long)ThOff[k+(ROW/2+ROW-1-i)*COLUMN];
					//el.
					VoltageLong+=(signed long)(Voltage[outbuffer][(k+i*COLUMN)%ELAMOUNTHALF+ELAMOUNTHALF+ELOFFSET]<<(16-Resolution));
					//VDD-Comp
					VoltageLongLong=(signed long long)VddGrad[k+(7-i%8)*COLUMN+ELAMOUNTHALF];
					VoltageLongLong*=(signed long long)PTATLong;
					VoltageLongLong/=(signed long long)DividerVdd;
					VoltageLongLong+=(signed long long)VddOff[k+(7-i%8)*COLUMN+ELAMOUNTHALF];
					VoltageLongLong*=(signed long long)DeltaVdd;
					VoltageLongLong/=(signed long long)DividerVdd2;
					VoltageLong+=(signed long)VoltageLongLong;					
				
					VoltageSigned=(signed short)Voltage[outbuffer][k+i*COLUMN]-(signed short)VoltageLong;
					//calc. Temperatures
					Voltage[outbuffer][k+i*32]=(unsigned short)calcTO(TA,VoltageSigned,PixC[k+(ROW/2+ROW-1-i)*COLUMN],1);
				}
			}	
		}
       
	    //push the data out via UDP, here is more code required!
		UDPPutArray((BYTE*)Voltage[outbuffer],DATALength+2);  
		UDPPutArray((unsigned char*)&Voltage[outbuffer][DataLengthHalf+1],DATALength-2);
		giveOut=0;
	}
}while((!INT1occ)&&(SingleFrame<3));

return;
}

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


unsigned int WriteConf(TAsicSFR reg){
	unsigned short ErrorCode;
	
	SSEL=1;
	asm("NOP");	asm("NOP");
	IEC0bits.SPI1RXIE=0;
	
	SPI1BUF=reg.Config;
	while(!SPI1STATbits.SPIRBF);
	ErrorCode=SPI1BUF;

	SPI1BUF=reg.sfrdata;
	while(!SPI1STATbits.SPIRBF);
	ErrorCode=SPI1BUF;
	asm("NOP");	asm("NOP");
	SSEL=0;

	return(ErrorCode);
}


/*********************************************************************
* Function:     Read_RAM()
*
* Input:		pointer to data, ReadAdress of RAM and amount of data to read
*
* Output:		data (changes global)
*
* Overview:		reads the RAM at the Adress ReadAdress for the amount of readnumber
*
* Note:			None
********************************************************************/

unsigned int Read_RAM(unsigned char* data,TAsicSFR reg, unsigned short readnumber){
    unsigned short ErrorCode,k,i;
	
	SSEL=1;
	asm("NOP");	asm("NOP");
	while(!SPI1STATbits.SPITBE);	
	SPI1BUF=reg.Config;
	while(!SPI1STATbits.SPIRBF);
	i=SPI1BUF;
	for(k=0;k<readnumber;k++){
		SPI1BUF=0xFF;
		while(!SPI1STATbits.SPIRBF);
		*data++=SPI1BUF;
	}
	asm("NOP");	asm("NOP");
	SSEL=0;
	return(ErrorCode);
}	


/********************************************************************
 * Function:        void calcTO(unsigned int TAmb, signed int dig, unsigned long PiC)
 *
 * Description:     calculate the object temperature via look-up table
 *
 * Dependencies:    ambient temperature (TAmb), pixel voltage (dig), pixel sensitivity coefficients (PiC)
 *******************************************************************/
unsigned int calcTO(unsigned int TAmb, signed int dig, signed long PiC, unsigned int dontCalcTA){
	unsigned int Tobject,y;
	signed int val;
	signed long vx,vy,ydist;
	static unsigned int CurTACol;
	static signed int dTA;
	signed long long scale;


	//first check the position in x-axis of table
	if(!dontCalcTA){
#ifdef TAEQUIDIST
		CurTACol=(TAmb-XTATemps[0])>>TADISTEXP;
#else
		for(CurTACol=0;CurTACol<NROFTAELEMENTS;CurTACol++){
			if((XTATemps[CurTACol]<=TAmb)&&(XTATemps[CurTACol+1]>TAmb))
				break;
		}
#endif
		dTA=TAmb-XTATemps[CurTACol];
		}

	if((CurTACol>=NROFTAELEMENTS-1))
			return 0;


	//now scale dig to PCSCALEVAL
	scale=(signed long long)PCSCALEVAL*(signed long long)dig;
	vx=(signed long)(scale/((signed long long)PiC));


	val=vx+TABLEOFFSET;
	//now determine row
	y=val>>ADEXPBITS;
	ydist=(signed long)ADEQUIDISTANCE;
	if(y<(NROFADELEMENTS-1)){
		if(TempTable[y][CurTACol]){
#ifdef TAEQUIDIST
			vx=((((signed long)TempTable[y][CurTACol+1]-(signed long)TempTable[y][CurTACol])*(signed long)dTA)>>TADISTEXP)+(signed long)TempTable[y][CurTACol];
			vy=((((signed long)TempTable[y+1][CurTACol+1]-(signed long)TempTable[y+1][CurTACol])*(signed long)dTA)>>TADISTEXP)+(signed long)TempTable[y+1][CurTACol];
#else		
			vx=((((signed long)TempTable[y][CurTACol+1]-(signed long)TempTable[y][CurTACol])*(signed long)dTA)/(signed long)TAEQUIDISTANCE)+(signed long)TempTable[y][CurTACol];
			vy=((((signed long)TempTable[y+1][CurTACol+1]-(signed long)TempTable[y+1][CurTACol])*(signed long)dTA)/(signed long)TAEQUIDISTANCE)+(signed long)TempTable[y+1][CurTACol];
#endif			
			Tobject=(unsigned int)((vy-vx)*((signed long)val-(signed long)YADValues[y])/ydist+(signed long)vx);
			}
		else
			return 0;
		}
	else
		return 0;
		
#ifdef AdjustOffsetGain
	val=(signed int)Tobject;
	val+=(signed int)GlobalOffset;
	Tobject=(unsigned short)val;
#endif		

	return (unsigned short)Tobject;
}

/********************************************************************
 * Function:        void ReadCalibData(void)
 *
 * Description:     read all required calibration data
 *
 * Dependencies:    
 *******************************************************************/
void ReadCalibData(unsigned char print){
TAsicSFR I2Creg;
unsigned short i,PixCRaw;
float common[2];
char eecpy[8];

//read the whole EEPROM and put them into the storage
	HighDensSequentialRead((unsigned short)AdrPixCMin,eecpy,sizeof(float)*2);
	memcpy((char*)&common,eecpy,sizeof(float)*2);
	PixCMin=common[0];
	PixCMax=common[1];

	//zum debuggen
    memcpy(eecpy,(char*)&common,sizeof(float)*2);
    HighDensPageWrite((unsigned short)AdrPixCMin,eecpy,sizeof(float)*2);

	HighDensSequentialRead((unsigned short)AdrGradScale,eecpy,sizeof(char));
	gradScale=(unsigned short)eecpy[0];
	HighDensSequentialRead((unsigned short)AdrExponent,eecpy,sizeof(char)*5);
	TableNumberSensor=(unsigned short)eecpy[1]<<8;
	TableNumberSensor+=(unsigned short)eecpy[0];
	epsilon=(unsigned short)eecpy[2];

	HighDensSequentialRead((unsigned short)AdrMBITPixC,eecpy,sizeof(char)*5);
	SetMBITCalib=(unsigned short)eecpy[0];
	SetBIASCalib=(unsigned short)eecpy[1];
	SetCLKCalib=(unsigned short)eecpy[2];
	SetBPACalib=(unsigned short)eecpy[3];
	SetPUCalib=(unsigned short)eecpy[4];
	HighDensSequentialRead((unsigned short)AdrTaTh1,eecpy,sizeof(char)*4);
	memcpy((char*)&TaTh,eecpy,sizeof(short)*2);
	HighDensSequentialRead((unsigned short)AdrPTATGrad,eecpy,sizeof(float)*2);
	memcpy((char*)&common,eecpy,sizeof(float)*2);
	PTATGrad=common[0];
	PTATOff=common[1];
	HighDensSequentialRead((unsigned short)AdrTaPixC,eecpy,sizeof(char)*2);
	memcpy((char*)&TaPixC,eecpy,sizeof(short));
	HighDensSequentialRead((unsigned short)AdrDevID,eecpy,sizeof(char)*2);
	memcpy((char*)&DevID,eecpy,sizeof(short));
#ifdef AdjustOffsetGain
	HighDensSequentialRead((unsigned short)AdrGlobalOffset,eecpy,sizeof(char));
	memcpy((char*)&GlobalOffset,eecpy,sizeof(char));
	if(GlobalOffset==(char)0xFF){
		GlobalOffset=0;
		eecpy[0]=GlobalOffset;
		HighDensPageWrite((unsigned short)AdrGlobalOffset,eecpy,sizeof(char));
	}
	HighDensSequentialRead((unsigned short)AdrGlobalGain,eecpy,sizeof(char)*2);
	memcpy((char*)&GlobalGain,eecpy,sizeof(short));
	//if values are not set before, do it now!
	if((GlobalGain==0x0000)||(GlobalGain==0xFFFF)){
		GlobalGain=10000;
		memcpy(eecpy,(char*)&GlobalGain,sizeof(short));
		HighDensPageWrite((unsigned short)AdrGlobalGain,eecpy,sizeof(short));
	}
#endif
	
	//read all settings for clock etc. from the EEPROM, if not set, use the default values!
	HighDensSequentialRead((unsigned short)AdrMBITUser,eecpy,sizeof(char));
	memcpy((unsigned char*)&SetMBITUser,eecpy,sizeof(unsigned char));
	if((SetMBITUser<0x09)||(SetMBITUser>0x3C)){		//EEPROM empty or setting out of spec -> set to default
		SetMBITUser=(unsigned char)MBITTRIMDefault;
		eecpy[0]=SetMBITUser;
		HighDensPageWrite((unsigned short)AdrMBITUser,eecpy,sizeof(char));
	}
	HighDensSequentialRead((unsigned short)AdrBIASUser,eecpy,sizeof(char));
	memcpy((unsigned char*)&SetBIASUser,eecpy,sizeof(unsigned char));
	if((!SetBIASUser)||(SetBIASUser>0x1F)){		//EEPROM empty or setting out of spec -> set to default
		SetBIASUser=(unsigned char)BIAScurrentDefault;
		eecpy[0]=SetBIASUser;
		HighDensPageWrite((unsigned short)AdrBIASUser,eecpy,sizeof(char));
	}
	HighDensSequentialRead((unsigned short)AdrCLKUser,eecpy,sizeof(char));
	memcpy((unsigned char*)&SetCLKUser,eecpy,sizeof(unsigned char));
	if(SetCLKUser>0x3F){		//EEPROM empty or setting out of spec -> set to default
		SetCLKUser=(unsigned char)CLKTRIMDefault;
		eecpy[0]=SetCLKUser;
		HighDensPageWrite((unsigned short)AdrCLKUser,eecpy,sizeof(char));
	}
	HighDensSequentialRead((unsigned short)AdrBPAUser,eecpy,sizeof(char));
	memcpy((unsigned char*)&SetBPAUser,eecpy,sizeof(unsigned char));
	if((!SetBPAUser)||(SetBPAUser>0x1F)){		//EEPROM empty or setting out of spec -> set to default
		SetBPAUser=(unsigned char)BPATRIMDefault;
		eecpy[0]=SetBPAUser;
		HighDensPageWrite((unsigned short)AdrBPAUser,eecpy,sizeof(char));
	}
	HighDensSequentialRead((unsigned short)AdrPUUser,eecpy,sizeof(char));
	memcpy((unsigned char*)&SetPUUser,eecpy,sizeof(unsigned char));
	if(!((SetPUUser==0x11)||(SetPUUser==0x22)||(SetPUUser==0x44)||(SetPUUser==0x88))){		//EEPROM empty or setting out of spec -> set to default
		SetPUUser=(unsigned char)PUTRIMDefault;
		eecpy[0]=SetPUUser;
		HighDensPageWrite((unsigned short)AdrPUUser,eecpy,sizeof(char));
	}
	
	//now load the Thermal-Grad and -Offset
	if(print){
		sprintf(tempstring,"Pixel, ThGrad, ThOff\r\n");
		putstringUDPUART(tempstring);
	}
	for(i=0;i<(unsigned short)Pixel;i++){
		HighDensSequentialRead((unsigned short)AdrTh1+i,eecpy,sizeof(char));
		ThGrad[i]=(signed char)eecpy[0];
		HighDensSequentialRead((unsigned short)AdrTh2+i*2,eecpy,sizeof(char)*2);
		memcpy((char*)&ThOff[i],eecpy,sizeof(short));
		if(print){
			sprintf(tempstring,"%u: %u, %u\r\n",i,ThGrad[i],ThOff[i]);
			putstringUDPUART(tempstring);
		}		
	}

#ifdef COMPDEFECTIVEPIX
    HighDensSequentialRead(AdrNrOfDefPix,(char*)&NrOfDefPix,1);  //read number of defective pixel
    if(NrOfDefPix==0xFF){
        NrOfDefPix=0;
        HighDensPageWrite(AdrNrOfDefPix,(char*)&NrOfDefPix,sizeof(unsigned char));
        }
	for(i=0;i<MAXNROFDEFECTS;i++){
		HighDensSequentialRead((unsigned short)AdrDeadPix+i*2,eecpy,sizeof(char)*2);
		memcpy((char*)&DeadPixAdr[i],eecpy,sizeof(short));
		HighDensSequentialRead((unsigned short)AdrDeadPixMask+i,eecpy,sizeof(char));
		memcpy((char*)&DeadPixMask[i],eecpy,sizeof(char));
	}
#endif

	//and finally the PixC
		if(print){
			sprintf(tempstring,"Pixel, PixC\r\n");
			putstringUDPUART(tempstring);
		}
		for(i=0;i<(unsigned short)Pixel;i++){
			HighDensSequentialRead((unsigned short)AdrPixC+i*2,eecpy,sizeof(char)*2);
			memcpy((char*)&PixCRaw,eecpy,sizeof(short));
			#ifdef AdjustOffsetGain
				PixC[i]=(unsigned long)((((float)PixCRaw/65535.0)*(PixCMax-PixCMin)+(float)PixCMin)*(float)epsilon/100.0*(float)GlobalGain/10000.0+0.5);
			#else
				PixC[i]=(unsigned long)((((float)PixCRaw/65535.0)*(PixCMax-PixCMin)+(float)PixCMin)*(float)epsilon/100.0+0.5);
			#endif
			if(print){
				sprintf(tempstring,"%u: %u\r\n",i,PixC[i]);
				putstringUDPUART(tempstring);
			}
		}	
	}
	for(i=0;i<(unsigned short)ELAMOUNT;i++){
		HighDensSequentialRead((unsigned short)AdrVddCompValues2+i*2,eecpy,sizeof(char)*2);
		memcpy((char*)&VddGrad[i],eecpy,sizeof(short));
		HighDensSequentialRead((unsigned short)AdrVddCompValues+i*2,eecpy,sizeof(char)*2);
		memcpy((char*)&VddOff[i],eecpy,sizeof(short));
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


void HighDensPageWrite(unsigned short address,unsigned char *data, unsigned short numbytes){
   unsigned char k;                // Loop counter
	unsigned char Local_8;
    unsigned int nr;

	InitSPIIF(0);
	if(!SSEL){
		SSEL=1;
		DelayMs(1);
	}

	if ((address%PAGESIZE)!=0){			//schreibt die erste Seite, wenn sie nicht von vorne beginnt
		if((PAGESIZE-(address%PAGESIZE))>=numbytes){	//Unterscheidet ob die Seite bis zum Ende beschrieben wird oder nicht
		    EEPROMWriteEnable();
		    mEEPROMSSLow();
		
		    Local_8 = writeSPI1(EEPROM_CMD_WRITE);
		
		    Local_8 = writeSPI1(Hi(address));
		    Local_8 = writeSPI1(Lo(address));
			for (i = 0; i < numbytes; i++)
		    	Local_8 = writeSPI1(data[i]);
			mEEPROMSSHigh();
	        nr=0;
	        while((nr++<NR_OF_WIPSTATUSPOLLS)&&(EEPROMReadStatus().Bits.WIP));
			EEPROMWriteDisable();
			DelayMs(5);
			InitSPIIF(1);
			SSEL=1;
			return;
			}
		else{								//Hier wird die erste Seite bis zum Ende beschrieben, beginnt aber nicht ganz vorne
		    EEPROMWriteEnable();
		    mEEPROMSSLow();
		
		    Local_8 = writeSPI1(EEPROM_CMD_WRITE);
		
		    Local_8 = writeSPI1(Hi(address));
		    Local_8 = writeSPI1(Lo(address));
			for(i=0;i<(PAGESIZE-(address%PAGESIZE));i++){
				Local_8 = writeSPI1(data[i]);
				address++;
			}
			mEEPROMSSHigh();
	        nr=0;
	        while((nr++<NR_OF_WIPSTATUSPOLLS)&&(EEPROMReadStatus().Bits.WIP));
			EEPROMWriteDisable();
			numbytes-=i;
			numbytes2=i;
			DelayMs(5);
		}
	}
	while (numbytes>=PAGESIZE){					//schreibt die anderen vollen Seiten, muss aber mit einer vollen beginnen und aufhören
	    EEPROMWriteEnable();
	    mEEPROMSSLow();
	
	    Local_8 = writeSPI1(EEPROM_CMD_WRITE);
	
	    Local_8 = writeSPI1(Hi(address));
	    Local_8 = writeSPI1(Lo(address));
		for (i = 0; i < PAGESIZE; i++)
			Local_8 = writeSPI1(data[numbytes2+i]);

		mEEPROMSSHigh();
        nr=0;
        while((nr++<NR_OF_WIPSTATUSPOLLS)&&(EEPROMReadStatus().Bits.WIP));
		EEPROMWriteDisable();
	    numbytes-=PAGESIZE;
		numbytes2+=PAGESIZE;
	    address+=PAGESIZE;
		DelayMs(5);
	    }
	if (numbytes!=0){					//Hier wird die letzte nicht leere Seite geschrieben
		EEPROMWriteEnable();
	    mEEPROMSSLow();
	
	    Local_8 = writeSPI1(EEPROM_CMD_WRITE);
	
	    Local_8 = writeSPI1(Hi(address));
	    Local_8 = writeSPI1(Lo(address));
		for(i=0;i<numbytes;i++){
			Local_8 = writeSPI1(data[numbytes2+i]);
			address++;
		}
		mEEPROMSSHigh();
        nr=0;
        while((nr++<NR_OF_WIPSTATUSPOLLS)&&(EEPROMReadStatus().Bits.WIP));
		EEPROMWriteDisable();
		DelayMs(5);
	}
	InitSPIIF(1);
	SSEL=1;
return;
}


/********************************************************************
 * Function:        void HighDensSequentialRead(unsigned char *data,
 *                                              unsigned char numbytes)
 *
 * Description:     This function reads multiple bytes from a
 *                  high-density (>= 32 Kb) serial EEPROM device.
 *
 * Dependencies:    'i2ccontrol' contains the control byte
 *                  'address' contains the address word
 *******************************************************************/
void HighDensSequentialRead(unsigned short address,unsigned char* data, unsigned short numbytes)
{
	unsigned int k;
	unsigned char Local_8;
	InitSPIIF(0);
    mEEPROMSSHigh();    //added for safety purposes.
    Delay1us(1);        //needs acc. to spec only 150ns 
    mEEPROMSSLow();
    
    Local_8 = writeSPI1(EEPROM_CMD_READ);

    Local_8 = writeSPI1(Hi(address));
    Local_8 = writeSPI1(Lo(address));
	for(k=0;k<numbytes;k++){
    	*data = writeSPI1(0);
		data++;
		}	
    mEEPROMSSHigh();
	InitSPIIF(1);
}

void InitSPIIF(char notEEPROM){
unsigned int ii,i;
SPI1CON=0; //Switch off
asm("NOP");
SPI1STAT=0x0;	//Interrupt when data is available in receive buffer (SRMPT bit is set)
if(notEEPROM){
	SPI1CON=0x0120;	//8 bit and SS-Disabled, CKP=0, idle clock state is low, MSTEN=1, Master Mode
	SPI1BRG=SPISpeed;		//F_SCK= F_PB/(2*(SPI1BRG+1)), 0x3=10 MHz
	for(i=0;i<18;i++)
		ii=SPI1BUF;
}
else{
	SPI1CON=0x0120;	//8 bit and SS-Disabled, CKP=0, idle clock state is low, MSTEN=1, Master Mode
	SPI1BRG=SPI1EEBRG;		//F_SCK= F_PB/(2*(SPI1BRG+1)), use 0x08 here for <5 MHz
	for(i=0;i<18;i++)
		ii=SPI1BUF;
}

SPI1STATbits.SPIROV=0;

IFS0bits.SPI1EIF=0;
IEC0bits.SPI1EIE=1;

IFS0bits.SPI1RXIF=0;
IPC5bits.SPI1IP=6;
SPI1CON|=0x8000;

return;
}


