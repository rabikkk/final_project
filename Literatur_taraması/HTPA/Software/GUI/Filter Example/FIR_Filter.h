//---------------------------------------------------------------------------

#ifndef FIR_FilterH
#define FIR_FilterH
//---------------------------------------------------------------------------

#ifdef BESSELFIR


#define SAMPLEFILTER_TAP_NUM 7

typedef struct {
  double history[SAMPLEFILTER_TAP_NUM];
  unsigned int last_index;
} SampleFilter;

void SampleFilter_init(SampleFilter* f);
void SampleFilter_put(SampleFilter* f, double input);
double SampleFilter_get(SampleFilter* f);
#endif

#endif
