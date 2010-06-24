#ifndef M8_CHARAGER_INCLUDE
#define M8_CHARAGER_INCLUDE

flash BYTE Product[] = "NI-MN Charger";
flash BYTE Version[] = "Version 1.0.6";

#define LED_ON          0
#define LED_OFF         1
#define PWM_ON          0
#define PWM_OFF         1
#define DIS_ON          1
#define DIS_OFF         0

#define LED1            PORTB.3
#define LED1_IS_ON      PINB.3

#define LED2            PORTB.4
#define LED2_IS_ON      PINB.4

#define LEDT            PORTB.4
#define LEDT_IS_ON      PINB.4

#define KEY1_IS_ON      PIND.2
#define KEY2_IS_ON      PIND.3

#define CHR1            PORTB.1
#define CHR2            PORTB.2
#define DCHR1           PORTC.5
#define DCHR2           PORTB.0

#define CHR1_PWM        OCR1B
#define CHR2_PWM        OCR1A
#define PWM1_OUT        BIT(COM1B1) | BIT(COM1B0)
#define PWM2_OUT        BIT(COM1A1) | BIT(COM1A0)

//ʱ���жϼĳ���
register WORD ms2Count;
register BYTE msLedCount;
register WORD sc3Count;
bit LEDStatus;

bit LED1Status_on_off;
bit LED1Status_inv;

bit LED2Status_on_off;
bit LED2Status_inv;

#define KEY1            1
#define KEY2            2

register BYTE Key;

volatile BYTE ChargerStatus;

#define OUTPUT_LCD      1
#define OUTPUT_USART    2

//��ؼ�����
struct BATTERY{
    BYTE Index;                  //��ز۱��
    BYTE Status;                 //��ز�״̬
    BYTE VoltageError;           //��ѹ����״̬����
    BYTE CurrentError;           //��������״̬����

    WORD Start_Voltage;          //��ظշ����ز�ʱ��ĵ�ѹ
    WORD Max_Voltage;            //�������е�ص���ߵ�ѹ
    WORD Last_Voltage[2];        //�����ĵ�ص�ѹ
    WORD Voltage;                //��ǰ�ĵ�ص�ѹ

    WORD Last_Current[2];        //�����ĳ�����
    WORD Current;                //��ǰ�ĳ�����

    WORD PWM;                    //��ǰ��PWMֵ
    WORD Time;                   //�ܼƳ��ʱ�䣬��λΪ������ŵ�����
    } 
    Batt1,                       //1�ŵ�ز�
    Batt2;                       //2�ŵ�ز�

//ϵͳ���̹���״̬��
#define WF_INIT             0
#define WF_BATTERY1         1
#define WF_BATTERY2         2
#define WF_START            3
#define WF_STOP             4
#define WF_DETECT_VOLTAGE   5
#define WF_DETECT_CURRENT   6
#define WF_OPEN_PWM         7
#define WF_ADJUST_PWM       8
#define WF_COMPLETE_PWM     9
#define WF_COMPLETE_DCHR    10
#define WF_WAIT_FOR         11
#define WF_SHOW_INFO        12
#define WF_CHECK_STATUS     13
#define WF_BATTERY_ERROR    14

#define WF_SHOW_LCDDATA     80
#define WF_LOOP_START       99
#define WF_COMPLETE         0x80

#define WorkFlowInit()        WorkFlow = WF_INIT
#define WorkFlowComplete()    WorkFlow |= WF_COMPLETE
#define WorkFlowIsComplete()  (WorkFlow & WF_COMPLETE)

//ϵͳ���̹���״̬�Ĵ���
register BYTE WorkFlow;

//ϵͳ�����̹���״̬�Ĵ���
register BYTE SubWorkFlow;

//ADCӲ�����ӵ�ͨ��
#define ADC_CH1_VOL          2
#define ADC_CH1_CUR          3
#define ADC_CH2_VOL          0
#define ADC_CH2_CUR          1

//��ѹADCȡ������
#define VOLTAGE_SIMPLE       8

//��ѹADCȡ����������
#define VSIMPLE_DELTA        4

//��ѹADCȡ��������������
#define VSIMPLE_SHIFT        3

//��ѹADCȡ�μ��ʱ��
#define VSIMPLE_WAITFOR      0

//����ADCȡ������
#define CURRENT_SIMPLE       32

//����ADCȡ����������
#define CSIMPLE_DELTA        16

//����ADCȡ��������������
#define CSIMPLE_SHIFT        5

//��ѹADCȡ�μ��ʱ��
#define CSIMPLE_WAITFOR      7

