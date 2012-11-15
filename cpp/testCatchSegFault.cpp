
// mtime:	2012-10-29 14:31:25
// auth:	liuy
//
// 验证C++中关于段错误的捕捉问题
// 确实使用默认的C++的异常处理机制,是无法捕捉的.
//
// 段错误也不是异常,而是空指针或者到不可读/写的指针而去操作它,将会引起段错误.
// 段错误是OS发送给进程的错误事件(SIGSEGV/在linux中).
//
// 在windows平台下,的__try__特性,可能可以捕捉这个错误.
//
// 可以通过预先加载的库来尝试在程序当中捕获SIGSEGV.

#include <iostream>

using namespace std;

int main(){
	try{
		*(int*)0=0;
	}catch(exception& e){
		cerr<<"Exception Segment Fault cathced :"<<e.what()<<endl;
	}
	return 0;
}
