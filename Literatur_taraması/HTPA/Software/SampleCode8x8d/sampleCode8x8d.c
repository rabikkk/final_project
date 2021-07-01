unsigned char data[128];
unsigned char *p;
unsigned short eloffstack[ELAMOUNT][StackSize];	//ELAMOUNT=64; StackSize=10 (arbitrary, should be chosen to match the data memory of the conctroller)
unsigned short gradScale,TA,Voltage[Pixel+PTATamount];	
unsigned short SetMBITCalib,SetMBITUser,SetBPACalib,SetBPAUser,SetPUCalib,SetPUUser,Resolution;
unsigned short SetBIASCalib,SetCLKCalib,SetBIASUser,SetCLKUser,DevID;
signed short ThGrad[Pixel],ThOff[Pixel];
unsigned long PixC[Pixel];


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
unsigned short k,PTAT0;
TAsicSFR I2Creg;
signed long VoltageLong,PowerGradScale;
signed short VoltageSigned;


//Wait until conversion is finished
	do{
		p=data;
		I2Creg.adr=0x1A;
		I2Creg.read=0x00;
		I2Creg.Config=0x02;
		Read_RAM(data,I2Creg,0x01);
	}while((data[0]&0x01)!=0x01);

//Now readout the sensor (El. offset is so far ignored)
	I2Creg.adr=0x1A;
	I2Creg.read=0x00;
	I2Creg.Config=0x0A;
	p=data;
	Read_RAM(data,I2Creg,0x82);

	//start now already the sensor again to shorten the calculation time
	I2Creg.adr=0x1A;
	I2Creg.read=0x00;
	I2Creg.Config=0x01;
	if(!SendActive){
		I2Creg.sfrdata=0x09;
	}
	else{
		I2Creg.sfrdata=0x0B;
	}
	WriteConf(I2Creg);

	//now push the data into the stack
	//IF ELOFFSETS ARE FETCHED THEY HAVE TO BE PUSHED INTO THE ELOFFSTACK!!!
	Voltage[PIXEL]=(*(unsigned char*)(p++))<<8;
	Voltage[PIXEL]|=(*(unsigned char*)(p++)&0x00FF);
	for(k=0;k<PIXEL;k++){
		Voltage[k]=(*(unsigned char*)(p++))<<8;
		Voltage[k]|=(*(unsigned char*)(p++)&0x00FF);
		Voltage[k]=Voltage[k]<<(16-Resolution);
	}
	//and do the temperature calculation if wanted
	Ta=(unsigned short)((float)PTAT0*PTATGrad+PTATOff+0.5);		//here should maybe made a stack of PTAT if the noise of the PTAT is too high. Keep the stack low to make the ambient temperature change fast
	PowerGradScale=(signed long)pow(2.0,(float)gradScale);
	for(k=0;k<PIXEL;k++){
		VoltageLong=(signed long)((signed short)ThGrad[k]);
		VoltageLong*=(signed long)TA;
		VoltageLong/=PowerGradScale;
		VoltageLong+=(signed long)ThOff[k];
		aveloff=eloffstack[k][0];
		for(j=1;j<StackSize;j++){						//StackSize can be selected, should at least be 10 or higher to reduce the noise of the el. Offset
			aveloff+=eloffstack[k%ELAMOUNTHALF][j];		//eloffstack needs to be filled from time to time with the blind pixels
					//this should be better done when the el. Offsets are read and the average should be pushed into another stack, saves a lot of CPU power
		}
		VoltageLong+=(signed long)(aveloff/(unsigned long)StackSize);				
		VoltageSigned=(signed short)Voltage[k]-(signed short)VoltageLong;
		//calc. Temperatures
		Voltage[k]=(unsigned short)calcTO(TA,VoltageSigned,PixC[k],k);
	}
	//now we have all Temperature-data and they can either be pushed out via UDP or been plotted or ...


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
TAsicSFR I2Creg;
unsigned long PTATLong;
signed long VoltageLong,PowerGradScale;
signed short VoltageSigned;
unsigned long aveloff;

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

I2Creg.adr=0x1A;
I2Creg.read=0x00;
I2Creg.Config=0x01;
I2Creg.sfrdata=0x0B;		//el. Offset should be sampled first to have data of them
WriteConf(I2Creg);

