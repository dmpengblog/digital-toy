
#ifndef _4x8SEG_VLED_H_
#define _4x8SEG_VLED_H_








void segoff( unsigned char segbit);
void segon(unsigned char segbit);
void testseg();
void testvled();

//scan display helper
void segchar(unsigned char c, unsigned char segbit);

void vled_flowtick();

void vledx0();
void vledx1();
/*============================================*/
void segvled_init();
void ms_scan_segvled(); //ɨ�� led ����


void update_vled();  // ʵ��vled�ļ�����˸ģʽ
void vledmod(char mod);
void segvled_demo();  //�ֻ��л�vled�ļ���ģʽ

void printhex(unsigned short n);

#endif
