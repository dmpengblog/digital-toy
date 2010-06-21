

;======================================================;
;               ����˹������Ϸ����                     ;
;======================================================;
;MCU:       AT89C2051 or AT89C4051                     ;
;OSC:       24MHz                                      ;
;VER:       V3.3                                       ;
;DESIGN:    Cowboy  2009/04                            ;
;======================================================;

;==============ϵͳ����===============
        RightKey        equ     10111111b      ;�ⲿ���Ǹ�����������ʱ�ļ���
        DownKey         equ     11011111b
        OKKey           equ     11101111b
        LeftKey         equ     11110111b
        UpKey           equ     11111011b
        AKey            equ     11111101b
        BKey            equ     11111110b
        EmptyKey        equ     11111111b

        Hsyn            equ     00000010b      ;�ⲿ����P3������ĸ����ź�/��ɫ
        Vsyn            equ     00000001b
        Blue          	equ     00110011b
        Green           equ     00101011b
        Red             equ     00100111b
        Orange          equ     00001111b
        Yellow          equ     00101111b
        Cyan            equ     00111011b
        Purple          equ     00110111b
        Gray            equ     00001011b
        White           equ     00111111b
        Blank           equ     00000011b

;==============��������==============
        Locate  equ     R2      ;����λ��
        Freq    equ     R3      ;����Ƶ��
        Counter equ     R4      ;���̶�ʱ������
        Wave    equ     TL0     ;���ַ������ڼ���
        Rnd     equ     TH0     ;�����
        Score   equ     TL1     ;���ֵ�λ
        ScoreH  equ     TH1     ;���ָ�λ
        Type2   equ     SCON    ;��һ��������

        Pos     data    0fh     ;������ת�ķ���
        Speed   data    0eh     ;��ǰ�ٶ�
        Key     data    0dh     ;���̼���
        Type    data    0ch     ;��ǰ��������
        Mus_Len data    0bh     ;��������
        Timer   data    0ah     ;�������䶨ʱ��

        Delete  bit     IT0     ;���/ɾ��״̬
        Pause   bit     IT1     ;��ͣ״̬
        Mute    bit     PSW.1   ;����ֹͣ״̬
        New     bit     TB8     ;�·���������
        Over    bit     RB8     ;��Ϸ�������

;============= ���߶��� ==============
        Beep    bit     P1.7    ;�������ӿ�
        CNK     bit     P1.4    ;���ļ��ӿ�
        RUK     bit     P1.2    ;�����������
        LDK     bit     P1.3    ;�����������
        RDK     bit     P1.5    ;�����������
        LUK     bit     P1.6    ;�����������

        IO_P    bit     P3.7    ;խ�����������
        IO_L    bit     P3.5    ;��ɫ���ȿ��ƿ���
        IO_B    bit     P3.4    ;��ɫ�ź��������
        IO_G    bit     P3.3    ;��ɫ�ź��������
        IO_R    bit     P3.2    ;��ɫ�ź��������
        IO_V    bit     P3.1    ;��ͬ�������������
        IO_H    bit     P3.0    ;��ͬ�������������

;======================= ����ʼ ==========================
        org     0000h
Boot:   ljmp    Welcome   ;�����MCU��4051,ѡ�ô����,������ֿ�������,�������źŷ�����
       ;ljmp    Start     ;�����MCU��2051,ѡ�ô����,û�п����������źŷ���������

;======================= ��Ϸ��ʼ ==========================
        org     0030h
Start:  mov     a,#Blank  ;����Դ�
        mov     r0,#7fh   ;0x10~0x7f���������ڵ���ʾ����
Clear:  mov     @r0,a
        dec     r0
        cjne    r0,#0fh,Clear
Zero:   clr     a         ;���������
        mov     @r0,a     ;0x00~0x0fΪ���������ڴ�
        djnz    r0,Zero
Init:   clr     a         ;������ʼ��
        mov     Score,a
        mov     ScoreH,a
        mov     Type,a
        mov     Type2,a
        mov     Key,#UpKey
        mov     Counter,#75
        mov     Speed,#90
        mov     Timer,Speed
        mov     dptr,#Music4
        clr     Delete
        clr     Mute
        clr     Over
        setb    Pause
        setb    New
        mov     Freq,#1
        mov     Mus_Len,#1
        ajmp    NewBlock  ;���Ȳ�����һ������
        
        
;====================== ��ͬ�� ======================== 0+2=2
;�µ�һ֡��������￪ʼ,��һ֡�����ʱ����,����ÿ��32us
;����һ����ͬ���ź�,�ۺ�Ϊ64�Ļ�������.�������ÿ�к���
;ע���е���������ͬ���󾭹��Ļ���������.��ʹ�ö��ж���
;��Ϊ�ж���Ӧʱ��Ĳ�ȷ����Ӱ����ȷ��ʱ��
;======================================================        


Loop:   acall   V_syn   ;14   ;��ͬ�����忪ʼ
        acall   Delay50 ;64   ;��ʱ50US
        ;------------------
        acall   V_syn   ;14   ;��ͬ������ʱ��Ҫ��������ʱ��
        inc     Rnd     ;15   ;�����������������
        ;============================
        jb      Delete,Search   ;17  ;��������״̬,ȷ���Ƿ���Ҫ��������
        mov     r6,#29          ;18  ;��û��Ҫ,�����ʱ���ǳ�������,
        acall   Delay44         ;62   ֻ�趨ʱ������ͬ��,û����������.
        ajmp    Wait            ;64

;==================== �������� ===================== 2+14=16
;��γ����Ǽ�����е�����,�Ƿ����һ���ж������˷���,
;����r1��¼�����������һ�е���ַ
;===================================================
Search: acall   Delay43 ;60     
        mov     r6,#14  ;61      
        mov     r0,#10h ;62      
        mov     r1,#0   ;63
Lp_Sch: nop             ;64
        acall   H_syn   ;14
        mov     r7,#8   ;15
        mov     a,#255  ;16
LpC8:   anl     a,@r0   ;+4*8
        inc     r0
        djnz    r7,LpC8 ;48
        jb      acc.5,S1;50
        sjmp    S2      ;52
     S1:mov     r1,0;r0 ;52      ;r1��¼���һ������λ��
     S2:acall   Delay09 ;61
        djnz    r6,Lp_Sch;63

;=================== ɾ��һ���� ======================= 16+15=31
;��һ�γ����Ǹ������������Ľ��r1,ȷ���Ƿ�Ҫɾ��һ����
;���ɾ����һ��,����ķ���Ҫ����������һ��,�����������
;======================================================
        mov     r6,#13  ;64   
Find:   acall   H_syn   ;14    
        mov     a,r1    ;15
        cjne    a,0,NoChg;17  R0��R1����ȣ�˵��R0���ڵ���δ��
        ;�ҵ�����,���� R1 ָ����һ��
        add     a,#-8   ;18
        mov     r1,a    ;19
        mov     a,#10   ;20
        acall   Add_Score;+19=39
Get_Mus:mov     dptr,#Music1 ;41
        clr     Mute    ;42
        acall   Delay20 ;62
        jmp     Copy    ;64

        ;����δ������������һ��
