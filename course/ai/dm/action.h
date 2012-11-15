#ifndef _ACTION_H_
#define _ACTION_H_

#include <map>
#include <utility>
using namespace std;

#define ACTION_REC			0
#define ACTION_MSG			1
#define ACTION_CLICK		2
class Action{
	private:
		 int		m_fromId;
		 int		m_toId;
		 int		m_round;
		 int		m_action;
	public:
		 Action(char *str);
		 Action();
		 ~Action();
		 void disp();
		 inline int getfromId(){		return m_fromId;}
		 inline int gettoId(){			return m_toId;}
		 inline int getRound(){			return m_round;}
		 inline int getAction(){		return m_action;}
};

class ActionSet{
	private:
	public:
		std::multimap<int,Action>		m_Data;
		typedef std::multimap<int,Action>::iterator	iterator;

		ActionSet();
		//ActionSet(char *filename);
		~ActionSet();
		int init(char *filename);

		inline unsigned int size(){	return m_Data.size();}
		// select method
		ActionSet selectByfromId(int fromId);
		//ActionSet selectBytoId(int toId);
		ActionSet selectByround(int round);
		inline unsigned int count(int fromId){
			return m_Data.count(fromId);
		}
		void disp(){
			printf("size: %d\n",m_Data.size());
		}
			
		void dispstat(){
			iterator iter,beg,end;
			unsigned int i,cnt,sum=0;
			unsigned int actionsum=0;
			for(iter=m_Data.begin();iter!=m_Data.end();){
				cnt=count(iter->first);
				for(i=0;i!=cnt;i++)
					iter++;
				if(cnt>2000&&cnt<3000){
					//printf("%d : %d\n",iter->first,cnt);
					sum+=cnt;
					beg=m_Data.lower_bound(iter->first);
					end=m_Data.upper_bound(iter->first);
					while(beg!=end){
						actionsum+=iter->second.getAction();
						if(iter->second.getAction()!=0)
							printf("\tid: %d cnt : %d\n",iter->first,cnt);
						beg++;
					}
				}
			}
			printf("<2000 counter: %d\n",sum);
			printf("<2000 action:  %d\n",actionsum);
			printf("average action:%f\n",actionsum/(sum+0.0));
		}
		//clear?	
};

class TestSet:public ActionSet{
	public:
		int init(char *filename);
};

int testAction();
int testActionSet();

#endif
