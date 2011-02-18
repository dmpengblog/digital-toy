/**
   DS1302 opration lib
*/


#include <AT89X52.H>
#include <intrins.h>  /* _nop_ */
#include <config.h>
#include <1602.h>
#include <delay.h>
#include <bitops.h>


sbit DS1302_SCLK = P0^4; /*ʵʱʱ��ʱ�������� */
sbit DS1302_SDA = P0^5; /*ʵʱʱ������������ */
sbit DS1302_RST = P0^3; /*ʵʱʱ�Ӹ�λ������ */

#define DS1302_WAIT udelay(10);_nop_()

#define Set_CLK DS1302_SCLK=1
#define Clr_CLK DS1302_SCLK=0
#define Set_SDA DS1302_SDA=1
#define Clr_SDA DS1302_SDA=0
#define Set_RST DS1302_RST=1
#define Clr_RST DS1302_RST=0

typedef unsigned char UI8_T;

/*********************************************************************
DS1302 C51 Driver
*********************************************************************/

#define DS1302_SEC_Reg 0x80
#define DS1302_MIN_Reg 0x82
#define DS1302_HR_Reg 0x84
#define DS1302_DATE_Reg 0x86
#define DS1302_MONTH_Reg 0x88
#define DS1302_DAY_Reg 0x8a
#define DS1302_YEAR_Reg 0x8c
#define DS1302_CONTROL_Reg 0x8e
#define DS1302_CHARGER_Reg 0x90
#define DS1302_CLKBURST_Reg 0xbe
#define DS1302_RAMBURST_Reg 0xfe
#define DS1302_RAM_BASE_ADDR 0xc0		//RAM��ʼ��ַ
#define DS1302_WP_DIS  RTC_Write(DS1302_CONTROL_Reg,0x00)		//enable write
#define DS1302_WP_EN RTC_Write(DS1302_CONTROL_Reg,0x80)		//disable write
#define DS1302_CHG_EN RTC_Write(DS1302_CHARGER_Reg,0xa9)	//ʹ��������
#define DS1302_CHG_DIS RTC_Write(DS1302_CHARGER_Reg,0x0)	//��ֹ������
#define DS1302_RAM_BURST_WRITE RTC_Write_Byte(DS1302_RAMBURST_Reg);
#define DS1302_RAM_BURST_READ RTC_Write_Byte(DS1302_RAMBURST_Reg+1);
//



/*
#define Set_CLK RTC_PORT|=1<<DS1302_SCLK
#define Clr_CLK RTC_PORT&=~(1<<DS1302_SCLK)
#define Set_SDA RTC_PORT|=1<<DS1302_SDA
#define Clr_SDA RTC_PORT&=~(1<<DS1302_SDA)
#define Set_RST RTC_PORT|=1<<DS1302_RST
#define Clr_RST RTC_PORT&=~(1<<DS1302_RST)
*/

/********************************************************************
*
* ����: RTC_Write_Byte
* ˵��:
* ����: ��DS1302д��1Byte����
* ����:
* ����: Dat д�������
* ����ֵ: ��
***********************************************************************/
void RTC_Write_Byte(UI8_T Dat)
{
	UI8_T i;
	for(i=8; i>0; i--)
	{
		(Dat&0x1)?(Set_SDA):(Clr_SDA);
		DS1302_WAIT; 
		Set_CLK;
		DS1302_WAIT; 
		Clr_CLK;
		DS1302_WAIT; 
		Dat>>= 1;
	}
}
/********************************************************************
*
* ����: RTC_Write
* ˵��: ��д��ַ����д����/����
* ����: ��DS1302д������
* ����: RTC_Write_Byte()
* ����: Addr: DS1302��ַ, Dat: Ҫд������
* ����ֵ: ��
***********************************************************************/
void RTC_Write(UI8_T Addr, UI8_T Dat)
{
	Clr_RST;
	DS1302_WAIT;
	Clr_CLK;
	DS1302_WAIT;
	Set_RST;    //enable chip
	DS1302_WAIT;
	RTC_Write_Byte(Addr); /* ��ַ������ */
	RTC_Write_Byte(Dat); /* д1Byte����*/
	Set_CLK;
	DS1302_WAIT;
	Clr_RST;
	DS1302_WAIT; 
}
/********************************************************************
*
* ����: UI8_T RTC_Read_Byte
* ˵��:
* ����: ��DS1302��ȡ1Byte����
* ����:
* ����:
* ����ֵ: dat
***********************************************************************/
UI8_T RTC_Read_Byte(void)
{
	UI8_T i,dat=0;
	Set_SDA;		//set sda to input mode
	DS1302_WAIT;
	for(i=8; i>0; i--)
	{
		if (DS1302_SDA)
		     _set_bit(dat,7);		//load into MSB
		else 
		     _clear_bit(dat,7);
		dat>>=1;
		DS1302_WAIT; 
		Set_CLK;
		DS1302_WAIT;
		Clr_CLK;
		DS1302_WAIT;
		
	}
	return  dat;
}
/********************************************************************
*
* ����: RTC_Read
* ˵��: ��д��ַ���������/����
* ����: ��ȡDS1302ĳ��ַ������
* ����: RTC_Write_Byte() , RTC_Read_Byte()
* ����: ucAddr: DS1302��ַ
* ����ֵ: ucDa :��ȡ������
***********************************************************************/
UI8_T RTC_Read(UI8_T Addr)
{
	UI8_T Dat;
	Addr|=1;		//�����������bitΪ1��
	Clr_RST;
	DS1302_WAIT;
	Clr_CLK;
	DS1302_WAIT;
	Set_RST;
	DS1302_WAIT;
	RTC_Write_Byte(Addr); /* ��ַ������ */
	Dat = RTC_Read_Byte(); /* ��1Byte���� */
	Set_CLK;
	DS1302_WAIT;
	Clr_RST;
	DS1302_WAIT; 
	return(Dat);
}


