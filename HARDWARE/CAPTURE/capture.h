#ifndef __CAPTURE_H
#define __CAPTURE_H
#define CHANNEL_SINGLE 4
#define CHANNEL_TOTAL 8
extern float base[2][CHANNEL_SINGLE], ori[CHANNEL_TOTAL], data[CHANNEL_TOTAL], fidata[CHANNEL_TOTAL];
void capture_init(void);
void capture_get(unsigned char select);
void get_stable(float* aver);
void data_show(void);
void train(void);
#endif

