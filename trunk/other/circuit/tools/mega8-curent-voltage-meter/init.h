
/*

  �Զ���ͷ�ļ�.����һЩȫ�ֱ����ͺ���
  
* Copyright (c) 2005, ������Դ������˾������
* All rights reserved.
* �ļ����ƣ�init.h

* ��ǰ�汾�� 1.0
* ���ߣ�	 �ź�ǿ
* ������ڣ� 2005.7.24
*/
#ifndef __INIT_h
#define __INIT_h

#ifdef   OS_GLOBALS
#define  OS_EXT
#else
#define  OS_EXT  extern
#endif

#define TRUE     1
#define FALSE    0

#define POWER_DOWN    0x01
#define A_V_DOWN      0x02
#define CC_CV_DOWN    0x04
#define AUTO_MAN_DOWN 0x08

#define POWER	  	4
#define OUT_AUTO	5
#define OUT_CV		6
#define OUT_A		7

#define NODOT		64
#define ONEDOT		1
#define TWODOT		2
#define THRDOT		3


//****************��������ÿ̨���ľ�������޸�����ֵ******************//

//#define MAX_A    150				//���屾�������������ֵ,��λΪ����
#define MAXV     20					//���屾���������ѹֵ,��λΪ����
#define MAX_V    MAXV*100	

//********************************************************************//


void init_devices(void);								//��ʼ���˿ں���
void key_board(void);									//���̺���							
void display(unsigned char DISP_DATA[4],unsigned char DOT);
														//��ʾ����
							
void delayMS(unsigned char count);					//����ʱ����		
OS_EXT volatile unsigned int MAX_A[3];
OS_EXT volatile float   adc_data[2];				//ADת���õ�������
OS_EXT volatile unsigned int adc_data_temp;

OS_EXT volatile unsigned int TIMER_NUM; 
//OS_EXT volatile unsigned int TIMER_PERS; 
OS_EXT volatile unsigned char TIMER_EN; 

OS_EXT unsigned char SET_STATU;
OS_EXT unsigned char KEY_TEMP	  	__attribute__ ((section (".noinit")));
OS_EXT unsigned char KEY_INPUT	  	__attribute__ ((section (".noinit")));

OS_EXT unsigned char KEY_A_V_STATE	     __attribute__ ((section (".noinit")));
OS_EXT unsigned char POWER_STATE 	     __attribute__ ((section (".noinit")));	
OS_EXT unsigned char KEY_CC_CV_STATE	 __attribute__ ((section (".noinit")));
OS_EXT unsigned char KEY_AUTO_MAN_STATE __attribute__ ((section (".noinit")));

OS_EXT unsigned int UP_DOWN_POWER      __attribute__ ((section (".noinit")));	

#endif 