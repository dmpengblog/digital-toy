 #include <AT89X52.H>


/*
 *  run about cyc*2+2 cycle
 */
void _2cycle(unsigned char cyc)
{
   while(--cyc);
   /* DFFE     DJNZ     R7,delay(C:0014), 2cycle*/
}

/*С��6us���ز�׼ȷ, �����ǳ�׼ȷ,ż�����Լ(+ -)1us(����) */
/*7-512us*/
#define udelay(t)  _2cycle((t/2)-2)



/*
 * mdelay(0) : 15 cycle, 12Mhz 30us
 * 1ms to 256ms
 *  +- 50us
 */
void mdelay(unsigned char ms)
{

/*
    35:    while(--t){     
C:0x0005    1E       DEC      R6               1 cyc
C:0x0006    EE       MOV      A,R6	           1 cyc
C:0x0007    600C     JZ       C:0015           1 cyc
    36:      _2cycle(250); 
C:0x0009    7FFA     MOV      R7,#0xFA	       1 cyc
C:0x000B    120027   LCALL    _2cycle(C:0027)   2 cyc
    37:         _2cycle(250); 
C:0x000E    7FFA     MOV      R7,#0xFA		   1 cyc
C:0x0010    120027   LCALL    _2cycle(C:0027)   2 cyc
    38:    } 
C:0x0013    80F0     SJMP     C:0005           2 cyc
*/
   while(ms--){	  
     udelay(489); //����12cyc*2 us
	 udelay(499);
   }
  
}

/*
 * +- 50ms
 */
void sleep(unsigned char t)
{
   while(t--){
   	 mdelay(250);
   	 mdelay(250);
     mdelay(250);
     mdelay(250);
   }
     
}


/***********************************************
�������ƣ�1602Һ����ʾ����
��Ҫ˵����ʹ��FM1602Һ����ʾ�������ݿڽ�P0�ڣ����ƶ˽�P2.0-P2.2
�Ĵ���ѡ���źŶ�RS-P2.0����д�źŶ�R/W-P2.1��ʹ���źŶ�EP-P2.2
��    д��JiangX.net
***********************************************/
typedef unsigned char BYTE;
typedef unsigned int WORD;
typedef bit BOOL ;

sbit rs = P1^0;   //RS
sbit rw = P1^1;   //��дѡ��RW
sbit ep = P1^2;   //ʹ���ź�E

BYTE code dis1[] = {"Welcome!"};
BYTE code dis2[] = {"Digital-Toy"};


/*****�б�LCDæµ״̬*****/
BOOL lcd_bz()
{
	BOOL result;
	rs = 0;
	rw = 1;
	ep = 1;
	result = (BOOL)(P0 & 0x80);
	ep = 0;
	return result;
}
/*****д��ָ�����ݵ�LCD*****/
void lcd_wcmd(BYTE cmd)
{
  while(lcd_bz());
  rs = 0;
  rw = 0;
	ep = 0;
	P0 = cmd;
	ep = 1;
	ep = 0;  
}

/*****�趨��ʾλ��*****/
void lcd_start(BYTE start)
{       
   lcd_wcmd(start | 0x80);
}

/*****д���ַ���ʾ���ݵ�LCD*****/
void lcd_data(BYTE dat)
{       
  while(lcd_bz());
  rs = 1;
  rw = 0;
  ep = 0;
  P0 = dat;
  ep = 1;
  ep = 0;
}

/*****LCD��ʼ���趨*****/
void lcd_init()
{
    mdelay(15);      
lcd_wcmd(0x38);    //�趨LCDΪ16*2��ʾ��5*7����8λ���ݽӿ�
mdelay(11);
lcd_wcmd(0x0c);    //����ʾ������ʾ���
mdelay(11);
lcd_wcmd(0x06);    //��ʾ����Զ����ƣ��������ƶ�
mdelay(11);
lcd_wcmd(0x01);    //��ʾ����
mdelay(11);
}

/*****������*****/
void main()
{
  BYTE i=0;
  char pos=0;

  lcd_init();    // ��ʼ��LCD   
  mdelay(200);

again: 
  pos=0; 
  lcd_start(0);    // ������ʾλ��Ϊ��һ�еĵ�5���ַ�
  mdelay(11);
  while(pos<15){
     lcd_data(i); // ��ʾ�ڶ����ַ�
     i++;pos++;
     mdelay(11);
  }   

  pos=0;
  lcd_start(0x40);   // ������ʾλ��Ϊ�ڶ��е�һ���ַ�
  while(pos<15){
     lcd_data(i); // ��ʾ�ڶ����ַ�
      i++;pos++;  
     mdelay(11);
  }   
  
  
  mdelay(255);
  mdelay(255);  
  mdelay(255);
  mdelay(255);
  mdelay(255);
  
  goto again;
}