do{
	//everything is done in the Timer-Interrupt, so simply do nothing here.
}while(!INT1occ);

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
	IdleI2C();						//Ensure Module is Idle
	StartI2C();						//Generate Start Condition
	WriteI2C((reg.adr<<1)|reg.read);	//Write device addr R/NW=0
	IdleI2C();
	ErrorCode = ACKStatus();		//Return ACK Status

	WriteI2C(reg.Config);			//Write Config addr
	IdleI2C();
	ErrorCode = ACKStatus();		//Return ACK Status

	WriteI2C(reg.sfrdata);
	IdleI2C();

	ErrorCode = ACKStatus();		//Return ACK Status
	return(ErrorCode);
}

/*********************************************************************
* Function:        StartI2C()
*
* Input:		None.
*
* Output:		None.
*
* Overview:		Generates an I2C Start Condition
*
* Note:			None
********************************************************************/
unsigned int StartI2C(void)
{
	//This function generates an I2C start condition and returns status 
	//of the Start.

	I2C1CONbits.SEN = 1;		//Generate Start COndition
	while (I2C1CONbits.SEN);	//Wait for Start COndition
	return(I2C1STATbits.S);	//Optionally return status
}


/*********************************************************************
* Function:        RestartI2C()
*
* Input:		None.
*
* Output:		None.
*
* Overview:		Generates a restart condition and optionally returns status
*
* Note:			None
********************************************************************/
unsigned int RestartI2C(void)
{
	//This function generates an I2C Restart condition and returns status 
	//of the Restart.

	I2C1CONbits.RSEN = 1;		//Generate Restart		
	while (I2C1CONbits.RSEN);	//Wait for restart	
	return(I2C1STATbits.S);	//Optional - return status
}


/*********************************************************************
* Function:        StopI2C()
*
* Input:		None.
*
* Output:		None.
*
* Overview:		Generates a bus stop condition
*
* Note:			None
********************************************************************/
unsigned int StopI2C(void)
{
	//This function generates an I2C stop condition and returns status 
	//of the Stop.

	I2C1CONbits.PEN = 1;		//Generate Stop Condition
	while (I2C1CONbits.PEN);	//Wait for Stop
	return(I2C1STATbits.P);		//Optional - return status
}


/*********************************************************************
* Function:        WriteI2C()
*
* Input:		Byte to write.
*
* Output:		None.
*
* Overview:		Writes a byte out to the bus
*
* Note:			None
********************************************************************/
unsigned int WriteI2C(unsigned char byte)
{
	//This function transmits the byte passed to the function
	//while (I2C1STATbits.TRSTAT);	//Wait for bus to be idle
	I2C1TRN = byte;					//Load byte to I2C1 Transmit buffer
	while (I2C1STATbits.TBF);		//wait for data transmission
	return 1;
}


/*********************************************************************
* Function:        IdleI2C()
*
* Input:		None.
*
* Output:		None.
*
* Overview:		Waits for bus to become Idle
*
* Note:			None
********************************************************************/
unsigned int IdleI2C(void)
{
	while (I2C1STATbits.TRSTAT);		//Wait for bus Idle
	return 1;
}


/*********************************************************************
* Function:        LDByteWriteI2C()
*
* Input:		Control Byte, 8 - bit address, data.
*
* Output:		None.
*
* Overview:		Write a byte to low density device at address LowAdd
*
* Note:			None
********************************************************************/
unsigned int LDByteWriteI2C(unsigned char ControlByte, unsigned char LowAdd, unsigned char data)
{
	unsigned short ErrorCode;

	IdleI2C();						//Ensure Module is Idle
	StartI2C();						//Generate Start COndition
	WriteI2C(ControlByte);			//Write Control byte
	IdleI2C();
    
	ErrorCode = ACKStatus();		//Return ACK Status
	WriteI2C(LowAdd);				//Write Low Address
	IdleI2C();

	ErrorCode = ACKStatus();		//Return ACK Status
	
	WriteI2C(data);					//Write Data
	IdleI2C();
	ErrorCode = ACKStatus();		//Return ACK Status
	StopI2C();						//Initiate Stop Condition
//	EEAckPolling(ControlByte);		//Perform ACK polling
	return(ErrorCode);
}


