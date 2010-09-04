/*
 *Fuse config:
 * 	CKSEL: 0111   �ⲿ��������CLKDIV8:1 ==> 10M ����Ĭ��
 *  SUT��  11   16K CK/65ms
 *  low byte:77  high byte: DF  exten byte: f9
 *   
 */
#include "include/bitops.h"
#include "include/avrio.h"
#include <avr/interrupt.h>
#include <util/delay.h>
#include "pwm.h"




#define KEY_PORT PORTD
#define KEYUP  2
#define KEYDOWN  3
#define _key_init() \
			 _pin_mode(KEY_PORT,KEYUP, INPUT);\
			 _pin_mode(KEY_PORT,KEYDOWN, INPUT)


#define _test_key(n) 								\
		if( ! _test_bit(_inb(KEY_PORT), n) ){			\
	    	_delay_ms(7); 							\
	    	if(! _test_bit(_inb(KEY_PORT), n) )		\
				return 1;							\
        }											\
       return 0										\


char key(char key)
{
   _test_key(key);
}

char i=0;

/* 
  0   5 6 7  8...   15
  PWM:000    Detect...
  0000mA        0000mV
       5        10
*/





#define LED_PORT PORTD
#define LED_On(i)  _clear_bit(LED_PORT,i)	/* ����͵�ƽ��ʹLED���� */
#define LED_Off(i) _set_bit(LED_PORT,i)	/* ����͵�ƽ��ʹLED���� */

char mod=2;
void sharp_flash()
{
    char x;

    for(x=0;x<19;x++){
   	   //LED_On(0);
	   //LED_On(1);
	   //LED_On(2);
       
	   //LED_On(3);
       //LED_On(4);
       //LED_On(5);
       //LED_On(6);
       LED_On(7);
       

       _delay_ms(100);
	 

	   //LED_Off(0);
 	   //LED_Off(1);
	   //LED_Off(2);
	   //LED_Off(3);
	   //LED_Off(4);
	   //LED_Off(5);
	   //LED_Off(6);
	   LED_Off(7);
	   	_delay_ms(100);
	}
   mod++;
}
int main()
{

    char force_stop = 0;
	char pwm =0;
	
_pins_mode(LED_PORT,0,7,OUTPUT);//_nm8(0b11,0,1);
	

sharp_flash();
    cli();

	timer0_init();
	sharp_flash();

	enable_timer0();
    sei();


    

	
	while(1){
	
	   sharp_flash();
	
	
	}

#if 0

    _key_init();

	pwm_init();
	pwm_setduty(0);
    adc_init();

	init_ic(&charger);

	lcd1602_init();




	while (1){

	}
#endif
}