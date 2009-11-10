
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <avr/delay.h>

/*===============================================*/
/* ����*/
#define F_CPU 8000000UL
#define	ftime	F_CPU/800


#define sign01s 0//10����ʱ�䵽
#define busy	 1//����æ
#define error	 2//���ͨѶ�����з���������λ
#define nowtx	 3//���ڽ��д������
#define nowrx	 4//���ڽ��д�������
#define rxok	 5//���н������
#define sign1s	 6//ʱ�䵽1��
#define isprog	 7//�Ѿ�������״̬

/*===============================================*/
/*���ܶ˿ڶ���*/

// ���� ��̶˿�
#define HVSPI_PORT     PORTA
#define HVSPI_PIN        PINA 
#define HVSPI_DDR       DDRA    

#define keyin	          PA7//��������Խ�һ��������
					//��������ͨѶ��ʱ�����´˼��Զ��ָ�����ֵ
#define testkey	(HVSPI_PIN & (1 << keyin))

//UART ͨѶ�˿�
#define  UART_PORT  PORTB
#define  UART_PIN     PINB
#define  UART_DDR   DDRB


/*HVSPI �˿����Ŷ���*/
#define VCC		5//���ӱ����оƬ��VCC
#define sci		4//���ӱ����оƬ��PB3
#define sdo		3//���ӱ����оƬ��PB2
#define sii		2//���ӱ����оƬ��PB1
#define  sdi		1//���ӱ����оƬ��PB0
#define	HVout	0//��ѹ����������ߵ�ƽ���0V���͵�ƽ���12V���������5V��

#define testvcc	HVSPI_PIN & (1 << VCC)
#define setvcc		HVSPI_PORT |= (1 << VCC)
#define clrvcc		HVSPI_PORT &= ~(1 << VCC)

#define testsii		HVSPI_PIN & (1 << sii)
#define setsii		HVSPI_PORT |= (1 << sii)
#define clrsii		HVSPI_PORT &= ~(1 << sii)

#define testsci	HVSPI_PIN & (1 << sci)
#define setsci		HVSPI_PORT |= (1 << sci)
#define clrsci		HVSPI_PORT &= ~(1 << sci)


#define testsdo	HVSPI_PIN & (1 << sdo)
#define setsdo	HVSPI_PORT |= (1 << sdo)
#define clrsdo		HVSPI_PORT &= ~(1 << sdo)
#define sdoin		HVSPI_DDR &= ~(1 << sdo)

#define testsdi	HVSPI_PIN & (1 << sdi)
#define setsdi		HVSPI_PORT |= (1 << sdi)
#define clrsdi		HVSPI_PORT &= ~(1 << sdi)



#define testHV	(~(HVSPI_PORT & (1 << HVout)))
#define clrHV		(HVSPI_PORT |= (1 << HVout))
#define setHV		(HVSPI_PORT &= ~(1 << HVout))


/*UART �˿ڹ��ܶ���*/
#define rxd		6//���н��ն���
#define txd		3//���з��Ͷ��ӡ�
#define sta		4//����״ָ̬ʾ��

#define teststa	(UART_PIN & (1 << sta))
#define clrsta		(UART_PORT |= (1 << sta))
#define setsta		(UART_PORT &= ~(1 << sta))
#define testrxd	(UART_PIN & (1 << rxd))
/*===============================================*/
/*��ʼ��ֵ*/
#define SPI_PORT_INIT			0B11000001//
#define SPI_PORT_INIT_POWER   	0B11100001//���磬��λΪ0V������������Ϊ0
#define SPI_DDR_INIT				0B00111111//��ʱ���Թ����н����е�A����Ϊ����,0B00001111
#define SPI_DDR_INIT_POWER		0B00111111

#define UART_PORT_INIT	0B11101111//B�ڴ�����
#define UART_DDR_INIT	0B00011000//��ʼ����ʱ���Ƚ����е�B����Ϊ����


/*timer resource , time 1*/
#define TMC_PRESCALER        0B0100    //  8M /8, 8��Ƶ
#define TMC_CTC		          0B10000000
#define TMC_PRESCALER_RESET            0B01000000

#define tc1aval	0B00110100;//��������ʱ1ƥ��B��ʱ�������λ���ڴ�ֱ�ӽ���һ��ǿ��ƥ�䡣
#define tc1bval	TMC_CTC|TMC_PRESCALER_RESET|TMC_PRESCALER




#define T0val	0//�ڴ�׼���ö�ʱ��0��
#define TCCR0val	0B00001011//�ڴ˸�λԤ��Ƶ�������ҷ�ƵΪ64.��Ҫ����λ�����͵�ֵ��0X80
#define txtimeval	100//�ڳ��ִ����ʱ���Զ����ڲ��ļ�����Ϊ��ֵ��
#define mintm		30//������С�Ľ���ʱ�����ڣ�Ҳ�Ƕ�������ͨѶ�ٶȡ�





/*===============================================*/
/*program flag*/
#define testpro	sign & (1 << isprog)
#define	setpro	sign |= (1 << isprog)
#define	clrpro	sign &= ~(1 << isprog)





#define testt0ie 	(TIMSK & (1 << TOIE0))
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
#define clrintf		GIFR |= (1 << INTF0)


#define setby		MCUCR |= (1 << ISC01)

#define set1bout	TCCR1A |= (1 << COM1B0)
#define clr1bout	TCCR1A &= ~(1 << COM1B0)


#define UCHAR unsigned char


/*-------------------------------------------------------------------*/

#define NOP		asm("nop")

extern UCHAR volatile sign;
extern UCHAR rxtime;//�˴��������ͨѶ��ʱ�䳤�ȡ�
extern UCHAR step ;//�ڳ������й���������ָʾ��ǰ����ִ�еĲ������������ģ��֮���ܹ��໥��Ϲ���
extern UCHAR txval,rxval,txstep,rxstep;


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


