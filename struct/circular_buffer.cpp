/*
 * ========================================================
 *
 *       Filename:  circular_buffer.cpp
 *
 *    Description:  测试boost的环形内存库
 *
 *        Version:  1.0
 *        Created:  11/21/2012 11:40:40 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  liuyang1 (liuy), liuyang1@mail.ustc.edu.cn
 *   Organization:  ustc
 *
 * ========================================================
 */

#include <boost/circular_buffer.hpp>
#include <stdio.h>

int main(int argc,char **argv)
{
	boost::circular_buffer<int> cb(1500);
	cb.push_back(1);
	cb.push_back(2);
	
	printf("%d\n",cb[1]);
}