NoChg:  mov     a,#-8   ;18     ;R0 ����һ��(-8)
        add     a,r0    ;19
        mov     r0,a    ;20
        acall   Delay42 ;62
        djnz    r6,Find ;64

        ;���е��ж�����
        acall   H_syn   ;14   line 30
        jnb     Mute,S3 ;16
        ajmp    NewBlock;18
     S3:mov     r6,#1   ;17
        acall   Delay45 ;62
        ajmp    Wait    ;64

        ;ɾ��һ�к����淽������������
Copy:   acall   H_syn   ;14
        mov     r7,#4   ;15
LpD8:   dec     r1      ;+10*4
        dec     r0
        mov     a,@r1
        mov     @r0,a
        dec     r1
        dec     r0
        mov     a,@r1
        mov     @r0,a
        djnz    r7,LpD8 ;55
        acall   Delay07 ;62
        djnz    r6,Copy ;64
        ;----------------------
        acall   H_syn;(31);14        ;��һ�����
        mov     r0,#18h ;15
LpC:    dec     r0      ;+4*8
        mov     @r0,#Blank
        cjne    r0,#10h,LpC;47
        acall   Delay15  ;62
        ajmp    Get_Key  ;64

;================ �ȴ�ɨ�赽��ʾ�� ================ 31+1=32
;�ڳ������ڼ�ֻ�趨�ڲ�����ͬ���ź�,û����������.
;���ڶ����ڲ�ͬ,r6��ֵҲ��ͬ,�����µȴ�ʱ�䲻ͬ,
;������Ч�����ǲ�һ�о͵�����Ч��ʾ��
;==================================================
Wait:   acall   H_syn	;14
        acall   Delay48 ;62
        djnz    r6,Wait	;64

;================== ����ɨ�� ======================
;�˴���ⰴ��״̬,��r5��¼����,�����,���������
;������ʹ��.
;==================================================
Get_Key:acall   H_syn       ;14
        mov     a,P1        ;15
        orl     a,#10000011b;16
        mov     r5,a        ;17
        orl     a,#10010011b;18
        cjne    a,#255,S6   ;20
        sjmp    S7          ;22
     S6:orl     5,#00010000b;22  5 equ r5
     S7:acall   Delay42     ;64
     
;=================== ��ʾ�� =======================
;�����￪ʼ������ʾ����,��480��,����ÿ32��ɨ����Ϊ
;1��������,14�������о��� 32*14 = 448 ��,��ʣ��32
;��ɨ����,��Ϊ������,����20���������ڴ����ϱ߿�,
;����10����ʾ�����±߿�
;==================================================     

;=============== ��ʾ�����ϱ߿����� ===============
        acall   H_syn   ;14
        acall   Line    ;56
        mov     r6,#20  ;57
        mov     r0,#10h	;58
        mov     r1,#32	;59
        acall   Delay05 ;64
        acall   H_syn   ;14
        acall   Line    ;56
        mov     b,dpl   ;58  ;����dptr��ȫ�ֱ���ʹ��,���������
        acall   Delay04 ;62   ��dptr������,�ʴ�����b��ʱ����һ��.
                
;================ ��ʾ�����ϱ߿� ===============
Border1:nop             ;63
        nop             ;64
LpB:    acall   H_syn	;14
        ;---------------------
        mov     a,Type2 ;15       ;�߿��Ҳ�Ҫ��ʾ��һ�������ͼ��
        swap    a       ;16       ;�ڴ��ȶ�Type2���ת��,�����r7
        rr      a       ;17
        anl     a,#00000111b;18
        mov     r7,a    ;19
        acall   Delay08 ;27
        ;---------------------
        setb    IO_G    ;28       ;ī��ɫ�ı߿�
        jb      Pause,Flash ;30   ;ѡ����ʾ��ʾͼ����,����ͣͼ��
        
        ;���³����Ǹ��ݲ�ͬ�ķ�������,���ò�ͬ���ӳ�������ʾС����ͼ��
        ;����ʱ���޵ú���,���ܲ��ó����ɢת����,�������һ�ַǵ��͵�
        ;�������,���س��ڵ��ӳ������,������ܱȽ������,��ѧ�߲�����
        ;��,��Ϊֱ�Ӳ�����ջָ����ʮ��С��,��������ܷ�.
        
Prompt: cjne    r6,#11,$+3  ;32   ;��һ������ʾͼ����ʾ����
        djnz    r6,L32      ;34
        mov     a,#low(LpV) ;35   ;�趨�ӳ��򷵻�·��
        push    acc         ;37
        mov     a,#high(LpV);38
        push    acc         ;40
        ajmp    L33         ;42
    L32:mov     a,#low(LpB) ;35
        push    acc         ;37
        mov     a,#high(LpB);38
        push    acc         ;40
        nop                 ;41
        nop                 ;42
        ;----------------------
    L33:nop               ;43 ����ɢת����ָ��
        mov     a,r7      ;44
        addc    a,r7      ;45 a = Type2*2 + c
        rl      a         ;46 a = Type2*4 + c*2
        mov     r7,a      ;47
        rl      a         ;48 a = Type2*8 + c*4
        rl      a         ;49 a = Type2*16 + c*8
        add     a,r7      ;50 a = Type2*20 + c*10
        ;clr c
        mov     dptr,#Icon;52
        mov     P3,#Blank ;54
        jmp     @a+dptr   ;56 ���ݲ�ͬͼ���ֱ���ʾ
                          ;+8 =64
        ;-----------------------
Flash:  mov     a,Rnd     ;31 ������ʾ��ͣ����
        mov     c,acc.5   ;32
        subb    a,acc     ;33
        anl     a,#00111100b;34
        acall   Delay18   ;52
        mov     P3,#Blank ;54
        nop               ;55
        clr     c         ;56
        xrl     P3,a      ;57
        xrl     P3,a      ;58
        xrl     P3,a      ;59
        xrl     P3,a      ;60
        djnz    r6,Border1;62
        sjmp    LpV       ;64

;=================== ��ʾ���� ===================== 
;�ⲿ������ʾ�����ڷ��������,���ڵ����ұ߿�,ͬʱ��
;���������ʾһ�Ʒ���,�Ʒ�����ɫ��ԭ���ĳ�ɫ,���ŵ�
;�ֵ�����,�𼶱�Ϊ��ɫ.�������Ʒ�������,���ڴ򱬻���
;==================================================
View:   clr     c       ;63 
        nop             ;64
LpV:    acall   H_syn	;14
        nop
        nop
        nop
        ;�ڴ��������ʾһ���Ƿ���
        mov     a,r0    ;0xxxxxxx
        rr      a
        rr      a       ;xx0xxxxx
        addc    a,ScoreH;xx?xxxxx
        anl     a,#00100000b;00?00000
        orl     a,#00001100b;00?01100
        xrl     P3,a
        xrl     P3,a

        ;��������ʾ�����ڵķ���
        mov     a,@r0   ;26
        mov     P3,#Gray;28 ��������߿�
        mov     P3,a    ;29
        inc     r0      ;30
        mov     P3,@r0
        inc     r0
        mov     P3,@r0
        inc     r0
        mov     P3,@r0
        inc     r0
        mov     P3,@r0
        inc     r0
        mov     P3,@r0
        inc     r0
        mov     P3,@r0
        inc     r0
        mov     P3,@r0	;+23 50
        mov     a,#Blank;51
        mov     P3,#Gray;53 �������ұ߿�
        mov     P3,a    ;54
        cjne    r1,#16,$+3;56 ;r1��16�Ƚ�,�������c��
        djnz    r1,NextL;58
        ;--------------------------------
        mov     r1,#32  ;59   ;ÿ32��ɨ����Ϊһ��������
        inc     r0      ;60
        cjne    r0,#80h,View;62
        ajmp    Border2
        ;--------------------------------
