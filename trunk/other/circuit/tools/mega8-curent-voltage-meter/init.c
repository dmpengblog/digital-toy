//ICC-AVR application builder : 2005-4-25 9:42:27
// Target : M8
// Crystal: 8.00Mhz
/*
* Copyright (c) 2005, ������Դ������˾������
* All rights reserved.
* �ļ����ƣ�init.c  �����ʼ��MCU���ڲ���Դ���ⲿ�˿�

* ��ǰ�汾�� 1.0
* ���ߣ�	 �ź�ǿ
* ������ڣ� 2005.7.25
*/
#include <avr/io.h>
//#include <avr/iom8.h>
#include <avr/interrupt.h>
#include <avr/signal.h>
#include <avr/wdt.h>
#include <avr/eeprom.h> 
#include "init.h"

void port_init(void)
{
	PORTB = 0x0F;									//PB0-4�Ӽ���,��Ϊ��������
	DDRB  = 0xF0;									//PB4-7�ӿ������,��Ϊ���������
	PORTC = 0x3C; 									//PC0,PC1��ģ���źŽ���ADת��,��Ϊ���벻����
	DDRC  = 0x3C;									//PC2,PC3,PC4,PC5��4λ7��LED��λѡ��,��Ϊ�������
	PORTD = 0xFF;									//PORTD�ڽ�7��LED�Ķ�ѡ��,ȫ����Ϊ�������
	DDRD  = 0xFF;
}

//TIMER0 initialize - prescale:1024
// desired value: 50Hz
// actual value: 50.080Hz (0.2%)
void timer0_init(void)
{
	TCCR0 = 0x00; 									//stop
	TCNT0 = 0xB2; 									//set count
	TCCR0 = 0x05; 									//start timer
}

//#pragma interrupt_handler timer0_ovf_isr:10
SIGNAL(SIG_OVERFLOW0)
{
	static unsigned char i = 0;
	static unsigned int  total = 0,max_value = 0,min_value = 0xff;
	TCNT0 = 0xB2; 									//reload counter value
	if ( TIMER_EN )
	{
		TIMER_NUM++;
	}
	if ( adc_data_temp > max_value )
	{
		max_value = adc_data_temp;
	}
	if ( adc_data_temp < min_value )
	{
		min_value = adc_data_temp;
	}
	total += adc_data_temp;
	if ( i == 49 )
	{
		adc_data[0] =( total - min_value - max_value ) / 48.0;
		adc_data_temp = 0;
		max_value = 0;
		min_value = 0xff;
		total = 0;
	}
	else if ( i == 99 )
	{
		adc_data[1] =( total - min_value - max_value ) / 48.0;
		adc_data_temp = 0;
		max_value = 0;
		min_value = 0xff;
		total = 0;
	}
	if ( i < 50 )
	{
		ADMUX &= ~( 1 << MUX0 );
	}
	else
	{
		ADMUX |= ( 1 << MUX0 );
	}	
	i++;
	i %= 100;
	ADCSR |= ( 1<< ADSC );							//�����µ�ADת��
}

//Watchdog initialize
// prescale: 512K
void watchdog_init(void)
{
 	wdt_reset(); 									//this prevents a timout on enabling
 	wdt_enable(WDTO_1S);							//WATCHDOG ENABLED - dont forget to issue WDRs��0.52S
}
	
//ADC initialize
// Conversion time: 117uS
void adc_init(void)
{
 	ADCSR = 0x00; 									//disable adc
 	ADMUX = 0xC0; 									//select adc input 0��ѡ��AVCCΪ�ο���Դ
 	ACSR  = 0x80; 									//��ֹģ��Ƚ���
 	ADCSR = 0xCE; 									//ʹ��ADC��ADCת����ʼ������ADC�жϣ�32��Ƶ
}

//call this routine to initialize all peripherals
void init_devices(void)
{													//stop errant interrupts until set up
	cli();											//disable all interrupts
	port_init();
	watchdog_init();
	timer0_init();
	adc_init();
	MCUCR = 0x00;
	GICR  = 0x00;
	TIMSK = 0x01; 									//timer interrupt sources
	if ( UP_DOWN_POWER != 0x55aa )
	{
			/*
		KEY_A_V_STATE = 0;
		KEY_AUTO_MAN_STATE = 0;
		KEY_CC_CV_STATE = 0;
		POWER_STATE = 0;
		*/
		eeprom_busy_wait();
		KEY_A_V_STATE      = eeprom_read_byte ( (char*)0x01 );
		eeprom_busy_wait();
		KEY_AUTO_MAN_STATE = eeprom_read_byte ( (char*)0x03 );
		eeprom_busy_wait();
		KEY_CC_CV_STATE    = eeprom_read_byte ( (char*)0x05 );
		eeprom_busy_wait();
		POWER_STATE        = eeprom_read_byte ( (char*)0x07 );
	}
		
	if ( POWER_STATE )
	{
		PORTB |= ( 1 << POWER );		//������Դ
	}
	else
	{
		PORTB &= ~( 1 << POWER );		//�رյ�Դ
	}
	if ( KEY_A_V_STATE )
	{
		PORTB |= ( 1 << OUT_A );		//��ʾ����
	}
	else
	{
		PORTB &= ~( 1 << OUT_A );		//��ʾ��ѹ
	}
	if ( KEY_CC_CV_STATE )
	{
		PORTB |= ( 1 << OUT_CV );		//��ѹ
	}
	else
	{
		PORTB &= ~( 1 << OUT_CV );		//����
	}
	if ( KEY_AUTO_MAN_STATE )
	{
		PORTB |= ( 1 << OUT_AUTO );	//�Զ�
	}
	else
	{
		PORTB &= ~( 1 << OUT_AUTO );	//�ֶ�
	}
	UP_DOWN_POWER = 0x55aa;
	sei(); 								//re-enable interrupts
}