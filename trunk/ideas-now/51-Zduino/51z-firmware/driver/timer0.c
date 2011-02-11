
#include  "config.h"
#include  "timer.h"

/*
 * timer0, 
 *   1) 1ms ticks
 *	 2) tickless driver 
 */

volatile unsigned  long jiffers=0;  //250us per jiffers





/*
 * Timer0: 256us, display driven ticks
 */
/*�ж�1�� �Ĵ�����1 (��ͨ����Ĭ���üĴ�����0)*/


#define settimer1(us) TH1 = (65535 - (us))>>8 ; /* (655535-us)/256 ����!!!!*/  \
                      TL1 = (65535 - (us))&0xFF ; 
 
				 

void timer1(void) interrupt 3 using 1   
{   
 	 // ms_scan_segvled(); //ɨ����ʾ, use LCD disable this
		 
	 jiffers++;
	 settimer1(MS_CYCLE);
}

 
void timer1_init()
{
  /* timer0 init */
   /*  timer 0 in mode 1   */
   TMOD |= 0x10;   
   // set timer speed   
   settimer1(MS_CYCLE);   	//per 250us

  
   // enable timer 1 interrupt   
   ET1 = 1;   
   
   TR1 = 1; //enable timer

}