NextL:  anl     0,#01111000b;60 ��һ��ɨ����,R0 ָ�ط�������
        nop                 ;61
        mov     r6,#8   ;62
        ajmp    LpV	;64


;===================== ��ʾ�±߿� ================= 2
Border2:acall   H_syn	;14
        acall   Delay13 ;27
        setb    IO_G    ;28
        acall   Delay25 ;53
        clr     IO_G    ;54
        acall   Delay08 ;62
        djnz    r6,Border2;64
        ;�ڱ߿��»������߹����������ǿ�����
        acall   H_syn   ;14
        acall   Line    ;56
        acall   Delay08 ;64
        acall   H_syn   ;14
        acall   Line    ;56
        mov     dpl,b   ;58  ;dptr��ǰ�����ƻ����ָ�����
        mov     dph,#high(Music0);60
        acall   Delay04 ;64

;===================== ����Ч������ ==================== -13
Music:
        acall   H_syn           ;14
        jb      Mute,Skip1      ;16  ;����,����
        djnz    Mus_Len,Skip2   ;18  ;��ǰ����δ����,Ҳ����
        clr     a               ;19
        movc    a,@a+dptr       ;21
        mov     Freq,a          ;22  ;��ȡ����
        clr     a               ;23
        inc     dptr            ;25
        movc    a,@a+dptr       ;27
        mov     Mus_Len,a       ;28  ;��ȡ����
        inc     dptr            ;30
        cjne    a,#2,S10        ;32  ;�б��Ƿ����ֽ���
    S10:mov     Mute,c          ;34
        acall   Delay08         ;42
        ajmp    Chk_End         ;44
        ;--------------------------
Skip1:  nop                     ;17
        nop                     ;18
Skip2:  acall   Delay26         ;44

;================= ������״̬�ֱ��� ==================
;�ⲿ��������״̬��,��ͬ��״̬,���в�ͬ�Ĵ���ʽ
;=======================================================
Chk_End:jnb     Over,Chk_New    ;46  ;�Ƿ���Ϸ����״̬
        cjne    r5,#UpKey,Stop  ;48  ;��"��"�����¿�ʼ��Ϸ
Reset:  ajmp    Start
        ;--------------------------
Stop:   mov     r6,#12          ;49
        jb      Mute,GetMus5    ;51
        mov     Rnd,#0          ;53
        acall   Delay07         ;60
        ajmp    Done            ;62
        ;--------------------------
GetMus5:mov     dptr,#Music5    ;53  ;��Ϸ����ʱҪ���ŵ�����
        mov     a,Rnd           ;54  if (Rnd == 0) Mute = 0; else Mute = 1;
        add     a,#255          ;55
        mov     Mute,c          ;57
        nop                     ;58
        nop                     ;59
        nop                     ;60
        ajmp    Done            ;62
        ;--------------------------
Chk_New:jnb     New,Chk_Del     ;48  ;�Ƿ��·������״̬
        acall   Delay16         ;64
        acall   H_syn           ;14
        mov     a,Locate        ;15
        mov     r7,a            ;16
        mov     r6,Pos          ;18
        mov     b,#0            ;20
        acall   Delay37         ;57
        ajmp    Move            ;59
        ;--------------------------
Chk_Del:acall   Delay09         ;57  ;�Ƿ����м��/ɾ��״̬
        mov     r6,#12          ;58
        jnb     Delete,Drop     ;60
        ajmp    Done            ;62

;=================== ���鶨ʱ�Զ����� ================== -12
Drop:   acall   Delay04         ;64
        acall   H_syn           ;14
        jb      Pause,Scn_Key   ;16  ;��ͣ״̬�����ƶ�
        djnz    Timer,Scn_Key2  ;18  
        mov     Timer,Speed     ;20
        mov     dptr,#Music0    ;22  ;�����Զ�������Ч
        clr     Mute            ;23
        mov     a,Locate        ;24
        add     a,#8            ;25
        mov     r7,a            ;26
        mov     r6,Pos          ;28
        setb    F0              ;29  ;�趨�����ƶ��ı�־
        mov     b,#0            ;31  ;�����ײ��־
        acall   Delay26         ;57
        ajmp    Move            ;59

;===================== ����ɨ�账�� ==================== -12.5
;�ⲿ���ǰ����������,�е㸴��,��ΪҪ���������϶�,��Ҫ
;����������,��������,��������,��ͬ����������,�����ƶ��Ķ�
;����,Ҫ����һ���µķ�������r7,�Լ�һ���µ���ת�Ƕ�r6,��
;�����ƶ�����ʹ��,����ʼ������־b,���ƶ������־f0.
;=======================================================
Scn_Key:nop                     ;17
        nop                     ;18
Scn_Key2:
        inc     Counter         ;19   ������ʱ������
        mov     a,r5            ;20   ȡ�ü���
        cjne    a,Key,KeyChg    ;22

Click:  cjne    Counter,#2,Brust;24   ����
        cjne    a,#EmptyKey,L40 ;26
        mov     Rnd,#0          ;28
        acall   Delay29         ;57
        ajmp    None            ;59
    L40:mov     dptr,#Music2    ;28   ������
        clr     Mute            ;29
        nop                     ;30
        sjmp    Branch          ;32

        ;---------------------------
KeyChg: mov     Key,a           ;23   ����״̬�б䶯
        mov     Counter,#0      ;24
        acall   Delay33         ;57
        ajmp    None            ;59
        ;---------------------------

Brust:  cjne    Counter,#30,None2;26  ��������
        mov     Counter,#24     ;27
        cjne    a,#EmptyKey,L41 ;29
        nop                     ;30
        sjmp    Branch          ;32
    L41:mov     dptr,#Music3    ;31   ������
        clr     Mute            ;32

;------------- ���ݰ���ִ����Ӧ���� ------------
Branch: nop                     ;33
Up:     cjne    a,#UpKey,Down   ;35   �ϼ�����ͣ
        mov     Timer,Speed     ;37
        cpl     Pause           ;38
        acall   Delay19         ;57
        ajmp    None            ;59

Down:   jb      Pause,None1     ;37   ��ͣʱ����Ӧ������
        cjne    a,#DownKey,Left ;39
        mov     a,Locate        ;40   �¼�����
        add     a,#8            ;41
        mov     r7,a            ;42
        mov     r6,Pos          ;44
        setb    F0              ;45
        mov     b,#0            ;47
        acall   Delay10         ;57
        ajmp    Move            ;59

Left:   cjne    a,#LeftKey,Right;41
        mov     a,Locate        ;42   �������
        mov     r7,a            ;43
        dec     r7              ;44
        mov     r6,Pos          ;47
        mov     b,#10000000b    ;48
        acall   Delay09         ;57
        ajmp    Move            ;59

Right:  cjne    a,#RightKey,OK  ;43
        mov     a,Locate        ;44   �Ҽ�����
        mov     r7,a            ;45
        inc     r7              ;46
        mov     r6,Pos          ;48
        mov     b,#01000000b    ;50
        acall   Delay07         ;57
        ajmp    Move            ;59

