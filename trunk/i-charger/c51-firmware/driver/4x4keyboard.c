
#include <config.h>
		   

// h1,h2--> p1_0, p1_1
void keyscan(void)

{

  unsigned char n;
/*
  ��·
  p1_0     -- R ---	+	   +
  p1_1     --R---	+	   +
  p1_2 ��1------------	   +
  p1_3 ��2-----------------
*/
  //ɨ���һ��
  P1=0xff; 
  udelay(5);
     
  P1=0xfb; //���͵�һ��	, 1011
  udelay(5);
  n=P1;
  if(n!=0xfb){
        switch(n){
             case(0xf9):
			        break;
             case(0xfa):
			        break;
			 case 0xf8 :
			        break;
			 default: 
			        break;
      }
  }


}


