#ifndef _USERSET_H_
#define _USERSET_H_

#include <map>
#include "user.h"
// using map struct to store userset
// map from userid to class user

class UserSet{
	private:
	public:
		std::map<int,User>		m_Data;

		// inline func
		inline double	getEloValue(int id)	{	return m_Data[id].getEloValue();};
		inline void		setEloValue(int id,double value){
			m_Data[id].setEloValue(value);
		}
		UserSet();
		UserSet(char *filename);
		~UserSet();
		// init from file
		int init(char *filename);
		inline void disp(){
			printf("size: %d\n",m_Data.size());
		}
};

typedef std::map<int,int>			BaseStat;

int		mode(const BaseStat bs);
//double	mean(const BaseStat bs);
double	variance(const BaseStat bs);

BaseStat statAge(const UserSet *pus);
BaseStat statHeight(const UserSet *pus);

int testUserSet();

#endif