/*********************************************************************
* Function:        LDByteReadI2C()
*
* Input:		Control Byte, Address, *Data, Length.
*
* Output:		None.
*
* Overview:		Performs a low density read of Length bytes and stores in *Data array
*				starting at Address.
*
* Note:			None
********************************************************************/
unsigned int LDByteReadI2C(unsigned char ControlByte, unsigned char Address, unsigned char *Data, unsigned char Length)
{								//note: wird nicht benötigt, da Read_EEPROM dasselbe macht
	IdleI2C();					//wait for bus Idle
	StartI2C();					//Generate Start Condition
	WriteI2C(ControlByte);		//Write Control Byte
	IdleI2C();					//wait for bus Idle
	WriteI2C(Address);			//Write start address
	IdleI2C();					//wait for bus Idle

	RestartI2C();				//Generate restart condition
	WriteI2C(ControlByte | 0x01);	//Write control byte for read
	IdleI2C();					//wait for bus Idle

	getsI2C(Data, Length);		//read Length number of bytes
	NotAckI2C();				//Send Not Ack
	StopI2C();					//Generate Stop
	return 1;
}


/*********************************************************************
* Function:        HDByteWriteI2C()
*
* Input:		ControlByte, HighAddr, LowAddr, Data.
*
* Output:		None.
*
* Overview:		perform a high density byte write of data byte, Data.
*				starting at the address formed from HighAdd and LowAdd
*
* Note:			None
********************************************************************/
unsigned int HDByteWriteI2C(unsigned char ControlByte, unsigned char HighAdd, unsigned char LowAdd, unsigned char data)
{
	unsigned short ErrorCode;

	IdleI2C();						//Ensure Module is Idle
	StartI2C();						//Generate Start COndition
	WriteI2C(ControlByte);			//Write Control byte
	IdleI2C();

	ErrorCode = ACKStatus();		//Return ACK Status
	
	WriteI2C(HighAdd);
	IdleI2C();						//Write High Address
	WriteI2C(LowAdd);				//Write Low Address
	IdleI2C();

	ErrorCode = ACKStatus();		//Return ACK Status

	WriteI2C(data);					//Write Data
	IdleI2C();
	StopI2C();						//Initiate Stop Condition
	EEAckPolling(ControlByte);		//perform Ack Polling
	return(ErrorCode);
}


/*********************************************************************
* Function:        HDByteReadI2C()
*
* Input:		Control Byte, HighAdd, LowAdd, *Data, Length.
*
* Output:		None.
*
* Overview:		Performs a low density read of Length bytes and stores in *Data array
*				starting at Address formed from HighAdd and LowAdd.
*
* Note:			None
********************************************************************/
unsigned int HDByteReadI2C(unsigned char ControlByte, unsigned char HighAdd, unsigned char LowAdd, unsigned char *Data, unsigned char Length)
{
	IdleI2C();						//Wait for bus Idle
	StartI2C();						//Generate Start condition
	WriteI2C(ControlByte);			//send control byte for write
	IdleI2C();						//Wait for bus Idle

	WriteI2C(HighAdd);				//Send High Address
	IdleI2C();						//Wait for bus Idle
	WriteI2C(LowAdd);				//Send Low Address
	IdleI2C();						//Wait for bus Idle

	RestartI2C();					//Generate Restart
	WriteI2C(ControlByte | 0x01);	//send control byte for Read
	IdleI2C();						//Wait for bus Idle

	getsI2C(Data, Length);			//Read Length number of bytes to Data
	NotAckI2C();					//send Not Ack
	StopI2C();						//Send Stop Condition
	return(0);
}


/*********************************************************************
* Function:        LDPageWriteI2C()
*
* Input:		ControlByte, LowAdd, *wrptr.
*
* Output:		None.
*
* Overview:		Write a page of data from array pointed to be wrptr
*				starting at LowAdd
*
* Note:			LowAdd must start on a page boundary
********************************************************************/
unsigned int LDPageWriteI2C(unsigned char ControlByte, unsigned char LowAdd, unsigned char *wrptr)
{
	IdleI2C();					//wait for bus Idle
	StartI2C();					//Generate Start condition
	WriteI2C(ControlByte);		//send controlbyte for a write
	IdleI2C();					//wait for bus Idle
	WriteI2C(LowAdd);			//send low address
	IdleI2C();					//wait for bus Idle
	putstringI2C(wrptr);		//send data
	IdleI2C();					//wait for bus Idle
	StopI2C();					//Generate Stop
	return(0);
}


