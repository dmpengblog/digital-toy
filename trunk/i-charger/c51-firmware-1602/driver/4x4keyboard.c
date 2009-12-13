
#include <config.h>
		   

// S52 P1  has internal pullup resistor
// h1,h2--> p1_0, p1_1
unsigned char keyscan(void)

{

  unsigned char scan;
  unsigned char col;
  char row;

/*
  ��·
  p1_0     -- R ---	+	   +
  p1_1     --R---	+	   +
  p1_2    -----------	   +
  p1_3 ��2-----------------
*/

 /*  
     	    col: 7  6  5  4
  row1-> bit3    k  k  k  k
  row1-> bit2    k  k  k  k
	             k  k  k  k
	             k  k  k  k

 */
  for(row=3; row>=0; row--)		
  {
    //ɨ��һ��

    P1=0xff; 
    udelay(2);
     
    P1=0xFF & (~( 1<< row)); //����һ��	, 11110111

    udelay(2);
    scan=P1;
    udelay(256); //��ʱ����

    if(scan!=P1) //�����Ƕ���
      continue;

	
    scan&= 0xF0;
	scan >>=4; 
	
	if(scan == 0xF0) //no key press down
	   continue;

    for(col=0;col<4;col++){
	    if(! ( scan&(1<<(3-col))) ){
          wait_release: 
		      scan = P1; 
			  if ((scan&0xF0) != 0xF0  )
			    goto  wait_release;

           return (3-row)*4+col;

		}
      }
  }

  return -1;


}