void RTC_Init(void)
{
	Clr_CLK;
	DS1302_WAIT; 
	Clr_RST;
	DS1302_WAIT; 
	DS1302_WP_DIS;
	//DS1302_CHG_EN;		//����������
	RTC_Write(DS1302_SEC_Reg,RTC_Read(DS1302_SEC_Reg)&0x7f);	//24Сʱ��
	DS1302_WP_EN;
}

/********************************************************************
*
* ����: RTC_Write_Burst
* ˵��: ��д��ַ����д����(ʱ�Ӷ��ֽڷ�ʽ)
* ����: ��DS1302д��ʱ������(���ֽڷ�ʽ)
* ����: RTC_Write_Byte()
* ����: pDat: ʱ�����ݵ�ַ ��ʽΪ: �� �� ʱ �� �� ���� �� ����
* 8Byte (BCD��) 1B 1B 1B 1B 1B 1B 1B 1B
* ����ֵ: ��
***********************************************************************/
#if 0
void RTC_Write_Burst(UI8_T *pDat)
{
	UI8_T i;
	DS1302_WP_DIS;/* ��������,WP=0,д����?*/
	Clr_RST;
	DS1302_WAIT;
	Clr_CLK;
	DS1302_WAIT;
	Set_RST;
	DS1302_WAIT;
	RTC_Write_Byte(DS1302_CLKBURST_Reg); /* 0xbe:ʱ�Ӷ��ֽ�д���� */
	for (i=8;i>0;i--) /*8Byte = 7Byte ʱ������ + 1Byte ����*/
	{
		RTC_Write_Byte(*pDat);/* д1Byte����*/
		pDat++;
	}
	Set_CLK;
	DS1302_WAIT;
	Clr_RST;
	DS1302_WP_EN;
}
#endif
/********************************************************************
*
* ����: RTC_Time_Get_Burst
* ˵��: ��д��ַ���������/����(ʱ�Ӷ��ֽڷ�ʽ)
* ����: ��ȡDS1302ʱ������
* ����: RTC_Write_Byte() , RTC_Read_Byte()
* ����: pDat: ʱ�����ݵ�ַ ��ʽΪ: �� �� ʱ �� �� ���� ��
* 7Byte (BCD��) 1B 1B 1B 1B 1B 1B 1B
* ����ֵ: ucDa :��ȡ������
***********************************************************************/
#if 0
void RTC_Time_Get_Burst(UI8_T *pDat)
{
	UI8_T i;
	Clr_RST;
	DS1302_WAIT;
	Clr_CLK;
	DS1302_WAIT;
	Set_RST;
	DS1302_WAIT;
	RTC_Write_Byte(DS1302_CLKBURST_Reg+1); /* 0xbf:ʱ�Ӷ��ֽڶ����� */
	for (i=8; i>0; i--)
	{
		*pDat = RTC_Read_Byte(); /* ��1Byte���� */
		pDat++;
	}
	Set_CLK;
	DS1302_WAIT;
	Clr_RST;
}
#endif
/********************************************************************
*
* ����: RTC_RAM_Write_Burst
* ˵��: ��д��ַ����д����(�Ĵ������ֽڷ�ʽ)
* ����: ��DS1302�Ĵ�����д������(���ֽڷ�ʽ)
* ����: RTC_Write_Byte()
* ����: pDat: �Ĵ������ݵ�ַ
* ����ֵ: ��
***********************************************************************/
#if 0
void RTC_RAM_Write_Burst(UI8_T *pDat)
{
	UI8_T i;
	DS1302_WP_DIS; //���д����
	Clr_RST;
	DS1302_WAIT;
	Clr_CLK;
	DS1302_WAIT;
	Set_RST;
	DS1302_WAIT;
	DS1302_RAM_BURST_WRITE; /* 0xfe:RAM���ֽ�д���� */
	for (i=31;i>0;i--) /*31Byte �Ĵ������� */
	{
		RTC_Write_Byte(*pDat); /* д1Byte����*/
		pDat++;
	}
	Set_CLK;
	DS1302_WAIT;
	Clr_RST;
	DS1302_WP_EN;
}
#endif
/********************************************************************
*
* ����: RTC_RAM_Read_Burst
* ˵��: ��д��ַ���������/����(�Ĵ������ֽڷ�ʽ)
* ����: ��ȡDS1302�Ĵ�������
* ����: RTC_Write_Byte() , RTC_Read_Byte()
* ����: pDat: �Ĵ������ݵ�ַ
* ����ֵ: ��
***********************************************************************/
#if 0
void RTC_RAM_Read_Burst(UI8_T *pDat)
{
	UI8_T i;
	Clr_RST;
	DS1302_WAIT;
	Clr_CLK;
	DS1302_WAIT;
	Set_RST;
	DS1302_WAIT;
	DS1302_RAM_BURST_READ;/* 0xff:RAM�ֽڶ����� */
	for (i=31; i>0; i--) /*31Byte �Ĵ������� */
	{
		*pDat = RTC_Read_Byte(); /* ��1Byte���� */
		pDat++;
	}
	Set_CLK;
	DS1302_WAIT;
	Clr_RST;
}
#endif
/********************************************************************
*
* ����: RTC_IniTime_Set
* ˵��:
* ����: ���ó�ʼʱ��
* ����: v_W1302()
* ����: pDat: ��ʼʱ���ַ����ʼʱ���ʽΪ: �� �� ʱ �� �� ���� ��
* 7Byte (BCD��) 1B 1B 1B 1B 1B 1B 1B
* ����ֵ: ��
***********************************************************************/
#if 0
void RTC_IniTime_Set(UI8_T *pDat)
{
	UI8_T i;
	UI8_T Addr = DS1302_SEC_Reg;
	DS1302_WP_DIS;
	for(i =7;i>0;i--)
	{
		RTC_Write(Addr,*pDat); /* �� �� ʱ �� �� ���� �� */
		pDat++;
		Addr +=2;
	}
	DS1302_WP_EN;
}
#endif

