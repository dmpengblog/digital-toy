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


#define DS18B20_CONVERT_TEMP     0x44
#define DS18B20_READ_SCRATCHPAD  0xBE

#define DS18B20_SKIPROM    0xCC

sbit ds = P1^3;
	  
/*12Mhz for 51, 1us one cycle*/
#define _udelay  udelay


//��ʼ��DS18B20
 //��DS18B20һ����Գ�ʱ��͵�ƽ, Ȼ��һ����Էǳ���ʱ��ߵ�ƽ, ��������
 void dallas_reset()
 {
   ds = 0;
   _udelay(480);
   
   ds = 1;    //pull up and prepare receive 
   _udelay(70);  


   if(0==ds);
   else
   { hd44870_send('!',0);}
    
    _udelay(410);

#if 1 
	//check bus
	ds=1;
	if(ds==1);//ok
	else  hd44870_send('@',0);
#endif

 }

bit dallas_read_bit()
{
   unsigned char b=0;
   ds = 0;
   //delay for reading bit
   _udelay(6);
   ds = 1;	//relase bus
   
   //delay appropiate time for read
   _udelay(9);
   if(ds)b=1;

   //finish bit slot
   _udelay(55);
    return b;
}

void dallas_write_bit(unsigned char b)
{
   ds = 0;
   //delay for reading bit
   _udelay(6);
   if(b)
      udelay(6);
    else
	   udelay(60);
   	
	ds = 1;	//relase bus
   
    if(b) 
	   udelay(64);
	else
	   udelay(10);
}


unsigned char dallas_read_byte()
{
   unsigned char i;
   unsigned char byte=0;
   for(i=0; i<8; i++)
   {
       if( dallas_read_bit())
	     byte |= 0x01<<i;
    }
    return byte;
}

//��DS18B20д��һ�ֽ�����
void writeByte(unsigned char byte)
{
   unsigned char i;
   for(i=0; i<8; i++){
       dallas_write_bit((byte>>i)&0x01);
	   //delay for each wirte
	   udelay(1);
    }
}

//��DS18B20�����¶�ת������
void ds18b20_start()
{
   dallas_reset();    //��ʼ��DS18B20, ����ʲô����, ���ȶ�Ҫ�����ʼ��
   writeByte(DS18B20_SKIPROM);
   writeByte(DS18B20_CONVERT_TEMP); //д���¶�ת�������� Convert T

   while(!dallas_read_bit());//wait it done
}


//��ȡ��ǰ�¶�ֵ
int getTmpValue()
{
    int value; //����¶���ֵ
    unsigned char low, high;
    
	dallas_reset();
    writeByte(DS18B20_SKIPROM);
    writeByte(DS18B20_READ_SCRATCHPAD); //д���ȡ�������� Read Scratchpad;
    //������ȡ�����ֽ�����
    low  = dallas_read_byte();
    high = dallas_read_byte();
    //���ߵ������ֽںϳ�һ�����α���
    //������ж��ڸ��������ò�������ʾ��
    //���Ǹ�ֵ, ��ȡ��������ֵ���ò����ʾ��, ��ֱ�Ӹ�ֵ��int�͵�value
    value = high;
    value <<= 8;
    value |= low;
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
    unsigned int t;
    lcd_cursor(0,1);
//	while(1)dallas_reset();
	ds18b20_start();
	t=getTmpValue();
	display(t>>4);//
	lcd_putc('.');
	display(t&0xF);//
	
	
}

