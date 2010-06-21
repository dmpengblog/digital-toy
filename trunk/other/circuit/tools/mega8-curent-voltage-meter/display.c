
/*
* Copyright (c) 2005, ������Դ������˾������
* All rights reserved.
* �ļ����ƣ�display.c  ��������LED���������ʾ����

* ��ǰ�汾�� 1.0
* ���ߣ�	 �ź�ǿ
* ������ڣ� 2005.7.25
*/

#include <avr/io.h>
#include "init.h"

unsigned char LED[13] = 
{ 0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xff,0x88,0xc1 };
	  		   			  						//{0123456789 AV} LED�����
												
void delay_1ms(void)
{
 	unsigned int i;
	for ( i = 0;i < 1422;i++ );					//3.6864MƵ��ʱѭ����525
}

void delayMS(unsigned char count)
{
 	unsigned char i;
	for ( i = 0;i < count;i++ )
	{
	 	delay_1ms();
	}
}
												
void display(unsigned char DISP_DATA[4],unsigned char DOT)
{	
	unsigned char i;
	for( i = 0;i < 4; i++ )					//4λLEDѭ��
	{
	  	PORTD = LED[DISP_DATA[i]]; 			//ȡ����ʾ������
		if ( DOT == i )						//��ʾС����
		{
			PORTD &= 0x7f;
		}
	 	PORTC = ~( 1 << (i+2) ); 				//����ʾλ
		delayMS(10);							//��ʱ2MS
		//PORTC = 0x3C;						    //����ʾ
		PORTD = 0xFF;						    //����ʾ
	}
}
