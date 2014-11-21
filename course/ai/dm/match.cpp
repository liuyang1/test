#include "match.h"
#include "user.h"

// 符合,则返回1,不符合返回0,未定义返回-1
int chkMmarriage(int require,int status){
	// 未婚状态
	if(status==1){
		if(require==0||require==1||require==4||require==5)
			return 1;
		else
			return 0;
	}
	// 离异状态
	if(status==2){
		if(require==0||require==2||require==4||require==6)
			return 1;
		else
			return 0;
	}
	// 丧偶状态
	if(status==3){
		if(require==0||require==3||require==5||require==6)
			return 1;
		else
			return 0;
	}
	return -1;
}

// 检测B是否符合A的需求
double matchBase(User A,User B){
	// match age 
	int info=B.getAge();
	// 若要求年龄限制,并且不符合,则返回0
	if((A.getMminage()!=0&&info<A.getMminage())||(A.getMmaxage()!=0&&info>A.getMmaxage()))
		return 0;
	info=B.getHeight();
	// 若要求身高限制,并且不满足身高要求,则返回0
	if((A.getMminheight()!=0&&info<A.getMminheight())||(A.getMmaxheight()!=0&&info>A.getMmaxheight()))
		return 0;
	// 星级会员要求?
	//info=B.getCert();
	info=B.getMarriage();
	if(chkMmarriage(A.getMmarriage(),info)==0)
		return 0;
	info=B.getEdu();
	if(A.getMedu()!=0){
		// 学历要求可以向上扩展
		if((A.getMedumore()==1&&info<A.getMedu())||(A.getMedumore()!=1&&info!=A.getMedu()))
			return 0;
	}
	info=B.getWorkLoc();
	if(A.getMworkloc()!=0&&info!=A.getMworkloc())
		return 0;
	info=B.getWorkSubLoc();
	if(A.getMworksubloc()!=0&&info!=A.getMworksubloc())
		return 0;
	return 1;
}

// 智能检测B是否接近A的需求
double matchSmart(User A,User B){
	// 若A为男,B为女
	int ret=1;
	if(A.getSex()==0&&B.getSex()==1){
		// 年龄偏向小
		// 
		return ret;
	}
	// 若A为女,B为男
	else if(A.getSex()==1&&B.getSex()==0){
	}
	return 0;
}

double matchInfo(User A,User B){
	return matchBase(A,B);
}

// 检测A和B的基本信息相似程序
double simlarInfo(User A,User B){
	int ret=0;
	if(A.getSex()==B.getSex()){
		// 年龄接近
		// 学历接近
		// 购房接近
		// 购车接近
		// 婚姻状况接近
		// 职业接近
		//
		return ret;
	}else{
		return 0;
	}
}

