#include <vector>
#include <list>
#include <deque>
#include <algorithm>
#include <iostream>

using namespace std;

int main()
{
    constexpr int arraySize=7;
    int ia[arraySize]={0,1,2,3,4,5,6};
    vector<int> ivec(ia,ia+arraySize);
    list<int> ilist(ia,ia+arraySize);
    deque<int> ideque(ia,ia+arraySize);
    vector<int>::iterator it1=find(ivec.begin(),ivec.end(),4);
    if(it1==ivec.end())
        cout<<"Unfind the number of 4\n";
    else
        cout<<"find\n";
    
    list<int>::iterator it2=find(ilist.begin(),ilist.end(),4);
    if(it2==ilist.end())
        cout<<"Unfind the number of 4\n";
    deque<int>::iterator it3=find(ideque.begin(),ideque.end(),4);
    if(it3==ideque.end())
        cout<<"Unfind the number of 4\n";
    return 0;
}