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


void ds_delay(unsigned char i)
{
  while(i--);

}
	  
/*12Mhz for 51, 1us one cycle*/
//#define _udelay _2cycle


//��ʼ��DS18B20
 //��DS18B20һ����Գ�ʱ��͵�ƽ, Ȼ��һ����Էǳ���ʱ��ߵ�ƽ, ��������
 void dallas_reset()
 {

   cli();

   ds = 0;
   _delay_us(480+20); //master pull low at least 480uS
   
   ds = 1;        //relase bus and prepare receive 
   _delay_us(50); //wait 16-60uS,(simulation:after 40uS, slave pull low)
     
   //Read the slave : slave pull low from 40uS ->40us+120us
   if(0==ds);
   else { hd44870_send('!',0);}
    
   _delay_us(480);

   sti();
   
   //check bus
	ds=1;

	if(ds==1);//ok
	else  hd44870_send('@',0);
}

/*60us for one read time slot*/
bit dallas_read_bit()
{
   unsigned char b=0;

   ds = 0;    //master pull low to read
   _delay_us(7); 
   
   ds = 1;	//relase bus

   //delay appropiate time for read
   _delay_us(9); 
    
   if(ds)
       b=1;

   //finish bit slot
   _delay_us(55);
         
   return b;
}

void dallas_write_bit(unsigned char b)
{
    ds = 0;
   //delay for write bit
   	if(b)
       _delay_us(7);
	else
	   _delay_us(60);
   	
	ds = 1;	//relase bus
   
    if(b) 
	   _delay_us(64);
	else
	   _delay_us(9);
}


unsigned char dallas_read_byte()
{
 
   unsigned char i;
   unsigned char byte=0;
   
   cli();
   for(i=0; i<8; i++)
   {
       if( dallas_read_bit())
	     byte |= 0x01<<i;
		 _nop_();
    }
   sti();
   return byte;
}

//��DS18B20д��һ�ֽ�����
void dallas_write_byte(unsigned char byte)
{
   unsigned char i;
   cli();
   for(i=0; i<8; i++){
       dallas_write_bit((byte>>i)&0x01);
	   //delay for each wirte
	   _nop_();
    }
   sti();
}

//��DS18B20�����¶�ת������
void ds18b20_start()
{
   dallas_reset();    //��ʼ��DS18B20, ����ʲô����, ���ȶ�Ҫ�����ʼ��
   dallas_write_byte(DS18B20_SKIPROM);
   dallas_write_byte(DS18B20_CONVERT_TEMP); //д���¶�ת�������� Convert T

   while(!dallas_read_bit());//wait it done
}


//��ȡ��ǰ�¶�ֵ
int getTmpValue()
{
    int value; //����¶���ֵ
    unsigned char low, high;
    
	dallas_reset();
    dallas_write_byte(DS18B20_SKIPROM);
    dallas_write_byte(DS18B20_READ_SCRATCHPAD); //д���ȡ�������� Read Scratchpad;
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
     
	 if((c!='0')||show){ lcd_putc(c); show =1;}

	 c= hex2c((n/10000));
	 n = n%10000;
     
	 if((c!='0')||show){ lcd_putc(c); show =1;}

	 c= hex2c(n/1000);
	 n = n%1000;
	 if((c!='0')||show) { lcd_putc(c); show =1;}	 
		   	
	 c= hex2c( (n/100) );
	 n = n%100;
	 if((c!='0')||show) { lcd_putc(c); show =1;}	 
	
	 c= hex2c( (n/10) );
     if((c!='0')||show) { lcd_putc(c); show =1;}	 
	
	 c= hex2c( (n%10) );;
	 lcd_putc(c);	 
  
}

ds18b20_demo()
{
    unsigned int t;
    lcd_cursor(0,1);
	//while(1){ds18b20_start();_delay_ms(1);};
	//while(1){ ds=1; _delay_us(7);ds=0; _delay_us(7); };
	ds18b20_start();
	t=getTmpValue();
	display((int)t>>4);//
	lcd_putc('.');
	display(((int)t)&0xF);//
	
	
}

