#include <stdio.h>
#include <stdlib.h>

#include <string>
#include <map>

using namespace std;

#define PyObject_HEAD   \
	int refCount;	\
	struct tagPyTypeObject *type

#define PyObject_HEAD_INIT(typePtr)\
	0,typePtr
typedef struct tagPyObject
{
	PyObject_HEAD;
}PyObject;
// --base PyObject

typedef void (*PrintFun)(PyObject* object);
typedef PyObject* (*AddFun) (PyObject* left,PyObject* right);
typedef long (*HashFun)(PyObject* object);

typedef struct tagPyTypeObject
{
	PyObject_HEAD;
	char *name;
	PrintFun print;
	AddFun add;
	HashFun hash;
}PyTypeObject;

PyTypeObject PyType_Type={
	PyObject_HEAD_INIT(&PyType_Type),
	"type",
	NULL,
	NULL,
	NULL
};

// --PyTypeObject

static void int_print(PyObject* obj);
static PyObject* int_add(PyObject* left,PyObject* right);
static long int_hash(PyObject* obj);
PyTypeObject PyInt_Type = 
{
	PyObject_HEAD_INIT(&PyType_Type),
	"int",
	int_print,
	int_add,
	int_hash
};
typedef struct tagPyIntObject
{
	PyObject_HEAD;
	int value;
}PyIntObject;


static void string_print(PyObject* obj);
static PyObject* string_add(PyObject* left,PyObject* right);
static long string_hash(PyObject* obj);
PyTypeObject PyString_Type=
{
	PyObject_HEAD_INIT(&PyType_Type),
	"str",
	string_print,
	string_add,
	string_hash
};
typedef struct tagPyStrObject
{
	PyObject_HEAD;
	int length;
	long hashValue;
	char value[50];
}PyStringObject;

inline void dumpPyStr(PyObject* obj)
{
	PyStringObject* objStr=(PyStringObject*)obj;
	printf("dumpStr\tobj: %p length: %d hashValue: %ld value: %s",objStr,objStr->length,objStr->hashValue,objStr->value);
}
typedef struct tagPyDictObject
{
	PyObject_HEAD;
	map<long,PyObject*> dict;
}PyDictObject;

static void dict_print(PyObject* obj)
{
	PyDictObject* dictObject = (PyDictObject*)obj;
	printf("{");
	map<long,PyObject*>::iterator it=(dictObject->dict).begin();
	map<long,PyObject*>::iterator end=(dictObject->dict).end();
	for(;it!=end;it++){
		printf("%ld : ",it->first);
		PyObject* value=it->second;
		(value->type)->print(value);
		printf(", ");
	}
	printf("}");
}

PyTypeObject PyDict_Type = {
	PyObject_HEAD_INIT(&PyType_Type),
	"dict",
	dict_print,
	0,
	0
};

void ExcuteAdd(string& target,string& source);
void ExcutePrint(string symbol);
