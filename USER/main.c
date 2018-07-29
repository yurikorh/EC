#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "fdc2214.h"
#include "algo.h"
#include "capture.h"
#include "Matching.h"
#include "key.h"
#include "image2lcd.h"
#include "Timer.h"
void Sys_Init(void);
void trainl(const unsigned char Mode);
void SwitchMode(void);
unsigned char Guess(void);
unsigned char Punch(void);

extern const unsigned char gImage_Paper[];
extern const unsigned char gImage_scissors[];
extern const unsigned char gImage_stone[];

int main(void)
{
	u8 count = 0;
	u8 i;
	u8 lcd_str[12];			//存放LCD显示字符串
	
	float bn[7]= {0};
  Sys_Init();
	while(1)
	{	
		//FDC_SELECT = 0;
//		get_stable(bn);
		
//		count++;
//				if(count<100)
//		{
//			
//		FDC_SELECT = 0;
//		capture_get(0);//采样
//		}
//		else if(count >200)
//		{
//			count = 0;
//		}
//		else
//		{
//					FDC_SELECT = 1;
//		capture_get(1);
//		}
//		if(Key_Mode == WKUP_PRES)
//		{
//			
//		FDC_SELECT = 0;
//		capture_get(0);//采样
//		}
//		else
//		{
//					FDC_SELECT = 1;
//		capture_get(1);
//		}
//		data_show()      ;//lcd显示
//		for(i = 0; i < 8; i++)
//		{
//			printf("CH%d:%3.3f  ", i, bn[i]);
//		}
//		printf("\r\n");
//		printf("ori:%3.3f   fidata:%3.3f  diff:%3.3f\r\n", ori[4] * 100, fidata[4] * 100, DIS[4].deriv);  
		//printf("%3.3f  %3.3f  %3.3f  %3.3f  %3.3f  %3.3f  %3.3f  %3.3f\r\n", base[0][0],base[0][1],base[0][2],base[0][3], base[1][0],base[1][1],base[1][2],base[1][3]);
//			for(i = 0; i < 8; i++)
//			{
//				printf("Scissors[%d]:%3.3f ",	i, Scissors[i]);
//			}
//			printf("\r\n");
		led=~led;
		delay_ms(1);
		//while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);
		LCD_ShowString(0,0, 200,16,16,"KEY1 enter the TraingMode");
		LCD_ShowString(0,20,200,16,16,"KEY0 enter the DistinguishMode");
		SwitchMode();
		sprintf((char*)lcd_str, "%d", GuessingMatching(data));
		LCD_ShowString(30, 200, 200, 24, 24, lcd_str);
		sprintf((char*)lcd_str, "%d", PunchMatching(data) - 3);
		LCD_ShowString(30, 220, 200, 24, 24, lcd_str);
			
	}
}


void ChooseTrainingMode(void)
{
	unsigned char i = 0;
	static unsigned char mode = 0;
	static unsigned char KeyFlag = 1;
	while(KeyFlag != 0)
	{
		LCD_ShowString(0, 0, 200, 16, 16,  "Welcome to Training Mode    ");
		LCD_ShowString(0, 20, 200, 16, 16, "Press Key1 to choose gesture");
		LCD_ShowString(0, 40, 200, 16, 16, "Press WKUP to start Training      ");
		switch(Key_Mode)
		{
			case KEY1_PRES:
			{
				i++;
				i = i%9;
				switch(i)
				{
					case 1:
						mode = 0x01;
						LCD_ShowString(10,140,200,24,24,"mode = Scissors  ");
					break;
					case 2:
						mode = 0x02;
						LCD_ShowString(10,140,200,24 ,24,"mode = Rock      ");
					break;
					case 3:
						mode = 0x03;
						LCD_ShowString(10,140,200,24,24,"mode = Paper     ");
					break;
					case 4:
						mode = 0x04;
						LCD_ShowString(10,140,200,24,24,"mode = Number1     ");
					break;
					case 5:
						mode = 0x05;
						LCD_ShowString(10,140,200,24,24,"mode = Number2     ");
					break;
					case 6:
						mode = 0x06;
						LCD_ShowString(10,140,200,24,24,"mode = Number3     ");
					break;
					case 7:
						mode = 0x07;
						LCD_ShowString(10,140,200,24,24,"mode = Number4     ");
					break;
					case 8:
						LCD_ShowString(10,140,200,24,24,"mode = Number5     ");
					break;
				}
			}break;
			case WKUP_PRES:
					if(mode > 0)
					{
						trainl(mode);
						KeyFlag = 0;
					}
			break;
		}
	}
	KeyFlag = 1;
}

void GuessModeDistinguish(void)
{
	switch(Guess())
	{
		case 0x01:  //剪刀
		{
			image_display(80,200,(unsigned char*)gImage_scissors);
		}break;
		case 0x02:  //石头
		{
			image_display(80,200,(unsigned char*)gImage_stone);
		}break;
		case 0x03:  //布
		{
			image_display(80,210,(unsigned char*)gImage_Paper);
		}break;
	}
}

