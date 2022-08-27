#ifndef _INT_STREAM_H_
#define _INT_STREAM_H_

#include "hls_stream.h"
#include "ap_axi_sdata.h"
#include "ap_int.h"
#include <inttypes.h>
typedef int  DataType;
typedef ap_axiu<32, 0, 0, 0> axis_t;

#define lrsw0_size 50176
#define lrsb0_size 64
#define lrsw2_size 2048
#define lrsb2_size 32
#define lrsw4_size 320
#define lrsb4_size 10

#define img_size 784
#define N 10
extern int lrsb0 [];
extern int lrsb2 [];
extern int lrsb4 [];
extern int lrsw0 [];
extern int lrsw2 [];
extern int lrsw4 [];

void top(hls::stream<axis_t> &in, hls::stream<axis_t> &out);
void nn(int img[784],
		int lrsw0[lrsw0_size],
		int lrsb0[lrsb0_size],
		int lrsw2[lrsw2_size],
		int lrsb2[lrsb2_size],
		int lrsw4[lrsw4_size],
		int lrsb4[lrsb4_size],
		int outcome[10]);
#endif
