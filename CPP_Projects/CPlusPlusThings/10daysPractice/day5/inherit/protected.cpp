#include <iostream>


class Base
{
public:
    int i;
protected:
    int j;
private:
    int k;
};

class Derived:public Base
{
public:
    void func()
    {
        i=1;
        j=2;
        //k=3;//˽�г�Ա���ɷ��ʣ��̳�Ҳ����
    }
};

int main()
{
    Base b;
    b.i=4;
    //b.j=5;protected ��private�����ɷ���
    //b.k=6;
}