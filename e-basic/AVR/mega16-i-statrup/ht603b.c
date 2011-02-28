/*
 * buy from taobao:
 * http://item.taobao.com/item.htm?id=2971207095
 * CopyLeft hyl 2009.12.19
 */

#include "include/avrio.h"
#include <util/delay.h>



#define HT_PORT PORTB
#define SDA 0
#define SC1 1
#define BLS 2


#define ssda  _set_bit(HT_PORT,SDA)	/* ����͵�ƽ��ʹLED���� */
#define csda   _clear_bit(HT_PORT,SDA)	/* ����͵�ƽ��ʹLED���� */
#define sscl  _set_bit(HT_PORT,SC1)	/* ����͵�ƽ��ʹLED���� */
#define cscl   _clear_bit(HT_PORT,SC1)	/* ����͵�ƽ��ʹLED���� */

#define io_delay() _delay_us(10)
//#define sback_light_switch  _clear_bit(HT_PORT,BLS)	/* ����͵�ƽ��ʹLED���� */
//#define cback_light_switch  _set_bit(HT_PORT,BLS)	/* ����͵�ƽ��ʹLED���� */

ht1603_init()
{
   _pins_mode(HT_PORT,SDA,BLS,OUTPUT);//_nm8(0b11,0,1);
}


// 0 --f �ı�����
#if 1
unsigned char num_table[16]={0xd7,0x03,0xb6,0xa7,0x63,0xe5,0xf5,0x07,0xf7,0xe7,0x77,0xf1,0xd4,0xb3,0xf4,0x74};

#else
unsigned char num_table[17]={0x73,0xf4,0xd0,0xd0,0xd7,0x00,0x00,0x00,
         
      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        
      0x00}; 
#endif
//��ʱ����======��ʱ2*t��������=====
void delay_char(unsigned char t)
{
    _delay_ms(t);
}
//===================================

 

/*============дһ���ֽ�=======================================================
ʱ��: �����ش������� ,scl = 0ʱ���ܸı�
sda ������,������ڰ˸�λ��,��Ҫ����һ��scl
==============================================================================*/
void write(unsigned char  dat)
{
    unsigned char i,d;
       
    for(i = 0;i < 8; i++)
    {
        cscl;
		io_delay() ;
        //sda = (dat << i) & 0x80;
		d = (dat << i) & 0x80;
		if(d)
		  ssda;
        else
		  csda;
		  io_delay() ;
		sscl;
    }
   
    cscl ;
	io_delay() ;
	cscl;
	io_delay() ;
    sscl ;
	io_delay() ;
	sscl;
	io_delay() ;
	sscl;
	io_delay() ;
    cscl ; 
}
//============================

 


/*======��ʼ�ź�=============
ʱ��:scl = 1 ʱsda �����½���
=============================*/
void start(void)
{
    ssda;
	io_delay() ;
    sscl;
	io_delay() ;
    csda;
	io_delay() ;
    cscl;
	io_delay() ;
    write(0x70);
}
//==============================



/*======�����ź�================
ʱ��:scl = 1 ʱ sda ����������
===============================*/
void stop(void)
{
    csda;
		io_delay() ;
    sscl;
		io_delay() ;
    ssda;
		io_delay() ;

}
//================================


//==============================

