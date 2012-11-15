#ifndef _USER_H_
#define _USER_H_


#define USERMEMNUM			35
#define MEMSTRLEN			20
//extern char user_mem_tables[USERMEMNUM][MEMSTRLEN];
#define ORIG_ELO_VALUE		1400.0;

class User{
	private:
		int			m_Var[USERMEMNUM];
	public:
		double		m_Avatar;
		double		m_EloValue;
		// init class User from str
		User(char* str);
		//User(const User& src);
		User();

		~User();
		int chkUser();

		void disp();

		// inline func
		inline void initEloValue(){			m_EloValue=ORIG_ELO_VALUE;}
		inline double getEloValue(){		return m_EloValue;}
		inline void	setEloValue(double EloValue){	m_EloValue=EloValue;}

		inline int getUid(){				return m_Var[0];}
		inline int getSex(){				return m_Var[1];}
		// skip reg time
		// skip login time
		inline int getAge(){				return 2011-m_Var[4];}
		// skip birthday
		inline int getWorkLoc(){			return m_Var[6];}
		inline int getWorkSubLoc(){			return m_Var[7];}
		inline int getStatus(){				return m_Var[8];}
		// skip login cnt
		// skip mobile
		inline int getEdu(){				return m_Var[11];}
		inline int getHouse(){				return m_Var[12];}
		inline int getCar(){				return m_Var[13];}
		inline int getMarriage(){			return m_Var[14];}
		inline int getChild(){				return m_Var[15];}
		inline int getIndustry(){			return m_Var[16];}
		inline int getPrivacy(){			return m_Var[17];}
		inline int getLevel(){				return m_Var[18];}
		inline int getNation(){				return m_Var[19];}
		inline int getHeight(){				return m_Var[20];}
		inline int getIncome(){				return m_Var[21];}
		inline int getAvatar(){				return m_Var[22];}
		inline int getBelief(){				return m_Var[23];}

		inline int getMminage(){			return m_Var[24];}
		inline int getMmaxage(){			return m_Var[25];}
		inline int getMminheight(){			return m_Var[26];}
		inline int getMmaxheight(){			return m_Var[27];}
		inline int getMcert(){				return m_Var[28];}
		inline int getMmarriage(){			return m_Var[29];}
		inline int getMedu(){				return m_Var[30];}
		inline int getMedumore(){			return m_Var[31];}
		inline int getMavatar(){			return m_Var[32];}
		inline int getMworkloc(){			return m_Var[33];}
		inline int getMworksubloc(){		return m_Var[34];}

		inline void setMarriage(int val){	m_Var[14]=val;}
};

int testUser();

#endif
