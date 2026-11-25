#include<lpc21xx.h>
#include"can2_init.h"
#include"delay.h"
#define TRIG 1<<4
#define ECHO 5
typedef unsigned int u32;
void DIST_INIT(void);
u32 DISTANCE(void);
u32 d;
int main()
{
	CAN2_MSG m1;
	CAN2_INIT();
	DIST_INIT();
	m1.id =0x100;
	m1.rtr=0;
	m1.dlc=2;
	delay_ms(500);
	while(1)
	{
		d=DISTANCE();
		m1.byteA = d;
		CAN2_TX(m1);
		delay_ms(100);
	} 
}
void DIST_INIT(void)
{
	IODIR0|=TRIG;
	IODIR0&=~ECHO;
}
u32 DISTANCE(void)
{
	u32 time,dist,timecount=0;

	IOCLR0=TRIG;
	delay_us(5);
	IOSET0=TRIG;
	delay_us(10);
	IOCLR0=TRIG;

    while(((IOPIN0>>ECHO)&1)==0)
    {
        timecount++;
        if(timecount>50000)
            return 100;
    }
    T1TCR=0X03;
    T1PR=59;
    T1TC=0;
    T1TCR=0X01;
	timecount=0;
    while(((IOPIN0>>ECHO)&1)!=0)
    {
        timecount++;
        if(timecount>50000)
        {
            T1TCR=0X00;
            return 100;
        }
    }

    T1TCR=0X00;

    time=T1TC;
    dist=time/58;
	if(dist>2 && dist<200)
		return dist;
	else
		return 100;
}


