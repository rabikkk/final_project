#include "Globals.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef ReadToFromTable
unsigned int calcTO(unsigned int TAmb, signed int dig, signed long PiC, unsigned int dontCalcTA){
unsigned int Tobject,y;
signed int val;
signed long vx,vy,ydist;
static unsigned int CurTACol;
static signed int dTA;
signed long long scale;
#ifdef FLOATTABLE
double val2,val3,calc;
#endif

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
#ifndef FLOATTABLE
	scale=(signed long long)PCSCALEVAL*(signed long long)dig;
	vx=(signed long)(scale/((signed long long)PiC));
#else
	val2=(double)PCSCALEVAL*(double)dig/(double)PiC+0.5;
	vx=(signed long)val2; //only decimal places are lost. good enough for finding spot in table
#endif

#ifdef EQUIADTABLE
	val=vx+sTABLEOFFSET;
	//now determine row
	y=val>>ADEXPBITS;
	ydist=(signed long)ADEQUIDISTANCE;
#else
	val=vx;
	for(y=0;y<NROFADELEMENTS-1;y++){
		if((YADValues[y]<=val)&&(YADValues[y+1]>val))
			break;
		}
	if(y<NROFADELEMENTS-1)
		ydist=YADValues[y+1]-YADValues[y];
#endif
if(y<(NROFADELEMENTS-1)){
	if(TempTable[y][CurTACol]){
#ifdef Multitable
		vx=((((signed long)TempTable[TableToUse][y][CurTACol+1]-(signed long)TempTable[TableToUse][y][CurTACol])*(signed long)dTA)/(signed long)TAEQUIDISTANCE)+(signed long)TempTable[TableToUse][y][CurTACol];
		vy=((((signed long)TempTable[TableToUse][y+1][CurTACol+1]-(signed long)TempTable[TableToUse][y+1][CurTACol])*(signed long)dTA)/(signed long)TAEQUIDISTANCE)+(signed long)TempTable[TableToUse][y+1][CurTACol];
#else
		vx=((((signed long)TempTable[y][CurTACol+1]-(signed long)TempTable[y][CurTACol])*(signed long)dTA)/(signed long)TAEQUIDISTANCE)+(signed long)TempTable[y][CurTACol];
		vy=((((signed long)TempTable[y+1][CurTACol+1]-(signed long)TempTable[y+1][CurTACol])*(signed long)dTA)/(signed long)TAEQUIDISTANCE)+(signed long)TempTable[y+1][CurTACol];
#endif
	#ifndef FLOATTABLE
		Tobject=(unsigned int)((vy-vx)*((signed long)val-(signed long)YADValues[y])/ydist+(signed long)vx);
	#else
		calc=(vy-vx);
		val3=val2-(double)YADValues[y];
		val2=calc*val3;
		Tobject=(unsigned int)(val2/ydist+vx);
	#endif
		}
	else
		return 0;
	}
else
	return 0;

return (unsigned short)Tobject;
}
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////
