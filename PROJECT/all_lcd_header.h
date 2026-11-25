#include<LPC21XX.H>
#include"delay.h"
#define LCD 0xff<<0
#define	RS  1<<8
#define	E	1<<9
void LCD_INIT(void);
void LCD_CMD(unsigned char);
void LCD_DATA(unsigned char);
void LCD_STR(unsigned char*);
void LCD_INT(int);
void LCD_FLT(float);

void LCD_INIT(void)
{
	IODIR0=LCD|RS|E;
	LCD_CMD(0x01);
	LCD_CMD(0x02);
	LCD_CMD(0x0c);
	LCD_CMD(0x38);
}
void LCD_CMD(unsigned char cmd)
{
	IOCLR0=LCD;
	IOSET0=cmd;
	IOCLR0=RS;
	IOSET0=E;
	delay_ms(2);
	IOCLR0=E;
}
void LCD_DATA(unsigned char d)
{
	IOCLR0=LCD;
	IOSET0=d;
	IOSET0=RS;
	IOSET0=E;
	delay_ms(2);
	IOCLR0=E;
}
void LCD_STR(unsigned char*s)
{
	unsigned char pos=0;
	while(*s)
	{
		LCD_DATA(*s++);
		pos++;
		if(pos==16)
			LCD_CMD(0xc0);
	}
}
void LCD_INT(int n)
{
	unsigned char a[10];
	signed int i=0;
	if(n==0)
		LCD_DATA('0');
	else
	{
		if(n<0)
		{
			LCD_DATA('-');
			n=-n;
		}
		while(n>0)
		{
			a[i++]=n%10;
			n=n/10;	
		}
		for(--i;i>=0;i--)
			LCD_DATA(a[i]+48);
	}
}
void LCD_FLT(float f)
{
	int temp;
	if(f<0)
	{
		LCD_DATA('-');
		f=-f;
	}
	temp=f;
	LCD_INT(temp);
	LCD_DATA('.');
	temp=(f-temp)*10;
	LCD_INT(temp);
}
void LCD_ROT(unsigned char*b)
{ 
	int i,j,len=0;
	while(b[len]!='\0')
		len++;
	for(i=0;i<len;i++)
	{
		LCD_CMD(0X80);
		for(j=0;j<20;j++)
		{
			if((i+j)<len)
				LCD_DATA(b[i+j]);
			else
				LCD_DATA(' ');
		}
		delay_ms(300);
	}
}
