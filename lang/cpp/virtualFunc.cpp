#include <stdio.h>

class Base
{
    public:
        virtual ~Base() {}
        // Base(){printf("Base Ctor\n"); foo();}
        // ~Base(){printf("Base Dtor\n"); foo();}
        virtual void foo() = 0; // {printf("Base foo\n");}
};

class Derived:public Base
{
    public:
        Derived(){printf("Derived Ctor\n"); foo();}
        ~Derived(){printf("Derived Dtor\n"); foo();}
        void foo(){printf("Derived foo\n");}
};

int main(){
    Base *q = new Base();
    // Base *p = new Derived(); 
    // p->foo();
    // delete p;
    return 0;
}