/*********************************************************************
* Function:        HDPageWriteI2C()
*
* Input:		ControlByte, HighAdd, LowAdd, *wrptr.
*
* Output:		None.
*
* Overview:		Write a page of data from array pointed to be wrptr
*				starting at address from HighAdd and LowAdd
*
* Note:			Address must start on a page boundary
********************************************************************/
unsigned int HDPageWriteI2C(unsigned char ControlByte, unsigned char HighAdd, unsigned char LowAdd, unsigned char *wrptr)
{
	IdleI2C();				//wait for bus Idle
	StartI2C();				//Generate Start condition
	WriteI2C(ControlByte);	//send controlbyte for a write
	IdleI2C();				//wait for bus Idle
	WriteI2C(HighAdd);		//send High Address
	IdleI2C();				//wait for bus Idle
	WriteI2C(LowAdd);		//send Low Address
	IdleI2C();				//wait for bus Idle
	putstringI2C(wrptr);	//Send data
	IdleI2C();				//wait for bus Idle
	StopI2C();				//Generate a stop
	return(0);
}


/*********************************************************************
* Function:        LDSequentialReadI2C()
*
* Input:		ControlByte, address, *rdptr, length.
*
* Output:		None.
*
* Overview:		Performs a sequential read of length bytes starting at address
*				and places data in array pointed to by *rdptr
*
* Note:			None
********************************************************************/
unsigned int LDSequentialReadI2C(unsigned char ControlByte, unsigned char address, unsigned char *rdptr, unsigned char length)
{
	IdleI2C();						//Ensure Module is Idle
	StartI2C();						//Initiate start condition
	WriteI2C(ControlByte);			//write 1 byte
	IdleI2C();						//Ensure module is Idle
	WriteI2C(address);				//Write word address
	IdleI2C();						//Ensure module is idle
	RestartI2C();					//Generate I2C Restart Condition
	WriteI2C(ControlByte | 0x01);	//Write 1 byte - R/W bit should be 1 for read
	IdleI2C();						//Ensure bus is idle
	getsI2C(rdptr, length);			//Read in multiple bytes
	NotAckI2C();					//Send Not Ack
	StopI2C();						//Send stop condition
	return(0);
}


/*********************************************************************
* Function:        HDSequentialReadI2C()
*
* Input:		ControlByte, HighAdd, LowAdd, *rdptr, length.
*
* Output:		None.
*
* Overview:		Performs a sequential read of length bytes starting at address
*				and places data in array pointed to by *rdptr
*
* Note:			None
********************************************************************/
unsigned int HDSequentialReadI2C(unsigned char ControlByte, unsigned char HighAdd, unsigned char LowAdd, unsigned char *rdptr, unsigned char length)
{
	IdleI2C();						//Ensure Module is Idle
	StartI2C();						//Initiate start condition
	WriteI2C(ControlByte);			//write 1 byte
	IdleI2C();						//Ensure module is Idle
	WriteI2C(HighAdd);				//Write High word address
	IdleI2C();						//Ensure module is idle
	WriteI2C(LowAdd);				//Write Low word address
	IdleI2C();						//Ensure module is idle
	RestartI2C();					//Generate I2C Restart Condition
	WriteI2C(ControlByte | 0x01);	//Write 1 byte - R/W bit should be 1 for read
	IdleI2C();						//Ensure bus is idle
	getsI2C(rdptr, length);			//Read in multiple bytes
	NotAckI2C();					//Send Not Ack
	StopI2C();						//Send stop condition
	return(0);
}


/*********************************************************************
* Function:        ACKStatus()
*
* Input:		None.
*
* Output:		Acknowledge Status.
*
* Overview:		Return the Acknowledge status on the bus
*
* Note:			None
********************************************************************/
unsigned int ACKStatus(void)
{
	return (!I2C1STATbits.ACKSTAT);		//Return Ack Status
}


/*********************************************************************
* Function:        NotAckI2C()
*
* Input:		None.
*
* Output:		None.
*
* Overview:		Generates a NO Acknowledge on the Bus
*
* Note:			None
********************************************************************/
unsigned int NotAckI2C(void)
{
	I2C1CONbits.ACKDT = 1;			//Set for NotACk
	I2C1CONbits.ACKEN = 1;
	while(I2C1CONbits.ACKEN);		//wait for ACK to complete
	I2C1CONbits.ACKDT = 0;			//Set for NotACk
	return 1;
}


