/*
 * ===========================================================================
 *
 *       Filename:  load.c
 *
 *    Description:  负载均衡算法示例一
 *					const hash算法
 *					ref:http://blog.csdn.net/java2king/article/details/5810080
 *
 *        Version:  1.0
 *        Created:  08/30/2012 10:02:07 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  liuyang (liuy), liuyang1@mail.ustc.edu.cn
 *   Organization:  
 *
 * ===========================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <list>

using namespace std;

class TaskQueue{
	public:
		TaskQueue(int No):
			m_TaskCount(0),
			m_TaskNo(No)
		{
		}
		~TaskQueue(){}
		void addTask(){		m_TaskCount++;}
		void delTask(){		m_TaskCount--;}
		void dump(){
			printf("task %3d count: %3d\n",m_TaskNo,m_TaskCount);
		}
	private:
		int m_TaskCount;
		int m_TaskNo;
};

class HashRing{
	typedef list<TaskQueue> TaskQueueList;
	public:
		HashRing();
		~HashRing();
		void addTask(){}
		void delTask(){}
		void dump(){
			TaskQueueList::iterator iter;
			for(iter=m_TaskQueueList.begin();iter!=m_TaskQueueList.end();iter++){
				iter->dump();
			}
		}
	private:
		int m_TaskQueueNum;
		int m_TaskCount;// 总和计数
		double m_SD;// 方差
		TaskQueueList m_TaskQueueList;
};
int main(){
	return 0;	
}
