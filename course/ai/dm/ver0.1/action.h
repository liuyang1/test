#ifndef _ACTION_H_
#define _ACTION_H_

#include <vector>

#define ACTION_REC			1
#define ACTION_MSG			2
#define ACTION_CLICK		3
class Action{
	private:
	public:
		 int		m_fromId;
		 int		m_toId;
		 int		m_round;
		 int		m_action;
		 
		 Action(char *str);
		 Action();
		 ~Action();
		 void disp();
};

class ActionSet{
	private:
	public:
		std::vector<Action>		m_Data;

		ActionSet();
		//ActionSet(char *filename);
		~ActionSet();
		int init(char *filename);

		inline unsigned int size(){	return m_Data.size();}
		// select method
		ActionSet selectByfromId(int fromId);
		ActionSet selectBytoId(int toId);
		ActionSet selectByround(int round);
		void disp(){
			printf("size: %d\n",m_Data.size());
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