OK:     cjne    a,#OKKey,A_Key  ;45   ȷ��/��ת������
        mov     7,Locate;r7     ;47
        mov     a,Pos           ;48
        inc     a               ;49
        anl     a,#00000011b    ;50
        mov     r6,a            ;51
        mov     b,#11000000b    ;53
        acall   Delay04         ;57
        ajmp    Move            ;59

A_Key:  cjne    a,#AKey,B_Key   ;47   �󱸼���û����
        acall   Delay10         ;57
        ajmp    None            ;59

B_Key:  cjne    a,#BKey,NoKey   ;49   �󱸼���û����
        acall   Delay08         ;57
        ajmp    None            ;59

NoKey:  acall   Delay08         ;57   ����ļ���
        ajmp    None            ;59
;----------------------------------
None2:  acall   Delay11         ;37
None1:  acall   Delay22         ;59
None:   mov     r6,#11          ;60
        ajmp    Done            ;62

;================== �ƶ����� ====================== -11
;��һ�����ǰѷ����ƶ�,����˼·���ȼ���µ�λ���Ƿ�
;Խ��������,���Խ��Ͳ�Ҫ�ƶ�,û��Խ��,��Ҫ�ж��Ƿ�
;����ײ��ķ��鷢����ײ,û����ײ������²����ƶ�,�ƶ�
;��������ԭλ�ò���,ȷ����λ��û�з�����ײ(�ص�),����
;λ����ͼ.
;==================================================
Move:
        ;r7     new Locate
        ;r6     new Position
        ;======= ��λ��Խ���� =====
        mov     a,Type          ;60   ������ָ����r1
        rl      a               ;61
        rl      a               ;62
        orl     a,r6            ;63
        mov     r1,a            ;64

        acall   H_syn           ;14
        mov     a,r1            ;15  ;����r1����ж��Ƿ�Խ��
        add     a,#(LU_over-$-3);16
        movc    a,@a+pc         ;18
        mov     r5,a            ;19
        jbc     b.7,Chk_L       ;21
        acall   Delay04         ;25
        sjmp    Chk_U           ;27
Chk_L:  xrl     a,r7            ;22   ����Ƿ���Խ��
        anl     a,#00000111b    ;23
        jz      S50             ;25
        sjmp    Chk_U           ;27
    S50:orl     b,#00000001b    ;27
Chk_U:  mov     a,r5            ;28   ����Ƿ���Խ��
        xrl     a,r7            ;29
        anl     a,#01111000b    ;30
        jz      S51             ;32
        sjmp    S52             ;34
    S51:orl     b,#00000100b    ;34

    S52:mov     a,r1            ;35
        add     a,#(RD_over-$-3);36
        movc    a,@a+pc         ;38
        mov     r5,a            ;39
        jbc     b.6,Chk_R       ;41
        acall   Delay04         ;45
        sjmp    Chk_D           ;47
Chk_R:  xrl     a,r7            ;42   ����Ƿ���Խ��
        anl     a,#00000111b    ;43
        jz      S53             ;45
        sjmp    Chk_D           ;47
    S53:orl     b,#00000010b    ;47
Chk_D:  mov     a,r5            ;48   ����Ƿ���Խ��
        xrl     a,r7            ;49
        anl     a,#01111000b    ;50
        jz      S54             ;52
        sjmp    S55             ;54
    S54:orl     b,#00001000b    ;54
    S55:acall   Delay10         ;64

    ;========= �����εĶ����ж� ========== -10
        acall   H_syn           ;14   ������Խ�������Ť
        mov     a,Key           ;15
        cjne    a,#OkKey,S56    ;17   if (Key = OkKey   \
        mov     a,Type          ;18
        jnz     S57             ;20       && Type == 0 )
        mov     a,r7            ;21      {
        anl     a,#00000111b    ;22      if (X == 7)
        jz      S58             ;24
        sjmp    S59             ;26           b |= 0x40;
    S58:setb    b.6             ;25
        nop                     ;26
    S59:mov     a,r7            ;27
        anl     a,#01111000b    ;28
        cjne    a,#10h,S60      ;30      else if (Y == 1)
        setb    b.7             ;31           b |= 0x80;
        sjmp    S60             ;33       }
    S56:nop                     ;18
        nop                     ;19
        nop                     ;20
    S57:acall Delay10           ;30

        ;======= �ж��Ƿ�Խ�� ============
    S60:mov     a,b             ;31     b ��Ϊ���ʾԽ��
        jnz     Outside         ;33
        jnb     New,Inside      ;35
        ;==== �����ɵķ���Ҫ�趨��ɫ =====
SetCol: mov     a,Type          ;36
        inc     a               ;37
        rl      a               ;38
        rl      a               ;39
        orl     a,#00100011b    ;40
        mov     dph,a           ;41     �� dph ������ɫ����
        acall   Delay23         ;64
        acall   H_syn           ;14
        acall   Delay47         ;61
        ajmp    Check           ;63

        ;======== ����ûԽ�� =============
Inside: acall   Delay27         ;62
        ajmp    Erase           ;64

        ;======== ����Խ���� =============
Outside:mov     c,acc.3         ;34
        mov     Delete,c        ;36
        acall   Delay23         ;59
        mov     r6,#9           ;60
        ajmp    Done            ;62


;=========== ���ַ������/�ϲ�Խ����==========
LU_over:db  1*8+1, 3*8+7, 1*8+1, 3*8+7
        db  1*8+7, 1*8+7, 1*8+7, 1*8+7
        db  1*8+0, 2*8+0, 2*8+0, 2*8+7
        db  2*8+0, 2*8+7, 1*8+0, 2*8+0
        db  2*8+0, 2*8+7, 1*8+0, 2*8+0
        db  2*8+0, 2*8+7, 2*8+0, 2*8+7
        db  1*8+0, 2*8+0, 1*8+0, 2*8+0

;=========== ���ַ����Ҳ�/�²�Խ����==========
RD_over:db  0*8+7,15*8+0, 0*8+7,15*8+0
        db 15*8+7,15*8+7,15*8+7,15*8+7
        db 15*8+7,15*8+0, 0*8+7,15*8+7
        db  0*8+7,15*8+7,15*8+7,15*8+0
        db  0*8+7,15*8+7,15*8+7,15*8+0
        db  0*8+7,15*8+7, 0*8+7,15*8+7
        db 15*8+7,15*8+0,15*8+7,15*8+0


        ;r7     new Locate
        ;r6     new Position
        ;======= ����ԭ���� ===== -9
Erase:  acall   H_syn         ;14
        nop                   ;15
        mov     0,Locate;r0   ;17
        mov     dph,@r0       ;19
        mov     r5,#4         ;20
        mov     a,Pos         ;21 ------PP
        rr      a             ;22
        rr      a             ;23 PP------
        orl     a,Type        ;24 PP---TTT
        swap    a             ;25 -TTTPP--
        add     a,#(Graph-$-5);26
        mov     r1,a          ;27
LpM1:   mov     a,r1          ;4*9=36
        movc    a,@a+pc
        add     a,Locate
        mov     r0,a
        mov     @r0,#Blank
        inc     r1
        djnz    r5,LpM1       ;63

        ;====== ����Ƿ��ص� ====== -8
Check:  mov     r5,#4   ;64
        acall   H_syn   ;14
        nop             ;15
        mov     b,#0    ;17
        mov     a,r6
        rr      a
        rr      a
        orl     a,Type
        swap    a
        add     a,#(Graph-$-5)
        mov     r1,a    ;24
