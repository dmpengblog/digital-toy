
/*
* Copyright (c) 2005, ������Դ������˾������
* All rights reserved.
* �ļ����ƣ�key.c  �������������Ի�ð���״̬

* ��ǰ�汾�� 1.0
* ���ߣ�	 �ź�ǿ
* ������ڣ� 2005.7.25
*/

#include <avr/io.h>
//#include <avr/iom8.h>
#include <avr/eeprom.h> 
#include <avr/interrupt.h>
#include "init.h"

extern void CHECK_EDATA_A(void);

unsigned char key_scan(void)	 					//���̺���
{
 	if(!( PINB & 0x01 ))							//�ǵ�һ��
 	{
		return POWER_DOWN;  	 	  				//��Դ������
 	}
 	else if(!( PINB & 0x02 ))						//�ǵڶ���
 	{
		return A_V_DOWN;  	 	  					//����/��ѹ�л�������
 	}
 	else if(!( PINB & 0x04 ))						//�ǵڶ���
 	{
		return CC_CV_DOWN;  	 	  				//����/��ѹ�л�������
 	}
 	else if(!( PINB & 0x08 ))						//�ǵڶ���
 	{
		return AUTO_MAN_DOWN;  	 	  			//�Զ�/�ֶ��л�������
 	}	
	return 0;  	 	  							//���ؼ�ֵ
}

unsigned char key_down(void)                      //�м����Ƿ�����
{    
    if (( PINB & 0x0f) != 0x0f )                   //����м�����
    {
        return 1;                 					//����1
    }
    else
    {
		return 0;                  				//���򷵻�0
	} 
}

