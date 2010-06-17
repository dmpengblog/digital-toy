/******************************************************************************
**  ��Ŀ����:    M8 ��������                                               **
**  ��ǰ�汾��   v1.0.6                                                      **
**  ��Ŀ����:    Aleyn.wu  (blog.e-mdias.cn / aleyn@e-midas.cn)              **
**  ��������:    2008-11-04                                                  **
**  ��Ŀ���:    ֧����֧AA�������أ�LCD��ʾ����״̬��LED��ʾ���״̬    **
**               ����������֧�ֳ��������õ�                                **
**  ��оƬ  :    AT-Mega8-16PI                                               **
**  ����Ƶ��:    16.0000MHz                                                  **
******************************************************************************/

#include <CVAVR.h>
#include <mega8.h>
#include <mega8ext.h>
#include <delay.h>
#include "M8_Config.h"

#pragma savereg-
interrupt [TIM0_OVF] void timer0_ovf_isr(void)
{
#asm
    CLI
  	ST   -Y,R30
	IN   R30,SREG
	ST   -Y,R30
#endasm
    msLedCount--;
    if (msLedCount==0)
    {
        msLedCount = 121;
        if (LED1Status_on_off == LED_OFF)
        {
            LED1 = LED_OFF;
        }
        else
        {
            if (LED1Status_inv == LED_OFF)
            {
                LED1 = LED_ON;
            }
            else
            {
                if (LEDStatus == LED_ON)
                {
                    LED1 = LED_ON;
                }
                else
                {
                    LED1 = LED_OFF;
                }
            }
        }

        if (LED2Status_on_off == LED_OFF)
        {
            LED2 = LED_OFF;
        }
        else
        {
            if (LED2Status_inv == LED_OFF)
            {
                LED2 = LED_ON;
            }
            else
            {
                if (LEDStatus == LED_OFF)
                {
                    LED2 = LED_ON;
                }
                else
                {
                    LED2 = LED_OFF;
                }
            }
        }

        if (LEDStatus)
            LEDStatus = 0;
        else
            LEDStatus = 1;
    }
#asm
	LD   R30,Y+
	OUT  SREG,R30
	LD   R30,Y+
    SEI
#endasm
}

interrupt [TIM2_COMP] void timer2_comp_isr(void)
{
#asm
    CLI
	ST   -Y,R30
	ST   -Y,R31
	IN   R30,SREG
	ST   -Y,R30
#endasm

    ms2Count ++;
    sc3Count ++;

#asm
    LD   R30,Y+
	OUT  SREG,R30
	LD   R31,Y+
	LD   R30,Y+
    SEI
#endasm
}
#pragma savereg+

interrupt [EXT_INT0] void ext_int0_isr(void)
{
    Key = KEY1;
}

// External Interrupt 1 service routine
interrupt [EXT_INT1] void ext_int1_isr(void)
{
    Key = KEY2;
}

#include <USARTv2.h>
#include "M8ChargeLCD.h"
#include "M8_Function.c"

/******************************************************************************
**  ��������:    Port_Init                                                   **
**  �������:    GPIO��ʼ������                                              **
**  ��������:    Aleyn.wu                                                    **
**  ��������:    2008-11-04                                                  **
**  �������:    ��                                                          **
**  ���ز���:    ��                                                          **
******************************************************************************/
void Port_Init()
{
    PORTB = BIT(PB0) | BIT(PB1) | BIT(PB2) | BIT(PB5);
    DDRB  = 0x3F;
    PORTC = BIT(PC5);
    DDRC  = 0xF0;
    PORTD = 0x00;
    DDRD  = 0xF0;
}

/******************************************************************************
**  ��������:    Timer_Init                                                  **
**  �������:    ��ʱ����ʼ������                                            **
**  ��������:    Aleyn.wu                                                    **
**  ��������:    2008-11-04                                                  **
**  �������:    ��                                                          **
**  ���ز���:    ��                                                          **
******************************************************************************/
void Timer_Init()
{

    TCCR0 = BIT(CS02);
    TCCR2 = BIT(WGM21) | BIT(CS22) | BIT(CS21);
    TCNT2 = 0x00;
    OCR2  = 63;
    ASSR  = 0x00;

	TIMSK = BIT(OCIE2) | BIT(TOIE0);
}

