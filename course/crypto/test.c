#include <stdio.h>
#include <stdlib.h>

void disp(unsigned long num){
	char buf[4*8+1]={0};
	int i=0;
	for(i=0;i<32;i++){
		buf[31-i]=num&0x01 ? '1':'0';
		num>>=1;
	}
//	buf[63]=='\0';
	printf("%s\n",buf);
}

unsigned long GenSig_Test(unsigned char Seed1,unsigned char Seed2)
 {
   unsigned char C = Seed1*Seed2;
   unsigned long Sig = 0x12345678UL;
   while(C--)
   {
	   printf("%d: %x\n",C,Sig);
	   disp(Sig);
     if(Sig&0x80000000UL)
     {
       Sig >>= 1;// right offset
       Sig |= 0x80000000UL;// top bit 1
     }
     else
     {
       Sig >>= 1;// right offset
     }
	 
   }
   return (unsigned long)Sig;
 }

int main(){
	unsigned long ret=GenSig_Test(3,2);
	printf("%b\n",ret);
	return 0;
}
