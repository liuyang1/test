#include "action.h"
#include "myutil.h"
#include <vector>

char action_mem_tables[][20]={
	"user_id_A",	"user_id_B",	"rec_times",	"action"
};

#define DISP_FORMAT		"%-20s: %10d\n"

Action::Action(char *str){
	char delim[]=" \t\n";
	char *p=NULL;
	p=strtok(str,delim);
	m_fromId=atoi(p);
	p=strtok(NULL,delim);
	m_toId=atoi(p);
	p=strtok(NULL,delim);
	m_round=atoi(p);
	p=strtok(NULL,delim);
	if(*p=='r')
		m_action=ACTION_REC;
	else if(*p=='m')
		m_action=ACTION_MSG;
	else if(*p=='c')
		m_action=ACTION_CLICK;
	else{
		m_action=-1;
		printf("could not reslove action\n");
	}
}

Action::Action(){
}

Action::~Action(){
}

void Action::disp(){
	printf(DISP_FORMAT,action_mem_tables[0],m_fromId);
	printf(DISP_FORMAT,action_mem_tables[1],m_toId);
	printf(DISP_FORMAT,action_mem_tables[2],m_round);
	printf(DISP_FORMAT,action_mem_tables[3],m_action);
}

/********************** Action set code *********************************/
ActionSet::ActionSet(){
}
/*
ActionSet::ActionSet(char *filename){
	init(filename);
}
*/
ActionSet::~ActionSet(){
}

int ActionSet::init(char *filename){
	FILE *fp=fopen(filename,"r");
	CHK_NULL(fp,"ActionSet: could not open file\n");

	char buf[BUF_LEN];
	fgets(buf,BUF_LEN,fp);//skip first line
	while(!feof(fp)){
		if(NULL==fgets(buf,BUF_LEN,fp))
			break;
		if(*buf=='0')
			break;
		Action tmp(buf);
		//m_Data.push_back(tmp);
		m_Data.insert(make_pair(tmp.getfromId(),tmp));
	}

	fclose(fp);
	return 0;
}

// select method using TRAVERSAL
ActionSet ActionSet::selectByfromId(int fromId){
	ActionSet ret;
	Action tmp;
	if(m_Data.count(fromId)>2000)
		return ret;
	iterator beg=m_Data.lower_bound(fromId);
	iterator end=m_Data.upper_bound(fromId);
	while(beg!=end){
		tmp=beg->second;
		ret.m_Data.insert(make_pair(tmp.getfromId(),tmp));
		beg++;
	}
	return ret;
}
/*
ActionSet ActionSet::selectBytoId(int toId){
	ActionSet ret;
	unsigned int iter;
	Action tmp;
	for(iter=0;iter!=m_Data.size();iter++){
		tmp=m_Data[iter];
		if(tmp.m_toId==toId)
			ret.m_Data.push_back(tmp);
	}
	return ret;
}
*/
ActionSet ActionSet::selectByround(int round){
	ActionSet ret;
	Action tmp;
	iterator iter;
	for(iter=m_Data.begin();iter!=m_Data.end();iter++){
		tmp=iter->second;
		if(tmp.getRound()==round)
			ret.m_Data.insert(make_pair(tmp.getfromId(),tmp));
	}
	return ret;
}
/************************ TestSet code **********************************/
int TestSet::init(char *filename){
	FILE *fp=fopen(filename,"r");
	CHK_NULL(fp,"ActionSet: could not open file\n");

	char buf[BUF_LEN];
	fgets(buf,BUF_LEN,fp);//skip first line
	while(!feof(fp)){
		if(NULL==fgets(buf,BUF_LEN,fp))
			break;
		if(*buf=='0')
			break;
		strcat(buf,"rec");
		Action tmp(buf);
		m_Data.insert(make_pair(tmp.getfromId(),tmp));
	}

	fclose(fp);
	return 0;
}

/********************** test code ***************************************/

int testAction(){
	char str[]="96 24158 1 rec";
	Action action(str);
	action.disp();
	return 0;
}

int testActionSet(){
	ActionSet actSet;
	actSet.init(TRAIN_FILE);

	return 0;
}
