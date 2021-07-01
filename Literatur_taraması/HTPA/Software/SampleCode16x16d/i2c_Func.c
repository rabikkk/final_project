/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
;                                                                     
;                     Software License Agreement                      
;                                                                     
;     ©2007 Microchip Technology Inc
;     Mirochip Technology Inc. ("Microchip") licenses this software to 
;     you solely for the use with Microchip Products. The software is
;     owned by Microchip and is protected under applicable copyright
;     laws. All rights reserved.
;
;     SOFTWARE IS PROVIDED "AS IS." MICROCHIP EXPRESSLY DISCLAIMS ANY
;     WARRANTY OF ANY KIND, WHETHER EXPRESS OR IMPLIED, INCLUDING BUT NOT
;     LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
;     PARTICULAR PURPOSE, OR NON-INFRINGEMENT. IN NO EVENT SHALL MICROCHIP
;     BE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR CONSEQUENTIAL
;     DAMAGES, LOST PROFITS OR LOST DATA, HARM TO YOUR EQUIPMENT, COST OF
;     PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY OR SERVICES, ANY CLAIMS
;     BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF),
;     ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION, OR OTHER SIMILAR COSTS.
;     
;                                                                
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
;	Filename:			    i2c_Func.c            
;	Date:				    February 21, 2007          
;	File Version:		  	1.0                             
;	Assembled using:		MPLAB IDE 7.51.00.0               
; 	Author:		  	    	Martin Bowman              
;	Company:			    Microchip Technology, Inc.
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#include "Globals.h"

//Function Prototypes
void bit_in(unsigned char *data);		//Bit Input function
void bit_out(unsigned char data);		//Bit Out function
//unsigned short swaps(unsigned int v);

/********************************************************************
 * Function:        void bstart(void)
 *
 * Description:     This function generates an I2C Start condition.
 *******************************************************************/
void bstart(void)
{

    SDA_TRIS = 1;                   // Ensure SDA is high
    SCL = 1;                        // Ensure SCL is high
	asm("NOP");
    SDA_TRIS = 0;                   // Configure SDA as an output
    SDA = 0;                        // Pull SDA low
	kHz400Delay();
    SCL = 0;                        // Pull SCL low
  
} // end bstart(void)

/********************************************************************
 * Function:        void bstop(void)
 *
 * Description:     This function generates an I2C Stop condition.
 *******************************************************************/
void bstop(void)
{
    SCL = 0;                        // Ensure SCL is low
    SDA_TRIS = 0;                   // Configure SDA as an output
    SDA = 0;                        // Ensure SDA low
	kHz400Delay();
    SCL = 1;                        // Pull SCL high
	kHz400Delay();
    SDA_TRIS = 1;                   // Allow SDA to be pulled high
} // end bstop(void)

/********************************************************************
 * Function:        void bit_out(unsigned char data)
 *
 * Description:     This function outputs a bit to the I2C bus.
 *******************************************************************/
void bit_out(unsigned char data)
{
    SCL = 0;                        // Ensure SCL is low
    if (data & 0x80)                // Check if next bit is high
    {
        SDA_TRIS = 1;               // Release SDA to be pulled high
	kHz400Delay();
    }
    else
    {
        SDA_TRIS = 0;               // Configure SDA as an output
        SDA = 0;                    // Pull SDA low
	kHz400Delay();
    }
    SCL = 1;                        // Pull SCL high to clock bit
	kHz400Delay();
    SCL = 0;                        // Pull SCL low for next bit
} // end bit_out(unsigned char data)

/********************************************************************
 * Function:        void bit_in(unsigned char *data)
 *
 * Description:     This function inputs a bit from the I2C bus.
 *******************************************************************/
void bit_in(unsigned char *data)
{

    SCL = 0;                        // Ensure SCL is low	
	kHz400Delay();
    SDA_TRIS = 1;                   // Configure SDA as an input
	asm("NOP");
    SCL = 1;                        // Bring SCL high to begin transfer
	kHz400Delay();   
    *data &= 0xFE;                  // Assume next bit is low
    if (SDA)                        // Check if SDA is high
    {
        *data |= 0x01;              // If high, set next bit
    }
	kHz400Delay();
    SCL = 0;                        // Bring SCL low again

} // end bit_in(unsigned char *data)

