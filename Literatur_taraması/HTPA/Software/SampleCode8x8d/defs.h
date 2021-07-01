#define AdrPixCMin 0x00
#define AdrPixCMax 0x02
#define AdrGradScale 0x08
#define AdrGlobalGain 0x09
#define AdrTableNumber 0x0C
#define AdrEpsilon 0x0D

#define AdrMBITPixC 0x1A
#define AdrBIASPixC 0x1B
#define AdrCLKPixC 0x1C
#define AdrBPAPixC 0x1D
#define AdrPUPixC 0x1E

#define AdrMBITUser 0x20
#define AdrBIASUser 0x21
#define AdrCLKUser 0x22
#define AdrBPAUser 0x23
#define AdrPUUser 0x24

#define AdrPTATGrad 0x34
//#define AdrPTATOff 0x38
#define AdrDevID 0x3C
#define AdrTh1 0x40
#define AdrTh2 0x80
#define AdrPixC 0xC0

#define PTATLength 0x01

//default settings
#define BIAScurrentDefault 0x0C
#define CLKTRIMDefault 0x15 //0x20 to let it run with 10 Hz
#define BPATRIMDefault 0x0C
#define MBITTRIMDefault 0x0C
#define PUTRIMDefault	0x88

//Sensor
#define PIXEL 64
#define ELAMOUNT 64
#define PTATamount 1

//EEPROM Communications
#define EEPROM_STANDBY 0x00
#define EEPROM_ACTIVE 0x01
#define EEPROM_NORMAL_ERASE 0x02
#define EEPROM_NORMAL_WRITE 0x03
#define EEPROM_BLOCK_ERASE 0x04
#define EEPROM_NORMAL_READ 0x06
#define EEPROM_SET_ADDR 0x09
#define EEPROM_SET_DATA 0x0A
#define EEPROM_GET_DATA 0x0B