LpM2:   mov     a,r1    ;4*10=40
        movc    a,@a+pc
        add     a,r7
        mov     r0,a
        mov     a,@r0
        orl     b,a
        inc     r1
        djnz    r5,LpM2 ;64

        ;==== ���ݼ�����ֱ���д��� ==== -7
        acall   H_syn   ;14
        jnb     b.5,Not_Hit;16
        ;�����и���
        jnb     New,Hit ;18
GameOver:;�·����������ˣ���Ϸ����
        setb    Over    ;19
        mov     Rnd,#225;21
        clr     New     ;22
        acall   Delay31 ;53
        ajmp    Draw    ;55

        ;���鵽��ײ����������м��
Hit:    mov     c,F0    ;19
        mov     Delete,c;21
        clr     F0      ;22
        mov     a,#1    ;23
        acall   Add_Score;+19=42
        acall   Delay11 ;53
        ajmp    Draw    ;55

        ;δ���ָ���,������λ��
Not_Hit:clr     F0      ;17
        mov     Locate,7;19 r7
        mov     Pos,r6  ;21
        acall   Delay34 ;55

        ;======== �ػ����� ============ -6
Draw:   mov     r6,#5;6 ;56
        mov     a,Pos   ;57   ------PP
        rr      a       ;58
        rr      a       ;59   PP------
        orl     a,Type  ;60   PP---TTT
        swap    a       ;61   -TTTPP--
        add     a,#(Graph-S5);62
        mov     r1,a    ;63
        mov     r5,#4	;64
        ;-------------------
        acall   H_syn   ;14
LpM3:   mov     a,r1    ;+40
        movc    a,@a+pc
     S5:add     a,Locate
        mov     r0,a
        mov     @r0,dph;Color   ;r5
        inc     r1
        djnz    r5,LpM3 ;54
        clr     New     ;55
        mov     dph,#High(Music0);57
        acall   Delay05 ;62

;============ ���д�������ɣ��ȴ���һ֡��ʼ =========
Done:   nop                     ;63
        nop                     ;64
        acall   H_syn           ;14
        acall   Delay46         ;60
        djnz    r6,Done         ;62
        ajmp    Loop            ;64
;=====================================================
;һ֡�Ĵ�����ȫ������,Ҳ������ѭ�����ص�
;=====================================================


;==================== ���������� ======================
Graph : db      -2,-1,0,1,-16,-8,0,8, -2,-1,0,1,-16,-8,0,8      ;����
        db        0,1,8,9,   0,1,8,9,   0,1,8,9,   0,1,8,9      ;����
        db       -1,0,1,8, -8,0,-1,8, -8,-1,0,1,  -8,1,0,8      ;T��
        db      -1,0,1,-7,  -8,0,8,9,  -1,0,1,7, -9,-8,0,8      ;L��
        db      -9,-1,0,1, -8,-7,0,8,  -1,0,1,9,  -8,0,7,8      ;��L��
        db      -9,-8,0,1,  -7,0,1,8, -9,-8,0,1,  -7,0,1,8      ;Z��
        db        0,1,7,8, -9,-1,0,8,   0,1,7,8, -9,-1,0,8      ;��Z��






;======================================================
;            ��������ѭ���б����õ����ӳ���
;======================================================

;================= ����һ���·��� =====================
NewBlock:
        mov     a,Type2         ;19  Tppe2 -> Type
        swap    a               ;20
        rr      a               ;21
        anl     a,#00000111b    ;22
        mov     Type,a          ;23
        ;--------------------------
        add     a,#(Tab-$-3)    ;24  ���ݷ������Ͳ��ȷ����ʼλ��
        movc    a,@a+pc         ;26
        mov     Locate,a        ;27
        ;--------------------------
        mov     a,Rnd           ;28  ���������һ����Ҫ���ֵķ�������
        anl     a,#00000111b    ;29
        cjne    a,#00000111b,S9 ;31  �����7,�ĳ�2
        mov     a,#2            ;32
        sjmp    S8              ;34
     S9:nop             ;32
        nop             ;33
        nop             ;34
        ;--------------------------
     S8:swap    a               ;35  ��Type2�ĸ�3λ��������
        rl      a               ;36
        anl     a,#11100000b    ;37
        anl     Type2,#00011111b;39
        orl     Type2,a         ;40

        ;--------------------------
        mov     Pos,#0          ;42  �趨����
        mov     r6,#1           ;43  �趨�ȴ�ʱ��
        clr     Delete          ;44  �˳��������״̬
        setb    New             ;45
        acall   Delay17         ;62
        ajmp    Wait            ;64

Tab:    db 20,20,20,28,28,28,20     ;�·����ʼλ�õ�����


;===================== ���ּ��� =========================
Add_Score:  ;2+15+2 = 19(T)
        add     a,Score ;1     ;���ӵ÷�
        mov     Score,a ;2
        clr     a       ;3
        addc    a,ScoreH;4
        xch     a,ScoreH;5
        cjne    a,ScoreH,Inc_Spd;7 ;�ٶ�����?
        mov     a,#3    ;8
        nop             ;9
        djnz    acc,$   ;15
        ret             ;17
        ;�չ�256�־�����12.5%
Inc_Spd:mov     a,Speed ;8     Speed *= 0.875
        swap    a       ;9
        rl      a       ;10
        anl     a,#00011111b;11 ;a = Speed/8
        cpl     a       ;12
        inc     a       ;13     ;a = -Speed/8
        add     a,Speed ;14
        mov     Speed,a ;15     ;Speed = Speed - Speed/8
        ret             ;17

;=================== ���ַ���Сͼ����ͼ ===================
Icon:
Ico11:  mov     a,#00000100b
        xrl     P3,a
        nop
        nop
        nop
        xrl     P3,a
        ret
Ico12:  mov     a,#00000000b
        xrl     P3,a
        nop
        nop
        nop
        xrl     P3,a
        ret
Ico21:  mov     a,#00101000b
        xrl     P3,a
        nop
        xrl     P3,a
        nop
        nop
        ret
Ico22:  mov     a,#00101000b
        xrl     P3,a
        nop
        xrl     P3,a
        nop
        nop
        ret
Ico31:  mov     a,#00101100b
        xrl     P3,a
        nop
        nop
        xrl     P3,a
        nop
        ret
Ico32:  mov     a,#00101100b
        nop
        xrl     P3,a
        xrl     P3,a
        nop
        nop
        ret
Ico41:  mov     a,#00010000b
        nop
        nop
        xrl     P3,a
        xrl     P3,a
        nop
        ret
Ico42:  mov     a,#00010000b
        xrl     P3,a
        nop
        nop
        xrl     P3,a
        nop
        ret
Ico51:  mov     a,#00010100b
        xrl     P3,a
        xrl     P3,a
        nop
        nop
        nop
        ret
Ico52:  mov     a,#00010100b
        xrl     P3,a
        nop
        nop
        xrl     P3,a
        nop
        ret
Ico61:  mov     a,#00111000b
        xrl     P3,a
        nop
        xrl     P3,a
        nop
        nop
        ret
Ico62:  mov     a,#00111000b
        nop
        xrl     P3,a
        nop
        xrl     P3,a
        nop
        ret
Ico71:  mov     a,#00111100b
        nop
        xrl     P3,a
        nop
        xrl     P3,a
        nop
        ret
Ico72:  mov     a,#00111100b
        xrl     P3,a
        nop
        xrl     P3,a
        nop
        nop
        ret

