
#ifndef _DELAY_H_
#define _DELAY_H_

void _2cycle(unsigned char cyc);


/*С��6us���ز�׼ȷ, �����ǳ�׼ȷ,ż�����Լ(+ -)1us(����) */
/*#define udelay(t)  _2cycle((t/2)-2) С��6��׼��Ϊ t/2-2 �ε���*/


/*7-512us*/
#define udelay(t)  _2cycle(  ((t>>1)&0xFE) +1 ) //_2cycle can't accept 0
//#define udelay(t)  _2cycle((t/2)-2) /* _2cycle( ((t>>1)&0xFC) )*/


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

