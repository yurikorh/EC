#ifndef MATCHING_H
#define MATCHING_H

/*@Mode: 
*0x01 剪刀
*0x02 石头
*0x03 布
*0x04 1
*0x05 2
*0x06 3
*0x07 4
*0x08 5

@AcquisitionCapacitanceVal 采集到的八通道电容值
*/

//训练模式
void TrainingMode(const unsigned char Mode,float *AcquisitionCapacitanceVal);

//猜拳
unsigned char GuessingMatching(float *AcquisitionCapacitanceVal);

//划拳
unsigned char PunchMatching(float *AcquisitionCapacitanceVal);
typedef train_struct
{
	float aver

#endif
