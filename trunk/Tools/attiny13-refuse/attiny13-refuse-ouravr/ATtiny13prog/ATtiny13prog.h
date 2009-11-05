
//�Լ������ͷ�ļ�

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/signal.h>
#include <avr/interrupt.h>
#include <avr/delay.h>

# define F_CPU 8000000UL
#define	ftime	F_CPU/800


#define sign01s 0//10����ʱ�䵽
#define busy	 1//����æ
#define error	 2//���ͨѶ�����з���������λ
#define nowtx	 3//���ڽ��д������
#define nowrx	 4//���ڽ��д�������
#define rxok	 5//���н������
#define sign1s	 6//ʱ�䵽1��
#define isprog	 7//�Ѿ�������״̬


//�˿�A״̬���ܶ���,�˿�A���������б���á�
#define keyin	PA7//��������Խ�һ����������������ͨѶ��ʱ�����´˼��Զ�ִ�лָ�����ֵ
#define VCC		5//���ӱ����оƬ��VCC
#define sci		4//���ӱ����оƬ��PB3
#define sdo		3//���ӱ����оƬ��PB2
#define sii		2//���ӱ����оƬ��PB1
#define	sdi		1//���ӱ����оƬ��PB0
#define	HVout	0//��ѹ����������ߵ�ƽ���0V���͵�ƽ���12V���������5V��


#define ppaval	0B11100001//���磬��λΪ0V������������Ϊ0
#define pdaval	0B00111111

#define T0val	0//�ڴ�׼���ö�ʱ��0��
#define TCCR0val	0B00001011//�ڴ˸�λԤ��Ƶ�������ҷ�ƵΪ64.��Ҫ����λ�����͵�ֵ��0X80
#define txtimeval	100//�ڳ��ִ����ʱ���Զ����ڲ��ļ�����Ϊ��ֵ��
#define mintm		30//������С�Ľ���ʱ�����ڣ�Ҳ�Ƕ�������ͨѶ�ٶȡ�

#define paval	0B11000001//
#define daval	0B00111111//��ʱ���Թ����н����е�A����Ϊ����,0B00001111
#define pbval	0B11101111//B�ڴ�����
#define dbval	0B00011000//��ʼ����ʱ���Ƚ����е�B����Ϊ����
#define tc1bval	0B11000100;//��Ƶ��Ϊ8,������ΪҪ����8M��Ƶ�ʣ����Է�Ƶ������Ϊԭ����8��
#define tc1aval	0B00110100;//��������ʱ1ƥ��B��ʱ�������λ���ڴ�ֱ�ӽ���һ��ǿ��ƥ�䡣

//�˿�B�Ĺ��ܶ���
#define rxd		6//���н��ն���
#define txd		3//���з��Ͷ��ӡ�
#define sta		4//����״ָ̬ʾ��

#define NOP		asm("nop")



//�����ǲ����ĺ궨��

#define testkey	(PINA & (1 << keyin))

#define testrxd	(PINB & (1 << rxd))

#define sdoin	DDRA &= ~(1 << sdo)

#define testpro	sign & (1 << isprog)
#define	setpro	sign |= (1 << isprog)
#define	clrpro	sign &= ~(1 << isprog)

#define teststa	(PINB & (1 << sta))
#define clrsta	PORTB |= (1 << sta)
#define setsta	PORTB &= ~(1 << sta)

#define testsci	PINA & (1 << sci)
#define setsci	PORTA |= (1 << sci)
#define clrsci	PORTA &= ~(1 << sci)

#define testsii	PINA & (1 << sii)
#define setsii	PORTA |= (1 << sii)
#define clrsii	PORTA &= ~(1 << sii)

#define testsdo	PINA & (1 << sdo)
#define setsdo	PORTA |= (1 << sdo)
#define clrsdo	PORTA &= ~(1 << sdo)

#define testsdi	PINA & (1 << sdi)
#define setsdi	PORTA |= (1 << sdi)
#define clrsdi	PORTA &= ~(1 << sdi)

#define testvcc	PINA & (1 << VCC)
#define setvcc	PORTA |= (1 << VCC)
#define clrvcc	PORTA &= ~(1 << VCC)

#define testHV	~(PORTA & (1 << HVout))
#define clrHV	PORTA |= (1 << HVout)
#define setHV	PORTA &= ~(1 << HVout)

#define nowbusy	sign & (1 << busy)
#define setbusy	sign |= (1 << busy)
#define clrbusy sign &= ~(1 << busy)

#define test01s	(sign & ( 1 << sign01s))
#define set01s	sign |= ( 1 << sign01s)
#define clr01s	sign &= ~( 1 << sign01s)

