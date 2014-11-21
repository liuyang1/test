#include <stdio.h>

#define s64		long long

//char
//8765 4321
//s64
//32~25 24~17 16~9 8~1

s64 transfer(char *pts){
	s64 m_pts=((s64)(pts[0])&0x0E)<<29|		//第1个字节的第2到4位,从第4位移到第33位
		((s64)(pts[1]<<22))|					//第2个字节的全部,从第8位移到第30位(30-8=22)
		((s64)(pts[2])&0xfe)<<14|			//第3个字节的前7位,从第8位移到第22位
		((s64)(pts[3]))<<7|					//第4个字节的全部,从第8位移到第15位
		((s64)(pts[4]))>>1;					//第5个字节的前7位,从第8为移到第7位
	return m_pts;
}

int invtransfer(s64 num,char *p){
	*(p++)=(num&0xE00000000ll)>>24|0x21;		//3bit
	*(p++)=(num&0x0FF000000ll)>>20;			//8bit
	*(p++)=(num&0x000FE0000ll)>>16|0x01;		//7bit
	*(p++)=(num&0x00000FF00ll)>>12;			//8bit
	*(p++)=(num&0x0000000FEll)>>8|0x01;		//7bit
	return 1;
}

int main(){
	//ans:0x0 0000 0000
	char pts0[]={0x21,0x00,0x01,0x00,0x01};//0x2E->0b00101110
	s64 m_pts0=0;
	m_pts0=transfer(pts0);
	printf("%0llx\n",m_pts0);
	
	//ans:0x1 FFFF FFFF
	char pts1[]={0x2F,0xFF,0xFF,0xFF,0xFF};//0x2E->0b00101110
	s64 m_pts1=0;
	m_pts1=transfer(pts1);
	printf("%0llx\n",m_pts1);


	char pts2[5];
	invtransfer(0x112341234ll,pts2);
	int i;
	for(i=0;i<5;i++){
		printf("%x ",pts2[i]);
	}
	printf("\n");

	return 0;
}