/********************************************************************
 * Function:        unsigned char byte_out(unsigned char data)
 *
 * Description:     This function outputs a byte to the I2C bus.
 *                  It also receives the ACK bit and returns 0 if
 *                  successfully received, or 1 if not.
 *******************************************************************/
unsigned char byte_out(unsigned char data)
{
    unsigned char i;                // Loop counter
    unsigned char ack;              // ACK bit

    ack = 0;
    for (i = 0; i < 8; i++)         // Loop through each bit
    {
        bit_out(data);              // Output bit
        data = data << 1;           // Shift left for next bit
    }

    bit_in(&ack);                   // Input ACK bit

    return ack;
} // end byte_out(unsigned char data)

/********************************************************************
 * Function:        unsigned char byte_in(unsigned char ack)
 *
 * Description:     This function inputs a byte from the I2C bus.
 *                  Depending on the value of ack, it will also
 *                  transmit either an ACK or a NAK bit.
 *******************************************************************/
unsigned char byte_in(unsigned char ack)
{
    unsigned char i;                // Loop counter
    unsigned char retval;         	// Return value

    retval = 0;
    for (i = 0; i < 8; i++)         // Loop through each bit
    {
        retval = retval << 1;       // Shift left for next bit
        bit_in(&retval);            // Input bit
    }
    bit_out(ack);                   // Output ACK/NAK bit
    return retval;
} // end byte_in(void)

/********************************************************************
 * Function:        void HighDensByteWrite(unsigned char data)
 *
 * Description:     This function writes a single byte to a
 *                  high-density (>= 32 Kb) serial EEPROM device.
 *
 * Dependencies:    'i2ccontrol' contains the control byte
 *                  'address' contains the address word
 *******************************************************************/
void HighDensByteWrite(unsigned short address, unsigned short data)
{
    bstart();                       // Generate Start condition
    byte_out(CONTROLBYTE);              // Output control byte
    byte_out((unsigned char)(address>>8));// Output address MSB
    byte_out((unsigned char)address);// Output address LSB
    byte_out(data);                 // Output data byte
    bstop();                        // Generate Stop condition
    ACK_Poll();                     // Begin ACK polling
} // end HighDensByteWrite(unsigned char data)

/********************************************************************
 * Function:        void HighDensPageWrite(unsigned int adress,
										   unsigned char *data,
 *                                         unsigned char numbytes)
 *
 * Description:     This function writes multiple bytes to a
 *                  high-density (>= 32 Kb) serial EEPROM device.
 *
 * Dependencies:    'i2ccontrol' contains the control byte
 *                  'address' contains the address word
 *******************************************************************/