#define testrxok	(sign & (1 << rxok))
#define setrxok	sign |= (1 << rxok)
#define clrrxok	sign &= ~(1 << rxok)


#define testerr	(sign & (1 << error))
#define seterr	sign |= (1 << error)
#define clrerr	sign &= ~(1 << error)

#define testtx	(sign & (1 << nowtx))
#define settx	sign |= (1 << nowtx)
#define clrtx	sign &= ~(1 << nowtx)

#define testrx	(sign & (1 << nowrx))
#define setrx	sign |= (1 << nowrx)
#define clrrx	sign &= ~(1 << nowrx)

#define testt0ie (TIMSK & (1 << TOIE0))
#define sett0ie	TIMSK |= (1 << TOIE0)
#define clrt0ie	TIMSK &= ~(1 << TOIE0)

#define sett1ie	TIMSK |= (1 << TOIE1)
#define clrt1ie	TIMSK &= ~(1 << TOIE1)

#define sett1ae	TIMSK |= (1 << OCIE1A)
#define clrt1ae	TIMSK &= ~(1 << OCIE1A)

#define sett1be	TIMSK |= (1 << OCIE1B)
#define clrt1be	TIMSK &= ~(1 << OCIE1B)

#define setint0	GIMSK |= (1 << INT0)
#define clrint0	GIMSK &= ~(1 << INT0)
#define tstint0	(GIMSK & (1 << INT0))
#define clrintf	GIFR |= (1 << INTF0)


#define setby	MCUCR |= (1 << ISC01)

#define set1bout	TCCR1A |= (1 << COM1B0)
#define clr1bout	TCCR1A &= ~(1 << COM1B0)


#define UCHAR unsigned char

//�ڽ��е�����ʱ�����õ�Ԫ

UCHAR volatile sign = 0;
UCHAR rxtime = txtimeval;//�˴��������ͨѶ��ʱ�䳤�ȡ�
UCHAR step = 0;//�ڳ������й���������ָʾ��ǰ����ִ�еĲ������������ģ��֮���ܹ��໥��Ϲ���
UCHAR txval,rxval,txstep,rxstep;


void outprog(void);
//��ͨѶ�����У���λ�����úò�������0X80,������ɹ���λ���ᷢ��һ�������Ĳ���������û���κ����ݷ��أ���λ����Ҫ�ظ�����0X80��
//�ڽ��յ����ݺ���λ��Ҫ����0X55,�Լ���ͨѶ��ȷ�ԣ���λ���ڼ����ȷ��᷵��0X55�����򷵻ص�ֵ����0X55��Ҫ���·��ط���0X80.
//�˳���������Զ����ò����ʵķ�ʽ������ʵ����ȫ˫��ͨѶ��ʵ�ʼ��ʵ����4800bps--19200bps�����շ���ȫ����
//���Թ����д��͵�������15KBytes��
//���Ʒ�ʽ�ı����£�
//��Ҫ���Ʒ��ڶ�ʱ��1�У��Ҷ�ʱ��1��ֹͣ�����Ź��ĸ�λҲ���������档
//��⴮�в����ʵ�ʱ���ö�ʱ��0����ʱ������ʱ��ʱ��0��������
//������ʱ�����OCR1C��
//������λ��Ҳ���ǲ���USI���ܣ���λ�������ʵ�֣��������INT0�ϣ��������OC1B������֤���ʱ��׼ȷ��
//��OCR1B���������������֣�������ƥ���ʱ���Զ��ı������ƽ��ͬʱ�����жϣ����ж��������´�ƥ��ʱ�������ƽ��
//�����ܹ������Ƿ�����ɣ��γ���Ӧ�ı�־��OCR1A�����ڽ��յ�����λʱ�Զ����á����Ź����������������
//OCR1A�������������Ҫ��������ⲿ�����롣��������λ�ɳ�����������⣬����⵽����λ����OCR1A�����Զ��������ã�
//��Ƭ���Ĺ���ʱ�ӽ�����ߣ�һ��Ҫ�ﵽ4M���ϣ����ھ�������8M��
//״ָ̬ʾ������OCR1B��ʵ�֣����û�д���ͨѶ����˸������ͳ����������������ٶ���ʵ��ͬ������ٶ���ɱ�����
//ʵ�ʹ��������У���λ������0x80�󣬵�Ƭ������м�Ⲣ��ͬ��������λ������һ���ڲ���������ֵ��
//���Լ��������з��֣������ڲ�����ʱ��ʵ�ʵ���Ƶ�ʱȱ��ֵ�������������ֵ������������ۼ��������ֵ��


