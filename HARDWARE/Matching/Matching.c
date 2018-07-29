#include "Matching.h"
#include "sys.h"
#include "usart.h"
#include <string.h>
#include <math.h>
/*@Mode: 
*0x01 剪刀
*0x02 石头
*0x03 布
*0x04 1
*0x05 2
*0x06 3
*0x07 4
*0x08 5
*/

float Rock[8] = {0.f};     //石头
float Paper[8] = {0.f};    //剪刀
float Scissors[8] = {0.f}; //布

float NumberOne[8] = {0.f};
float NumberTwo[8] = {0.f};
float NumberThree[8] = {0.f};
float NumberFour[8] = {0.f};
float NumberFive[8] = {0.f};



void TrainingMode(const unsigned char Mode,float *AcquisitionCapacitanceVal)
{	
	switch(Mode)
	{
		case 0x01:
			memcpy(Scissors,AcquisitionCapacitanceVal, 8 * sizeof(float));  //AcquisitionCapacitanceVal2Scissors
			break;
		case 0x02:
			memcpy(Rock,AcquisitionCapacitanceVal, 8 * sizeof(float));			//AcquisitionCapacitanceVal2Rock
			break;
		case 0x03:
			memcpy(Paper,AcquisitionCapacitanceVal, 8 * sizeof(float));			//AcquisitionCapacitanceVal2Paper
			break;
		case 0x04:
			memcpy(NumberOne,AcquisitionCapacitanceVal, 8 * sizeof(float));
			break;
		case 0x05:
			memcpy(NumberTwo,AcquisitionCapacitanceVal, 8 * sizeof(float));
			break;
		case 0x06:
			memcpy(NumberThree,AcquisitionCapacitanceVal, 8 * sizeof(float));
			break;
		case 0x07:
			memcpy(NumberFour,AcquisitionCapacitanceVal, 8 * sizeof(float));
			break;
		case 0x08:
			memcpy(NumberFive,AcquisitionCapacitanceVal, 8 * sizeof(float));
			break;
	}
}

//计算匹配度
float CalculateMatchingDegree(const float *AcquisitionCapacitanceVal,const float* StandCapacitanceVal)
{
	unsigned char i;
	float difference[8];
	float sum = 0;
	float DegreeValue;
	for(i = 0; i < 8; i++)
	{
		
		difference[i] = fabs(AcquisitionCapacitanceVal[i] - StandCapacitanceVal[i]);
		if(i == 4||i == 5||i ==0 ||i == 7|| i == 3) difference[i]*=64;
		//if(i == 2) difference[i] /= 1.3;
		//if(i == 7) difference[i]*=4;
		sum += difference[i];
	}
	DegreeValue = sum/8.0;
	return DegreeValue;
}



//猜拳
unsigned char GuessingMatching(float *AcquisitionCapacitanceVal)
{
	float RockDegreeValue = CalculateMatchingDegree(AcquisitionCapacitanceVal,Rock);

	float ParperDegreeValue = CalculateMatchingDegree(AcquisitionCapacitanceVal,Paper);
	float ScissorsDegreeValue = CalculateMatchingDegree(AcquisitionCapacitanceVal,Scissors);
		//printf("%3.3f %3.3f %3.3f\r\n", RockDegreeValue, ParperDegreeValue, ScissorsDegreeValue);
	if(ParperDegreeValue < RockDegreeValue && ParperDegreeValue < ScissorsDegreeValue)
	{
		return 0x03;
	}
	else if(RockDegreeValue < ParperDegreeValue && RockDegreeValue < ScissorsDegreeValue)
	{
		return 0x02;
	}
	else if(ScissorsDegreeValue < ParperDegreeValue && ScissorsDegreeValue < RockDegreeValue)
	{		
		return 0x01;
	}
	else
	{
		return 0xff; //有数相等
	}
}


//划拳
unsigned char PunchMatching(float *AcquisitionCapacitanceVal)
{
u8 i;	
	for(i = 0; i < 8; i++)
		{
			printf("CH%d:%3.3f  ", i, AcquisitionCapacitanceVal[i]);
		}
		printf("\r\n");
	float Num_1_DegreeValue = CalculateMatchingDegree(AcquisitionCapacitanceVal,NumberOne);
	float Num_2_DegreeValue = CalculateMatchingDegree(AcquisitionCapacitanceVal,NumberTwo);
	float Num_3_DegreeValue = CalculateMatchingDegree(AcquisitionCapacitanceVal,NumberThree);
//	float Num_4_DegreeValue = CalculateMatchingDegree(AcquisitionCapacitanceVal,NumberFour);
//	float Num_5_DegreeValue = CalculateMatchingDegree(AcquisitionCapacitanceVal,NumberFive);
	if(AcquisitionCapacitanceVal[0] >= NumberFour[0]*0.9)
	{
		if(AcquisitionCapacitanceVal[7] >= NumberFive[7]*0.4+NumberFour[7]*0.6)
		{
			return 0x08;
		}
		else
		{
			return 0x07;
		}
	}
	if(Num_1_DegreeValue < Num_2_DegreeValue && Num_1_DegreeValue < Num_3_DegreeValue )
	{
		if(AcquisitionCapacitanceVal[5]>(NumberOne[5] + NumberTwo[5])/2 +7) return 0x05;
		return 0x04;
	}
	else if(Num_2_DegreeValue < Num_1_DegreeValue && Num_2_DegreeValue < Num_3_DegreeValue )
	{
		 if(AcquisitionCapacitanceVal[1]>(NumberThree[1] + NumberTwo[1])/2 +7) return 0x06;
		if(AcquisitionCapacitanceVal[5]<(NumberOne[5] + NumberTwo[5])/2 - 7) return 0x04;
		return 0x05;
	}
	else if(Num_3_DegreeValue < Num_1_DegreeValue && Num_3_DegreeValue < Num_2_DegreeValue)
	{
		if(AcquisitionCapacitanceVal[3]<(NumberThree[3] + NumberTwo[3])/2 -7) return 0x05;
		return 0x06;
	}
//	else if(Num_4_DegreeValue < Num_1_DegreeValue && Num_4_DegreeValue < Num_2_DegreeValue && Num_4_DegreeValue < Num_3_DegreeValue && Num_4_DegreeValue < Num_5_DegreeValue)
//	{
//		if(AcquisitionCapacitanceVal[7]>(NumberFour[7]+NumberFive[7])/2 + 10) return 0x08;
//		return 0x07;
//	}
//	else if(Num_5_DegreeValue < Num_1_DegreeValue && Num_5_DegreeValue < Num_2_DegreeValue && Num_5_DegreeValue < Num_3_DegreeValue && Num_5_DegreeValue < Num_4_DegreeValue)
//	{
//		if(AcquisitionCapacitanceVal[0]<(NumberFive[0] + NumberTwo[0])/2 ) return 0x06;
//		return 0x08;
//	}
	else
	{
		return 0xff;
	}
}
