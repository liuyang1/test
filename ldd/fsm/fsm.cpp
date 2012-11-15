#include "fsm.h"
#include <stdexcept>

using namespace std;

int	fsm::addStr(char *str)
{
	unsigned int state=0;
	unsigned int stateOld;
	while(*str!='\0')
	{
		stateOld=state;
		if(isStateTrans(state,*str)){
			state=goStateTrans(state,*str);
		}
		else
		{
			addStateTrans(state,*str,mNnode);
			state=mNnode;
			mNnode++;
		}
		//反向添加节点
		//需要节点的深度信息,这个还需要占用存储空间,而且建立树之后,这个就没有用处了
		//需要知道父节点信息,这个影响就更大了
		//原先的设计,图是单向的,并没有考虑寻找父节点的情况.
		//还有需要考虑,这个后向建立网络的过程在那里比较合适
		//BackTrack(stateOld);
	}
	return 0;
}

int fsm::chkTxt(char* text){
	return 0;
}

int fsm::chkLst(int n){
	return 0;
}

int fsm::isStateTrans(unsigned int state,char c){
	if(state>mFSMNet.size())
		throw range_error("is state isn't exist!");
	//若存在state(c)->state',则返回1,否则返回0
	return	mFSMNet[state].find(c)!=mFSMNet[state].end();
}

int fsm::addStateTrans(unsigned int stateSrc,char c,unsigned int stateDst){
	return mFSMNet[stateSrc][c]=stateDst;
}

unsigned int  fsm::goStateTrans(unsigned int stateSrc,char c){
	if(isStateTrans(stateSrc,c)==0)
		throw range_error("go state isn't exist!");
	return mFSMNet[stateSrc][c];
}

