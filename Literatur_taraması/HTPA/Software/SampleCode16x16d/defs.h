#ifndef DEFS_H
#define	DEFS_H

#define PCBR2



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

#define AdrVddMeasTh1 0x25
#define AdrVddScaling 0x3E
#define AdrVddScalingOff 0x3F

#define AdrPTATTh1 0x2C
#define AdrPTATGrad 0x34
#define AdrDevID 0x3A

#define AdrVddCompValues2 0x40			//12 bit signed
#define AdrVddCompValues 0xA0			//12 bit signed

#define AdrTh1 0x100					//16 bit signed
#define AdrTh2 0x200					//16 bit signed
#define AdrPixC 0x300					//16 bit unsigned


#ifdef PCBR2
	#define FINCLK	8.0e6 //Crystal frequency in Hz
	#define MULT 20.0
	#define DIV1 1.0		//CHECK EEPCLK WITH SCOPE WHEN CHANGING TCY!!!!!!!!!!!
	#define DIV2 2.0
#else
	#define FINCLK	4.0e6 //Crystal frequency in Hz
	#define MULT 20.0
	#define DIV1 1.0		//CHECK EEPCLK WITH SCOPE WHEN CHANGING TCY!!!!!!!!!!!
	#define DIV2 1.0
#endif
#define FCY	(double)((FINCLK*MULT)/(DIV1*DIV2))
#define TCY (1.0/(double)FCY)
#define T1INCSFOR400KHZ	(unsigned short)((10e-8/(double)TCY)+15) //(unsigned short)((5e-8/(double)TCY)+5)


//some constants
#define PAGESIZE	32			//page size for the EEPROM (Sensor)
#define CorrOffset 4681//32767				//add an Offset of 0.7 V (=0.7/(0.7*2)*65535) for the Corr mode to send positive data
#define BIAScurrentDefault 0x05		//Michael: noch zu übrerprüfen
#define CLKTRIMDefault 0x15//0x15
#define BPATRIMDefault 0x0C
#define MBITTRIMDefault 0x2C		//Michael: noch zu übrerprüfen
#define PUTRIMDefault	0x88

#define Pixel 256				//16x16
#define PIXEL Pixel
#define PixelEighth 64
#define ZEILE 16
#define SPALTE 16
#define DATALength 780//=2*390					//length of UDP packet
#define PTATamount 4
#define ELOFFSET 256			//start address of el. Offset
#define Pixelforth 128
#define ELAMOUNT 128
#define ELAMOUNTHALF 64
#define StackSize 10//16
#define STACKSIZEPTAT 30		//should be an even number
#define STACKSIZEVDD 50			//should be an even number
#define PTATSTARTADSRESS 386
#define VDDADDRESS 384

//EEPROM Communications
#define EEPROM_STANDBY 0x00
#define EEPROM_ACTIVE 0x01
#define EEPROM_NORMAL_ERASE 0x02
#define EEPROM_NORMAL_WRITE 0x03
#define EEPROM_NORMAL_READ 0x06
#define EEPROM_SET_ADDR 0x09
#define EEPROM_SET_DATA 0x0A
#define EEPROM_GET_DATA 0x0B

#define GetElEveryFrameX 19		//amount of normal frames to capture after which the el. Offset is fetched

#define ReadToFromTable
#define Multitable		//several tables are used in the programm and determined via the TABLENUMBER

