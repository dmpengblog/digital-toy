//��������Ҫ��ɶ��������ĵ���/��ѹ����ADת������ʾ�Ĺ���
/*
* Copyright (c) 2005, ������Դ������˾������
* All rights reserved.
* �ļ����ƣ�main.c   ������

* ��ǰ�汾�� 1.0
* ���ߣ�	 �ź�ǿ
* ������ڣ� 2005.7.25
*/

#define   OS_GLOBALS

#include <avr/io.h>
//#include <avr/iom8.h>
#include <avr/interrupt.h>
#include <avr/signal.h>
#include <avr/wdt.h>
#include <stdlib.h>
#include <avr/eeprom.h>   
#include "init.h"

void convert_v( unsigned char* pch);
void convert_a( unsigned char* pch);
void SET_A( unsigned char* pch );
void CHECK_EDATA_A(void);

//char CS[4] = {0,2,1,0};

int main(void)
{
	unsigned char chS_V[]={0,0,0,0},chS_A[]={0,0,0,0};
	static unsigned char chdot;
	delayMS(250);									//��ʱ250MS
	delayMS(250);									//��ʱ250MS
	eeprom_busy_wait();
	cli();
	eeprom_read_block((char*)MAX_A,(char*)0x20,6 );
	CHECK_EDATA_A();
	init_devices();									//��ʼ���˿�
	
    for (;;)
    {   
		wdt_reset();								//ι��
		convert_v( chS_V );
		convert_a( chS_A );	
		key_board();
		if ( !SET_STATU )
		{
			if ( KEY_A_V_STATE )
			{
				if ( MAX_A[0] >= 500 )
				{
					chdot = NODOT;
				}
				else
				{
					chdot = ONEDOT;
				}
				display(chS_A,chdot);				//��ʾ����ֵ
			}
			else
			{
				chdot = TWODOT;
				display(chS_V,chdot);				//��ʾ��ѹֵ
			}
		}
		else
		{
			SET_A( chS_A );							//�������õ���ֵ״̬				
			chdot = NODOT;
			display(chS_A,chdot);					//��ʾ�趨����ֵ
		}
		//delayMS(10);								//��ʱ10MS
	}  
}
   
void convert_v( unsigned char* pch )
{
	unsigned int intV,i;
	intV = (int) (((float)MAX_V * (float) adc_data[0]) /1024.0) ;
													//�����ѹֵ										
	if ( intV > 70 )
	{
		intV -= 70;
	}
	else
	{
		intV = 0;
	}
	for ( i = 0; i < 4; i++ )
	{
		pch[i] = intV % 10;
		intV /= 10;
	}
}

void convert_a( unsigned char* pch)
{
	unsigned int i,n,k;
	float temp = 0,intA;
	CHECK_EDATA_A();
	if ( MAX_A[0] < 500 )
	{
		k = 10;
	}
	else
	{
		k = 1;
	}
	intA = ((float)MAX_A[0] * 2.0 * k * adc_data[1]) / 1024.0;
													//�������ֵ
										
	temp = (float)MAX_A[0]* 7.93 * k / 100.0 ;	
								
	if ( intA > temp )
	{
		intA -= temp;
		if( adc_data[1] < 33)
		{
			intA += ( (float) MAX_A[0] * k / 375.0 );
		}
		else if ( adc_data[1] < 136)
		{
			intA += ( (float) MAX_A[0] * k / 600.0 );
		}
	}
	else
	{
		intA = 0.0;
	}	
	n = (int)intA;								
	for ( i = 0; i < 4; i++ )
	{
		pch[i] = n % 10 ;
		n /= 10.0;
	}
}

void SET_A( unsigned char* pch )
{
	unsigned int intA,i;
	intA = MAX_A[0] ;
	for ( i = 0; i < 4; i++ )
	{
		pch[i] = intA % 10;
		intA /= 10;
	}
	if ( pch[3] == 0 )
	{
		pch[3] = 0x0a;
		if ( pch[2] == 0 )
		{
			pch[2] = 0x0a;
			if ( pch[1] == 0 )
			{
				pch[1] = 0x0a;
			}
		}
	}
}

void CHECK_EDATA_A(void)
{
	if ( MAX_A[1] == MAX_A[2] )
	{
		if ( MAX_A[0] != MAX_A[1] )
		{
			MAX_A[0] = MAX_A[1];
			eeprom_busy_wait();
			cli();
			eeprom_write_block((char*)MAX_A,(char*)0x20,6 );
			sei();
		}
	}
	else
	{
		MAX_A[2] = MAX_A[1] = MAX_A[0];
		eeprom_busy_wait();
		cli();
		eeprom_write_block((char*)MAX_A,(char*)0x20,6 );
		sei();
	}
}

//#pragma interrupt_handler adc_isr:15
SIGNAL(SIG_ADC)
{
	adc_data_temp = ADC & 0x3FF;
}
