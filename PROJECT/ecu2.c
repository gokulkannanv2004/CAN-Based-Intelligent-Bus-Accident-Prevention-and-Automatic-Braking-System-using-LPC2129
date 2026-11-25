#include<lpc21xx.h>
#include"can2_init.h"
//#include"uart_header.h"
#include"all_lcd_header.h"
void PWM_INIT(void);
void MOTOR_CTRL(u32);
u8 a[]="CAN Based Intelligent Bus Accident Prevention and Automatic Braking System using LPC2129";
u32 distance,speed,mtr_cl;
int main()
{
	CAN2_MSG m;
	CAN2_INIT();
	PWM_INIT();
	LCD_INIT();
//	UART0_INIT();
	LCD_CMD(0X80);
	LCD_ROT(a);
	LCD_CMD(0X01);
	LCD_CMD(0X86);
	LCD_STR("MODE");
	LCD_CMD(0X94);
	LCD_STR("DISTANCE");
	LCD_CMD(0X9E);
	LCD_STR("SPEED");

	while(1)
	{
		CAN2_RX(&m);
		if(m.id==0x100 && m.rtr==0){
			distance=m.byteA;
//			UART0_INT(m.byteA);
			}
		else if(m.id==0x200 && m.rtr==0)
			speed=m.byteA;

		if(distance>=100)
		{
			mtr_cl=speed;
			LCD_CMD(0XC6);
			LCD_STR("SAFE");
			LCD_CMD(0XD8);
			LCD_STR("cm");
		}
		else if(distance>=60 && distance<=99)
		{
			mtr_cl=(speed*75)/100;	
			LCD_CMD(0XC6);
			LCD_STR("CAUTION");
			LCD_CMD(0XD7);
			LCD_STR("cm");
		}
		else if(distance>=30 && distance<60)
		{
			mtr_cl=(speed*55)/100;
			LCD_CMD(0XC6);
			LCD_STR("ALERT");
			LCD_CMD(0XD7);
			LCD_STR("cm");
		}
		else if(distance<30)
		{	
			mtr_cl=0;
			LCD_CMD(0XC6);
			LCD_STR("BRAKE");
			LCD_CMD(0XD7);
			LCD_STR("cm");
		}
		
		LCD_CMD(0XD4);
		LCD_INT(distance);
		MOTOR_CTRL(mtr_cl);
		LCD_CMD(0XDE);
		LCD_INT(mtr_cl);
		LCD_CMD(0XE1);
		LCD_STR("km/hr");
		delay_sec(1);
		LCD_CMD(0XD4);
		LCD_STR("                ");
		LCD_CMD(0XC0);
		LCD_STR("                ");
	}
}
void PWM_INIT(void)
{
	PINSEL1|=0X400;
	VPBDIV  =1; 
	PWMMR0  =60000-1;
	PWMMR5  =0;
	PWMMCR  =0X3;
	PWMPCR  =0X2000;
	PWMTCR  =0X9;
	PWMLER  =0X20;
}
void MOTOR_CTRL(u32 s)
{
	if(s>100)
		s=100;
	
	PWMMR5=(s*60000)/100;
	PWMLER=0X20;		
} 
