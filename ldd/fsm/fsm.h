#ifndef _FSM_H_
#define _FSM_H_

#include <vector>
#include <map>

using namespace std;

class fsm{
public:
	//fsm有向网络,
	vector<map<char,unsigned int> > mFSMNet;
	unsigned int					mNnode;
	vector<unsigned int>			mTargetNo;

	fsm()
	{
		mNnode=1;
	}
	~fsm(){
	}

	//添加字符串
	int	addStr(char *str);
	//检查文本
	int chkTxt(char *text);			
	//检查是否为目标字符串
	int chkLst(int n);
private:
	int isStateTrans(unsigned int state,char c);
	int addStateTrans(unsigned int stateSrc,char c,unsigned int stateDst);
	unsigned int goStateTrans(unsigned int stateSrc,char c);
};
#endif
