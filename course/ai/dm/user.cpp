#include <string>
#include "user.h"
#include <assert.h>

char user_mem_tables[USERMEMNUM][MEMSTRLEN]={
		"Uid",		"Sex",			"RegTime",		"LogTime",		"BirthYear",	
		"BirthDay",	"WrokLoc",		"WorkSubLoc",	"Status",		"LogCnt",	
		"Mobile",	"Edu",			"House",		"Car",			"Marriage",	
		"Children",	"Industry",		"Privacy",		"Level",		"Nation",
		"Height",	"Income",		"Avatar",		"Belief",	

		"M_MinAge",	"M_MaxAge",		"M_MinHeight",	"M_MaxHeight",	"M_Cert",	
		"M_Marriage","M_Edu",		"M_EduMore",	"M_Avatar",		"M_WorkLoc",	
		"M_WorkSubLoc"
};

int User::chkUser(){
	int marriage=getMarriage();
	if(marriage<1||marriage>3)
		return -1;
	int c=getAge();
	if(c<18||c>99)
		return -1;
	c=getHeight();
	if(c<130||c>260)
		return -1;
	return 0;
}

User::User(char *str){
	char delim[]=" \t\n";
	char *p=NULL;
	p=strtok(str,delim);	
	m_Var[0]=atoi(p);
	p=strtok(NULL,delim);
	if(*p=='f')
		m_Var[1]=1;		
	else if(*p=='m')
		m_Var[1]=0;			// default is male(0)
	else
		m_Var[1]=-1;
	int i;
	for(i=2;i<USERMEMNUM;i++){
		p=strtok(NULL,delim);
		if(p!=NULL)
			m_Var[i]=atoi(p);
		else{
			printf("err\n");
		}
	}
	initEloValue();
	//chkUser();
}
/*
User::User(const User& src){
	int i;
	for(i=0;i<USERMEMNUM;i++)
		m_Var[i]=src.m_Var[i];
	m_EloValue=src.m_EloValue;
}
*/
User::User(){
	initEloValue();
}

User::~User(){
}

void User::disp(){
	int i;
	for(i=0;i<USERMEMNUM;i++){
		printf("%-20s: %10d\n",user_mem_tables[i],m_Var[i]);
		//printf(" %10d\n",m_Var[i]);
	}
	printf("\n");
}
/************************** test code ********************************/
int tUserInit(){
	char str[]="521	f	1081591500	1165214231	1977	0301	33	3301	1	183	0	30	1	1	1	1	42	4	1	1	165	20	0	1	0	0	0	0	0	0	0	0	0	0	0";
	User user(str);

	return 0;
}

int tUserDisp(){
	char str[]="521	f	1081591500	1165214231	1977	0301	33	3301	1	183	0	30	1	1	1	1	42	4	1	1	165	20	0	1	0	0	0	0	0	0	0	0	0	0	0";
	User user(str);
	user.disp();
	return 0;
}

int testUser(){
	tUserInit();
	tUserDisp();

	return 0;
}
