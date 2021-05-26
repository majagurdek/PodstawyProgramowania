#include <iostream>
#include <string>
#include <vector>

#include "VectoredList.h"

using namespace std;

//int main()
//{
//    VectoredList v;
//
//    for(int i=0;i<101;i++)
//    {
//        v.push_back(to_string(i));
//    }
//    for (int i = v.getSize()-1; i >= 0; i--)
//    {
//        cout<<v[i]<<" ";
//    }
//    cout<<"\n~~~~~~~~"<<endl;
//
//    for(auto& element:v )
//    {
//        cout<<element<<' ';
//    }
//    cout<<"\n~~~~~~~~"<<endl;
//
//    VectoredList::Iterator it = v.begin();
//    it+=3;
//    v.erase(it);
//    v.show();
//    cout<<"~~~~~~~~"<<endl;
//    {
//        VectoredList v2;
//        for (int i = 0; i < 23; i++)
//        {
//            v2.push_back( to_string(24-i) );
//        }
//        v = v2;
//    }
//    v.show();
//    cout<<"end"<<endl;
//
//    return 0;
//}


int main()
{
    VectoredList v1;
    for (int i = 0; i < 10; i++)
        {
            v1.push_back( to_string(i));
        }
    cout<<"Test 1"<<endl;
    cout<<"v1 :"<<endl;
    for (int i = 0; i < v1.getSize(); ++i)
    {
        cout<<v1[i]<<" ";
    }
    VectoredList v2 = move(v1);
    cout<<endl;
    cout<<"v2 :"<<endl;
    for (int i = 0; i < v2.getSize(); ++i)
    {
        cout<<v2[i]<<" ";
    }
    cout<<endl;
    cout<<"v1 :"<<endl<<endl;
    for (int i = 0; i < v1.getSize(); ++i)
    {
        cout<<v1[i]<<" ";
    }


    cout<<"Test 2"<<endl;
    VectoredList v3;
    cout<<"v2 :"<<endl;
    for (int i = 0; i < v2.getSize(); ++i)
    {
        cout<<v2[i]<<" ";
    }
    cout<<endl;
    v3 = move(v2);
    cout<<"v3 :"<<endl;
    for (int i = 0; i < v3.getSize(); ++i)
    {
        cout<<v3[i]<<" ";
    }
    cout<<endl;

    cout<<endl<<"Test 3"<<endl;
    VectoredList v4;
    v4 = v3+v3;
    for (int i = 0; i < v4.getSize(); i++)
    {
        cout<<v4[i]<<" ";
    }

    cout<<endl<<endl<<"Test 4"<<endl;
    std::vector<string> v{"a", "1", "b", "2"};
    VectoredList v5;
    v5.assign(v);
    cout<<"v5 :"<<endl;
    for (int i = 0; i < v5.getSize(); ++i)
    {
        cout<<v5[i]<<" ";
    }
    cout<<endl;


}