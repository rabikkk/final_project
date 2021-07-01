#include "Globals.h"



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

unsigned int Read_RAM(unsigned char* RAMdata,TAsicSFR reg, unsigned short readnumber){
    unsigned short ErrorCode;
//	memset((unsigned short*)RAMdata,0,readnumber*2);		//not needed!

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

	//getsI2C(ReadAdr,0x01);
	IdleI2C();
	getsI2C(RAMdata, readnumber);		//read Length number of bytes		
	StopI2C();						//Initiate Stop Condition

	return(ErrorCode);
}	

/*********************************************************************
* Function:     Read_EEPROM()
*
* Input:		pointer to data, ReadAdress of EEPROM and amount of data to read
*
* Output:		data (changes global)
*
* Overview:		reads the EEPROM1 (Sensor) at the Adress ReadAdress for the amount of readnumber
*
* Note:			None
********************************************************************/

unsigned int Read_EEPROM(unsigned char* data,unsigned short ReadAdress, unsigned short readnumber,TAsicSFR reg){
    unsigned short ErrorCode,i;
//	memset((unsigned short*)data,0,sizeof(data));

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
	WriteI2C((ReadAdress&0xFF00)>>8);
	IdleI2C();    
	WriteI2C(ReadAdress&0xFF);
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

/*********************************************************************
* Function:     Write_Conf()
*
* Input:		w (refrehrate)
*
* Output:		
*
* Overview:		Writes the refreshrate into the RAM and calculates the framerate
*
* Note:			None
********************************************************************/

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