;=============== �����±߿������ ===================
Line:   ;2+38+2
        mov     r7,#4
        nop
        djnz    r7,$
        mov     P3,#Green
        mov     r7,#11
        nop
        djnz    r7,$
        mov     P3,#Blank
        ret

;================= ��ͬ�� ======================
H_syn:  mov     P3,#Hsyn   ;2        ;(2+10+2)=14T
        jb      Mute,Hold1 ;4        ;�������ֵĵײ�����
        djnz    Wave,Hold2 ;6
        mov     Wave,Freq  ;8
        cpl     Beep       ;9
    L99:setb    IO_H       ;10
        ret                ;12
Hold1:  setb    Beep       ;5
        nop                ;6
Hold2:  nop                ;7
        sjmp    L99        ;9

;================= ��ͬ�� ======================
V_syn:  mov     P3,#(Hsyn and Vsyn);2
        jb      Mute,Hold3 ;4        ;�������ֵĵײ�����
        djnz    Wave,Hold4 ;6
        mov     Wave,Freq  ;8
        cpl     Beep       ;9
    L98:setb    IO_H       ;10
        ret                ;12
Hold3:  setb    Beep       ;5
        nop                ;6
Hold4:  nop                ;7
        sjmp    L98        ;9


;==================== ��ʱ���� ======================
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
Delay04:ret

;==================== �������� ======================
;63 => 500Hz  126=>250Hz
;126,119,112,106,100,94,89,84,79,75,71,67,63,59,56,53,50,47,44,42,40,38,35,33,32
; 1       2       3   4     5     6     7  1     2     3  4     5     6     7  1
; 5       6       7   1     2     3  4     5     6     7  1     2     3  4     5
; 3   4       5       6     7  1     2     3  4     5     6     7  1     2     3
        org 0700h
Music0: db 47,3,38,3,32,4,1,1
Music1: db 32,5,28,5,25,5,23,5,21,5,19,5,17,5,16,5,1,1
Music2: db 32,5,44,5,1,1
Music3: db 63,2,1,1
Music4: db 1,30,63,30,1,15,56,15,50,30,42,30,38,15,32,15,38,15,42,15,50,30,42,30
        db 38,13,1,2,38,13,1,2,38,13,1,2,38,13,1,2,42,30,50,15,63,15,50,15,1,15,50,15,1,15,56,30,1,30
        db 63,30,1,15,56,15,50,30,42,30,38,15,32,15,38,15,42,15,50,30,42,30
        db 38,14,1,2,38,14,1,2,38,14,1,2,38,14,1,2,42,30,50,15,63,15,50,28,1,2,50,10,56,20,63,30,1,1
Music5: db 63,36, 47,12, 40,36, 47,12, 50,36, 47,6, 42,6, 47,48
        db 59,36, 53,6, 47,6, 63,36, 79,12, 84,36, 63,12, 94,48
        db 47,36, 40,12, 32,36, 40,12, 42,36, 40,6, 35,6, 40,48
        db 59,6,53,6,47,6,59,6,63,6,59,6,53,6,63,6,71,6,63,6,59,6,79,6,84,36,63,12,94,48,1,1

;       end   ;AT89C2051 program end














;========================================================================;
;            �ϵ��Ķ��� /�źŷ�����(AT89C4051����)                     ;
;========================================================================;
;
;���²����ǿ����������źŷ������ĳ���,��Ϊ�źŷ�����ʹ�ý�,�밴סOK������
;��,����������źŷ���������,ÿ��һ��OK���ı�һ������.Ŀǰ�в����ź�,����
;�ź�,�����ź�����,����Ȥ���ѿ���������չ����ʽ��.�ⲿ�ֵ��߼��Բ��㸴��,
;��δ����ϸע��,���Ҳ����˵�.��ҽ����ſ���.
;========================================================================

        r8      equ      10h

        org     0800h
Welcome:mov     Rnd,#255    ;�ַ����켣������
        mov     Timer,#1    ;��ʱ������
        mov     Freq,#1     ;Ƶ�ʾ���
        mov     Mus_Len,#1  ;����
        mov     dptr,#Music6;�����������ݱ�ָ��
        clr     Mute        ;���ֿ���

        mov     r0,#100     ;���ڴ�100~120�����ɫ
        mov     a,#Orange
Fill:   mov     @r0,a
        inc     r0
        cjne    r0,#121,Fill

        mov     r0,#41h     ;�ڴ�41H~49Hװ�ػ�ͼ������
Vector: mov     a,r0
        add     a,#(Tab2-$-3-41h)
        movc    a,@a+pc
        mov     @r0,a
        inc     r0
        cjne    r0,#4ah,Vector
        jb      CNK,LpL
        mov     Freq,#32
        ajmp    LC0

Tab2:   db  bmp9-bmp1,bmp8-bmp1,bmp7-bmp1 ;��ͼ������
        db  bmp6-bmp1,bmp5-bmp1,bmp4-bmp1
        db  bmp3-bmp1,bmp2-bmp1,bmp1-bmp1

;===============��ͬ��===========================
LpL:    lcall   V_syn   ;14     ;��ͬ����ʼ
        djnz    Rnd,LB2 ;16     ;�켣�������ݼ�
        inc     Rnd     ;17     ;Ϊ��󱣳�Ϊ1����
        inc     Timer   ;18     ;��ʱ������
    LB1:mov     a,#-171 ;19
        add     a,Rnd   ;20
        jnc     LB3     ;22      if (Rnd > 171)
        mov     r2,#1   ;23      { r2 = 1;
        mov     r8,#254 ;25        r8 = 254;
        lcall   Delay37 ;62        Delay(36);
        ajmp    LB4     ;64      }
    LB2:ajmp    LB1     ;18
        ;------------------
    LB3:mov     a,Rnd   ;23     ;else
        add     a,#(Tab3-$-3);24 {
        movc    a,@a+pc ;26
        mov     r8,a    ;27        r8 = Tab3[Rnd];
        cpl     a       ;28
        mov     r2,a    ;29        r2 = ~r8;
        lcall   Delay35 ;64        Delay(34);
        ;------------------      }
    LB4:lcall   V_syn   ;14
        mov     a,Timer ;15
        jnz     LB6     ;17      if (Timer == 0) Start();
    LB5:ljmp    Start
    LB6:jnb     P1.4,LB5;19      if (P1_4 == 0)Start();
        mov     r6,#30  ;20
        lcall   Delay42 ;62      Delay(41);
        ajmp    LL1     ;64      for(r6 = 30; r6 != 0; r6--)
                        ;        {
Tab3:   ;�����������ݱ�
db 1,3,6,9,11,12,13,14,15,14,13,12,11,9,6,3
db 1,4,8,12,15,19,22,25,27,29,30,31,31,31,30,29,27,25,22,19,16,12,8,4
db 1,5,11,16,21,27,31,36,41,45,49,52,55,58,60,61,63,63,63,63,63,61,60
db 58,55,52,49,45,41,36,32,27,21,16,11,5,1,7,14,22,29,36,43,50,57,63
db 70,76,82,87,93,98,102,106,110,114,117,120,122,124,126,127,127,127
db 127,127,126,124,122,120,117,114,110,106,102,98,93,87,82,76,70,64
db 57,50,43,36,29,22,14,7,1,9,19,29,39,49,59,68,78,87,96,105,114,123
db 132,140,148,156,164,172,179,186,192,199,205,211,216,221,226,230
db 235,238,242,245,247,250,252,253,254,254,254,254

;--------------������ǰ��-----------------
LL1:    lcall   H_syn   ;14       H_syn();
        lcall   Delay48 ;62        Delay(48);
        djnz    r6,LL1  ;64      }
                        ;          for( ; r2 != 0; r2--)