/******************************************************************************
**  ��������:    Key_Init                                                    **
**  �������:    �����жϳ�ʼ������                                          **
**  ��������:    Aleyn.wu                                                    **
**  ��������:    2008-11-04                                                  **
**  �������:    ��                                                          **
**  ���ز���:    ��                                                          **
******************************************************************************/
void Key_Init()
{   
    GICR  = 0xC0;
    MCUCR = 0x0A;
    GIFR  = 0xC0;  
}

/******************************************************************************
**  ��������:    ADC_Init                                                    **
**  �������:    ADC��ʼ������                                               **
**  ��������:    Aleyn.wu                                                    **
**  ��������:    2008-11-04                                                  **
**  �������:    ��                                                          **
**  ���ز���:    ��                                                          **
******************************************************************************/
void ADC_Init(void )
{
    ADCSRA = BIT(ADEN) | BIT(ADPS2) | BIT(ADPS1) | BIT(ADPS0);
}

void ADC_Stop(void )
{
    ADCSRA = 0;
}


/******************************************************************************
**  ��������:    Device_Init                                                 **
**  �������:    �豸��ʼ������                                              **
**  ��������:    Aleyn.wu                                                    **
**  ��������:    2008-11-04                                                  **
**  �������:    ��                                                          **
**  ���ز���:    ��                                                          **
******************************************************************************/
void Device_Init()
{
    Port_Init();
    Timer_Init();
    Key_Init();
    USART_Init(25);
   
    CHR1  = PWM_OFF;
    CHR2  = PWM_OFF;
    DCHR1 = DIS_OFF;
    DCHR2 = DIS_OFF;

    delay_ms(500);
    LCD_Init();
    LED1 = LED_OFF;
    LED2 = LED_OFF;
}

/******************************************************************************
**  ��������:    DisplayMessage                                              **
**  �������:    ��LCD����ʾһЩ��Ϣ                                         **
**  ��������:    Aleyn.wu                                                    **
**  ��������:    2008-11-04                                                  **
**  �������:    ��                                                          **
**  ���ز���:    ��                                                          **
******************************************************************************/
void DisplayMessage(BYTE Index)
{
    LCD_Clear();
    LCD_Position(1,1);
    LCD_PutStrF(Product);
    LCD_Position(2,1);

    if (Index == WF_SHOW_INFO)          LCD_PutStrF(Version);
    if (Index == WF_INIT)               LCD_PutStrF("Init...");
    if (Index == WF_START)              LCD_PutStrF("Start...");
    if (Index == WF_STOP)               LCD_PutStrF("Stop...");
}

/******************************************************************************
**  ��������:    StartCharger                                                **
**  �������:    �������ʼ����                                              **
**  ��������:    Aleyn.wu                                                    **
**  ��������:    2008-11-04                                                  **
**  �������:    ��                                                          **
**  ���ز���:    ��                                                          **
******************************************************************************/
void StartCharger(void)
{
    InitBatteryVaule(&Batt1);
    InitBatteryVaule(&Batt2);
    ChargerStatus = WF_START;
    WorkFlow = WF_LOOP_START;
    SubWorkFlow = WF_INIT;
    ADC_Init();
}

/******************************************************************************
**  ��������:    StopCharger                                                 **
**  �������:    ���ֹͣ����                                                **
**  ��������:    Aleyn.wu                                                    **
**  ��������:    2008-11-04                                                  **
**  �������:    ��                                                          **
**  ���ز���:    ��                                                          **
******************************************************************************/
void StopCharger(void)
{
    StopChargePWM();
    StopDisCharge();
    ADC_Stop();
    Batt1.Status = BATT_EMPTY;
    Batt2.Status = BATT_EMPTY;
    ShowLedStatus();
    ChargerStatus = WF_STOP;
    WorkFlow = WF_INIT;
    SubWorkFlow = WF_INIT;
}

