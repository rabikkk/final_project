//---------------------------------------------------------------------------


#pragma hdrstop

#include "Unit1.h"
#include "IIR_Filter.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
#ifdef BESSELIIR
static double iir_tapsA[IIRFILTER_TAP_NUM+1] = {
		0.35890242532894062000,
		0.71780485065788124000,
		0.35890242532894062000
};
static double iir_tapsB[IIRFILTER_TAP_NUM+1] = {
		1.00000000000000000000,
		0.33890147866868908000,
		0.09670822264707341900
};

void IIRFilter_init(IIRFilter* f) {
  int i;
  for(i = 0; i < IIRFILTER_TAP_NUM+1; ++i){
	f->x[i] = 0;
	f->y[i] = 0;
	}
}

double IIRFilter_putget(IIRFilter* f, double input) {
  unsigned int i;

	for(i=IIRFILTER_TAP_NUM; i>0; i--) {      //shift them
	   f->x[i] = f->x[i-1];
	   f->y[i] = f->y[i-1];
	}
  f->x[0] = input;
  f->y[0] = iir_tapsA[0]*f->x[0];

  for(i=1; i<=IIRFILTER_TAP_NUM; i++)
	f->y[0] += iir_tapsA[i] * f->x[i] - iir_tapsB[i] * f->y[i];
  return f->y[0];
  }
#endif