/*====�����Һ����ʾ����=======
�β�: float dat ---����ʾ������
����:��ʾ float dat ����ֵ��λС����ֵ��ΧΪ[-99999.99 , 99999.99];
================================*/
void display(float dat)
{
    unsigned long  temp;
    char i,sum;
    unsigned char num[8]= {0};  //num[]�洢7λʮ��������num[7]Ϊ����λ,lengthΪ��Ч����λ��
  
  
    if(dat < 0)
    {
        dat = - dat;
        num[7] = 0x20;  //����λΪ -
    }

    else
    {
        num[7] = 0;     //����λΪ +
    }


       
        start();
        write(0x48);    //ģʽ����
        stop();
   
        start();
        write(0xe0);    //�豸ѡ��
        write(0);        //����ָ������
     //   stop();
       // start();
          

    if(dat < 100000 ) //ֻ����ʾ2λС��,5λ����
    {
       
        temp = 100 * dat;       //ȡ�ø���λ���ϵ���ֵ
        num[6] = temp / 1000000;
        num[5] = temp / 100000 %10;
        num[4] = temp / 10000 %10;
        num[3] = temp / 1000 %10;
        num[2] = temp / 100 %10;
        num[1] = temp / 10 %10;
        num[0] = temp %10;
       
        sum = 0;
        for(i = 6;i >2;i--) //��������������Чλ����ʼλ
        {
            sum = num[i] + sum;
            if(sum > 0) break;
        }
        sum = i;    //sum����Чλ����ʼλ��ʾʱ��num[sum]��ʼ��ʾ����
       
        for(i = 0;i <= sum;i++)   //����ÿλ��Чλ���ֵı�����
        {
            num[i] = num_table[num[i]];
        }
            num[sum + 1] = num[7];  //����λ
            num[2] = num[2] | 0x08; //���С����
           
       
        for(i = 0; i < 14-sum; i++)  //ǰ1��15-sum��λ�ò���
        {
            write(0);
        }
      
        for(i = sum +1; i >= 0 ;i--) //��num[sum + 1]��ʼ��ʾ
        {
            write(num[i]);
        }
    }

    else    //���ݳ�����ʾ��Χ������Ϊ��ʾ����
    {
        for(i = 0; i < 16; i++)
        {
            write(0x20);
        }   
    }
    write(0);           //��17���ֽ�д0,������ʾ���Ͻǵķ���
    stop();      //����
}
//================================



#if 0
/*============дһ���ֽ�=======================================================
ʱ��: �����ش������� ,scl = 0ʱ���ܸı�
sda ������,������ڰ˸�λ��,��Ҫ����һ��scl
==============================================================================*/
void write(unsigned char  dat)
{
    unsigned char i,d;
      
    for(i = 0;i < 8; i++)
    {
        cscl; //= 0;
        //sda = (dat << i) & 0x80;
		d = (dat << i) & 0x80;
		if(d)
		  ssda;
        else
		  csda;
		HT_PORT |=  (  (((dat << i) & 0x80)>>7) <<SDA);
        sscl ;//= 1;
    }
  
    cscl;// = 0;
	cscl ;//= 0;
    sscl ;//= 1;
	sscl;// = 1;
	sscl ;//= 1;
    cscl;// = 0;
}
//============================

 


/*======��ʼ�ź�=============
ʱ��:scl = 1 ʱsda �����½���
=============================*/
void start(void)
{
    ssda;// = 1;
    sscl ;//= 1;
    csda;// = 0;
    cscl ;//= 0;
    write(0x70);           //�����֣������������
}
//==============================
  

 

/******************************************************/


//����Ҷ���������ĳ��������ֲ,ֻҪ����һ��IO������
//������ֲ������оƬ��AVR PIC 51 ARM HT STC C8051F��

//��I2C������ʽ��������


// Һ������Ϊ17���ֽڣ������������ϱ������Ϊ��ʾHELLO�Ķ���

//                                 H     E    L    L    0

unsigned char num_table[17]={0x73,0xf4,0xd0,0xd0,0xd7,0x00,0x00,0x00,
         
      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        
      0x00}; 

void stop(void);
void start(void);
void display(void);
void write(unsigned char  dat);
void delay_char(unsigned char t);


void main()
{


   while(1)display();
 
}



 

/*============дһ���ֽ�=======================================================
ʱ��: �����ش������� ,scl = 0ʱ���ܸı�
sda ������,������ڰ˸�λ��,��Ҫ����һ��scl
==============================================================================*/
void write(unsigned char  dat)
{
    unsigned char i,d;
      
    for(i = 0;i < 8; i++)
    {
        cscl; //= 0;
        //sda = (dat << i) & 0x80;
		d = (dat << i) & 0x80;
		if(d)
		  ssda;
        else
		  csda;
		HT_PORT |=  (  (((dat << i) & 0x80)>>7) <<SDA);
        sscl ;//= 1;
    }
  
    cscl;// = 0;
	cscl ;//= 0;
    sscl ;//= 1;
	sscl;// = 1;
	sscl ;//= 1;
    cscl;// = 0;
}
//============================

 


/*======��ʼ�ź�=============
ʱ��:scl = 1 ʱsda �����½���
=============================*/
void start(void)
{
    ssda;// = 1;
    sscl ;//= 1;
    csda;// = 0;
    cscl ;//= 0;
    write(0x70);           //�����֣������������
}


#endif