/*********************************************************************
* Function:        AckI2C()
*
* Input:		None.
*
* Output:		None.
*
* Overview:		Generates an Acknowledge.
*
* Note:			None
********************************************************************/
unsigned int AckI2C(void)
{
	I2C1CONbits.ACKDT = 0;			//Set for ACk
	I2C1CONbits.ACKEN = 1;
	while(I2C1CONbits.ACKEN);		//wait for ACK to complete
	return 1;
}


/*********************************************************************
* Function:       getsI2C()
*
* Input:		array pointer, Length.
*
* Output:		None.
*
* Overview:		read Length number of Bytes into array
*
* Note:			None
********************************************************************/
unsigned int getsI2C(unsigned char *rdptr, unsigned short Length)
{
	while (Length --)
	{
		*rdptr++ = getI2C();		//get a single byte
		
		if(I2C1STATbits.BCL)		//Test for Bus collision
		{
			return(-1);
		}

		if(Length)
		{
			AckI2C();				//Acknowledge until all read
		}
	}
	return(0);
}


/*********************************************************************
* Function:        getI2C()
*
* Input:		None.
*
* Output:		contents of I2C1 receive buffer.
*
* Overview:		Read a single byte from Bus
*
* Note:			None
********************************************************************/
unsigned int getI2C(void)
{
	I2C1CONbits.RCEN = 1;			//Enable Master receive
	Nop();
	while(!I2C1STATbits.RBF);		//Wait for receive bufer to be full
	return(I2C1RCV);				//Return data in buffer
}


/*********************************************************************
* Function:        EEAckPolling()
*
* Input:		Control byte.
*
* Output:		error state.
*
* Overview:		polls the bus for an Acknowledge from device
*
* Note:			None
********************************************************************/
unsigned int EEAckPolling(unsigned char control)
{
	IdleI2C();				//wait for bus Idle
	StartI2C();				//Generate Start condition
	
	if(I2C1STATbits.BCL)
	{
		return(-1);			//Bus collision, return
	}

	else
	{
		if(WriteI2C(control))
		{
			return(-3);		//error return
		}

		IdleI2C();			//wait for bus idle
		if(I2C1STATbits.BCL)
		{
			return(-1);		//error return
		}

		while(ACKStatus())
		{
			RestartI2C();	//generate restart
			if(I2C1STATbits.BCL)
			{
				return(-1);	//error return
			}

			if(WriteI2C(control))
			{
				return(-3);
			}

			IdleI2C();
		}
	}
	StopI2C();				//send stop condition
	if(I2C1STATbits.BCL)
	{
		return(-1);
	}
	return(0);
}


