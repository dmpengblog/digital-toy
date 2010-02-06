
#include "bitops.h"
#include <avr/io.h>
#include <util/delay.h>

#define LED_On(i)  _clear_bit(PORTB,i)	/* ����͵�ƽ��ʹLED���� */
#define LED_Off(i) _set_bit(PORTB,i)	/* ����͵�ƽ��ʹLED���� */




void led_init()
{
	/* ��ǰ���� */
    char i;
		for (i = 0; i < 3; i++){

			PORTB = 0;
			_delay_ms(200);
 		    LED_Off(1);
 		    LED_Off(2);
			_delay_ms(200);
		}

}	


char mod=2;


void sharp_flash()
{
    char x;

    for(x=0;x<7;x++){
       //LED_On(mod&1?0:7);
	   LED_On(mod&2?1:7);
	   LED_On(mod&4?2:7);

       _delay_ms(10);
	   //LED_Off(0);
	   LED_Off(1);
 	   LED_Off(2);
	   	_delay_ms(10);
	}
   
}
