#ifndef __ALGO_H
#define __ALGO_H
#include "sys.h"
#define BUFFER_SIZE 100
#define rdt 0.035
#define rdi 0.1

typedef struct IIR_Filter_struct{		//struct for IIR filter
	float avg;
	float lst_avg;
}IIR_Struct;

typedef struct  D_I_struct{
	float lst_x;
	float integ;
	float lst_integ;
	float deriv;
	float DT;	//derivative threshold
	float IT;	//intergration threshold
}DI_Struct;

typedef struct Naive_bayesian_struct{
	float buffer[BUFFER_SIZE];
	float average;
	float variance;
}BAY_Struct;

extern IIR_Struct IIRS[8];
extern DI_Struct DIS[8];

float IIR_Filter_Init(IIR_Struct* iirfilter, float firstval);
float IIR_Filter_cal(IIR_Struct* iirfilter, float val);
float D_I_Init(DI_Struct* di, float firstval, float dth, float ith);
float D_I_cal(DI_Struct* di, float x);
void filter_init(void);
//void train(BAY_Struct* parameter, float* data);
#endif


