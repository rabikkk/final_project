//---------------------------------------------------------------------------

#ifndef IIR_FilterH
#define IIR_FilterH
//---------------------------------------------------------------------------
#ifdef BESSELIIR


#define IIRFILTER_TAP_NUM 2

typedef struct {
  double y[IIRFILTER_TAP_NUM+1];     //output samples
  double x[IIRFILTER_TAP_NUM+1];	//input smaples
} IIRFilter;

void IIRFilter_init(IIRFilter* f);
double IIRFilter_putget(IIRFilter* f, double input);
#endif



#endif

