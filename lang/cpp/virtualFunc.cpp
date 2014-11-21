#include <stdio.h>

class Base
{
    public:
        Base(){printf("Base Ctor\n"); foo();}
        ~Base(){printf("Base Dtor\n"); foo();}
        virtual void foo(){printf("Base foo\n");}
};

class Derived:public Base
{
    public:
        Derived(){printf("Derived Ctor\n"); foo();}
        ~Derived(){printf("Derived Dtor\n"); foo();}
        void foo(){printf("Derived foo\n");}
};

int main(){
    Base *p = new Derived(); 
    p->foo();
    delete p;
    return 0;
}
