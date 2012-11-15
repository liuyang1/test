#ifndef _TRAIN_H_
#define _TRAIN_H_

#include "userset.h"
#include "action.h"

//class UserSet;
//class ActionSet;

class Train{
	private:
		UserSet		m_Users_f;
		UserSet		m_Users_m;
		ActionSet	m_Actions;
		TestSet		m_Test;
	public:
		Train();
		~Train();

		int init(char *profile_f,char *profile_m,char *train,char *test);
		inline void disp(){
			m_Users_f.disp();
			m_Users_m.disp();
			m_Actions.disp();
			m_Test.disp();
		}

		void EloRating();
		void Predict();
};

//void EloRating(UserSet &us,ActionSet as);


/************************ test ************************************/
void testTrain();
#endif
