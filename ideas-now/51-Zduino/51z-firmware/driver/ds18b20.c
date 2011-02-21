/*
 *  DS18B02 opration lib
 *  hyl 2011.2.21
 */


#include <AT89X52.H>
#include <intrins.h>  /* _nop_ */
#include <math.h>  /*abs*/
#include <config.h>
#include <1602.h>
#include <delay.h>
#include <bitops.h>

sbit ds = P1^3;


//��ʼ��DS18B20
 //��DS18B20һ����Գ�ʱ��͵�ƽ, Ȼ��һ����Էǳ���ʱ��ߵ�ƽ, ��������
 void ds18b20_init()
 {
   //����11.0592MHzʱ��, unsigned int�͵�i, ��һ��i++������ʱ�����?us
   unsigned int i;
   ds = 0;
   i = 19000;   //����Լ800us, ����Э��Ҫ���480us����
   while(i-->0)_nop_;
   ds = 1;    //����һ��������, ����ȴ�Ӧ��״̬
   i = 1200;
   while(i>0) i--;
 }
 
 void dsWait()
 {
    unsigned int i=1000;
    while(ds);
	i=1000;
    while((~ds));  //��⵽Ӧ������
    i = 4;
    while(i > 0) i--;
}

//��DS18B20��ȡһλ����
//��һλ, ��DS18B20һС���ڵ͵�ƽ, Ȼ����С���ڸߵ�ƽ,
//֮��DS18B20����������һ��ʱ���һλ����
bit readBit()
{
    unsigned int i;
   bit b;
   ds = 0;
  i++;   //��ʱԼ8us, ����Э��Ҫ�����ٱ���1us
  ds = 1;
  i++; i++;  //��ʱԼ16us, ����Э��Ҫ���������ʱ15us����
   b = ds;
    i = 8;
    while(i>0) i--;  //��ʱԼ64us, ���϶�ʱ϶������60usҪ��
   return b;
}

//��ȡһ�ֽ�����, ͨ������readBit()��ʵ��
unsigned char readByte()
{
   unsigned int i;
     unsigned char j, dat;
    dat = 0;
   for(i=0; i<8; i++)
   {
        j = readBit();
       //���ȶ����������λ����
       dat = (j << 7) | (dat >> 1);
    }
    return dat;
}

//��DS18B20д��һ�ֽ�����
void writeByte(unsigned char dat)
{
 unsigned int i;
   unsigned char j;
    bit b;
  for(j = 0; j < 8; j++)
   {
       b = dat & 0x01;
       dat >>= 1;
       //д"1", ��DQ����15us��, ��15us~60us�ڽ�DQ����, �����д1
        if(b) 
       {
            ds = 0;
           i++; i++;  //����Լ16us, ����Ҫ��15~60us��
            ds = 1;  
           i = 8; while(i>0) i--;  //��ʱԼ64us, ����дʱ϶������60usҪ��
       }
      else  //д"0", ��DQ����60us~120us
           ds = 0;
            i = 8; while(i>0) i--;  //����Լ64us, ����Ҫ��
            ds = 1;
           i++; i++;  //����д0ʱ϶�����Ѿ�����60us, ����Ͳ�����д1����, ����ʱ64us��
     
    }
}

//��DS18B20�����¶�ת������
void sendChangeCmd()
{
   ds18b20_init();    //��ʼ��DS18B20, ����ʲô����, ���ȶ�Ҫ�����ʼ��
   dsWait();   //�ȴ�DS18B20Ӧ��
   mdelay(1);    //��ʱ1ms, ��ΪDS18B20������DQ 60~240us��ΪӦ���ź�
   writeByte(0xcc); //д���������к������� Skip Rom
   writeByte(0x44); //д���¶�ת�������� Convert T
}

//��DS18B20���Ͷ�ȡ��������
void sendReadCmd()
{
   ds18b20_init();
   dsWait();
   mdelay(1);
   writeByte(0xcc); //д���������к������� Skip Rom
   writeByte(0xbe); //д���ȡ�������� Read Scratchpad
}

//��ȡ��ǰ�¶�ֵ
int getTmpValue()
{
    unsigned int tmpvalue=0;
    int value; //����¶���ֵ
    float t;
    unsigned char low, high;
    sendReadCmd();
    //������ȡ�����ֽ�����
    low = readByte();
    high = readByte();
    //���ߵ������ֽںϳ�һ�����α���
    //������ж��ڸ��������ò�������ʾ��
    //���Ǹ�ֵ, ��ȡ��������ֵ���ò����ʾ��, ��ֱ�Ӹ�ֵ��int�͵�value
    tmpvalue = high;
    tmpvalue <<= 8;
    tmpvalue |= low;
    value = tmpvalue;
 
    //ʹ��DS18B20��Ĭ�Ϸֱ���12λ, ��ȷ��Ϊ0.0625��, ���������ݵ����λ����0.0625��
    t = value * 0.0625;
    //�����Ŵ�100��, ʹ��ʾʱ����ʾС�������λ, ����С������������4��5��
   //��t=11.0625, ���м�����, �õ�value = 1106, ��11.06 ��
   //��t=-11.0625, ���м�����, �õ�value = -1106, ��-11.06 ��
    value = t * 100 + (value > 0 ? 0.5 : -0.5); //����0��0.5, С��0��0.5
    return value;
}


#define lcd_putc(c) hd44870_send(c,0)
char hex2c(char hex) ;

void display(int n)
{
    
	 int c;
	 char show=0;
     //irqoff();
	 c= hex2c((n/100000));
	 n = n%100000;
     
	 if(c||show){ lcd_putc(c); show =1;}

	 c= hex2c((n/10000));
	 n = n%10000;
     
	 if(c||show){ lcd_putc(c); show =1;}

	 c= hex2c(n/1000);
	 n = n%1000;
	 if(c||show) { lcd_putc(c); show =1;}	 
		   	
	 c= hex2c( (n/100) );
	 n = n%100;
	 if(c||show) { lcd_putc(c); show =1;}	 
	
	 c= hex2c( (n/10) );
     lcd_putc(c);	 

	 c= hex2c( (n%10) );;
	 lcd_putc(c);	 
  
}

ds18b20_demo()
{
    lcd_cursor(0,1);

	display(getTmpValue());
	
	
}

