#ifndef _R_H_
#define _R_H_

#include "hls_stream.h"
#include "ap_axi_sdata.h"
#include "ap_int.h"
#include <inttypes.h>
typedef float  DataType;


typedef ap_axiu<32, 0, 0, 0> axis_t; // must use ap_axis or ap_axiu to infer TLAST

#define lrsw0_size 50176
#define lrsb0_size 64
#define lrsw2_size 2048
#define lrsb2_size 32
#define lrsw4_size 320
#define lrsb4_size 10

#define img_size 784
#define N 10
extern float lrsb0 [];
extern float lrsb2 [];
extern float lrsb4 [];
extern float lrsw0 [];
extern float lrsw2 [];
extern float lrsw4 [];
// use a union to "convert" between integer and floating-point
typedef union converter {
  DataType d;// floating-point alias
  int i;// integer alias
} converter_t;
// reference https://support.xilinx.com/s/question/0D52E00007DnHxuSAF/streaming-floats-with-tlast?language=en_US

void top(hls::stream<axis_t> &in, hls::stream<axis_t> &out);
void nn(float img[784],
		float lrsw0[lrsw0_size],
		float lrsb0[lrsb0_size],
		float lrsw2[lrsw2_size],
		float lrsb2[lrsb2_size],
		float lrsw4[lrsw4_size],
		float lrsb4[lrsb4_size],
		float outcome[10]);
#endif
