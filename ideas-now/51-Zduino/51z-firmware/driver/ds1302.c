#include "config.h"
#include <adc0832.h>
#include <timer.h>
typedef unsigned char uchar;

define DS1302_SEC_Reg 0x80
#define DS1302_MIN_Reg 0x82
#define DS1302_HR_Reg 0x84
#define DS1302_DATE_Reg 0x86
#define DS1302_MONTH_Reg 0x88
#define DS1302_DAY_Reg 0x8a
#define DS1302_YEAR_Reg 0x8c
#define DS1302_CONTROL_Reg 0x8e
#define DS1302_CHARGER_Reg 0x90
#define DS1302_CLKBURST_Reg 0xbe

/*********************************************************************/
/* ʵʱʱ��ģ�� ʱ��оƬ�ͺţ�DS1302 */
/*/
/*********************************************************************/
sbit T_CLK = P0^4; /*ʵʱʱ��ʱ�������� */
sbit T_IO = P0^5; /*ʵʱʱ������������ */
sbit T_RST = P0^3; /*ʵʱʱ�Ӹ�λ������ */
/********************************************************************
*
* ����: v_RTInputByte
* ˵��:
* ����: ��DS1302д��1Byte����
* ����:
* ����: ucDa д�������
* ����ֵ: ��
***********************************************************************/
void v_RTInputByte(uchar ucDa)
{
	uchar i;
	ACC = ucDa;
	for(i=8; i>0; i--)
	{
		T_IO = ACC0; /*�൱�ڻ���е� RRC */
		T_CLK = 1;
		T_CLK = 0;
		ACC = ACC >> 1;
	}
}
/********************************************************************
*
* ����: uchar uc_RTOutputByte
* ˵��:
* ����: ��DS1302��ȡ1Byte����
* ����:
* ����:
* ����ֵ: ACC
***********************************************************************/
uchar uc_RTOutputByte(void)
{
uchar i;
for(i=8; i>0; i--)
{
ACC = ACC >>1; /*�൱�ڻ���е� RRC */
ACC7 = T_IO;
T_CLK = 1;
T_CLK = 0;
}
return(ACC);
}
/********************************************************************
*
* ����: v_W1302
* ˵��: ��д��ַ����д����/����
* ����: ��DS1302д������
* ����: v_RTInputByte()
* ����: ucAddr: DS1302��ַ, ucDa: Ҫд������
* ����ֵ: ��
***********************************************************************/
void v_W1302(uchar ucAddr, uchar ucDa)
{
T_RST = 0;
T_CLK = 0;
T_RST = 1;
v_RTInputByte(ucAddr); /* ��ַ������ */
v_RTInputByte(ucDa); /* д1Byte����*/
T_CLK = 1;
T_RST =0;
}
/********************************************************************
*
* ����: uc_R1302
* ˵��: ��д��ַ���������/����
* ����: ��ȡDS1302ĳ��ַ������
* ����: v_RTInputByte() , uc_RTOutputByte()
* ����: ucAddr: DS1302��ַ
* ����ֵ: ucDa :��ȡ������
***********************************************************************/
uchar uc_R1302(uchar ucAddr)
{
uchar ucDa;
T_RST = 0;
T_CLK = 0;
T_RST = 1;
v_RTInputByte(ucAddr); /* ��ַ������ */
ucDa = uc_RTOutputByte(); /* ��1Byte���� */
T_CLK = 1;
T_RST =0;
return(ucDa);
}
/********************************************************************
*
* ����: v_BurstW1302T
* ˵��: ��д��ַ����д����(ʱ�Ӷ��ֽڷ�ʽ)
* ����: ��DS1302д��ʱ������(���ֽڷ�ʽ)
* ����: v_RTInputByte()
* ����: pSecDa: ʱ�����ݵ�ַ ��ʽΪ: �� �� ʱ �� �� ���� �� ����
* 8Byte (BCD��) 1B 1B 1B 1B 1B 1B 1B 1B
* ����ֵ: ��
***********************************************************************/
void v_BurstW1302T(uchar *pSecDa)
{
uchar i;
v_W1302(0x8e,0x00); /* ��������,WP=0,д����?*/
T_RST = 0;
T_CLK = 0;
T_RST = 1;
v_RTInputByte(0xbe); /* 0xbe:ʱ�Ӷ��ֽ�д���� */
for (i=8;i>0;i--) /*8Byte = 7Byte ʱ������ + 1Byte ����*/
{
v_RTInputByte(*pSecDa);/* д1Byte����*/
pSecDa++;
}
T_CLK = 1;
T_RST =0;
}
/********************************************************************
*
* ����: v_BurstR1302T
* ˵��: ��д��ַ���������/����(ʱ�Ӷ��ֽڷ�ʽ)
* ����: ��ȡDS1302ʱ������
* ����: v_RTInputByte() , uc_RTOutputByte()
* ����: pSecDa: ʱ�����ݵ�ַ ��ʽΪ: �� �� ʱ �� �� ���� ��
* 7Byte (BCD��) 1B 1B 1B 1B 1B 1B 1B
* ����ֵ: ucDa :��ȡ������
***********************************************************************/
void v_BurstR1302T(uchar *pSecDa)
{
uchar i;
T_RST = 0;
T_CLK = 0;
T_RST = 1;
v_RTInputByte(0xbf); /* 0xbf:ʱ�Ӷ��ֽڶ����� */
for (i=8; i>0; i--)
{
*pSecDa = uc_RTOutputByte(); /* ��1Byte���� */
pSecDa++;
}
T_CLK = 1;
T_RST =0;
}
/********************************************************************
*
* ����: v_BurstW1302R
* ˵��: ��д��ַ����д����(�Ĵ������ֽڷ�ʽ)
* ����: ��DS1302�Ĵ�����д������(���ֽڷ�ʽ)
* ����: v_RTInputByte()
* ����: pReDa: �Ĵ������ݵ�ַ
* ����ֵ: ��
***********************************************************************/
void v_BurstW1302R(uchar *pReDa)
{
uchar i;
v_W1302(0x8e,0x00); /* ��������,WP=0,д����?*/
T_RST = 0;
T_CLK = 0;
T_RST = 1;
v_RTInputByte(0xfe); /* 0xbe:ʱ�Ӷ��ֽ�д���� */
for (i=31;i>0;i--) /*31Byte �Ĵ������� */
{
v_RTInputByte(*pReDa); /* д1Byte����*/
pReDa++;
}
T_CLK = 1;
T_RST =0;
}
/********************************************************************
*
* ����: uc_BurstR1302R
* ˵��: ��д��ַ���������/����(�Ĵ������ֽڷ�ʽ)
* ����: ��ȡDS1302�Ĵ�������
* ����: v_RTInputByte() , uc_RTOutputByte()
* ����: pReDa: �Ĵ������ݵ�ַ
* ����ֵ: ��
***********************************************************************/
void v_BurstR1302R(uchar *pReDa)
{
uchar i;
T_RST = 0;
T_CLK = 0;
T_RST = 1;
v_RTInputByte(0xff); /* 0xbf:ʱ�Ӷ��ֽڶ����� */
for (i=31; i>0; i--) /*31Byte �Ĵ������� */
{
*pReDa = uc_RTOutputByte(); /* ��1Byte���� */
pReDa++;
}
T_CLK = 1;
T_RST =0;
}
/**************	/