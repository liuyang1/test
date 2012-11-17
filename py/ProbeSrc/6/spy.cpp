#include "spy.h"
#include <ctype.h>
#include <iostream>
using namespace std;

// global env var
string m_Command;
PyObject* m_LocalEnvironment;

PyObject* PyInt_Create(int value)
{
	PyIntObject* obj=new PyIntObject;
	obj->refCount=1;
	obj->type=&PyInt_Type;
	obj->value=value;
	return (PyObject*)obj;
}

static void int_print(PyObject* obj)
{
	PyIntObject* intObj = (PyIntObject*)obj;
	printf("%d\n",intObj->value);
}

static PyObject* int_add(PyObject* left,PyObject* right)
{
	PyIntObject* leftInt = (PyIntObject*)left;
	PyIntObject* rightInt = (PyIntObject*)right;
	PyIntObject* ret = (PyIntObject*)PyInt_Create(0);
	if(ret==NULL){
		printf("no more memory!\n");
		exit(1);
	}else{
		ret->value=leftInt->value+rightInt->value;
	}
	return (PyObject*)ret;
}

static long int_hash(PyObject* object)
{
	return (long)((PyIntObject*)object)->value;
}

// int

PyObject* PyStr_Create(const char* value)
{
	PyStringObject* obj = new PyStringObject;
	obj->refCount  = 1;
	obj->type      = &PyString_Type;
	obj->length    = (value==NULL)?0:strlen(value);
	obj->hashValue = -1;
//obj->hashValue = string_hash((PyObject*)obj);
	memset(obj->value,0,50);
	if(value!=NULL){
		strcpy(obj->value,value);
	}
	return (PyObject*)obj;
}

static void string_print(PyObject* obj)
{
	PyStringObject* strObj = (PyStringObject*)obj;
	printf("\"%s\"\n",strObj->value);
}

static long string_hash(PyObject* obj)
{
	PyStringObject* strObj = (PyStringObject*)obj;
	if(strObj->hashValue!=-1){
		return strObj->hashValue;
	}
	register int len;
	register unsigned char *p;
	register long x;
	len = strObj->length;
	p = (unsigned char *)strObj->value;
	x = *p <<7;
	while(--len>=0)
		x = (1000003*x)^*p++;
	x^=strObj->length;
	if(x==-1)//avoid == -1
		x=-2;
	strObj->hashValue = x;
	return x;
}

static PyObject* string_add(PyObject* left,PyObject* right)
{
	PyStringObject* leftStr = (PyStringObject*)left;
	PyStringObject* rightStr = (PyStringObject*)right;
	PyStringObject* ret = (PyStringObject*)PyStr_Create(NULL);
	if(ret ==NULL){
		printf("no more memory!\n");
		exit(1);
	}else{
		strcpy(ret->value,leftStr->value);
		strcat(ret->value,rightStr->value);
	}
	return (PyObject*)ret;
}


PyObject* PyDcit_Create()
{
	PyDictObject* obj = new PyDictObject;
	obj->refCount=1;
	obj->type= &PyDict_Type;
	return (PyObject*)obj;
}

PyObject* PyDict_GetItem(PyObject* target,PyObject* key)
{
	long keyHashValue = (key->type)->hash(key);
	map<long,PyObject*>& dict = ((PyDictObject*)target)->dict;
	map<long,PyObject*>::iterator it=dict.find(keyHashValue);
	map<long,PyObject*>::iterator end=dict.end();
	if(it==end){
		return NULL;
	}
	return it->second;
}

int PyDict_SetItem(PyObject* target,PyObject* key,PyObject* value)
{
	long keyHashValue = (key->type)->hash(key);
	PyDictObject* dictObject=(PyDictObject*)target;
	(dictObject->dict)[keyHashValue]=value;
	return 0;
}

void ExcuteCommand(string &command)
{
	string::size_type pos = 0;
	if((pos=command.find("print "))!=string::npos){
		ExcutePrint(command.substr(strlen("print ")));
	}else if((pos=command.find("="))!=string::npos){
		string target = command.substr(0,pos);
		string source = command.substr(pos+strlen("="));
		ExcuteAdd(target,source);
	}else{
		printf("unkown token\n");
	}
}

int IsSourceAllDigit(string& str)
{
	for(unsigned int i=0;i<str.size();i++){
		if(!isdigit(str.at(i)))
			return 0;
	}
	return 1;
}

PyObject* GetObjectBySymbol(string name){
	PyObject* key = PyStr_Create(name.c_str());
	PyObject* ret = PyDict_GetItem(m_LocalEnvironment,key);
	return ret;
}
void ExcuteAdd(string& target,string& source)
{
	string::size_type pos;
	PyObject* strValue = NULL;
	PyObject* retValue = NULL;
	if(IsSourceAllDigit(source)){
		PyObject* intValue = PyInt_Create(atoi(source.c_str()));
		PyObject* key = PyStr_Create(target.c_str());
		PyDict_SetItem(m_LocalEnvironment,key,intValue);
	}else if(source.find("\"")!=string::npos){
		strValue = PyStr_Create(source.substr(1,source.size()-2).c_str());
		PyObject* key = PyStr_Create(target.c_str());
		PyDict_SetItem(m_LocalEnvironment,key,strValue);
	}else if((pos=source.find("+"))!=string::npos){
		PyObject* leftObj = GetObjectBySymbol(source.substr(0,pos));
		PyObject* rightObj = GetObjectBySymbol(source.substr(pos+1));
		if(leftObj!=NULL && rightObj!=NULL && leftObj->type==rightObj->type){
			retValue = (leftObj->type)->add(leftObj,rightObj);
			PyObject* key = PyStr_Create(target.c_str());
			PyDict_SetItem(m_LocalEnvironment,key,retValue);
		}
		//m_LocalEnvironment->type->print(m_LocalEnvironment);
	}
}

void ExcutePrint(string symbol)
{
	PyObject* obj = GetObjectBySymbol(symbol);
	if(obj!=NULL)
	{
		PyTypeObject* type = obj->type;
		type->print(obj);
	}
}

inline void prompt(){
	printf(">> ");
}

void Excute()
{
	printf("********* small python ********\n");
	prompt();
	while(getline(cin,m_Command)){
		if(m_Command.size()==0){
			//nothing
		}else{
			ExcuteCommand(m_Command);
		}
		prompt();
	}
}

int main(){
	PyObject* LocalEnvironment = PyDcit_Create();
	m_LocalEnvironment = LocalEnvironment;
	Excute();
	return 0;
}
