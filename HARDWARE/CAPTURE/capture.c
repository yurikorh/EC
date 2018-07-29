#include "capture.h"
#include "algo.h"
#include "fdc2214.h"
#include "sys.h"
#include "delay.h"
#include "lcd.h"
#include "Matching.h"

float base[2][CHANNEL_SINGLE], ori[CHANNEL_TOTAL], data[CHANNEL_TOTAL], fidata[CHANNEL_TOTAL];
void capture_init(void)		//get the base line values
{
	u8 i, j;	
	FDC_SELECT = 0;
	delay_ms(2);

	for(i = 0; i < CHANNEL_SINGLE; i++)
	{
		base[0][i] = 0;
	}
	for(j = 0; j < 32; j++)
	{
		for(i = 0; i < CHANNEL_SINGLE; i++)
		{
			base[0][i] += Cap_Calculate(i);
		}
		delay_ms(10);
	}
	for(i = 0; i < CHANNEL_SINGLE; i++)
	{
		base[0][i] /= 32;
		//D_I_Init(&DIS[i], IIR_Filter_Init(&IIRS[i], 0), rdt, rdi)；
	}
	
		FDC_SELECT = 1;
		delay_ms(500);
	for(i = 0; i < CHANNEL_SINGLE; i++)
	{
		base[1][i] = 0;
	}
	for(j = 0; j < 32; j++)
	{
		for(i = 0; i < CHANNEL_SINGLE; i++)
		{
			base[1][i] += Cap_Calculate(i);
		}
		delay_ms(10);
	}
	for(i = 0; i < CHANNEL_SINGLE; i++)
	{
		base[1][i] /= 32;
		//D_I_Init(&DIS[i], IIR_Filter_Init(&IIRS[i], 0), rdt, rdi)；
	}
	
//	for(i = 0; i < CHANNEL_SINGLE; i++)
//	{
//		base[0][i] = Cap_Calculate(i);
//	}
//	FDC_SELECT = 1;
//	delay_ms(800);
//	for(i = 0; i < CHANNEL_SINGLE; i++)
//	{
//		base[1][i] = Cap_Calculate(i);
//	}	
}


//采样 - 4个通道
void capture_get(unsigned char select)
{
	int i, index;
	for(i = 0, index = select * 4 ; i < CHANNEL_SINGLE; i++, index++)
	{
		ori[index] = Cap_Calculate(i) - base[select][i];
		
		fidata[index] = D_I_cal(&DIS[index], IIR_Filter_cal(&IIRS[index], ori[index]));
		
		if(fidata[index] < DIS[index].IT)
		{
			data[index] = 0;
		}
		else 
		{
			data[index] = fidata[index] * 100;
		}
		//printf("CH%d:%3.3f  ",index,data[index]);
	}
}

void get_stable(float* aver)
{
	static float lstdata[8] = {0};
	unsigned char i = 0, j = 0, flag1 = 0, flag2 = 0, timeout = 0, count = 0;
	FDC_SELECT = 0;
	delay_ms(50);
	filter_init();
	while(1)
	{
		capture_get(0);
		for(i = 0; i < 4; i++)
		{
			if(fabs(data[i] - lstdata[i]) > 5)	
			{
				flag1 = 1;
				break;
			}
			if(data[i] > 0.1)	flag2 = 1;
			lstdata[i] = data[i];
		}
		if(flag1 == 0 && flag2 == 1)
		{
			count++;
		}
		timeout++;
		if(timeout > 100 || count > 0x40) 
		{
			timeout = 0;
			count = 0;
			break;
		}
		delay_ms(3);
	}
	
	
		for(i = 0; i < 8; i++)
	{
		capture_get(0);
		for(j = 0; j < 4; j++)
		{		

			aver[j] += data[j];
		}
		delay_ms(1);
	}
	for(j = 0; j < 4; j++)
	{
		aver[j] /= 8;
	}
	

	flag1 = 0; flag2 = 0;
	FDC_SELECT = 1;
	filter_init();
				delay_ms(50);
	while(1)
	{
		capture_get(1);
		for(i = 4; i < 8; i++)
		{
			if(fabs(data[i] - lstdata[i]) > 5)	
			{
				flag1 = 1;
				break;
			}
			if(data[i] > 0.1)	flag2 = 1;
			lstdata[i] = data[i];
		}
		if(flag1 == 0 && flag2 == 1)
		{
			count++;
		}
		timeout++;
		if(timeout > 100 || count > 0x40) 
		{
			timeout = 0;
			count = 0;          
			break;
		}
		delay_ms(3);

	}
	
	
	
	for(i = 0; i < 8; i++)
	{
		capture_get(1);
		for(j = 4; j < 8; j++)
		{		

			aver[j] += data[j];
		}
		delay_ms(1);
	}
	for(j = 4; j < 8; j++)
	{
		aver[j] /= 8;
	}
	FDC_SELECT = 1;
}
		
		
void data_show(void)
{
	int i, y;
	u8 lcd_str[12];			//存放LCD显示字符串
	for(i = 0, y = 40;  i < 8; i++, y += 20)
	{
		sprintf((char*)lcd_str, "CH%d:%3.3f", i, data[i]);
		LCD_ShowString(30, y, 200, 24, 24, lcd_str);
	}
}

