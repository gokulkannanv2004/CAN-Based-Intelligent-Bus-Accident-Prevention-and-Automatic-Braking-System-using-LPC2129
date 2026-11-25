#include<lpc21xx.h>
#include"can2_init.h"
#include"delay.h"
#define MSTR_BIT  5
#define MODE3     0X18
#define SPIF_BIT  7
#define CS        1<<7 
void SPI_INIT(void);
u8 SPI0(u8);
u32 MCP3204(u8);
u32 s;
int main()
{
	CAN2_MSG m2;
	CAN2_INIT();
	SPI_INIT();
	
	m2.id =0x200;
	m2.rtr=0;
	m2.dlc=2;

	while(1)
	{
		s=MCP3204(1);
		m2.byteA= s;
		CAN2_TX(m2);
		delay_ms(100);
	}
}
void SPI_INIT(void)
{
	PINSEL0|= 0x1500;
	S0SPCCR = 150;
	S0SPCR  = (1<<MSTR_BIT)|MODE3;	
	IODIR0  = CS;
	IOSET0=CS;
}
u32 MCP3204(u8 ch_no)
{
	u8 hb,lb;
	u32 adcval=0;
	IOCLR0=CS;
	SPI0(0X06);
	hb=SPI0(ch_no<<6);
	lb=SPI0(0X00);
	IOSET0=CS;
	adcval = ((hb&0xf)<<8)|lb;
	return (((adcval*3.3)/4096)*18.46);
}
u8 SPI0(u8 data)
{
	u8 stat;
	stat=S0SPSR;
	S0SPDR=data;
	while(((S0SPSR>>SPIF_BIT)&1)==0);
	return S0SPDR;
}

