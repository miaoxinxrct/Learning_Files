#include <iostream>

using namespace std;

struct A
{
    A(int i){}
    operator bool() const{
        return true;
        }
};

struct B{
    explicit B(int i){}
    explicit operator bool () const{return true;}

};

void doA(A a){}

void doB(B b)

int main()
{
    A a1(1);//ֱ�ӳ�ʼ��
    A a2=1;//���Ƴ�ʼ��
    A a3{1};//�б��ʼ��
    A a4={1};//�����б��ʼ��
    A a5=(A)1;//���� static_cast��ʽת��
    doA(1);//����int��A����ʽת��
    if(a1)
    ;//ʹ��ת��������A����operator bool()��A��bool����ʽת��
    bool a6(a1);
    bool a7=a1;
    bool a8=static_cast<bool>(a1);


    B b1(1);
    //B b2=1;//����ǲ��е�
    B b3{1};
    //B b4={1};����ǲ����Ե�
    B b5=(B)1;//�������ʽת�� static_cast
    //doB(b1);����ǲ����Ե�
    if(b1)
    ;

    bool b6(b1);
    //bool b7=b1;����ǲ�����
    bool b8=static_cast<bool> (b1);
    return 0;


};