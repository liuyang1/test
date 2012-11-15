#ifndef _CELL_H_
#define _CELL_H_

#include <set>

class Cell{
	private:
		std::set<int>	m_Prob;
		int				m_Val;
	public:
		Cell(){
			m_Val=0;
			//m_Prob={1,2,3,4,5,6,7,8,9};
		}
		~Cell(){}

		void eraseProb(int n);
		int getProbCount(){		return m_Prob.size();}
};

#endif
