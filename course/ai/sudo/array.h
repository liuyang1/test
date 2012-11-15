#ifndef _ARRAY_H_
#define _ARRAY_H_

#include "cell.h"

// 横9个,0~8;	9~17;	...;	72~80;
// 竖9个,0,9,..72;	1,10,...,72;	8,17,...80
// 方9个,0,1,2,9,10,11,18,19,20;...
int logicCluster[9*3][9];

class Array{
	private:
		Cell[9*9];
	public:
		Array(int *p);
		~Array();

		logic();
};

// 横9个,0~8;	9~17;	...;	72~80;
// 竖9个,0,9,..72;	1,10,...,72;	8,17,...80
// 方9个,0,1,2,9,10,11,18,19,20;...
inline initLogicCluster(){
	int i,j;
	for(i=0;i!=9;i++){
		for(j=0;j!=9;j++){
			logicCluster[i][j]=i*9+j;
		}
	}
	for(i=9;i!=18;i++){
		for(j=0;j!=9;j++){
			logicCluster[i][j]=j*9+i-9;
		}
	}
	for(i=19;i!=27;i++){
		for(j=0;j!=3;j++){
			logicCluster[i][j]=;
		}
		for(j=3;j!=6;j++){
			logicCluster[i][j]=;
		}
		for(j=6;j!=9;j++){
			logicCluster[i][j]=
		}
	}
}
#endif
