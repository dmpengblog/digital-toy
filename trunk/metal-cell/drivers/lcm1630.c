/*
������:
http://www.lcdmk.com/tbpic/pcf8576.pdf
LCD:
http://www.lcdmk.com/tbpic/LCM1603.pdf

*/

#include <AT89X52.H>
#include <intrins.h>  /* _nop_ */
#include <timer.h>
#include <bitops.h>
#include <drivers/lcd.h>



sbit   sda = P3^6;
sbit   scl = P3^7;
sbit   back_light_switch = P1^2;

// 0 --f �ı�����
unsigned char code num_table[16]={0xd7,0x03,0xb6,0xa7,0x63,0xe5,0xf5,0x07,0xf7,0xe7,0x77,0xf1,0xd4,0xb3,0xf4,0x74};



 

/*============дһ���ֽ�=======================================================
ʱ��: �����ش������� ,scl = 0ʱ���ܸı�
sda ������,������ڰ˸�λ��,��Ҫ����һ��scl
==============================================================================*/
void write(unsigned char  dat)
{
    unsigned char i;
       
    for(i = 0;i < 8; i++)
    {
        scl = 0;
        sda = (dat << i) & 0x80;
        scl = 1;
    }
   
    scl = 0;scl = 0;
    scl = 1;scl = 1;scl = 1;
    scl = 0; 
}
//============================

 


/*======��ʼ�ź�=============
ʱ��:scl = 1 ʱsda �����½���
=============================*/
void start(void)
{
    sda = 1;
    scl = 1;
    sda = 0;
    scl = 0;
    write(0x70);
}
//==============================


/*======�����ź�================
ʱ��:scl = 1 ʱ sda ����������
===============================*/
void stop(void)
{
    sda = 0;
    scl = 1;
    sda = 1;

}
//================================

 

 

/*====�����Һ����ʾ����=======
�β�: float dat ---����ʾ������
����:��ʾ float dat ����ֵ��λС����ֵ��ΧΪ[-99999.99 , 99999.99];
================================*/
void lcm1630_display(float dat)
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
      //  stop();
     //   start();
          

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

 

void lcm1630_demo()
{
   float i;
   unsigned char k;

 //   while(1)display(-99999.99);
  
   for(i = -1.10;i<1.5;i = i + 0.01)
   {
            lcm1630_display(i);
			_delay_ms(10);
   }
   
    
}




/*====�����Һ����ʾ����=======

//���ݸ�ʽwrite(0x48)-->stop()-->start()-->write(0xe0)-->write(0)-->
//{17���ֽ����ݽ��з���}-->write(0)-->stop()���������ʾ����

================================*/
// Һ������Ϊ17���ֽڣ������������ϱ������Ϊ��ʾHELLO�Ķ���

//                                 H     E    L    L    0
unsigned char code Hello[17]={0x73,0xf4,0xd0,0xd0,0xd7,0x00,0x00,0x00,
         
      			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}; 



void lcm1630_demo2(void)
{

    char i;

        
    start();
    write(0x48);    //ģʽ���������֣������������
    stop();
  
    start();
    write(0xe0);    //�豸ѡ��
    write(0);        //����ָ������

    for(i = 0; i <16; i++)
    {
        write(Hello[i]|0x8);
    }  
 
    write(0xff);      
    stop();      //����
}
 

 
 