;---------------������----------------
LL2:    lcall   H_syn   ;14        { H_syn();
        mov     b,#9    ;16          b  = 9;
        mov     r0,#10  ;17          r0 = 10;
        mov     P3,#Blue;19          P3 = Blue;
        mov     r4,#low(LpG);20      r4 = LpG & 0xff;
        mov     r5,#high(LpG);21     r5 = LpG >> 8;
        mov     r6,#low(bmp1);22     r6 = bmp1 & 0xff;
        mov     r7,#high(bmp1);23    r7 = bmp1 >> 8;
        lcall   Delay37 ;60          Delay(33);
        mov     P3,#Blank;62         P3 = Blank;
        djnz    r2,LL2  ;64        }

;---------------��Ծ�ַ�---------------
LpG2:   lcall   H_syn      ;14
        mov     a,#40h     ;15
        orl     a,b        ;16
        mov     r1,a       ;17
        mov     a,@r1      ;18
        setb    IO_B       ;19
        mov     r6,a       ;20
        ret;acall (R7R6)  ;+36=56
        ;---------------------
LL0:    nop                ;61
        clr     IO_B       ;62
        ajmp    LpG2       ;64
        ;---------------------
    LpG:mov     SP,#7      ;58
        djnz    r0,LL0     ;60
        mov     r0,#10     ;61
        clr     IO_B       ;62
        djnz    b,LpG2     ;64


;---------------������-----------------
LpH:    lcall   H_syn      ;14
        lcall   Delay04    ;18
        setb    IO_B       ;19
        mov     r5,#120    ;20
        mov     r6,#15     ;21
        mov     a,Timer    ;22 �������������
        add     a,#-100    ;23
        jnc     LB7        ;25
        add     a,#-21     ;26
        jc      LB8        ;28
        mov     a,Timer    ;29
        mov     r0,a       ;30
        mov     @r0,#Yellow;31
        ajmp    LB9        ;33
    LB7:nop                ;26
        nop                ;27
        nop                ;28
    LB8:lcall   Delay05    ;33
    LB9:lcall   Delay27    ;60
        mov     P3,#Blank  ;62
        djnz    r8,LpH     ;64

;--------------- ������ -----------------
LpI:    lcall   H_syn      ;14
        nop
        nop
        nop
        mov     P3,100     ;19
        mov     P3,101
        mov     P3,102
        mov     P3,103
        mov     P3,104
        mov     P3,105
        mov     P3,106
        mov     P3,107
        mov     P3,108
        mov     P3,109
        mov     P3,110
        mov     P3,111
        mov     P3,112
        mov     P3,113
        mov     P3,114
        mov     P3,115
        mov     P3,116
        mov     P3,117
        mov     P3,118
        mov     P3,119
        mov     P3,120     ;59
        nop                ;60
        mov     P3,#Blank  ;62
        djnz    r6,LpI     ;64

;---------------������-----------------
LpJ:    lcall   H_syn      ;14
        lcall   Delay04    ;18
        setb    IO_B       ;19
        mov     r6,#12     ;20
        lcall   Delay40    ;60
        mov     P3,#Blank  ;62
        djnz    r5,LpJ     ;64

;--------------���������-----------------
LpK:    lcall   H_syn      ;14
        lcall   Delay46    ;60
        djnz    r6,LpK2    ;62
        ajmp    Sound      ;64

LpK2:   ajmp    LpK        ;64


Sound:  lcall   H_syn           ;14
        jb      Mute,Skip3      ;16
        djnz    Mus_Len,Skip4   ;18
        clr     a               ;19
        movc    a,@a+dptr       ;21
        mov     Freq,a          ;22  ;��ȡ����
        clr     a               ;23
        inc     dptr            ;25
        movc    a,@a+dptr       ;27
        mov     Mus_Len,a       ;28  ;��ȡ����
        inc     dptr            ;30
        cjne    a,#2,S19        ;32  ;�б��Ƿ����ֽ���
    S19:mov     Mute,c          ;34
        lcall   Delay28         ;62
        ajmp    LpL             ;64
        ;--------------------------
Skip3:  nop                     ;17
        nop                     ;18
Skip4:  lcall   Delay44         ;62
        ajmp    LpL             ;64
        
        
;================= �����źŷ����� =================

LC0:    lcall   V_syn     ;14
        lcall   Delay46   ;60
        
        jb      CNK,LC8
        jnb     F0,LC9
        clr     F0
        ajmp    LD0
        
    LC8:setb    F0
        nop
    LC9:lcall   V_syn     ;14
        lcall   Delay42
        acall   Delay7    ;63
        mov     r6,#30
        
LC1:    lcall   H_syn     ;14
        mov     r0,#high(-480)
        mov     r1,#low (-480)
        lcall   Delay46   ;62
        djnz    r6,LC1    ;64
        
LC2:    lcall   H_syn     ;14
        mov     P3,#White ;16
        setb    c
        mov     P3,#Yellow;19
        clr     a
        mov     P3,#Cyan  ;22
        addc    a,r1
        mov     P3,#Green ;25
        mov     r1,a
        mov     P3,#Purple;28
        clr     a
        mov     P3,#Red   ;31
        addc    a,r0
        mov     P3,#Blue  ;34
        mov     r0,a
        mov     P3,#Blank ;37
        mov     r6,#13
        mov     P3,#White ;40
        nop
        mov     P3,#Yellow;43
        nop
        mov     P3,#Cyan  ;46
        nop
        mov     P3,#Green ;49
        nop
        mov     P3,#Purple;52
        nop
        mov     P3,#Red   ;55
        nop
        mov     P3,#Blue  ;58
        nop
        mov     P3,#Blank ;61
        nop               ;62
        jnz     LC2       ;64
        
LC3:    lcall   H_syn     ;14
        lcall   Delay46   ;60
        djnz    r6,LC4    ;62
        ajmp    LC0       ;64
LC4:    ajmp    LC3       ;64        
        
;================= ����ŷ����� =================

LD0:    lcall   V_syn     ;14
        lcall   Delay46   ;60

        jb      CNK,LD8
        jnb     F0,LD9
        clr     F0
        ajmp    LE0
        
    LD8:setb    F0
        nop
    LD9:lcall   V_syn     ;14
        lcall   Delay42
        acall   Delay7    ;63
        mov     r6,#30
        
LD1:    lcall   H_syn     ;14
        mov     r0,#8     ;15
        mov     r1,#1     ;16
        mov     a,#Green  ;17
        setb    New       ;18
        lcall   Delay44   ;62
        djnz    r6,LD1    ;64
;-------------------------------------        
                
LD2:    lcall   H_syn      ;14
        djnz    r1,G_Dot   ;16
G_Line: mov     P3,a       ;17 a = #Green
        jnb     New,LDA    ;19
        ;--- 2nd ---
        mov     r1,#59     ;20
        clr     New        ;21
        sjmp    LDB        ;23
        
        ;------------------------