/******************************************************************************
**  ��������:    Params_Init                                                 **
**  �������:    ��ʼ���ڴ������������eeprom���ݵ��ڴ���                    **
**  ��������:    Aleyn.wu                                                    **
**  ��������:    2008-11-04                                                  **
**  �������:    ��                                                          **
**  ���ز���:    ��                                                          **
******************************************************************************/
void Params_Init(void)
{
    Key = 0;
    LEDStatus = 0;
    LcdStatus = 1;
    //Debug = 0;

    Set_Current = eSet_Current;
    Set_MaxTime = eSet_MaxTime;
    Set_Delta   = eSet_Delta;
    Set_DBV     = eSet_DBV;
    CheckGlobalConfig();

    InitBatteryVaule(&Batt1);
    InitBatteryVaule(&Batt2);

    Batt1.Index = 1;
    Batt2.Index = 2;
}
/******************************************************************************
**  ��������:    main                                                        **
**  �������:    ������                                                      **
**  ��������:    Aleyn.wu                                                    **
**  ��������:    2008-11-04                                                  **
**  �������:    ��                                                          **
**  ���ز���:    ��                                                          **
******************************************************************************/
void main(void)
{   
    Device_Init();

    DisplayMessage(WF_INIT);
    TransmitLineF(Product);
    TransmitLineF(Version);

    Params_Init();
    StopCharger();

    if ((KEY1_IS_ON == 0) && (KEY2_IS_ON == 0)) Setup();

    _sei();

    while (1)
    {
        if ( sc3Count >= 975 ))
        {
            CheckTimeStatus(&Batt1);
            CheckTimeStatus(&Batt2);
            sc3Count = 0;
        }

        if (Key == KEY1)
        {
            Key = 0;

            if ( ChargerStatus == WF_START )
            {
                StopCharger();
                DisplayMessage(WF_STOP);
            }
            else
            {
                StartCharger();
                DisplayMessage(WF_START);
            }
        }

        if (WorkFlow == WF_LOOP_START)
        {
            if (Key == KEY2)
            {
                Key = 0;
                
                LcdStatus++;

                if ( LcdStatus > 4 )
                {
                    LcdStatus = 1;
                }

                if ( ChargerStatus == WF_STOP)
                {
                    WorkFlow = WF_SHOW_INFO;
                    SubWorkFlow = WF_INIT;
                }
            }
            else
            {
                if ( ChargerStatus == WF_START)
                {
                    WorkFlow = WF_DETECT_VOLTAGE;
                    SubWorkFlow = WF_BATTERY1;
                }
            }

            continue;
        }

        if (WorkFlow == WF_WAIT_FOR)
        {
            if ( ms2Count >= MAX_WAIT_FOR )
            {
                ms2Count = 0;
                WorkFlow = WF_LOOP_START;
                SubWorkFlow = WF_INIT;
            }

            continue;
        }

        if (WorkFlow == WF_COMPLETE_DCHR)
        {
            if ( ms2Count >= MAX_DCHR_TIME )
            {
                ms2Count = 0;
                StopDisCharge();
                WorkFlow = WF_WAIT_FOR;
                SubWorkFlow = WF_INIT;
            }

            continue;
        }

        if (WorkFlow == WF_COMPLETE_PWM)
        {
            if ( ms2Count >= MAX_PWM_TIME )
            {
                ms2Count = 0;
                StopChargePWM();
                OpenDisCharge();
                WorkFlow = WF_COMPLETE_DCHR;
                SubWorkFlow = WF_INIT;
            }

            continue;
        }

        if (WorkFlow == WF_ADJUST_PWM)
        {
            AdjustChargePWM();
            WorkFlow = WF_COMPLETE_PWM;
            SubWorkFlow = WF_INIT;
            TransmitByte(13);
            
            continue;
        }

        if (WorkFlow == WF_DETECT_CURRENT)
        {
            if (SubWorkFlow == WF_BATTERY2)
            {
                if ( DetectCurrent(&Batt2) )
                {
                    CheckCurrentStatus(&Batt2);
                    SetLcdData(&Batt2);
                    TransmitStringF("C2=");
                    OutputInteger(Batt2.Current,OUTPUT_USART);
                    TransmitByte(13);
                    WorkFlow = WF_CHECK_STATUS;
                    SubWorkFlow = WF_ADJUST_PWM;
                }
            }

            if (SubWorkFlow == WF_BATTERY1)
            {
                if ( DetectCurrent(&Batt1) )
                {
                    CheckCurrentStatus(&Batt1);
                    SetLcdData(&Batt1);
                    TransmitStringF("C1=");
                    OutputInteger(Batt1.Current,OUTPUT_USART);
                    TransmitByte(13);
                    SubWorkFlow = WF_BATTERY2;
                }
            }

            continue;
        }


        if (WorkFlow == WF_OPEN_PWM)
        {
            if (SubWorkFlow == WF_INIT)
            {
                ms2Count = 0;

                if ( OpenChargePWM() ) 
                {
                    SubWorkFlow = WF_WAIT_FOR;
                }
                else
                {
                    WorkFlow = WF_COMPLETE_PWM;
                    SubWorkFlow = WF_INIT;
                }
            }

            if (SubWorkFlow == WF_WAIT_FOR)
            {
                if (ms2Count >= MAX_AFTER_TIME)
                {
                    ms2Count = 0;
                    WorkFlow = WF_DETECT_CURRENT;
                    SubWorkFlow = WF_BATTERY1;
                }
            }

            continue;
        }

        if (WorkFlow == WF_DETECT_VOLTAGE)
        {
            if (SubWorkFlow == WF_BATTERY2)
            {
                if ( DetectVoltage(&Batt2) ) 
                {
                    CheckBatteryStatus(&Batt2);
                    SetLcdData(&Batt2);
                    TransmitByte(LcdLineData[17]);
                    TransmitStringF("2=");
                    OutputInteger(Batt2.Voltage,OUTPUT_USART);
                    TransmitByte(13);
                    WorkFlow =  WF_CHECK_STATUS;
                    SubWorkFlow = WF_OPEN_PWM;

                }
            }

            if (SubWorkFlow == WF_BATTERY1)
            {
                if ( DetectVoltage(&Batt1) )  
                {
                    CheckBatteryStatus(&Batt1);
                    SetLcdData(&Batt1);
                    TransmitByte(LcdLineData[1]);
                    TransmitStringF("1=");
                    OutputInteger(Batt1.Voltage,OUTPUT_USART);
                    TransmitByte(13);
                    SubWorkFlow = WF_BATTERY2;
                }
            }

            continue;
        }

        if (WorkFlow == WF_CHECK_STATUS)
        {
            ShowLedStatus();
            WorkFlow = WF_SHOW_LCDDATA;
            continue;
        }
        
        if (WorkFlow == WF_SHOW_LCDDATA)
        {
            if ( ShowLcdData() ) 
            {
                WorkFlow = SubWorkFlow;
                SubWorkFlow = WF_INIT;
            }

            continue;
        }

        if (WorkFlow == WF_BATTERY_ERROR)
        {
            LCD_Clear();
            LCD_Position(1,1);
            LCD_PutStrF("Battery Error!!");
            LCD_Position(2,1);
            LCD_PutStrF("Press Stop Key.");
            WorkFlow = WF_STOP;
            SubWorkFlow = WF_STOP;
            continue;
        }

        if (WorkFlow == WF_INIT)
        {
            if (SubWorkFlow == WF_INIT)
            {
                ms2Count = 0;
                StopChargePWM();
                OpenDisCharge();
                SubWorkFlow = WF_WAIT_FOR;
            }

            if (SubWorkFlow == WF_WAIT_FOR)
            {
                if ( ms2Count >= 3000 )
                {
                    StopDisCharge();
                    WorkFlow = WF_LOOP_START;
                    SubWorkFlow = WF_INIT;
                    DisplayMessage(WF_SHOW_INFO);
                }
            }

            continue;
        }

        if (WorkFlow == WF_SHOW_INFO)
        {
            if (Key == KEY2)
            {
                Key = 0;
                LcdStatus++;

                if ( LcdStatus > 4 )
                {
                    LcdStatus = 1;
                }

                SubWorkFlow = WF_INIT;
            }

            if (SubWorkFlow == WF_INIT)
            {
                SetLcdData(&Batt1);
                SetLcdData(&Batt2);
                SubWorkFlow = WF_SHOW_LCDDATA;
            }

            if (SubWorkFlow == WF_SHOW_LCDDATA)
            {
                if ( ShowLcdData() ) 
                {
                    SubWorkFlow = WF_WAIT_FOR;
                    ms2Count = 0;
                }
            }

            if (SubWorkFlow == WF_WAIT_FOR)
            {
                if (ms2Count >= 3000)
                {
                    WorkFlow = WF_LOOP_START;
                    SubWorkFlow = WF_INIT;

                    if (ChargerStatus == WF_STOP)
                    {
                        DisplayMessage(WF_SHOW_INFO);
                    }
                }
            }

            continue;
        }
    }
}
