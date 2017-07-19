#include <iostream>

using namespace std;

#include "myStrBlob.h"

int main()
{
    StrBlob b1;
    {
        StrBlob b2 = {"a", "an", "the"};
        b1 = b2;
        b2.push_back("about");
        cout << "b2的大小 ：" << b2.size() << endl;
        cout << "b2首尾元素为： " << b2.front() << " " << b2.back() << endl;

        cout << "b1的大小 ：" << b1.size() << endl;
        cout << "b1首尾元素为： " << b1.front() << " " << b1.back() << endl;
    }
    cout << "+++++++++++++++b2离开局部作用域，已被销毁+++++++++++++++++++++" << endl;
    cout << "b1的大小 ：" << b1.size() << endl;
    cout << "b1首尾元素为： " << b1.front() << " " << b1.back() << endl;

    StrBlob b3 = b1;
    b3.push_back("next");
    cout << "b3的大小 ：" << b3.size() << endl;
    cout << "b3首尾元素为： " << b3.front() << " " << b3.back() << endl;
    cout << "b1的全部元素：" << endl;
    for (auto it = b1.begin(); neq(it, b1.end()); it.incr())
        cout << it.deref() << " ";
    cout << endl;

    return 0;
}
