#include<LPC21XX.H>
void delay_sec(unsigned int);
void delay_ms(unsigned int);

void delay_sec(unsigned int s)
{
	T0PR=60000000-1;
	T0TCR=0x1;
	while(T0TC<s);
	T0TCR=0x3;
	T0TCR=0x0;
} 
void delay_ms(unsigned int ms)
{
	T0PR=60000-1;
	T0TCR=0x1;
	while(T0TC<ms);
	T0TCR=0x3;
	T0TCR=0x0;
}
void delay_us(unsigned int us)
{
	T0PR=60-1;
	T0TCR=0x1;
	while(T0TC<us);
	T0TCR=0x3;
	T0TCR=0x0;
}