/*********************************************************************
* Function:        putstringI2C()
*
* Input:		pointer to array.
*
* Output:		None.
*
* Overview:		writes a string of data upto PAGESIZE from array
*
* Note:			None
********************************************************************/
unsigned int putstringI2C(unsigned char *wrptr)
{
	unsigned char x;

	for(x = 0; x < PAGESIZE; x++)		//Transmit Data Until Pagesize
	{	
		if(WriteI2C(*wrptr))			//Write 1 byte
		{
			return(-3);				//Return with Write Collision
		}
		IdleI2C();					//Wait for Idle bus
		if(I2C1STATbits.ACKSTAT)
		{
			return(-2);				//Bus responded with Not ACK
		}
		wrptr++;
	}
	return(0);
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
    unsigned short ErrorCode;
	memset((unsigned short*)data,0,readnumber);

	IdleI2C();						//Ensure Module is Idle
	StartI2C();						//Generate Start Condition
	WriteI2C((reg.adr<<1)|reg.read);	//Write device addr
	IdleI2C();    
	ErrorCode = ACKStatus();		//Return ACK Status

	WriteI2C(reg.Config);			//Write Config addr
	IdleI2C();
	ErrorCode = ACKStatus();		//Return ACK Status
//	if(ErrorCode)					//maybe the slave is just in data conversion and so it does not send the ACK
//		return;

	IdleI2C();
	RestartI2C();					//Generate I2C Restart Condition

	WriteI2C((reg.adr<<1)|0x01);	//Write device addr again with R/W=1

	IdleI2C();
	getsI2C(data, readnumber);		//read Length number of bytes		
	StopI2C();						//Initiate Stop Condition
	return(ErrorCode);
}	
//instead of "getsI2C(data, readnumber);" if the readout needs to be done separatley due to memmory issues, the line could be replaced by e.g.:
//if(readnumber>1){
//	getsI2C(data, readnumber/2);		//read Length/2 number of bytes
//	AckI2C();
//	data+=readnumber/2;
//	getsI2C(data, readnumber/2);		//read Length/2 number of bytes
//}
//else{
//	getsI2C(data, readnumber);
//}
//The readout can simply be stopped by not sending any clock transitions anymore and continued by sending further clock transitions

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
		for(CurTACol=0;CurTACol<NROFTAELEMENTS;CurTACol++){
			if((XTATemps[CurTACol]<=TAmb)&&(XTATemps[CurTACol+1]>TAmb))
					break;
			}
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
			vx=((((signed long)TempTable[y][CurTACol+1]-(signed long)TempTable[y][CurTACol])*(signed long)dTA)/(signed long)TAEQUIDISTANCE)+(signed long)TempTable[y][CurTACol];
			vy=((((signed long)TempTable[y+1][CurTACol+1]-(signed long)TempTable[y+1][CurTACol])*(signed long)dTA)/(signed long)TAEQUIDISTANCE)+(signed long)TempTable[y+1][CurTACol];
			Tobject=(unsigned int)((vy-vx)*((signed long)val-(signed long)YADValues[y])/ydist+(signed long)vx);
			}
		else
			return 0;
		}
	else
		return 0;

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
unsigned char eecpy[8];

//read the whole EEPROM and put them into the storage
	p=data;
	Read_EEPROM(data,0x00,0xFF,I2Creg);
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

	//now get the thermal gradient and offset
	for(i=0;i<64;i++){
		memcpy((char*)&ThGrad[i],(char*)&data[((unsigned short)AdrTh1+i)*2],2);
		memcpy((char*)&ThOff[i],(char*)&data[((unsigned short)AdrTh2+i)*2],2);	
	}	
	//and the Pixel sensitivity
	for(i=0;i<64;i++){
		memcpy((char*)&PixCRaw,(char*)&data[((unsigned short)AdrPixC+i)*2],2);
		PixC[i]=(unsigned long)((((float)PixCRaw/65535.0)*(PixCMax-PixCMin)+(float)PixCMin)*(float)epsilon/100.0*(float)GlobalGain/10000.0+0.5);
	}	

//now check if the EEPROM was empty before
	if((SetMBITUser<0x09)||(SetMBITUser>0x0C)){		//EEPROM empty or setting out of spec -> set to default
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


return;
}

unsigned int Read_EEPROM(unsigned char* data,unsigned char ReadAdress, unsigned short readnumber,TAsicSFR reg){
    unsigned short ErrorCode,i;
	memset((unsigned short*)data,0,sizeof(data));

	reg.adr=0x1B;
	//first activate the EEPROM
	IdleI2C();						//Ensure Module is Idle
	StartI2C();						//Generate Start Condition
	WriteI2C(reg.adr<<1);	//Write device addr
	IdleI2C();    
	WriteI2C(EEPROM_ACTIVE);
	IdleI2C();    
	ErrorCode = ACKStatus();		//Return ACK Status
	StopI2C();						//Initiate Stop Condition

	//Wait for at least 15 µs
	for(i=0;i<800;i++)
		asm("NOP");

	//set the address to be read
	IdleI2C();						//Ensure Module is Idle
	StartI2C();						//Generate Start Condition
	WriteI2C(reg.adr<<1);	//Write device addr
	IdleI2C();    
	WriteI2C(EEPROM_SET_ADDR);
	IdleI2C();    
	ErrorCode = ACKStatus();		//Return ACK Status
	IdleI2C();    
	WriteI2C(ReadAdress);
	IdleI2C();    
	ErrorCode = ACKStatus();		//Return ACK Status
	StopI2C();						//Initiate Stop Condition

	//normal read sequence
	IdleI2C();						//Ensure Module is Idle
	StartI2C();						//Generate Start Condition
	WriteI2C(reg.adr<<1);	//Write device addr
	IdleI2C();    
	WriteI2C(EEPROM_NORMAL_READ);					//Reset
	IdleI2C();    
	ErrorCode = ACKStatus();		//Return ACK Status
	IdleI2C(); 
	StopI2C();						//Initiate Stop Condition

	//now read the EEPROM
	for(i=0;i<readnumber;i++){
		IdleI2C();						//Ensure Module is Idle
		StartI2C();						//Generate Start Condition
		WriteI2C(reg.adr<<1);	//Write device addr
		IdleI2C();     
		WriteI2C(EEPROM_GET_DATA);		
		IdleI2C();   			
		ErrorCode = ACKStatus();		//Return ACK Status
		IdleI2C();   
		RestartI2C();					//Generate I2C Restart Condition
	
		IdleI2C(); 
		WriteI2C((reg.adr<<1)|0x01);	//Write device addr
		IdleI2C();
		getsI2C(data, 2);		//read Length number of bytes
		data+=2;
		StopI2C();	
	}

	//now set the EEPROM into Standby
	IdleI2C();						//Ensure Module is Idle
	StartI2C();						//Generate Start Condition
	WriteI2C(reg.adr<<1);	//Write device addr
	IdleI2C();    
	ErrorCode = ACKStatus();		//Return ACK Status
	WriteI2C(EEPROM_STANDBY);					//Reset
	StopI2C();						//Initiate Stop Condition

	return(ErrorCode);

}

