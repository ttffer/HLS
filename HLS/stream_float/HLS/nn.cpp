#include "r.h"
#include "ap_int.h"
#include "hls_stream.h"

void nn(float img[784],
		float lrsw0[lrsw0_size],
		float lrsb0[lrsb0_size],
		float lrsw2[lrsw2_size],
		float lrsb2[lrsb2_size],
		float lrsw4[lrsw4_size],
		float lrsb4[lrsb4_size],
		float outcome[10]

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

	nn_label2:for (int i=0;i<lrsb2_size;i++){
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
	//reset out buffer
	for (int i=0;i<10;i++){
		outcome[i]=0;
	}
	for (int i=0;i<10;i++){
		nn_label1:for(int j=0;j<lrsb2_size;j++){
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



void top(hls::stream<axis_t> &in, hls::stream<axis_t> &out){
#pragma HLS INTERFACE s_axilite port = return bundle = control
#pragma HLS INTERFACE axis port = in
#pragma HLS INTERFACE axis port = out

	DataType b_img[img_size];
	DataType b_out[10];
	converter_t converter;

	for(int i=0;i<img_size;i++){
		axis_t temp= in.read();
		converter.i=temp.data; // convert int to float
		b_img[i]=converter.d;
	}


	nn(b_img,lrsw0,lrsb0,lrsw2,lrsb2,lrsw4,lrsb4,b_out);

	for(int i=0;i<N;i++){
		axis_t temp;
		converter.d=b_out[i]; //float
		temp.data=converter.i; //int
		bool last=0;
		if(i==N-1){
			last=1;
		}

		temp.last=last;
		temp.keep=-1;
		out.write(temp);

	}
}