//���״̬�ͳ��ģʽ
#define BATT_EMPTY          0
#define BATT_PRI_CH         1
#define BATT_QUICK_CH       2
#define BATT_TINY_CH        3
#define BATT_ERROR          4

//PWM����ģʽ����ֵ��ֻ��DEBUGģʽʹ��
#define BATT_TEST_CH           5
#define PWM_TEST_CH            730

//PWMԤ���Ԥ��ֵ��150mA~200mA
#define PWM_PRI_CH             450
//PWM������Ԥ��ֵ��25~35mA
#define PWM_TINY_CH            180

//PWM���ٳ��Ԥ��ֵ
#define PWM_QUICK_CH          500
//PWM���ٳ�����ֵ��Լ1000mA
#define PWM_QUICK_MAX         730
//PWM���ٳ����Сֵ280mA~320mA
#define PWM_QUICK_MIN         500
//PWM���ٳ�粽��ֵ
#define PWM_QUICK_STEP         1

//����������������ΧֵmA
#define CURRENT_AREA           45

//���������
#define MAX_ERROR              10

//��С��Ч��ѹ����춴˵�ѹ�ж�Ϊ�޵�أ���λ������
#define MIN_VOLTAGE            100

//��ʼ�ж��޳���ص�ʱ�䣬��λ����
#define CHECK_CURRENT_TIME     3

//��ʼ�жϸ�Delta��ʱ�䣬��λ����
#define CHECK_DELTA_TIME       180

//����ѭ������������ʱ�䣬��λ������
#define CURRENT_ADJ_TIME       50

//��PWM�󣬼����������ʱ�䣬��λ������
#define MAX_AFTER_TIME         100

//������PWM�󣬼������PWMʱ�䣬��λ������
#define MAX_PWM_TIME           500

//���δ򿪷ŵ�DCHRʱ�䣬��λ������
#define MAX_DCHR_TIME         20

//һ����ŵ����ں�ֹͣ��Ϣ��ʱ�䣬��λ������
#define MAX_WAIT_FOR          20

//��ص�춴�ֵ������Ԥ���ģʽ
#define VOLTAGE_PRI      900

//��ظ��PRIֵ������춴�ֵ��������ٳ��ģʽ
#define VOLTAGE_QUICK    1450

//��ظ��QUICKֵ������춴�ֵ������������ģʽ
//��������QUICKģʽ�У���춴�ֵ��ʱ�򣬽���������ģʽ
#define VOLTAGE_TINY     1600

//��ظ�춴�ֵ��ʱ�����ж��ǲ���������͵ĵ�ء�
//�п���Ҳ����ΪӲ����ĵ�����������⡣
#define VOLTAGE_ERROR    2000

//�ο���ѹ
#define AREF_VOLTAGE         3084

//��ѹת����������
#define V_AD_HIGH             3
#define V_AD_LOW              4
#define V_AD_SHIFT            8
#define ADC_TO_VOL(V,D)       V = (D * V_AD_HIGH) + ( (D * V_AD_LOW) >> V_AD_SHIFT )


//����ת����������
#define A_AD_HIGH             6
#define A_AD_LOW              53
#define A_AD_SHIFT            7
#define ADC_TO_CUR(A,D)       A = (D * A_AD_HIGH) + ( (D * A_AD_LOW) >> A_AD_SHIFT )

//LCD��ʾ������
BYTE LcdLineData[33];
//LCD��ʾ��Ϣ��ʽ
register BYTE LcdStatus;
//LCD����������
register BYTE LcdLineIndex;

//����������λ������
#define MAXSET_CURRENT  1000
#define MINSET_CURRENT  100
//�û��趨����߳�����
volatile WORD  Set_Current;
eeprom   WORD eSet_Current = 500;

//���ʱ�䣬��λ����
#define MAXSET_MAXTIME   28800
#define MINSET_MAXTIME   3600
//�����ʱ��
volatile WORD  Set_MaxTime;
eeprom   WORD eSet_MaxTime = MAXSET_MAXTIME;

//��Delta�ж�ֵ
#define MAXSET_DELTA     9
#define MINSET_DELTA     3

volatile BYTE  Set_Delta;
eeprom   BYTE eSet_Delta = MINSET_DELTA;

//��ʼ�жϸ�Delta�ĳ�ʼ��ѹ����λ������
#define MAXSET_DBV     1500
#define MINSET_DBV     1400

volatile WORD  Set_DBV;
eeprom   WORD eSet_DBV = MINSET_DBV;

#endif


