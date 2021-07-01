//---------------------------------------------------------------------------


#pragma hdrstop

#include "Unit1.h"
#include "FIR_Filter.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

#ifdef BESSELFIR
static double filter_taps[SAMPLEFILTER_TAP_NUM] = {
		0.00549501218579605350,
		-0.06131611153788362600,
		0.24622474695046576000,
		0.61919270480324340000,
        0.24622474695046576000,
		-0.06131611153788362600,
		0.00549501218579605350
};

void SampleFilter_init(SampleFilter* f) {
  int i;
  for(i = 0; i < SAMPLEFILTER_TAP_NUM; ++i)
    f->history[i] = 0;
  f->last_index = 0;
}

void SampleFilter_put(SampleFilter* f, double input) {
  f->history[f->last_index++] = input;
  if(f->last_index == SAMPLEFILTER_TAP_NUM)
	f->last_index = 0;
}


double SampleFilter_get(SampleFilter* f) {
  double acc = 0;
  int index = f->last_index, i;
  for(i = 0; i < SAMPLEFILTER_TAP_NUM; ++i) {
	index = index != 0 ? index-1 : SAMPLEFILTER_TAP_NUM-1;
	acc += f->history[index] * filter_taps[i];
  };
  return acc;
}

#endif


