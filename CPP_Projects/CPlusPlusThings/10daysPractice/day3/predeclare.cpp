#include <iostream>


class Fred;

class Barney
{
public:
    void func()
    {
        f1->funcInFred();//���ʱ����Ǵ���ģ�������������������ʵ��ָ��ָ�����Ĳ�������f1ֻ��ռ��ָ���λ�ã�δʵ��
    }
private:
    Fred f;//�˴��Ǵ���ģ���Ϊֻ��ǰ���������޷�ʵ��
    Fred* f1;//����ǶԵģ�����������ָ�룬���ù�������ôʵ��
}