void key_board(void)
{
	unsigned char key_first, key_end, key_press = 0;
	static unsigned int KEY_DOWN_TIME = 0,NUM_CHANGE = FALSE,i = 0;
	
	if ( ( TIMER_NUM > 500 ) && SET_STATU )			
													//�����ʱ
	{
		SET_STATU = FALSE;
		TIMER_EN = FALSE;
		if ( NUM_CHANGE )
		{
			NUM_CHANGE = FALSE;
			eeprom_busy_wait();
			cli();
			eeprom_write_block( (char*)MAX_A,(char*)0x20,6 );
			sei();
		}
		KEY_DOWN_TIME = 0;
		TIMER_NUM = 0;
		delayMS(100);						//��ʱ10MS
	}
	if ( key_down() )
	{  
		key_first = key_scan();						//�õ���ֵ
		delayMS(20);								//��ʱ10MS��
		key_end   = key_scan(); 
		if ( key_end == key_first )
		{
			key_press  = KEY_TEMP ^ key_end;
			if ( key_end == A_V_DOWN )				//����ǵ�ѹ/�����л���
			{
				KEY_DOWN_TIME++;					//������ʱ
				if ( KEY_DOWN_TIME > 1000 )			//�����������
				{
					SET_STATU = TRUE;
					TIMER_EN = TRUE;
					KEY_DOWN_TIME = 0;
				}
			}
			if (( KEY_INPUT == AUTO_MAN_DOWN ) && SET_STATU )			
													//����ǵ�ѹ/�����л���
			{
				SET_STATU = FALSE;
				TIMER_EN = FALSE;
				if ( NUM_CHANGE )
				{
					NUM_CHANGE = FALSE;
					eeprom_busy_wait();
					cli();
					eeprom_write_block( (char*)MAX_A,(char*)0x20,6 );
					sei();
				}
				key_press = 0x00;
				KEY_INPUT = 0x00;
				KEY_DOWN_TIME = 0;
				TIMER_NUM = 0;
				delayMS(100);						//��ʱ10MS
			}
		}
		if ( key_press ) 
		{
			KEY_INPUT = KEY_TEMP = key_end;
			if ( !SET_STATU )
			{
				//�����Դ��
				if ( KEY_INPUT == POWER_DOWN )
				{
					POWER_STATE = !POWER_STATE;		//�ı��Դ������״̬
					KEY_INPUT = 0;
					if ( POWER_STATE )
					{
						PORTB |= ( 1 << POWER );	//������Դ
						eeprom_busy_wait();
						cli();
						eeprom_write_byte ( (char*)0x07, 0x01 );
						sei();
					}
					else
					{
						PORTB &= ~( 1 << POWER );	//�رյ�Դ
						eeprom_busy_wait();
						cli();
						eeprom_write_byte ( (char*)0x07, 0x00 );
						sei();
					}
				}	
				//�������/��ѹ�л���Ĭ��״̬����ʾ��ѹ
				else if ( KEY_INPUT == A_V_DOWN )
				{
					KEY_A_V_STATE = !KEY_A_V_STATE;		//�л�AV״̬
					KEY_INPUT = 0;
					if ( KEY_A_V_STATE )
					{
						PORTB |= ( 1 << OUT_A );		//��ʾ����
						eeprom_busy_wait();
						cli();
						eeprom_write_byte ( (char*)0x01, 0x01 );
						sei();
					}
					else
					{
						PORTB &= ~( 1 << OUT_A );		//��ʾ��ѹ
						eeprom_busy_wait();
						cli();
						eeprom_write_byte ( (char*)0x01, 0x00 );
						sei();
					}
				}
				//��������/��ѹ�л���Ĭ��״̬������
				if ( KEY_INPUT == CC_CV_DOWN )
				{
					KEY_CC_CV_STATE = !KEY_CC_CV_STATE;
												//�ı�����/��ѹ������״̬
					KEY_INPUT = 0;
					if ( KEY_CC_CV_STATE )
					{
						PORTB |= ( 1 << OUT_CV );		//��ѹ
						eeprom_busy_wait();
						cli();							
						eeprom_write_byte ( (char*)0x05, 0x01 );		
						sei();
					}
					else
					{
						PORTB &= ~( 1 << OUT_CV );		//����
						eeprom_busy_wait();
						cli();							
						eeprom_write_byte ( (char*)0x05, 0x00 );		
						sei();
					}
				}
				//�����ֶ�/�Զ��л���Ĭ��״̬���ֶ�
				if ( KEY_INPUT == AUTO_MAN_DOWN )
				{
					KEY_AUTO_MAN_STATE = !KEY_AUTO_MAN_STATE;
												//�ı��ֶ�/�Զ�������״̬
					KEY_INPUT = 0;
					if ( KEY_AUTO_MAN_STATE )
					{
						PORTB |= ( 1 << OUT_AUTO );	//�Զ�
						eeprom_busy_wait();
						cli();							
						eeprom_write_byte ( (char*)0x03, 0x01 );		
						sei();
					}
					else
					{
						PORTB &= ~( 1 << OUT_AUTO );	//�ֶ�
						eeprom_busy_wait();
						cli();							
						eeprom_write_byte ( (char*)0x03, 0x00 );		
						sei();
					}
				}
			}
		}
	}
	else
	{
		KEY_INPUT = KEY_TEMP = 0x00;
		KEY_DOWN_TIME = 0;
	}
	if ( SET_STATU )
	{
		CHECK_EDATA_A();
		if ( KEY_INPUT == POWER_DOWN )
		{
			i++;
			TIMER_NUM = 0;
			NUM_CHANGE = TRUE;
			if ( ( i % 25 ) == 0 )
			{
				if ( MAX_A[0] > 0 )
				{
					MAX_A[0]--;
					MAX_A[1] = MAX_A[2] = MAX_A[0];
				}
			}
		}	
		else if ( KEY_INPUT == CC_CV_DOWN  )
		{
			i++;
			TIMER_NUM = 0;
			NUM_CHANGE = TRUE;
			if ( ( i % 25 ) == 0 )
			{
				if ( MAX_A[0] < 2000 )
				{
					MAX_A[0]++;
					MAX_A[1] = MAX_A[2] = MAX_A[0];
				}
				else
				{
					MAX_A[1] = MAX_A[2] = MAX_A[0] = 0;
				}
			}
		}
	}
}