G_Dot:  setb    IO_P       ;17
        nop
        nop
        mov     r5,#8      ;20
    LD6:clr     IO_P       ;+8*5
        setb    IO_P
        nop
        djnz    r5,LD6     ;60
        clr     IO_P       ;61
        setb    IO_P       ;62
        ajmp    LD2        ;64        

        ;--- 1st ---
    LDA:dec     r0         ;20
        setb    New        ;21
        inc     r1         ;22
        nop                ;23

    LDB:lcall   Delay36    ;59
        mov     r6,#13     ;60
        mov     P3,#Blank  ;62
        cjne    r0,#0,LD2  ;64

        
;-----------------------------------        
LD3:    lcall   H_syn     ;14
LD5:    lcall   Delay46   ;60
        djnz    r6,LD4    ;62
        ajmp    LD0       ;64
LD4:    ajmp    LD3       ;64        
        
;================= ���̺ŷ����� =================

LE0:    lcall   V_syn     ;14
        lcall   Delay46   ;60

        jb      CNK,LE8
        jnb     F0,LE9
        clr     F0
        ajmp    LC0
        
    LE8:setb    F0
        nop
    LE9:lcall   V_syn     ;14
        lcall   Delay42
        acall   Delay7    ;63
        mov     r6,#30
        
LE1:    lcall   H_syn     ;14
        mov     r0,#8     ;15
        mov     r1,#60    ;16
        mov     a,#White  ;17
        lcall   Delay45   ;62
        djnz    r6,LE1    ;64
;-------------------------------------        
                
LE2:    lcall   H_syn      ;14
        djnz    r1,LE6     ;16
        mov     P3,a       ;17
        mov     r1,#60     ;18
        xrl     a,#00111100b;19
        mov     r5,#7      ;20
        xrl     P3,#00111100b;22
        dec     r0         ;23
        sjmp    LpE8       ;25

LE6:    mov     P3,a      ;17
        ajmp    LE7       ;19 
        
LE7:    mov     r5,#8      ;20
   LpE8:xrl     P3,#00111100b;+8*5
        mov     r6,#13     ;18
        djnz    r5,LpE8    ;60
        mov     P3,#Blank  ;62
        cjne    r0,#0,LE2  ;64
        
;-----------------------------------        
LE3:    lcall   H_syn     ;14
LE5:    lcall   Delay46   ;60
        djnz    r6,LE4    ;62
        ajmp    LE0       ;64
LE4:    ajmp    LE3       ;64  
      

        
;============== "Tetris"ͼ����ͼ ===============
        org 0e00h
bmp1:
        ;FEH 00H 00H 00H
        setb    IO_R
        acall   Delay6
        clr     IO_R
        lcall   Delay24
        ret
bmp2:
        ;92H 04H 01H 00H
        setb    IO_R
        clr     IO_R
        nop
        setb    IO_R
        clr     IO_R
        nop
        setb    IO_R
        clr     IO_R
        acall   Delay5
        setb    IO_R
        clr     IO_R
        acall   Delay8
        setb    IO_R
        clr     IO_R
        acall   Delay7
        ret

bmp3:
        ;10H 04H 00H 00H
        movc    a,@a+pc
        nop
        setb    IO_R
        clr     IO_R
        acall   Delay8
        setb    IO_R
        clr     IO_R
        lcall   Delay17
        ret

bmp4:
        movc    a,@a+pc
        nop
        setb    IO_R
        clr     IO_R
        movc    a,@a+pc
        nop
        setb    IO_R
        nop
        clr     IO_R
        nop
        setb    IO_R
        movc    a,@a+pc
        clr     IO_R
        nop
        setb    IO_R
        nop
        clr     IO_R
        setb    IO_R
        clr     IO_R
        nop
        setb    IO_R
        clr     IO_R
        movc    a,@a+pc
        nop
        setb    IO_R
        movc    a,@a+pc
        clr     IO_R
        ret

bmp5:
        movc    a,@a+pc
        nop
        setb    IO_R
        clr     IO_R
        movc    a,@a+pc
        setb    IO_R
        clr     IO_R
        nop
        setb    IO_R
        clr     IO_R
        nop
        setb    IO_R
        clr     IO_R
        nop
        movc    a,@a+pc
        setb    IO_R
        nop
        clr     IO_R
        nop
        setb    IO_R
        nop
        clr     IO_R
        movc    a,@a+pc
        setb    IO_R
        clr     IO_R
        nop
        setb    IO_R
        clr     IO_R
        ret

bmp6:
        movc    a,@a+pc
        nop
        setb    IO_R
        clr     IO_R
        movc    a,@a+pc
        setb    IO_R
        movc    a,@a+pc
        nop
        clr     IO_R
        nop
        setb    IO_R
        clr     IO_R
        nop
        movc    a,@a+pc
        setb    IO_R
        clr     IO_R
        movc    a,@a+pc
        nop
        setb    IO_R
        clr     IO_R
        movc    a,@a+pc
        setb    IO_R
        nop
        clr     IO_R
        movc    a,@a+pc
        ret

bmp7:
        movc    a,@a+pc
        nop
        setb    IO_R
        clr     IO_R
        movc    a,@a+pc
        setb    IO_R
        clr     IO_R
        acall   Delay4
        setb    IO_R
        clr     IO_R
        nop
        movc    a,@a+pc
        setb    IO_R
        clr     IO_R
        movc    a,@a+pc
        nop
        setb    IO_R
        clr     IO_R
        movc    a,@a+pc
        movc    a,@a+pc
        setb    IO_R
        nop
        clr     IO_R
        ret


bmp9:
        movc    a,@a+pc
        setb    IO_R
        movc    a,@a+pc
        clr     IO_R
        movc    a,@a+pc
        setb    IO_R
        nop
        clr     IO_R
        movc    a,@a+pc
        setb    IO_R
        nop
        clr     IO_R
        movc    a,@a+pc
        setb    IO_R
        clr     IO_R
        movc    a,@a+pc
        setb    IO_R
        movc    a,@a+pc
        clr     IO_R
        nop
        setb    IO_R
        movc    a,@a+pc
        clr     IO_R
        nop
        ret

bmp8:

        movc    a,@a+pc
        nop
        setb    IO_R
        clr     IO_R
        movc    a,@a+pc
        setb    IO_R
        clr     IO_R
        nop
        setb    IO_R
        clr     IO_R
        nop
        setb    IO_R
        clr     IO_R
        setb    IO_R
        clr     IO_R
        nop
        setb    IO_R
        clr     IO_R
        movc    a,@a+pc
        nop
        setb    IO_R
        clr     IO_R
        movc    a,@a+pc
        setb    IO_R
        clr     IO_R
        nop
        setb    IO_R
        clr     IO_R
        ret


;==================== �������� =====================
Music6: db 32,6,1,2,32,6,1,2,32,6,1,2,32,46,1,2
        db 32,6,1,2,32,6,1,2,32,6,1,2,32,46,1,2
        db 32,6,1,2,32,6,1,2,32,6,1,2,32,12,38,12,47,12,38,12,32,12,38,12,32,12,23,12
        db 32,10,1,2,32,10,1,2,32,10,1,2,32,10,1,2,32,10,1,2,32,10,1,2,32,10,1,2,32,10,1,2
        db 32,96,1,1

;=================== ��ʱ�ӳ��� =====================
Delay8: nop
Delay7: nop
Delay6: nop
Delay5: nop
Delay4: ret


;==================== ������� ======================
        end