void PunchModeDistinguish(void)
{
	switch(Punch())
	{
		case 0x04:
			LCD_ShowString(30,200,200,24,24,"Number ONE  ");
		break;
		case 0x05:
			LCD_ShowString(30,200,200,24,24,"Number TWO  ");
		break;
		case 0x06:
			LCD_ShowString(30,200,200,24,24,"Number THREE");
		break;
		case 0x07:
			LCD_ShowString(30,200,200,24,24,"Number Four ");
		break;
		case 0x08:
			LCD_ShowString(30,200,200,24,24,"Number Five  ");
		break;
	}
}
void GuessorPunch(unsigned char mode)
{
	switch(mode)
	{
		case 0:
		{
			GuessModeDistinguish();
		}break;
		case 1:
		{
			PunchModeDistinguish();
		}break;
	}
}

void ChooseDistinguishMode(void)
{
	static unsigned char mode = 0xff;
	while(Key_Mode != WKUP_PRES)
	{
		LCD_ShowString(0,0, 200,12,12,"Welcome to Distinguish Mode                                   ");
		LCD_ShowString(0,20,200,12,12,"Press Key1 to choose guess");
		LCD_ShowString(0,40,200,12,12,"Press Key0 to choose Punch");
		switch(Key_Mode)
		{
			case KEY1_PRES:
			{
				LCD_Clear(WHITE);
				LCD_ShowString(0,0, 200,12,12,"Welcome to Distinguish Mode                                   ");
				LCD_ShowString(0,20,200,12,12,"Press Key1 to choose guess");
				LCD_ShowString(0,40,200,12,12,"Press Key0 to choose Punch");
				LCD_ShowString(30,120,200,24,24,"Gueess Mode");
				mode  = 0;  //猜拳模式
			}break;
			case KEY0_PRES:
			{
				LCD_Clear(WHITE); 
				LCD_ShowString(0,0, 200,12,12,"Welcome to Distinguish Mode                                   ");
				LCD_ShowString(0,20,200,12,12,"Press Key1 to choose guess");
				LCD_ShowString(0,40,200,12,12,"Press Key0 to choose Punch");
				LCD_ShowString(30,120,200,24,24,"Punch Mode");
				mode = 1;   //划拳模式
			}break;
		}
		GuessorPunch(mode);
	}
	Key_Mode = 0xff;
	LCD_Clear(WHITE);
}

void SwitchMode()
{
		switch(Key_Mode)
		{
			case KEY1_PRES:
			{
				led0 = !led0; 
				LCD_Clear(WHITE);
				ChooseTrainingMode();
			}break;
			case KEY0_PRES:
			{
				LCD_Clear(WHITE);
				ChooseDistinguishMode();
			}break;
		}
}

void Sys_Init(void)
{
	delay_init();	 
	NVIC_Configuration();
	uart_init(115200);	 //串口波特率115200
	LED_Init();
	LCD_Init();	
	KEY_Init();
	LCD_Clear(WHITE);	
	POINT_COLOR=RED;
	FDC2214_Init();
	TIM3_Int_Init(499,7199);
//	led=0;
	delay_ms(500);
	filter_init();
	capture_init();
}

void trainl(const unsigned char Mode)	//将手放置好后按键开始训练 
{
	float averdata[8] = {0};
	//count = 0;
	filter_init();

			//等待数据稳定
	LCD_Clear(WHITE);
	LCD_ShowString(30,60,200,24,24,"  Waiting.....     ");
	led0 = 0;
	get_stable(averdata);
//	while(1)
//	{
//		capture_get();
//		//等待数据稳定
//		flag1 = 0; 
//		for(i = 0; i < 8; i++)
//		{
//			if(fabs(DIS[i].deriv) > DIS[i].DT * 5)	flag1 = 1;
//		}
//		if(flag1==0)
//		{
//			count++;
//		}
////		else
////		{
////			count *= 0.25;
////		}
//		if(count > 0x10)
//		{
//			count = 0;
//			break;
//		}
//		led0 = 0;
//	}
	LCD_Clear(WHITE);
//	for(i = 0; i < 32; i++)
//	{
//		capture_get();
//		for(j = 0; j < 8; j++)
//		{
//			averdata[j] += data[j];
//		}
//		delay_ms(3);
//	}
//	for(j = 0; j < 8; j++)
//	{
//		averdata[j] /= 32;
//		printf("averdata[%d]:%3.3f ", j, averdata[j]); 
//	}
//	printf("\r\n");
	TrainingMode(Mode, averdata);
	led0 = 1;
}
		
unsigned char Guess(void)
{
	unsigned char result;
	float averdata[8] = {0};
	led0 = 0;
	get_stable(      averdata);
	result = GuessingMatching(averdata);
	return result;
}

		
unsigned char Punch(void)
{
	unsigned char result;
	float averdata[8] = {0};
	led0 = 0;
	get_stable(averdata);
	result = PunchMatching(averdata);
	return result;
}

