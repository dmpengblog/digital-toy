#include "delay.h"
#include "1602.h"

#include <AT89X52.H>			 

#define io_delay mdelay(8)
/***********************************************
�������ƣ�1602Һ����ʾ����
��Ҫ˵����ʹ��FM1602Һ����ʾ�������ݿڽ�P0�ڣ����ƶ˽�P2.0-P2.2
�Ĵ���ѡ���źŶ�RS-P2.0����д�źŶ�R/W-P2.1��ʹ���źŶ�EP-P2.2
��    д��JiangX.net
***********************************************/
typedef unsigned char BYTE;
typedef unsigned int WORD;
typedef bit BOOL ;

sbit rs = P2^0;   //RS
sbit rw = P2^1;   //��дѡ��RW
sbit ep = P2^2;   //ʹ���ź�E

BYTE code dis1[] = {"Digital-Toy"};
BYTE code dis2[] = {"i-Charger V1.0"};


/*****�б�LCDæµ״̬*****/
BOOL lcd_bz()
{
	BOOL result;
	rs = 0;
	rw = 1;
	ep = 1;
	result = (BOOL)(P0 & 0x80);
	ep = 0;
	return result;
}
/*****д��ָ�����ݵ�LCD*****/
void lcd_wcmd(BYTE cmd)
{
  while(lcd_bz());
  rs = 0;
  rw = 0;
  ep = 0;
  udelay(100);
  P0 = cmd;

  ep = 1;
  ep = 0;  
}

/*****�趨��ʾλ��*****/
void lcd_start(BYTE start)
{       
   lcd_wcmd(start | 0x80);
}

/*****д���ַ���ʾ���ݵ�LCD*****/
void lcd_data(BYTE dat)
{       
  while(lcd_bz());
  rs = 1;
  rw = 0;
  ep = 0;
  P0 = dat;
  ep = 1;
  ep = 0;
}

/*****LCD��ʼ���趨*****/
void lcd_init()
{
    io_delay;      
	lcd_wcmd(0x38);    //�趨LCDΪ16*2��ʾ��5*7����8λ���ݽӿ�
	io_delay;
	lcd_wcmd(0x0c);    //����ʾ������ʾ���
	io_delay;
	lcd_wcmd(0x06);    //��ʾ����Զ����ƣ��������ƶ�
	io_delay;
	lcd_wcmd(0x01);    //��ʾ����
	io_delay;

	
}


void lcd_logo()
{
  int pos=0,i=0;
  lcd_start(0);    // ������ʾλ��Ϊ��һ�еĵ�5���ַ�
  io_delay;
  while(pos < (sizeof(dis1)-1)){
     lcd_data(dis1[i]); // ��ʾ�ڶ����ַ�
     i++;pos++;
     io_delay;
  }   

  pos=0;i=0;
  lcd_start(0x40);   // ������ʾλ��Ϊ�ڶ��е�һ���ַ�
  while(pos<(sizeof(dis2)-1)){
     lcd_data(dis2[i]); // ��ʾ�ڶ����ַ�
     i++;pos++;
     io_delay;
  }   


}
#if 0

/*pos: 0-15, 16-31*/
void lcd_puts(char pos, char *str)
{
   if(pos>15)
		lcd_start(0x40|(pos-16));
   else 
		lcd_start(pos);

   pos=0;
   while(str[pos]){
     lcd_data(str[pos]);
	 pos++; 
   }
    
}


void demo_1602()
{
  BYTE i=0;
  char pos=0;

  lcd_init();    // ��ʼ��LCD   
  mdelay(200);

again: 
  pos=0; 
  lcd_start(0);    // ������ʾλ��Ϊ��һ�еĵ�5���ַ�
  io_delay;
  while(pos<15){
     lcd_data(i); // ��ʾ�ڶ����ַ�
     i++;pos++;
     io_delay;
  }   

  pos=0;
  lcd_start(0x40);   // ������ʾλ��Ϊ�ڶ��е�һ���ַ�
  while(pos<15){
     lcd_data(i); // ��ʾ�ڶ����ַ�
      i++;pos++;  
     io_delay;
  }   
  
  
  
  goto again;
}

#endif