#ifdef ReadToFromTable
	#ifdef Multitable
		#define NrOfTables 11
		#define HTPA32x32dL5_0HiGeMult
		#define HTPA32x32dL2_1HiSiF5Mult
		#define HTPA32x32dL3_6HiSiMult	
		#define HTPA32x32dL7_0HiSiMult		
		//#define HTPA32x32dL2_85HiSiMult		
		#define HTPA32x32dL5_0HiGeF7_7Mult
		#define HTPA32x32dL1_6HiGe_Mult 
		#define HTPA32x32dR1L2_1HiSiF5_0_Gain3k3Mult 
		#define HTPA32x32dR1L5_0HiGeF7_7_Gain3k3Mult
		#define HTPA32x32dR1L3_6HiSi_Rev1_Gain3k3Mult
		#define HTPA32x32dR1L7_0HiSi_Gain3k3Mult
		#define HTPA32x32dR1L1_6HiGe_Gain3k3Mult
	#else
		//#define HTPA32x32dL2_1HiSiF5_0
		//#define HTPA32x32dL2_1HiSiF5_0_withSiFilter
		//#define HTPA32x32dL3_6HiSi_Rev1
		//#define HTPA32x32dL7_0HiSi
		//#define HTPA32x32dL5_0HiGeF7_7
		//#define HTPA32x32dL1_6HiGe
        //#define HTPA32x32dL1k8_0k7HiGe
		//#define HTPA32x32dR1L1k8_0k7HiGe
		//#define HTPA32x32dR1L1k8_0k7HiGe_Bodo
        //#define HTPA32x32dL3_6HiSi_uncoated	
		//#define HTPA32x32dR1L5_0HiGeF7_7_Gain3k3
		#define HTPA32x32dR1L2_1HiSiF5_0_Gain3k3
		//#define HTPA32x32dR1L2_1SiF5_0_N2
		//#define HTPA32x32dR1L2_1HiSiF5_0_Precise
		//#define HTPA32x32dR1L2_85Hi_Gain3k3
		//#define HTPA32x32dR1L3_6HiSi_Rev1_Gain3k3
		//#define HTPA32x32dR1L7_0HiSi_Gain3k3
		//#define HTPA32x32dR1L1_6HiGe_Gain3k3
	#endif

   #ifdef HTPA32x32dL5_0HiGe
	 	#define TABLENUMBER		79
		#define PCSCALEVAL		100000000 //327000000000		//PixelConst scale value for table... lower 'L' for (long)
		#define NROFTAELEMENTS 	7
		#define NROFADELEMENTS 	471	//130 possible due to Program memory, higher values possible if NROFTAELEMENTS is decreased
		#define TAEQUIDISTANCE	100		//dK
		#define ADEQUIDISTANCE	64		//dig
		#define ADEXPBITS		6		//2^ADEXPBITS=ADEQUIDISTANCE
		#define TABLEOFFSET		512
		#define EQUIADTABLE		//if defined, ADELEMENTS have to be 2^N quantizied! else more CPU Power is needed
		#ifdef EQUIADTABLE
			#undef FLOATTABLE
		#endif   
		#define MBITTRIMDefault 0x0C
		#define SensRv 0
    #endif

    #ifdef HTPA32x32dL5_0HiGeF7_7
	 	#define TABLENUMBER		92
		#define PCSCALEVAL		100000000 //327000000000		//PixelConst scale value for table... lower 'L' for (long)
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
		#ifdef AUTOCHANGEGRADOFF
			#define AUTOCHANGEGADRVALUE -0.03781//-0.07241
			#define AUTOCHANGEOFFVALUE 107.8//182.7//212.7265	//changed by -30 Digits
			#define DEFAULTGLOBALOFF 0			//set to -30 due to last measurements
		#endif
		#define MBITTRIMDefault 0x0C
		#define SensRv 0
    #endif

    #ifdef HTPA32x32dR1L5_0HiGeF7_7_Gain3k3
	 	#define TABLENUMBER		113
		#define PCSCALEVAL		100000000 //327000000000		//PixelConst scale value for table... lower 'L' for (long)
		#define NROFTAELEMENTS 	7
		#define NROFADELEMENTS 	1595	//130 possible due to Program memory, higher values possible if NROFTAELEMENTS is decreased
		#define TAEQUIDISTANCE	100		//dK
		#define ADEQUIDISTANCE	64		//dig
		#define ADEXPBITS		6		//2^ADEXPBITS=ADEQUIDISTANCE
		#define TABLEOFFSET		1024
		#define EQUIADTABLE		//if defined, ADELEMENTS have to be 2^N quantizied! else more CPU Power is needed
		#ifdef EQUIADTABLE
			#undef FLOATTABLE
		#endif   
		#define MBITTRIMDefault 0x2C
		#define SensRv 1
		#define EXPONENT 4.0//2.9234487420098
		#ifdef ADAPTCAPFOV
			#define FCPOW4 0.0
			#define FCPOW3 0.000061064
			#define FCQUAD -0.01298
			#define FCLIN 0.0055364
			#define FCOFF 62.338
			#define DELTATOCAP 0.00		//in K
			#define TA0TAYLOR 306.0		//in K
			#define DIVIDEFACTOR 0.0159185585611478
		#endif
		#define DEFINEDAPPSETOFFSET 0		//tbd
		#define DEFINEDAPPSETGAIN 0			//tbd
		#define DEFAULTGLOBALGAIN 6155
    #endif

    #ifdef HTPA32x32dR1L2_1HiSiF5_0_Gain3k3
	 	#define TABLENUMBER		114
		#define PCSCALEVAL		100000000 //327000000000		//PixelConst scale value for table... lower 'L' for (long)
		#define NROFTAELEMENTS 	7
		#define NROFADELEMENTS 	1595	//130 possible due to Program memory, higher values possible if NROFTAELEMENTS is decreased
		#define TAEQUIDISTANCE	100		//dK
		#define ADEQUIDISTANCE	64		//dig
		#define ADEXPBITS		6		//2^ADEXPBITS=ADEQUIDISTANCE
		#define TABLEOFFSET		1024
		#define EQUIADTABLE		//if defined, ADELEMENTS have to be 2^N quantizied! else more CPU Power is needed
		#ifdef EQUIADTABLE
			#undef FLOATTABLE
		#endif   
		#define MBITTRIMDefault 0x2C
		#define SensRv 1
		#define EXPONENT 4.0//3.72781049994684
		#ifdef ADAPTCAPFOV
			#define FCPOW4 -0.0013105//-0.00187324
			#define FCPOW3 0.037154//0.05712135
			#define FCQUAD -0.36463//-0.5230756
			#define FCLIN 1.4831//1.490655
			#define FCOFF 118.43//119.143
			#define DELTATOCAP 0.015//0.07	//0.00005	//in K
			#define TA0TAYLOR 306.0		//in K
			#define DIVIDEFACTOR 0.0159185585611478
		#endif
		#define DEFINEDAPPSETOFFSET 0
		#define DEFINEDAPPSETGAIN -15
		#define DEFAULTGLOBALGAIN 8400
    #endif

    #ifdef HTPA32x32dR1L2_1SiF5_0_N2
	 	#define TABLENUMBER		130
		#define PCSCALEVAL		100000000 //327000000000		//PixelConst scale value for table... lower 'L' for (long)
		#define NROFTAELEMENTS 	7
		#define NROFADELEMENTS 	1595	//130 possible due to Program memory, higher values possible if NROFTAELEMENTS is decreased
		#define TAEQUIDISTANCE	100		//dK
		#define ADEQUIDISTANCE	64		//dig
		#define ADEXPBITS		6		//2^ADEXPBITS=ADEQUIDISTANCE
		#define TABLEOFFSET		192
		#define EQUIADTABLE		//if defined, ADELEMENTS have to be 2^N quantizied! else more CPU Power is needed
		#ifdef EQUIADTABLE
			#undef FLOATTABLE
		#endif   
		#define MBITTRIMDefault 0x2C
		#define SensRv 1
		#define EXPONENT 4.0//3.72781049994684
		#ifdef ADAPTCAPFOV
			#define FCPOW4 -0.0013105//-0.00187324
			#define FCPOW3 0.037154//0.05712135
			#define FCQUAD -0.36463//-0.5230756
			#define FCLIN 1.4831//1.490655
			#define FCOFF 118.43//119.143
			#define DELTATOCAP 0.015//0.07	//0.00005	//in K
			#define TA0TAYLOR 306.0		//in K
			#define DIVIDEFACTOR 0.0159185585611478
		#endif
		#define DEFINEDAPPSETOFFSET 0
		#define DEFINEDAPPSETGAIN -5
		#define DEFAULTGLOBALGAIN 10000
    #endif

    #ifdef HTPA32x32dR1L2_1HiSiF5_0_Precise
	 	#define TABLENUMBER		116
		#define PCSCALEVAL		100000000 //327000000000		//PixelConst scale value for table... lower 'L' for (long)
		#define NROFTAELEMENTS 	22
		#define NROFADELEMENTS 	1000	//130 possible due to Program memory, higher values possible if NROFTAELEMENTS is decreased
		#define TAEQUIDISTANCE	50		//dK
		#define ADEQUIDISTANCE	32		//dig
		#define ADEXPBITS		5		//2^ADEXPBITS=ADEQUIDISTANCE
		#define TABLEOFFSET		1024
		#define EQUIADTABLE		//if defined, ADELEMENTS have to be 2^N quantizied! else more CPU Power is needed
		#ifdef EQUIADTABLE
			#undef FLOATTABLE
		#endif   
		#define MBITTRIMDefault 0x2C
		#define SensRv 1
		#define EXPONENT 4.0//3.72781049994684
		#ifdef ADAPTCAPFOV
			#define FCPOW4 -0.0013105//-0.00187324
			#define FCPOW3 0.037154//0.05712135
			#define FCQUAD -0.36463//-0.5230756
			#define FCLIN 1.4831//1.490655
			#define FCOFF 118.43//119.143
			#define DELTATOCAP 0.020		//in K
			#define TA0TAYLOR 306.0		//in K
			#define DIVIDEFACTOR 0.0159185585611478
		#endif
		#define DEFINEDAPPSETOFFSET 0
		#define DEFINEDAPPSETGAIN -15
		#define DEFAULTGLOBALGAIN 7600
    #endif

    #ifdef HTPA32x32dR1L2_85Hi_Gain3k3
	 	#define TABLENUMBER		127
		#define PCSCALEVAL		100000000 //327000000000		//PixelConst scale value for table... lower 'L' for (long)
		#define NROFTAELEMENTS 	7
		#define NROFADELEMENTS 	1595	//130 possible due to Program memory, higher values possible if NROFTAELEMENTS is decreased
		#define TAEQUIDISTANCE	100		//dK
		#define ADEQUIDISTANCE	64		//dig
		#define ADEXPBITS		6		//2^ADEXPBITS=ADEQUIDISTANCE
		#define TABLEOFFSET		1024
		#define EQUIADTABLE		//if defined, ADELEMENTS have to be 2^N quantizied! else more CPU Power is needed
		#ifdef EQUIADTABLE
			#undef FLOATTABLE
		#endif   
		#define MBITTRIMDefault 0x2C
		#define SensRv 1
		#define EXPONENT 4.0//3.72781049994684
		#ifdef ADAPTCAPFOV
			#define FCPOW4 -0.0013105//-0.00187324
			#define FCPOW3 0.037154//0.05712135
			#define FCQUAD -0.36463//-0.5230756
			#define FCLIN 1.4831//1.490655
			#define FCOFF 118.43//119.143
			#define DELTATOCAP 0.015//0.07	//0.00005	//in K
			#define TA0TAYLOR 306.0		//in K
			#define DIVIDEFACTOR 0.0159185585611478
		#endif
		#define DEFINEDAPPSETOFFSET 0
		#define DEFINEDAPPSETGAIN -15
		#define DEFAULTGLOBALGAIN 10000
    #endif

    #ifdef HTPA32x32dL1_6HiGe
	 	#define TABLENUMBER		101	
		#define PCSCALEVAL		100000000		//PixelConst scale value for table... lower 'L' for (long)
		#define NROFTAELEMENTS 	7
		#define NROFADELEMENTS 	471		//possible due to Program memory, higher values possible if NROFTAELEMENTS is decreased
		#define TAEQUIDISTANCE	100		//dK
		#define ADEQUIDISTANCE	64		//dig
		#define ADEXPBITS		6		//2^ADEXPBITS=ADEQUIDISTANCE
		#define TABLEOFFSET		256
		#define EQUIADTABLE		//if defined, ADELEMENTS have to be 2^N quantizied! else more CPU Power is needed
		#ifdef EQUIADTABLE
			#undef FLOATTABLE
		#endif  
		#define MBITTRIMDefault 0x0C 
		#define SensRv 0
    #endif

    #ifdef HTPA32x32dR1L1_6HiGe_Gain3k3
	 	#define TABLENUMBER		119
		#define PCSCALEVAL		100000000 //327000000000		//PixelConst scale value for table... lower 'L' for (long)
		#define NROFTAELEMENTS 	7
		#define NROFADELEMENTS 	1595	//130 possible due to Program memory, higher values possible if NROFTAELEMENTS is decreased
		#define TAEQUIDISTANCE	100		//dK
		#define ADEQUIDISTANCE	64		//dig
		#define ADEXPBITS		6		//2^ADEXPBITS=ADEQUIDISTANCE
		#define TABLEOFFSET		1024
		#define EQUIADTABLE		//if defined, ADELEMENTS have to be 2^N quantizied! else more CPU Power is needed
		#ifdef EQUIADTABLE
			#undef FLOATTABLE
		#endif   
		#define MBITTRIMDefault 0x2C
		#define SensRv 1
		#define EXPONENT 4.0//3.72781049994684
		#ifdef ADAPTCAPFOV
			#define FCPOW4 -0.0013105//-0.00187324
			#define FCPOW3 0.037154//0.05712135
			#define FCQUAD -0.36463//-0.5230756
			#define FCLIN 1.4831//1.490655
			#define FCOFF 118.43//119.143
			#define DELTATOCAP 0.015//0.07		//in K
			#define TA0TAYLOR 306.0		//in K
			#define DIVIDEFACTOR 0.0159185585611478
		#endif
		#define DEFINEDAPPSETOFFSET 0		//tbd
		#define DEFINEDAPPSETGAIN -15			//tbd
		#define DEFAULTGLOBALGAIN 10000
    #endif

    #ifdef HTPA32x32dL2_1HiSi
	 	#define TABLENUMBER		80
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
		#define MBITTRIMDefault 0x0C
		#define SensRv 0
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
		#ifdef AUTOCHANGEGRADOFF
			#define AUTOCHANGEGADRVALUE -0.086641
			#define AUTOCHANGEOFFVALUE 241.4016
			#define DEFAULTGLOBALOFF 0
		#endif
		#define MBITTRIMDefault 0x0C
		#define SensRv 0
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
		#define MBITTRIMDefault 0x0C
		#define SensRv 0
    #endif

    #ifdef HTPA32x32dL3_6HiSi
	 	#define TABLENUMBER		81
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
		#define MBITTRIMDefault 0x0C
		#define SensRv 0
	#endif

    #ifdef HTPA32x32dL3_6HiSi_Rev1
	 	#define TABLENUMBER		106
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
		#define MBITTRIMDefault 0x0C
		#define SensRv 0
		#define EXPONENT 4.0//3.8731851756617
		#ifdef ADAPTCAPFOV
			#define FCPOW4 0.0
			#define FCPOW3 0.00025799
			#define FCQUAD -0.026976
			#define FCLIN 0.021503
			#define FCOFF 56.85
			#define DELTATOCAP 0.005		//in K
			#define TA0TAYLOR 306.0		//in K
			#define DIVIDEFACTOR 0.0159185585611478
		#endif
		#define DEFINEDAPPSETOFFSET 0		//tbd
		#define DEFINEDAPPSETGAIN 0			//tbd
		#define DEFAULTGLOBALGAIN 10000
	#endif

    #ifdef HTPA32x32dR1L3_6HiSi_Rev1_Gain3k3
	 	#define TABLENUMBER		117
		#define PCSCALEVAL		100000000		//PixelConst scale value for table... lower 'L' for (long)
		#define NROFTAELEMENTS 	7
		#define NROFADELEMENTS 	1595	//130 possible due to Program memory, higher values possible if NROFTAELEMENTS is decreased
		#define TAEQUIDISTANCE	100		//dK
		#define ADEQUIDISTANCE	64		//dig
		#define ADEXPBITS		6		//2^ADEXPBITS=ADEQUIDISTANCE
		#define TABLEOFFSET		1024
		#define EQUIADTABLE		//if defined, ADELEMENTS have to be 2^N quantizied! else more CPU Power is needed
		#ifdef EQUIADTABLE
			#undef FLOATTABLE
		#endif
		#define MBITTRIMDefault 0x2C
		#define SensRv 1
		#define EXPONENT 4.0//3.8731851756617
		#ifdef ADAPTCAPFOV
			#define FCPOW4 0.0
			#define FCPOW3 0.00025799
			#define FCQUAD -0.026976
			#define FCLIN 0.021503
			#define FCOFF 56.85
			#define DELTATOCAP 0.005		//in K
			#define TA0TAYLOR 306.0		//in K
			#define DIVIDEFACTOR 0.0159185585611478
		#endif
		#define DEFINEDAPPSETOFFSET 0		//tbd
		#define DEFINEDAPPSETGAIN -22			//tbd
		#define DEFAULTGLOBALGAIN 5100
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
		#define EXPONENT 4.0//3.8731851756617
		#ifdef ADAPTCAPFOV
			#define FCPOW4 0.0
			#define FCPOW3 0.000024288
			#define FCQUAD -0.0061591
			#define FCLIN 0.0021083
			#define FCOFF 46.394
			#define DELTATOCAP 0.21		//in K
			#define TA0TAYLOR 306.0		//in K
			#define DIVIDEFACTOR 0.0159185585611478
		#endif
		#define DEFINEDAPPSETOFFSET 0		//tbd
		#define DEFINEDAPPSETGAIN -20			//tbd
		#define DEFAULTGLOBALGAIN 10000
	#endif

    #ifdef HTPA32x32dL7_0HiSi
	 	#define TABLENUMBER		107
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
		#define MBITTRIMDefault 0x0C
		#define SensRv 0
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
		#define EQUIADTABLE		//if defined, ADELEMENTS have to be 2^N quantizied! else more CPU Power is needed
		#ifdef EQUIADTABLE 
			#undef FLOATTABLE
		#endif
		#define MBITTRIMDefault 0x0C
		#define SensRv 0
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
		#define EQUIADTABLE		//if defined, ADELEMENTS have to be 2^N quantizied! else more CPU Power is needed
		#ifdef EQUIADTABLE 
			#undef FLOATTABLE
		#endif
		#define MBITTRIMDefault 0x0C
		#define SensRv 0
    #endif
	
   #ifdef HTPA32x32dL1k8_0k7HiGe
	 	#define TABLENUMBER		111	
		#define PCSCALEVAL		100000000		//PixelConst scale value for table... lower 'L' for (long)
		#define NROFTAELEMENTS 	7
		#define NROFADELEMENTS 	471		//possible due to Program memory, higher values possible if NROFTAELEMENTS is decreased
		#define TAEQUIDISTANCE	100		//dK
		#define ADEQUIDISTANCE	64		//dig
		#define ADEXPBITS		6		//2^ADEXPBITS=ADEQUIDISTANCE
		#define TABLEOFFSET		256
		#define EQUIADTABLE		//if defined, ADELEMENTS have to be 2^N quantizied! else more CPU Power is needed
		#ifdef EQUIADTABLE
			#undef FLOATTABLE
		#endif   
		#define MBITTRIMDefault 0x0C
		#define SensRv 0
    #endif

    #ifdef HTPA32x32dR1L1k8_0k7HiGe
	 	#define TABLENUMBER		115	
		#define PCSCALEVAL		100000000		//PixelConst scale value for table... lower 'L' for (long)
		#define NROFTAELEMENTS 	7
		#define NROFADELEMENTS 	471		//possible due to Program memory, higher values possible if NROFTAELEMENTS is decreased
		#define TAEQUIDISTANCE	100		//dK
		#define ADEQUIDISTANCE	64		//dig
		#define ADEXPBITS		6		//2^ADEXPBITS=ADEQUIDISTANCE
		#define TABLEOFFSET		512
		#define EQUIADTABLE		//if defined, ADELEMENTS have to be 2^N quantizied! else more CPU Power is needed
		#ifdef EQUIADTABLE
			#undef FLOATTABLE
		#endif   
		#define MBITTRIMDefault 0x2C
		#define SensRv 1
    #endif

    #ifdef HTPA32x32dR1L1k8_0k7HiGe_Bodo
		#ifdef ADAPTCAPFOV
		    #define FIXEDCENTER         //ignores the calculated value for the chip center and replaces it with 15.5/15.5
		#endif
		#define WRITEPIXCINSTEADMASKING //writes a very low PixC instead of masking the PixC
	 	#define TABLENUMBER		115	
		#define PCSCALEVAL		100000000		//PixelConst scale value for table... lower 'L' for (long)
		#define NROFTAELEMENTS 	10
		#define NROFADELEMENTS 	471		//possible due to Program memory, higher values possible if NROFTAELEMENTS is decreased
		#define TAEQUIDISTANCE	100		//dK
		#define ADEQUIDISTANCE	64		//dig
		#define ADEXPBITS		6		//2^ADEXPBITS=ADEQUIDISTANCE
		#define TABLEOFFSET		1024
		#define EQUIADTABLE		//if defined, ADELEMENTS have to be 2^N quantizied! else more CPU Power is needed
		#ifdef EQUIADTABLE
			#undef FLOATTABLE
		#endif   
		#define EXPONENT 4.0//3.72781049994684
		#ifdef ADAPTCAPFOV
			#define FCPOW4 -0.000990456//-0.0013105
			#define FCPOW3 0.021952734//0.037154
			#define FCQUAD -0.137999129//-0.36463
			#define FCLIN -0.091222115//1.490655
			#define FCOFF 68.44848677//118.43
			#define DELTATOCAP 0.07		//in K
			#define TA0TAYLOR 306.0		//in K
			#define DIVIDEFACTOR 0.0159185585611478
		#endif
		#define DEFINEDAPPSETOFFSET 0		//tbd
		#define DEFINEDAPPSETGAIN 0			//tbd
		#define DEFAULTGLOBALGAIN 10000
		#define MBITTRIMDefault 0x2C
		#define SensRv 1
    #endif

    #ifdef HTPA32x32dL3_6HiSi_uncoated
	 	#define TABLENUMBER		112	
		#define PCSCALEVAL		100000000		//PixelConst scale value for table... lower 'L' for (long)
		#define NROFTAELEMENTS 	7
		#define NROFADELEMENTS 	471		//possible due to Program memory, higher values possible if NROFTAELEMENTS is decreased
		#define TAEQUIDISTANCE	100		//dK
		#define ADEQUIDISTANCE	64		//dig
		#define ADEXPBITS		6		//2^ADEXPBITS=ADEQUIDISTANCE
		#define TABLEOFFSET		256
		#define EQUIADTABLE		//if defined, ADELEMENTS have to be 2^N quantizied! else more CPU Power is needed
		#ifdef EQUIADTABLE
			#undef FLOATTABLE
		#endif   
		#define MBITTRIMDefault 0x0C
		#define SensRv 0
    #endif	

   #ifdef HTPA32x32dL5_0HiGeMult
	 	#define TABLENUMBER0		79
		#define PCSCALEVAL		100000000 //327000000000		//PixelConst scale value for table... lower 'L' for (long)
		#define NROFTAELEMENTS 	7
		#define NROFADELEMENTS 	251	//130 possible due to Program memory, higher values possible if NROFTAELEMENTS is decreased
		#define TAEQUIDISTANCE	100		//dK
		#define ADEQUIDISTANCE	128		//dig
		#define ADEXPBITS		7		//2^ADEXPBITS=ADEQUIDISTANCE
		#define TABLEOFFSET0	256
		#define EQUIADTABLE		//if defined, ADELEMENTS have to be 2^N quantizied! else more CPU Power is needed
		#ifdef EQUIADTABLE
			#undef FLOATTABLE
		#endif   
		#define MBITTRIMDefault0 0x0C
		#define SensRv0 0
		#define DEFAULTGLOBALGAIN 10000
    #endif

    #ifdef HTPA32x32dL2_1HiSiMult
	 	#define TABLENUMBER1		80
		#define TABLEOFFSET1	256
		#define MBITTRIMDefault1 0x0C
		#define SensRv1 0
    #endif

    #ifdef HTPA32x32dL2_1HiSiF5Mult
	 	#define TABLENUMBER1		96
		#define TABLEOFFSET1	256
		#define MBITTRIMDefault1 0x0C
		#define SensRv1 0
    #endif

    #ifdef HTPA32x32dL3_6HiSiMult
	 	#define TABLENUMBER2		106
		#define TABLEOFFSET2	256
		#define MBITTRIMDefault2 0x0C
		#define SensRv2 0
	#endif

    #ifdef HTPA32x32dL7_0HiSiMult
	 	#define TABLENUMBER3		107
		#define TABLEOFFSET3	256
		#define MBITTRIMDefault3 0x0C
		#define SensRv3 0
	#endif

	#ifdef HTPA32x32dL5_0HiGeF7_7Mult
		#define TABLENUMBER4	92
		#define TABLEOFFSET4	256		
		#define MBITTRIMDefault4 0x0C
		#define SensRv4 0
	#endif

	#ifdef HTPA32x32dL1_6HiGe_Mult
		#define TABLENUMBER5	101
		#define TABLEOFFSET5	256
		#define MBITTRIMDefault5 0x0C
		#define SensRv5 0
	#endif

	#ifdef HTPA32x32dR1L2_1HiSiF5_0_Gain3k3Mult
		#define TABLENUMBER6	114
		#define TABLEOFFSET6	1024
		#define MBITTRIMDefault6 0x2C
		#define SensRv6 1
	#endif

	#ifdef HTPA32x32dR1L5_0HiGeF7_7_Gain3k3Mult
		#define TABLENUMBER7	113
		#define TABLEOFFSET7	1024
		#define MBITTRIMDefault7 0x2C
		#define SensRv7 1
	#endif

	#ifdef HTPA32x32dR1L3_6HiSi_Rev1_Gain3k3Mult
		#define TABLENUMBER8	117
		#define TABLEOFFSET8	1024
		#define MBITTRIMDefault8 0x2C
		#define SensRv8 1
	#endif

	#ifdef HTPA32x32dR1L7_0HiSi_Gain3k3Mult
		#define TABLENUMBER9	118
		#define TABLEOFFSET9	640
		#define MBITTRIMDefault9 0x2C
		#define SensRv9 1
	#endif

	#ifdef HTPA32x32dR1L1_6HiGe_Gain3k3Mult
		#define TABLENUMBER10	119
		#define TABLEOFFSET10	1024
		#define MBITTRIMDefault10 0x2C
		#define SensRv10 1
	#endif
#endif


#ifdef PCBR4
    #define ENCRESETDESIGN
    #ifdef ENCRESETDESIGN
        #define ENCRESETPIN LATCbits.LATC1 //das muss natürlich auf deinen Pin gematched sein!
    #endif
#endif  


