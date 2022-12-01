#include <iostream>
#include "Date.h"
using namespace std;


/*
main function for testing,
You can test your code here.
*/
int main()
{
    Date a(1900,1,1);
    Date b = a;
    Date c = a;
    b.sub(500);
    
    c.add(100000000);
    cout << b.toString() << endl;
    cout << c.toString() << endl;
    //cout << c.diff(a) << endl;
    return 0;
}
/*
Code for testing your class implementation via judger.
Please comment your main function and uncomment the code downside
before running judger.
*//*
void test1();
void test2();
void test3();
void test7();
void test8();

int main()
{
    int i;
    cin >> i;
    switch (i){
        case 1:
            test1();
            break;
        case 2:
            test2();
            break;
        case 3:
            test3();
            break;
        case 7:
            test7();
            break;
        case 8:
            test8();
            break;
    }
    return 0;
}

void test1()
{
    Date a(1900,1,1);
    Date b;
    cout << a.toString() << endl;
    cout << b.toString() << endl;
}

void test2()
{
    Date a(2022,5,1);
    cout << a.getYear() << a.getMonth() << a.getDay() << endl;
}

void test3()
{
    Date a(2022,4,20);
    a.add(5);
    cout << a.toString() <<endl;
    a.sub(5);
    cout << a.toString() <<endl;
}

void test7()
{
    Date a(2900,12,26);
    a.add(20);
    cout << a.toString() << endl;
    a.sub(40);
    cout << a.toString() << endl;
}

void test8()
{
    Date a(2000,2,20);
    Date b = a;
    Date c = a;
    b.add(500);
    c.sub(500);
    cout << b.diff(a) << endl;
    cout << c.diff(a) << endl;
}

*/