/********************************************************************
*
* ����: RTC_CurTime_Get
* ˵��:
* ����: ��ȡDS1302��ǰʱ��
* ����: RTC_Read()
* ����: Curtime: ���浱ǰʱ���ַ����ǰʱ���ʽΪ: �� �� ʱ �� �� ���� ��
* 7Byte (BCD��) 1B 1B 1B 1B 1B 1B 1B
* ����ֵ: ��
***********************************************************************/
UI8_T Curtime[8];
#define SEC    (Curtime[0])
#define MIN    (Curtime[1])
#define HOUR   (Curtime[2])
#define YEAR   (Curtime[3])
#define DATE   (Curtime[4])
#define WEEK   (Curtime[5])


void RTC_CurTime_Get(UI8_T Curtime[])
{
	UI8_T i;
	UI8_T Addr = DS1302_SEC_Reg|1;	 //������
//	for (i=0;i<7;i++){
		Curtime[i] = RTC_Read(Addr); /*��ʽΪ: �� �� ʱ �� �� ���� �� */
		Addr += 2;
//	}
}

unsigned char sec,min,hour;

void lcd_showhex(unsigned char x);

ds1302_test()
{
    
//	RTC_CurTime_Get(Curtime);
	lcd_cursor(0,1);
	
	
	//sec = SEC&0x0f+(SEC>>4)*10;
	lcd_puts("s:");
	print10(RTC_Read(DS1302_SEC_Reg));

	lcd_puts("s:");
	print10(RTC_Read(DS1302_SEC_Reg+2));
	lcd_puts("s:");
	print10(RTC_Read(DS1302_SEC_Reg+4));
	
	
}