#define ReadToFromTable
#ifdef ReadToFromTable
	//#define HTPA32x32dL5_0HiGe
	//#define HTPA32x32dL5_0HiGeF7_7
	//#define HTPA32x32dL2_1HiSi
	#define HTPA32x32dL2_1HiSiF5_0
	//#define HTPA32x32dL2_1HiSiF5_0_withSiFilter	
	//#define HTPA32x32dL3_6HiSi	//same as L2.85 and L7.0, will be measured and adapted in the future
	//#define HTPA32x32dL2_1HiSiDLC
	//#define HTPA32x32dL2_1Si_withSiFilter
	//#define HTPA32x32dR1L7_0HiSi_Gain3k3

    #ifdef HTPA32x32dL5_0HiGe
		#define TABLENUMBER		79
		#define PCSCALEVAL		100000000		//PixelConst scale value for table
		#define NROFTAELEMENTS 	7
		#define NROFADELEMENTS 	471
		#define TAEQUIDISTANCE	100		//dK
		#define ADEQUIDISTANCE	64		//dig
		#define ADEXPBITS		6		//2^ADEXPBITS=ADEQUIDISTANCE
		#define TABLEOFFSET		512
    #endif
	
	#ifdef HTPA32x32dL5_0HiGeF7_7
	 	#define TABLENUMBER		92
		#define PCSCALEVAL		100000000 		//PixelConst scale value for table
		#define NROFTAELEMENTS 	7
		#define NROFADELEMENTS 	471
		#define TAEQUIDISTANCE	100		//dK
		#define ADEQUIDISTANCE	64		//dig
		#define ADEXPBITS		6		//2^ADEXPBITS=ADEQUIDISTANCE
		#define TABLEOFFSET		256
    #endif

    #ifdef HTPA32x32dL2_1HiSi
		#define TABLENUMBER		80
		#define PCSCALEVAL		100000000		//PixelConst scale value for table
		#define NROFTAELEMENTS 	7
		#define NROFADELEMENTS 	471
		#define TAEQUIDISTANCE	100		//dK
		#define ADEQUIDISTANCE	64		//dig
		#define ADEXPBITS		6		//2^ADEXPBITS=ADEQUIDISTANCE
		#define TABLEOFFSET		256
    #endif
	
	#ifdef HTPA32x32dL2_1HiSiF5_0
	 	#define TABLENUMBER		96
		#define PCSCALEVAL		100000000		//PixelConst scale value for table... lower 'L' for (long)
		#define NROFTAELEMENTS 	7
		#define NROFADELEMENTS 	471	//130 possible due to Program memory, higher values possible if NROFTAELEMENTS is decreased
		#define TAEQUIDISTANCE	100		//dK
		#define ADEQUIDISTANCE	64		//dig
		#define ADEXPBITS		6		//2^ADEXPBITS=ADEQUIDISTANCE
		#define TABLEOFFSET		256
		#define EQUIADTABLE		//if defined, ADELEMENTS have to be 2^N quantizied! else more CPU Power is needed
		#ifdef EQUIADTABLE
			#undef FLOATTABLE
		#endif
    #endif

    #ifdef HTPA32x32dL2_1HiSiF5_0_withSiFilter
	 	#define TABLENUMBER		97
		#define PCSCALEVAL		100000000		//PixelConst scale value for table... lower 'L' for (long)
		#define NROFTAELEMENTS 	7
		#define NROFADELEMENTS 	471	//130 possible due to Program memory, higher values possible if NROFTAELEMENTS is decreased
		#define TAEQUIDISTANCE	100		//dK
		#define ADEQUIDISTANCE	64		//dig
		#define ADEXPBITS		6		//2^ADEXPBITS=ADEQUIDISTANCE
		#define TABLEOFFSET		256
		#define EQUIADTABLE		//if defined, ADELEMENTS have to be 2^N quantizied! else more CPU Power is needed
		#ifdef EQUIADTABLE
			#undef FLOATTABLE
		#endif
    #endif

    #ifdef HTPA32x32dL3_6HiSi
		#define TABLENUMBER		81
		#define PCSCALEVAL		100000000		//PixelConst scale value for table
		#define NROFTAELEMENTS 	7
		#define NROFADELEMENTS 	471
		#define TAEQUIDISTANCE	100		//dK
		#define ADEQUIDISTANCE	64		//dig
		#define ADEXPBITS		6		//2^ADEXPBITS=ADEQUIDISTANCE
		#define TABLEOFFSET		256
    #endif
	
    #ifdef HTPA32x32dL2_1HiSiDLC
		#define TABLENUMBER		83
		#define PCSCALEVAL		100000000		//PixelConst scale value for table
		#define NROFTAELEMENTS 	7
		#define NROFADELEMENTS 	471
		#define TAEQUIDISTANCE	100		//dK
		#define ADEQUIDISTANCE	64		//dig
		#define ADEXPBITS		6		//2^ADEXPBITS=ADEQUIDISTANCE
		#define TABLEOFFSET		512
    #endif

    #ifdef HTPA32x32dL2_1Si_withSiFilter
		#define TABLENUMBER		88
		#define PCSCALEVAL		100000000		//PixelConst scale value for table
		#define NROFTAELEMENTS 	7
		#define NROFADELEMENTS 	471
		#define TAEQUIDISTANCE	100		//dK
		#define ADEQUIDISTANCE	8		//dig
		#define ADEXPBITS		3		//2^ADEXPBITS=ADEQUIDISTANCE
		#define TABLEOFFSET		64
    #endif	
	
    #ifdef HTPA32x32dR1L7_0HiSi_Gain3k3
	 	#define TABLENUMBER		118
		#define PCSCALEVAL		100000000		//PixelConst scale value for table... lower 'L' for (long)
		#define NROFTAELEMENTS 	7
		#define NROFADELEMENTS 	1595	//130 possible due to Program memory, higher values possible if NROFTAELEMENTS is decreased
		#define TAEQUIDISTANCE	100		//dK
		#define ADEQUIDISTANCE	64		//dig
		#define ADEXPBITS		6		//2^ADEXPBITS=ADEQUIDISTANCE
		#define TABLEOFFSET		640
		#define EQUIADTABLE		//if defined, ADELEMENTS have to be 2^N quantizied! else more CPU Power is needed
		#ifdef EQUIADTABLE
			#undef FLOATTABLE
		#endif
		#define MBITTRIMDefault 0x2C
		#define SensRv 1
	#endif	
#endif

unsigned int ACKStatus(void);
unsigned int EEAckPolling(unsigned char control);    
unsigned int getsI2C(unsigned char *rdptr, unsigned short Length);
unsigned int NotAckI2C(void);
unsigned int putstringI2C(unsigned char *wrptr);
unsigned int getI2C(void);
unsigned int LDByteWriteI2C(unsigned char ControlByte, unsigned char LowAdd, unsigned char data);
unsigned int InitI2C(unsigned char EEPROM);
unsigned int StopI2C(void);
unsigned int LDByteReadI2C(unsigned char ControlByte, unsigned char Address, unsigned char *Data, unsigned char Length);
unsigned int Read_STATUS(unsigned char* data);
void StartStreaming(char Temps,char Stream);
unsigned int calcTO(unsigned int TAmb, signed int dig, signed long PiC, unsigned int dontCalcTA);
void InitCLKTRIM(unsigned char user);
void InitBIASTRIM(unsigned char user);
void InitBPATRIM(unsigned char user);
void InitMBITTRIM(unsigned char user);
void InitPUTRIM(unsigned char user);
void ReadCalibData(unsigned char print);
void SensorSleep(void);
void SensorWakeup(unsigned char user);