#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <utility>

using namespace std;

#define ORIENN	8
const int RuleX[ORIENN]={1,2,-1,-2,-2,-1,1,2};
const int RuleY[ORIENN]={2,1,2,1,-1,-2,-2,-1};

static const int ArouseOffset=2;
inline int min(int x,int y){
	return x<y?x:y;
}
class ArouseBoard{
	public:
		int m_width;
		int m_height;
		int *m_ArouseFlag;
		ArouseBoard(int width,int height):
			m_width(width),m_height(height){
				int len=(width+ArouseOffset*2)*(height+ArouseOffset*2);
				m_ArouseFlag=(int *)malloc(sizeof(int)*len);
				memset(m_ArouseFlag,0,sizeof(int)*len);
			}
		~ArouseBoard(){
			free(m_ArouseFlag);
		}
		void init(){
			int i,j;
			int x,y;
			for(i=0;i<m_width;i++){
				for(j=0;j<m_height;j++){
					x = min(i,m_width-i);
					y = min(j,m_height-j);
					set(i,j,x*y);//todo
				}
			}
		}
		int get(int x,int y){
			return *(m_ArouseFlag+(m_width+ArouseOffset*2)*x+y+ArouseOffset);
		}
		void set(int x,int y,int val){
			*(m_ArouseFlag+(m_width+ArouseOffset*2)*x+y+ArouseOffset)=val;
		}
};

class ChessBoard{
	public:
		int m_width;
		int m_height;
		int *m_PassedOrder;
		int m_Count;
		int m_Posx;
		int m_Posy;
		ChessBoard(int width,int height):
			m_width(width),m_height(height){
				m_PassedOrder=(int *)malloc(sizeof(int)*width*height);
				memset(m_PassedOrder,0,sizeof(int)*width*height);
				m_Posx=0;
				m_Posy=0;
				m_Count=0;
			}
		~ChessBoard(){
			free(m_PassedOrder);
		}
		void setPassed(int x,int y){
			*(m_PassedOrder+m_width*x+y)=m_Count;
			m_Count++;
		}
		void unsetPassed(int x,int y){
			*(m_PassedOrder+m_width*x+y)=0;
			m_Count--;
		}
		int chkFinish(){
			return m_Count==m_width*m_height;
		}
};

size_t comp(const void *a,const void *b){
	return ((pair<int,int> *)a)->first-((pair<int,int>*)b)->first;
}

int DepSearch(ChessBoard cb,ArouseBoard ab){
	int i;
	//int *testseq=(int*)malloc(sizeof(int)*OREINN);
	pair<int,int>	*testseq=(pair<int,int>*)malloc(sizeof(pair<int,int>)*ORIENN);
	for(i=0;i<ORIENN;i++){
		(testseq+i)->first=ab.get(cb.m_Posx+RuleX[i],cb.m_Posy+RuleY[i]);
		(testseq+i)->second=i;
	}
	qsort(testseq,ORIENN,sizeof(pair<int,int>),comp);
		
	free(testseq);
	return 1;
}

int main(){
	return 0;
}
