#define AdrPixCMin 0x00
#define AdrPixCMax 0x04
#define AdrGradScale 0x08
#define AdrTableNumber 0x0B 		//changed to 0x0B with Rev 0.14 and adpated TN readout
#define AdrEpsilon 0x0D

#define AdrMBITPixC 0x1A
#define AdrBIASPixC 0x1B
#define AdrCLKPixC 0x1C
#define AdrBPAPixC 0x1D
#define AdrPUPixC 0x1E

#define AdrPTATGrad 0x34

#define AdrVddScaling 0x4E
#define AdrVddScalingOff 0x4F

#define AdrVddMeasTh1 0x26
#define AdrPTATTh1 0x3C

#define AdrMBITUser 0x60
#define AdrBIASUser 0x61
#define AdrCLKUser 0x62
#define AdrBPAUser 0x63
#define AdrPUUser 0x64

#define AdrDevID 0x74

#define AdrGlobalOffset 0x54
#define AdrGlobalGain 0x55

#define AdrNrOfDefPix 0x7F
#define AdrDeadPix 0x80		//currently reserved space for 24 Pixel
#define AdrDeadPixMask 0xB0	//currently reserved space for 24 Pixel
#define AdrVddCompValues2 0x800			//16 bit signed
#define AdrVddCompValues 0x1200			//16 bit signed
#define AdrTh1 0x1c00					//Gradient as 8 bit signed
#define AdrTh2 0x3000
#define AdrPixC 0x5800

#define BIAScurrentDefault 0x05
#define CLKTRIMDefault 0x15 //0x20 to let it run with 10 Hz
#define BPATRIMDefault 0x0C
#define MBITTRIMDefault 0x1C
#define PUTRIMDefault	0x88


//pixelcount etc. for 80x64d
#define Pixel 5120				//=80x64
#define PixelEighth 640
#define ROW 64
#define COLUMN 80
#define DATALength 1282			//lenght of each packet, without the first Byte for indication
#define DATALength2 1282		//lenght of second/last packet
#define AmountOfPackets 10
#define DataLengthHalf 641
#define PTATamount 8
#define ELOFFSET 5120			//start address of el. Offset
#define Pixelforth 1280
#define ELAMOUNT 1280
#define ELAMOUNTHALF 640
#define PTATSTARTADSRESS 6402
#define VDDADDRESS 6400

#define GetElEveryFrameX 10		//amount of normal frames to capture after which the el. Offset is fetched
#define StackSize 10
#define STACKSIZEPTAT 30		//should be an even number
#define STACKSIZEVDD 50			//should be an even number
#define VddStackAmount 30

#define EEPROM_PAGE_MASK    (unsigned)0x003f
#define EEPROM_CMD_READ     (unsigned)0b00000011
#define EEPROM_CMD_WRITE    (unsigned)0b00000010
#define EEPROM_CMD_WRDI     (unsigned)0b00000100
#define EEPROM_CMD_WREN     (unsigned)0b00000110
#define EEPROM_CMD_RDSR     (unsigned)0b00000101
#define EEPROM_CMD_WRSR     (unsigned)0b00000001

