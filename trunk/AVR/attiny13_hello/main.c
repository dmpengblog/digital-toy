#include <avr/io.h>
#include <avr/delay.h>



void LED_On(int i)
{
  	 PORTB &=~(1<<i);	/* ����͵�ƽ��ʹLED���� */
}

void sharp_flash()
{
    char x;
    PORTB = 0xff;  

	static char i=1;

    for(x=0;x<7;x++){
       LED_On(i&1?0:7);
	   LED_On(i&2?1:7);
	   LED_On(i&4?2:7);

       _delay_ms(10);
	   PORTB = 0xff;
	   	_delay_ms(10);
	}

    i++;
	if(i>=7) 
	  i=1;  
    
}

void led_test()
{
	/* ��ǰ���� */
    char i;
		for (i = 0; i < 3; i++){

			PORTB = 0;
			_delay_ms(200);
			PORTB = 0xff;
			_delay_ms(200);
		}

	

}	

int main()
{

	int i;

	DDRB = 0xFF;	/* ����B��Ϊ���*/

	PORTB = 0xFF;	/* �ر�ȫ��LED */

    led_test();
	

	while (1){
          sharp_flash();			
          
	}
}
