/**
 * @file InhertTest.cpp
 * @brief 测试类的继承情况
 * 虚函数,子类是可以覆盖的.
 * 非虚函数,子类不能覆盖.
 *
 * @author liuyang1 liuyang1@mail.ustc.edu.cn
 * @version 0.1
 * @date 2012-11-15
 */

#include <stdio.h>
class Father
{
	public:
		virtual						//virtual 启用 dynamic binding
			// 非 virtual 函数,在编译器确定,函数的调用.
			// virtual 函数,在运行期确定,调用对象.
		void virtualSay(){
			printf("virtualSay:\tI'm father\n");
		}

		void say(){
			printf("say:\tI'm father\n");
		}
};

class Child:public Father
{
	public:
		void virtualSay(){
			printf("virtualSay:\tI'm son\n");
		}

		void say(){
			printf("say:\tI'm son\n");
		}
};

int main(){
	/* 在C++中,通过基类指针调用虚函数时,发生动态绑定(dynamic binding),也就是其既可以指向父类,也可以指向子类 */
	printf("Fatcher static\n");
	Father f;
	f.virtualSay();
	f.say();
	printf("Child static\n");
	Child c;
	c.virtualSay();
	c.say();
	printf("dynamic binding\n");
	Father *p=new Child;
	p->say();
	return 0;
}
