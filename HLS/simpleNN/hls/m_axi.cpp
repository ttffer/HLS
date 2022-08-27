#include <stdio.h>
#include <string.h>
#define lrsw0_size 50176
#define lrsb0_size 64
#define lrsw2_size 2048
#define lrsb2_size 32
#define lrsw4_size 320
#define lrsb4_size 10

void nn(float img[784],
		float lrsw0[lrsw0_size],
		float lrsb0[lrsb0_size],
		float lrsw2[lrsw2_size],
		float lrsb2[lrsb2_size],
		float lrsw4[lrsw4_size],
		float lrsb4[lrsb4_size],
		float outcome[10],
		float lrsb0_out[lrsb0_size],
		float lrsb2_out[lrsb2_size]
		){
	float out1[lrsb0_size]={0};
	float out2[lrsb2_size]={0};

	for (int i =0;i<lrsb0_size;i++){
		for (int j=0;j<784;j++){
			out1[i]+=lrsw0[i*784+j]*img[j];
		}
		out1[i]+=lrsb0[i];
	}
	for (int i =0;i<lrsb0_size;i++){
		if(out1[i]<0){
			out1[i]=0;
		}
	}
/////////////////
	for (int i =0;i<lrsb0_size;i++){
		lrsb0_out[i]=out1[i];
	}
/////////////////
	for (int i=0;i<lrsb2_size;i++){
		for (int j=0;j<lrsb0_size;j++){
			out2[i]+=lrsw2[i*lrsb0_size+j]*out1[j];
		}
		out2[i]+=lrsb2[i];
	}
	for(int i=0;i<lrsb2_size;i++){
		if(out2[i]<0){
			out2[i]=0;
		}
	}
//////////////////
	for (int i =0;i<lrsb2_size;i++){
		lrsb2_out[i]=out2[i];
	}
/////////////////
	//reset out buffer
	for (int i=0;i<10;i++){
		outcome[i]=0;
	}
	for (int i=0;i<10;i++){
		for(int j=0;j<lrsb2_size;j++){
			outcome[i]+=lrsw4[i*lrsb2_size+j]*out2[j];
		}
		outcome[i]+=lrsb4[i];
	}
	//reset accumulator
		for (int i =0;i<lrsb0_size;i++){
			out1[i]=0;
		}
	for(int i=0;i<lrsb2_size;i++){
		out2[i]=0;
	}
}


void example(volatile float *img,
		volatile float *lrsw0,
		volatile float *lrsb0,
		volatile float *lrsw2,
		volatile float *lrsb2,
		volatile float *lrsw4,
		volatile float *lrsb4,
		volatile float *outcome,

		volatile float *lrsb0_out,
		volatile float *lrsb2_out,

		int lrsw0_len,
		int lrsb0_len,
		int lrsw2_len,
		int lrsb2_len,
		int lrsw4_len,
		int lrsb4_len,
		int outcome_len,
		int img_len
		){

#pragma HLS INTERFACE m_axi port=img depth=16 offset=slave

#pragma HLS INTERFACE m_axi port=lrsw0 depth=16 offset=slave
#pragma HLS INTERFACE m_axi port=lrsb0 depth=16 offset=slave
#pragma HLS INTERFACE m_axi port=lrsw2 depth=16 offset=slave
#pragma HLS INTERFACE m_axi port=lrsb2 depth=10 offset=slave
#pragma HLS INTERFACE m_axi port=lrsw4 depth=16 offset=slave
#pragma HLS INTERFACE m_axi port=lrsb4 depth=16 offset=slave

//////////////////
#pragma HLS INTERFACE m_axi port=lrsb0_out depth=16 offset=slave
#pragma HLS INTERFACE m_axi port=lrsb2_out depth=16 offset=slave
/////////////////

#pragma HLS INTERFACE m_axi port=outcome depth=10 offset=slave

#pragma HLS INTERFACE s_axilite port=img_len

#pragma HLS INTERFACE s_axilite port=lrsw0_len
#pragma HLS INTERFACE s_axilite port=lrsb0_len
#pragma HLS INTERFACE s_axilite port=lrsw2_len
#pragma HLS INTERFACE s_axilite port=lrsb2_len
#pragma HLS INTERFACE s_axilite port=lrsw4_len
#pragma HLS INTERFACE s_axilite port=lrsb4_len

#pragma HLS INTERFACE s_axilite port=outcome_len

#pragma HLS INTERFACE s_axilite port=return

	int i;

	float buffer_img[784];

	float buffer_lrsw0[lrsw0_size];
	float buffer_lrsb0[lrsb0_size];
	float buffer_lrsw2[lrsw2_size];
	float buffer_lrsb2[lrsb2_size];
	float buffer_lrsw4[lrsw4_size];
	float buffer_lrsb4[lrsb4_size];

	float buffer_lrsb2_out[lrsb0_size];
	float buffer_lrsb4_out[lrsb2_size];


	float buffer_outcome[10];
  //memcpy creates a burst access to memory
  //multiple calls of memcpy cannot be pipelined and will be scheduled sequentially
  //memcpy requires a local buffer to store the results of the memory transaction

  memcpy(buffer_img,(const float*)img,img_len*sizeof(float));

  memcpy(buffer_lrsw0,(const float*)lrsw0,lrsw0_len*sizeof(float));
  memcpy(buffer_lrsb0,(const float*)lrsb0,lrsb0_len*sizeof(float));
  memcpy(buffer_lrsw2,(const float*)lrsw2,lrsw2_len*sizeof(float));
  memcpy(buffer_lrsb2,(const float*)lrsb2,lrsb2_len*sizeof(float));
  memcpy(buffer_lrsw4,(const float*)lrsw4,lrsw4_len*sizeof(float));
  memcpy(buffer_lrsb4,(const float*)lrsb4,lrsb4_len*sizeof(float));




  nn(buffer_img,buffer_lrsw0,buffer_lrsb0,buffer_lrsw2,buffer_lrsb2,buffer_lrsw4,buffer_lrsb4,buffer_outcome,buffer_lrsb2_out,buffer_lrsb4_out);

  memcpy((float *)lrsb0_out,buffer_lrsb2_out,lrsb0_len*sizeof(float));
  memcpy((float *)lrsb2_out,buffer_lrsb4_out,lrsb2_len*sizeof(float));

  memcpy((float *)outcome,buffer_outcome,outcome_len*sizeof(float));
}
