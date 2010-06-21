;======================================================;
;               ����˹������Ϸ����                      ;
;======================================================;
;MCU:       AT89C2051                                  ;
;OSC:       24MH                                       ;
;VER:       V1.1                                       ;
;DESIGN:    Cowboy  2009/02                            ;

;640*480@60Hz,��ʱ��Ƶ��31.46875K,��Ƶ����59.94Hz
;ϵͳ����ѭ��ѡ����ʾһ֡��ʱ��Լ16.66MS(60Hz),���������������ʱ���ڴ������.
;ֻ��128�ֽ��ڴ�,Ҫ����һ������,��ʾ�������ռ�󲿷�.�������ڵ����Խ��,ռ���ڴ��Խ��.
;�����㴰������14��*8�бȽϺ���.��ʱ��ʾ����ռ��112�ֽ�.
;���µ�16�ֽ����ڳ����еĹ�������5�ֽ�, ��ջ2�ֽ�,����Ϊȫ����.
;======================================================;
/*
����10H��7FH,ÿ�ֽ���1��С����,
��8��14�У�С�������37.5����,
����3��ʱ�����ھ���,����32����,�ظ�ɨ��32�еó�.
����VGA������ʱ��,��24M����,ÿ��Լ64��������,
����ÿ64���ڶ�Ҫ����һ����ͬ������.���鴰�ڵ�
����32*14=448��,�ϱ߿�22��,�±߿�10��,�ϼƸ�
��480��.����һ����,����2�г�ͬ��,30�г�����ǰ��,
480�п�����,13�г��������,��525��
*/
;==============ϵͳ����===============
        Hsyn            equ     02h
        Vsyn            equ     01h;�ֱ��ʾHsyn��Vsynλ

        red          	equ     13h;00010011  
        green           equ     0bh;00001011
        blue            equ     07h;00000111
        yellow          equ     1bh;00011011
	lightblue	equ	0fh;00001111
	purple		equ	17h;00010111
        white           equ     1Fh;00011111;�ֱ��ʾҪ��ʾ����ɫ
        blank           equ     03h;00000011;����ʾ����ɫΪ�հ�


;============= ���߶��� ============== 
	dataP	equ	P3       
;========================= ����ʼ =======================
        org     0000h     
;=================��ѭ��====================== 0+2=2
Loop:
        acall   V_syn;17
	acall   Delay45;62
;================ �ȴ�ɨ�赽��ʾ�� ================ 31+1=32
	nop
        mov     r6,#31;64

Wait:   acall   H_syn;17
        acall   Delay45;62
	nop
        djnz    r6,Wait;64
/*******************************
��ʾһ��
********************************/
oneline:
        acall   H_syn   ;17
	mov	dataP,#red;1
	mov	dataP,#blank;2
	mov	dataP,#yellow;3
	mov	dataP,#blank;4
	mov	dataP,#white;5
	mov	dataP,#blank;6
	mov	dataP,#purple;7
	mov	dataP,#blank;8
	mov	dataP,#lightblue;9
	mov	dataP,#blank;10
	mov	dataP,#green;11
	mov	dataP,#blank;12
	mov	dataP,#blue;13
	mov	dataP,#blank;14
	mov	dataP,#red;15
	mov	dataP,#blank;16
	mov	dataP,#yellow;17
	mov	dataP,#blank;18
	mov	dataP,#white;19
	mov	dataP,#blank;20
	mov	dataP,#purple;21
	mov	dataP,#blank;22;22*2=44;;;;;;61
	acall	delay22
	inc	r6;62
	nop
	cjne	r6,#255,oneline;64
	;---------------------
	;-------
	acall   H_syn	;17
	acall   Delay45 ;62
	nop
	mov	r6,#224;64

ddd:
	acall	H_syn;17
	acall	Delay45;62
	nop
	djnz	r6,ddd;64
	;-----------------------480


	acall   H_syn   ;17
	acall	delay45;62
	nop
	mov	r6,11;64-------------------1

xiaoyih:
	acall	H_syn;17
	acall	delay46;62
	nop
	djnz	r6,xiaoyih;64;-----------12

        acall   H_syn   ;17---------------13;13���������
	acall   Delay45;62
;============ ���д�������ɣ��ȴ���һ֡��ʼ =========
	nop
Done:  ajmp loop;64
;================= ��ͬ�� ======================		
H_syn:  nop
	nop
	mov     dataP,#Hsyn   ;2        ;(2+13+2)=17T
	nop;3
	nop;4
	nop;5
	nop;6
	;nop;7;��������������Ҳ����ֻҪ61������?
	;nop;8
	;nop;9

	nop
	mov    dataP,#(Hsyn or Vsyn)  ;11
	nop;12
	nop;13
	nop
        ret;
;================= ��ͬ�� ======================		
V_syn:  nop
	nop
	mov     dataP,#0;2
	nop;3
	nop;4
	nop;5
	nop;6
	nop;7
	nop;8
	nop;9
	nop
	mov  dataP,#Vsyn   ;11
	nop;12
	nop;13
	nop
        ret                
;==================== ��ʱ���� ======================
Delay69:nop
Delay68:nop
Delay67:nop
Delay66:nop
Delay65:nop
Delay64:nop
Delay63:nop
Delay62:nop
Delay61:nop
Delay60:nop
Delay59:nop
Delay58:nop
Delay57:nop
Delay56:nop
Delay55:nop
Delay54:nop
Delay53:nop
Delay52:nop
Delay51:nop
Delay50:nop
Delay49:nop
Delay48:nop
Delay47:nop
Delay46:nop
Delay45:nop
Delay44:nop
Delay43:nop
Delay42:nop
Delay41:nop
Delay40:nop
Delay39:nop
Delay38:nop
Delay37:nop
Delay36:nop
Delay35:nop
Delay34:nop
Delay33:nop
Delay32:nop
Delay31:nop
Delay30:nop
Delay29:nop
Delay28:nop
Delay27:nop
Delay26:nop
Delay25:nop
Delay24:nop
Delay23:nop
Delay22:nop
Delay21:nop
Delay20:nop
Delay19:nop
Delay18:nop
Delay17:nop
Delay16:nop
Delay15:nop
Delay14:nop
Delay13:nop
Delay12:nop
Delay11:nop
Delay10:nop
Delay09:nop
Delay08:nop
Delay07:nop
Delay06:nop
Delay05:nop
Delay04:
nop
nop
ret
;==================== ������� ======================
        end
