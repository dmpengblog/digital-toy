
#ifndef _DELAY_H_
#define _DELAY_H_



#define F_CPU 12000000UL

#ifndef F_CPU
#warning "F_CPU not defined for delay.h"
#define F_CPU 12000000UL
#endif



void _2cycle(unsigned char cyc);


/*С��6us���ز�׼ȷ, �����ǳ�׼ȷ,ż�����Լ(+ -)1us(����) */
/*#define udelay(t)  _2cycle((t/2)-2) С��6��׼��Ϊ t/2-2 �ε���*/


/*7-512us*/
#define udelay(t)  _2cycle(  ((t>>1)&0xFE) +1 ) //_2cycle can't accept 0
//#define udelay(t)  _2cycle((t/2)-2) /* _2cycle( ((t>>1)&0xFC) )*/



/*
 * always use the constant of __us
 * 1-255 cycles, 2-510us at 12Mhz
 */
#define _xdelay_us(__us)  /* usage: _delay_us(8), wrong: i=0; _delay_us(i);*/ \
do{ \
    unsigned char __ticks; \
	double __tmp = ((F_CPU)/24e6)*__us##UL;  /*24Mhz for 1us ticks*/				\
	if(__tmp<1.0)  																	\
		__ticks=1; 																	\
    else if (__tmp>255)																\
		__ticks = 0;																\
	else 																			\
		__ticks = (unsigned char)__tmp;											    \
    /*12Mhz 2us per ticks*/															 \
    while(--__ticks);	 /*while(--cyc) have bug when cyc==0, but we must use --cyc to genarate one inst*/\
    /* DFFE     DJNZ     R7,delay(C:0014), 2cycle*/														 \
} while(0)


#define _delay_us(__us)  /* usage: _delay_us(8), wrong: i=0; _delay_us(i);*/ \
do{ \
	char ticks = __us<<2;	   \
    /*12Mhz 2us per ticks*/															 \
    while(--ticks);	 /*while(--cyc) have bug when cyc==0, but we must use --cyc to genarate one inst*/\
    /* DFFE     DJNZ     R7,delay(C:0014), 2cycle*/														 \
} while(0)



/*
 * mdelay(0) : 15 cycle, 12Mhz 30us
 * 1ms to 256ms
 *  +- 50us
 */
void mdelay(unsigned char ms);




/*
 * +- 50ms
 */
void sleep(unsigned char t);

#endif

