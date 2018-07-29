#include <algo.h>
#define N 4
#define L 0.82	//0.82	//leakage factor


IIR_Struct IIRS[8];
DI_Struct DIS[8];

float IIR_Filter_Init(IIR_Struct* iirfilter, float firstval)		//Initalize IIR filter
{
	iirfilter->avg = 0;
	iirfilter->lst_avg = firstval;
	return firstval;
}

float IIR_Filter_cal(IIR_Struct* iirfilter, float val)
{
	iirfilter->avg = (iirfilter->lst_avg * N  - iirfilter->lst_avg + val) / N;
	iirfilter->lst_avg = iirfilter->avg;
	return iirfilter->avg;
}

float D_I_Init(DI_Struct* di, float firstval, float dth, float ith)
{
	di->lst_x = firstval;
	di->lst_integ = firstval;
	di->DT = dth;
	di->IT = ith;
	return firstval;
}

float D_I_cal(DI_Struct* di, float x)
{
	di->deriv = x - di->lst_x;
//	if(fabs(di->deriv) >= di->DT)
//	{
//		di->DT = fabs(di->deriv);
//	}
	if(fabs(di->deriv) >= di->DT)
	{
		di->integ = di->lst_integ + di->deriv;
	}
	else
	{
		di->integ = di->lst_integ;
	}
	/*process the integ below here to figure out whether there is an object*/
	if(di->integ >= di->IT)
	{
		di->lst_integ = di->integ;
	}
	else
	{
		di->lst_integ = di->integ * L;
	}
	di->lst_x = x;
	return di->integ;
}

void filter_init(void)
{
		D_I_Init(&DIS[0], IIR_Filter_Init(&IIRS[0], 0),  0.0015, 0.3);			//响应过慢 适当减小dth；0.023
		D_I_Init(&DIS[1], IIR_Filter_Init(&IIRS[1], 0),  0.001, 0.2);			//抖动过大 保证响应正常的前提下适当增加dth 
		D_I_Init(&DIS[2], IIR_Filter_Init(&IIRS[2], 0),  0.002, 0.3);			//无法归零 增大ith，但ith值应小于最小的有效值
		D_I_Init(&DIS[3], IIR_Filter_Init(&IIRS[3], 0),  0.003, 0.2);
		D_I_Init(&DIS[4], IIR_Filter_Init(&IIRS[4], 0), 0.002, 0.15);
		D_I_Init(&DIS[5], IIR_Filter_Init(&IIRS[5], 0),  0.003, 0.2);
		D_I_Init(&DIS[6], IIR_Filter_Init(&IIRS[6], 0),  0.003, 0.2);
		D_I_Init(&DIS[7], IIR_Filter_Init(&IIRS[7], 0),  0.003, 0.2);
}

//void train(BAY_Struct* parameter, float* data)
//{
//	int index = 0, diff = 0;
//	parameter->average = 0;
//	parameter->variance = 0;
//	for(index = 0; index < BUFFER_SIZE; index++)
//	{
//		parameter->average += data[index];
//	}
//	parameter->average = parameter->average / BUFFER_SIZE;
//	for(index = 0; index < BUFFER_SIZE; index++)
//	{
//		diff = data[index] - parameter->average;
//		parameter->variance += diff * diff;
//	}
//}