#define ReadToFromTable
#ifdef ReadToFromTable
	//#define HTPA80x64dL3k9_0k8Hi_Gain3k3
	//#define HTPA80x64dL5k0_0k95Hi_Gain3k3
	//#define HTPA80x64dL5k0_0k95F8_14Hi_Gain3k3
	//#define HTPA80x64dL10_0k7F7k7Hi_Gain3k3
	//#define HTPA80x64dL10k5_0k95F7k7Hi_Gain3k3
	//#define HTPA80x64dL11_1k0F7k7HiSi_Gain3k3		//Table is different from previous one
	//#define HTPA80x64dL22k5_1k0Hi_Gain3k3
	#define HTPA80x64dL22k5_1k0N2_Gain3k3
	//special vesions:
	#define HTPA80x64dL10k5_0k95F7k7Hi_Gain3k3_TaExtended

   #ifdef HTPA80x64dL3k9_0k8Hi_Gain3k3
	 	#define TABLENUMBER		129
		#define PCSCALEVAL		100000000 //327000000000		//PixelConst scale value for table... lower 'L' for (long)
		#define NROFTAELEMENTS 	17
		#define NROFADELEMENTS 	1595	//130 possible due to Program memory, higher values possible if NROFTAELEMENTS is decreased
		#define TAEQUIDISTANCE	64		//dK
		#define ADEQUIDISTANCE	64		//dig
		#define ADEXPBITS		6		//2^ADEXPBITS=ADEQUIDISTANCE
		#define TABLEOFFSET		1024
		#define EQUIADTABLE		//if defined, ADELEMENTS have to be 2^N quantizied! else more CPU Power is needed
		#define TAEQUIDIST		//Ta must the be 2^N quantized!
		#define TADISTEXP		6
		#ifdef EQUIADTABLE
			#undef FLOATTABLE
		#endif     
    #endif	
	
    #ifdef HTPA80x64dL5k0_0k95Hi_Gain3k3
	 	#define TABLENUMBER		123
		#define PCSCALEVAL		100000000 //327000000000		//PixelConst scale value for table... lower 'L' for (long)
		#define NROFTAELEMENTS 	11
		#define NROFADELEMENTS 	1595	//130 possible due to Program memory, higher values possible if NROFTAELEMENTS is decreased
		#define TAEQUIDISTANCE	100		//dK
		#define ADEQUIDISTANCE	64		//dig
		#define ADEXPBITS		6		//2^ADEXPBITS=ADEQUIDISTANCE
		#define TABLEOFFSET		1024
		#define EQUIADTABLE		//if defined, ADELEMENTS have to be 2^N quantizied! else more CPU Power is needed
		#ifdef EQUIADTABLE
			#undef FLOATTABLE
		#endif   
    #endif
	
   #ifdef HTPA80x64dL5k0_0k95F8_14Hi_Gain3k3
	 	#define TABLENUMBER		133
		#define PCSCALEVAL		100000000 //327000000000		//PixelConst scale value for table... lower 'L' for (long)
		#define NROFTAELEMENTS 	17
		#define NROFADELEMENTS 	1595	//130 possible due to Program memory, higher values possible if NROFTAELEMENTS is decreased
		#define TAEQUIDISTANCE	64		//dK
		#define ADEQUIDISTANCE	64		//dig
		#define ADEXPBITS		6		//2^ADEXPBITS=ADEQUIDISTANCE
		#define TABLEOFFSET		1024
		#define EQUIADTABLE		//if defined, ADELEMENTS have to be 2^N quantizied! else more CPU Power is needed
		#define TAEQUIDIST		//Ta must the be 2^N quantized!
		#define TADISTEXP		6
		#ifdef EQUIADTABLE
			#undef FLOATTABLE
		#endif  
    #endif	

    #ifdef HTPA80x64dL10_0k7F7k7Hi_Gain3k3
	 	#define TABLENUMBER		124
		#define PCSCALEVAL		100000000 //327000000000		//PixelConst scale value for table... lower 'L' for (long)
		#define NROFTAELEMENTS 	11
		#define NROFADELEMENTS 	1595	//130 possible due to Program memory, higher values possible if NROFTAELEMENTS is decreased
		#define TAEQUIDISTANCE	100		//dK
		#define ADEQUIDISTANCE	64		//dig
		#define ADEXPBITS		6		//2^ADEXPBITS=ADEQUIDISTANCE
		#define TABLEOFFSET		1024
		#define EQUIADTABLE		//if defined, ADELEMENTS have to be 2^N quantizied! else more CPU Power is needed
		#ifdef EQUIADTABLE
			#undef FLOATTABLE
		#endif   
    #endif

    #ifdef HTPA80x64dL10k5_0k95F7k7Hi_Gain3k3
	 	#define TABLENUMBER		125
		#define PCSCALEVAL		100000000 //327000000000		//PixelConst scale value for table... lower 'L' for (long)
		#define NROFTAELEMENTS 	11
		#define NROFADELEMENTS 	1595	//130 possible due to Program memory, higher values possible if NROFTAELEMENTS is decreased
		#define TAEQUIDISTANCE	100		//dK
		#define ADEQUIDISTANCE	64		//dig
		#define ADEXPBITS		6		//2^ADEXPBITS=ADEQUIDISTANCE
		#define TABLEOFFSET		1024
		#define EQUIADTABLE		//if defined, ADELEMENTS have to be 2^N quantizied! else more CPU Power is needed
		#ifdef EQUIADTABLE
			#undef FLOATTABLE
		#endif   
    #endif
	
    #ifdef HTPA80x64dL10k5_0k95F7k7Hi_Gain3k3_TaExtended
	 	#define TABLENUMBER		125
		#define PCSCALEVAL		100000000 //327000000000		//PixelConst scale value for table... lower 'L' for (long)
		#define NROFTAELEMENTS 	12
		#define NROFADELEMENTS 	1595	//130 possible due to Program memory, higher values possible if NROFTAELEMENTS is decreased
		#define TAEQUIDISTANCE	100		//dK
		#define ADEQUIDISTANCE	64		//dig
		#define ADEXPBITS		6		//2^ADEXPBITS=ADEQUIDISTANCE
		#define TABLEOFFSET		1024
		#define EQUIADTABLE		//if defined, ADELEMENTS have to be 2^N quantizied! else more CPU Power is needed
		#ifdef EQUIADTABLE
			#undef FLOATTABLE
		#endif   
    #endif	
	
    #ifdef HTPA80x64dL11_1k0F7k7HiSi_Gain3k3
	 	#define TABLENUMBER		126
		#define PCSCALEVAL		100000000 //327000000000		//PixelConst scale value for table... lower 'L' for (long)
		#define NROFTAELEMENTS 	11
		#define NROFADELEMENTS 	1595	//130 possible due to Program memory, higher values possible if NROFTAELEMENTS is decreased
		#define TAEQUIDISTANCE	100		//dK
		#define ADEQUIDISTANCE	64		//dig
		#define ADEXPBITS		6		//2^ADEXPBITS=ADEQUIDISTANCE
		#define TABLEOFFSET		640
		#define EQUIADTABLE		//if defined, ADELEMENTS have to be 2^N quantizied! else more CPU Power is needed
		#ifdef EQUIADTABLE
			#undef FLOATTABLE
		#endif   
    #endif	

    #ifdef HTPA80x64dL22k5_1k0Hi_Gain3k3
	 	#define TABLENUMBER		122
		#define PCSCALEVAL		100000000 //327000000000		//PixelConst scale value for table... lower 'L' for (long)
		#define NROFTAELEMENTS 	11
		#define NROFADELEMENTS 	1595	//130 possible due to Program memory, higher values possible if NROFTAELEMENTS is decreased
		#define TAEQUIDISTANCE	100		//dK
		#define ADEQUIDISTANCE	64		//dig
		#define ADEXPBITS		6		//2^ADEXPBITS=ADEQUIDISTANCE
		#define TABLEOFFSET		1024
		#define EQUIADTABLE		//if defined, ADELEMENTS have to be 2^N quantizied! else more CPU Power is needed
		#ifdef EQUIADTABLE
			#undef FLOATTABLE
		#endif   
    #endif
	
    #ifdef HTPA80x64dL22k5_1k0N2_Gain3k3
	 	#define TABLENUMBER		132
		#define PCSCALEVAL		100000000 //327000000000		//PixelConst scale value for table... lower 'L' for (long)
		#define NROFTAELEMENTS 	11
		#define NROFADELEMENTS 	1595	//130 possible due to Program memory, higher values possible if NROFTAELEMENTS is decreased
		#define TAEQUIDISTANCE	100		//dK
		#define ADEQUIDISTANCE	64		//dig
		#define ADEXPBITS		6		//2^ADEXPBITS=ADEQUIDISTANCE
		#define TABLEOFFSET		320
		#define EQUIADTABLE		//if defined, ADELEMENTS have to be 2^N quantizied! else more CPU Power is needed
		#ifdef EQUIADTABLE
			#undef FLOATTABLE
		#endif   
    #endif	
 
#endif
