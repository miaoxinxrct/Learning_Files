#include <iostream>

using namespace std;

class A
{
public:
    void vf(){cout<<"I come from class A\n";}
};

class B:virtual public A{};

class C:virtual public A{};

class D:public B,public C{};

int main()
{
    D d;
    d.vf();//error
    return 0;
}