void HighDensPageWrite(unsigned short address,unsigned char *data, unsigned short numbytes){
	unsigned char HighAdd,LowAdd;
	unsigned short ErrorCode;
	TAsicSFR reg;

	InitI2C(1);
	numbytes/=2;	//divide them by 2 due to 16 and 8 bit scaling of EEPROM
	if(!numbytes)
		numbytes=1;

	reg.adr=0x1B;
	//first activate the EEPROM
	IdleI2C();						//Ensure Module is Idle
	StartI2C();						//Generate Start Condition
	WriteI2C(reg.adr<<1);			//Write device addr
	IdleI2C();    
	ErrorCode = ACKStatus();		//Return ACK Status
	WriteI2C(EEPROM_ACTIVE);
	StopI2C();						//Initiate Stop Condition

	//Wait for at least 15 µs
	for(i=0;i<800;i++)
		asm("NOP");

	//now do a loop with erasing and writing the data
	for(i=0;i<numbytes;i++){
		//Set the address
		IdleI2C();						//Ensure Module is Idle
		StartI2C();						//Generate Start Condition
		WriteI2C(reg.adr<<1);	//Write device addr
		IdleI2C();    
		ErrorCode = ACKStatus();		//Return ACK Status
		WriteI2C(EEPROM_SET_ADDR);
		IdleI2C();    
		ErrorCode = ACKStatus();		//Return ACK Status
		WriteI2C(address+i);
		StopI2C();						//Initiate Stop Condition

		//Normal Erase
		IdleI2C();						//Ensure Module is Idle
		StartI2C();						//Generate Start Condition
		WriteI2C(reg.adr<<1);	//Write device addr
		IdleI2C();    
		ErrorCode = ACKStatus();		//Return ACK Status
		WriteI2C(EEPROM_NORMAL_ERASE);					//Reset
		StopI2C();						//Initiate Stop Condition

		//wait for at least 5 ms
		DelayMs(5);

		//set the new EEPROM data
		IdleI2C();						//Ensure Module is Idle
		StartI2C();						//Generate Start Condition
		WriteI2C(reg.adr<<1);	//Write device addr
		IdleI2C();    
		ErrorCode = ACKStatus();		//Return ACK Status
		WriteI2C(EEPROM_SET_DATA);	
		IdleI2C();    
		ErrorCode = ACKStatus();		//Return ACK Status
		WriteI2C(data[i*2]);	
		IdleI2C();    
		WriteI2C(data[i*2+1]);			
		StopI2C();						//Initiate Stop Condition

		//command for writing
		IdleI2C();						//Ensure Module is Idle
		StartI2C();						//Generate Start Condition
		WriteI2C(reg.adr<<1);			//Write device addr
		IdleI2C();    
		ErrorCode = ACKStatus();		//Return ACK Status
		WriteI2C(EEPROM_NORMAL_WRITE);
		StopI2C();						//Initiate Stop Condition

		DelayMs(5);
	}
	//wait for at least 5 ms
	DelayMs(5);

	InitI2C(0);
return;
}