void HighDensPageWrite(unsigned short address,unsigned char *data, unsigned short numbytes){
unsigned short i;                // Loop counter
unsigned short numbytes2=0;		//Zähler für die bereits geschriebenen Daten

	InitI2C(1);
	unsigned short ErrorCode;
	TAsicSFR reg;

	if(numbytes==1){
		data[1]=0x00;
	}
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
        WriteI2C(((address+i)&0xFF00)>>8);
        IdleI2C();
        WriteI2C(((address+i)&0x00FF));
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
void HighDensSequentialReadIP(unsigned short address,unsigned char* data, unsigned short numbytes)
{
    unsigned short i;                 // Loop counter
    bstart();                       // Generate Start condition
    byte_out(CONTROLBYTE);              // Output control byte
    byte_out((unsigned char)(address>>8));// Output address MSB
    byte_out((unsigned char)address);// Output address LSB
    bstart();                       // Generate Start condition
    byte_out(CONTROLBYTE | 0x01);       // Output control byte
    for (i = 0; i < numbytes; i++)  // Loop through data bytes
    {
        if (i < (numbytes - 1))     // Check if more data will be read
        {
            data[i] = byte_in(ACKBIT); // If not last, input byte & send ACK
        }
        else
        {
            data[i] = byte_in(NAKBIT); // If last byte, input byte & send NAK
        }
    }
    bstop();                        // Generate Stop condition
} // end HighDensSequentialRead(unsigned char *data, unsigned char numbytes)

/********************************************************************
 * Function:        void HighDensByteRead(unsigned char *data)
 *
 * Description:     This function reads a single byte from a
 *                  high-density (>= 32 Kb) serial EEPROM device.
 *
 * Dependencies:    'i2ccontrol' contains the control byte
 *                  'address' contains the address word
 *******************************************************************/
void HighDensByteRead(unsigned short address, unsigned char *data)
{
    bstart();                       // Generate Start condition
    byte_out(CONTROLBYTE);              // Output control byte
    byte_out((unsigned char)(address>>8));// Output address MSB
    byte_out((unsigned char)address);// Output address LSB
    bstart();                       // Generate Start condition
    byte_out(CONTROLBYTE | 0x01);       // Output control byte
    *data = byte_in(NAKBIT);        // Input data byte
    bstop();                        // Generate Stop condition
} // end HighDensByteRead(unsigned char data)

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

   unsigned short ErrorCode,i;
//	memset((unsigned short*)data,0,sizeof(data));
	TAsicSFR reg;

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
	WriteI2C((address&0xFF00)>>8);
	IdleI2C();    
	WriteI2C(address&0xFF);
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
	for(i=0;i<numbytes;i++){
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

} // end HighDensSequentialRead(unsigned char *data, unsigned char numbytes)

/********************************************************************
 * Function:        void ACK_Poll(void)
 *
 * Description:     This function implements Acknowledge polling.
 *
 * Dependencies:    'control' contains the control byte
 *******************************************************************/
void ACK_Poll(void)
{
    unsigned char result;           // Polling result
	unsigned short count=0;
    result = 1;                     // Initialize result
    do
    {
        bstart();                   // Generate Start condition
        result = byte_out(CONTROLBYTE); // Output control byte
       // printf("ACK_poll");
    } while ((result == 1) && (count++<1000));
    if (count>=1000) printf("Timeout EEPROM ");
    bstop();                        // Generate Stop condition
} // end ACK_Poll(void)


/********************************************************************
 * Function:        void DumpEEPROM()
 *
 * Description:     This function reads the whole content of
*					a (>= 32 Kb) serial EEPROM device.
*					Can be interrupted by pressing any button
 *
 * Dependencies:    
 *******************************************************************/
void DumpEEPROM(){
	unsigned short page,i;
	unsigned short *d, address;
	unsigned char data2[PAGESIZE2];
	address = 0x0000; 
	for (page=0; page<(16384/PAGESIZE2); page++){
		d=(unsigned short*)data2;
		sprintf(tempstring,"\r\n%04x: ",address);
		putstringUDPUART(tempstring);
		HighDensSequentialRead(address,data2,PAGESIZE2);// Read a full page
		for (i = 0; i < PAGESIZE2; i+=2){			    // Loop through full page
			sprintf(tempstring,"%04x ",*(unsigned short*)(d++));
			putstringUDPUART(tempstring);
		}
		address+=PAGESIZE2;	
		if (U2STAbits.URXDA) {
			i=U2RXREG;
			sprintf(tempstring,"\r\n");
			putstringUDPUART(tempstring);
			return;
			}
		}
	sprintf(tempstring,"\r\n");
	putstringUDPUART(tempstring);
	}	 

/********************************************************************
 * Function:        void FillEEPROM(unsigned char data,
 *                                         unsigned char numbytes)
 *
 * Description:     This function fills multiple bytes to a
 *                  high-density (>= 32 Kb) serial EEPROM device.
 *
 * Dependencies:    'i2ccontrol' contains the control byte
 *                  'address' contains the address word
 *******************************************************************/
void FillEEPROM(unsigned short address, unsigned char data, long long numbytes){
unsigned char i;                // Loop counter
numbytes=0x4000;
	while (numbytes>0){
	    bstart();                       // Generate Start condition
	    byte_out(CONTROLBYTE);              // Output control byte
	    byte_out((unsigned char)(address>>8));// Output address MSB
	    byte_out((unsigned char)address);// Output address LSB
	    for (i=0; i<((numbytes>=32)?32:numbytes); i++){  // Loop through data bytes
	//    	printf("%x ",i); 
	    	byte_out(data);          // Output next data byte

	    	}	    
	 //   	printf("\r\n");
	    numbytes-=32;
	    address+=32;
	    bstop();                        // Generate Stop condition
	    ACK_Poll();                     // Begin ACK polling
	    }
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void kHz400Delay(void){  //delay for 400kHz-CLK of EEPROM
unsigned short test;

test=T1INCSFOR400KHZ;
T1CONCLR=0xFF;
TMR1CLR=0xFFFFFFFF;
T1CONbits.TON=1;
while (TMR1<T1INCSFOR400KHZ);
T1CONbits.TON=0;
}
//////////////////////////////////////////////////////////////////////////////////////////////////