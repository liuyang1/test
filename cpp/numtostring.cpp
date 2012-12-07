/*
 * ========================================================
 *
 *       Filename:  numtostring.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/07/2012 01:56:52 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  liuyang1 (liuy), liuyang1@mail.ustc.edu.cn
 *   Organization:  ustc
 *
 * ========================================================
 */

#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <stdio.h>

using namespace std;

string numtostring(long long num)
{
	stringstream oss;
	oss.setf(ios::scientific,ios::floatfield);
	oss<<setiosflags(ios::scientific)<<num;

	cout.setf(ios::scientific,ios::floatfield);
	cout<<num<<endl;
	cout<<num+0.0<<endl;
	return oss.str();
}
int main(){
	long long num=1234567890123545l;
	printf("%s\n",numtostring(num).c_str());
	return 0;
}
