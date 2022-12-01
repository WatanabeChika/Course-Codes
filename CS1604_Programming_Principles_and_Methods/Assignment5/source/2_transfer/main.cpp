#include "Sender.h"
#include <iostream>
using namespace std;


/*
main function for testing,
You can test your code here.
*/
/*
int main()
{
    Sender sd("11111");
    cout << sd.bitStuffing(sd.getOriginData()) << endl;
    return 0;
}

Code for testing your class implementation via judger.
Please comment your main function and uncomment the code downside
before running judger.
*/


void test0();
void test1();
void test2();
void test3();
void test4();

int main()
{
    int i;
    cin >> i;
    switch (i){
        case 0:
            test0();
            break;
        case 1:
            test1();
            break;
        case 2:
            test2();
            break;
        case 3:
            test3();
            break;
        case 4:
            test4();
            break;
    }
    return 0;
}

void test0()
{
    Sender sd("001111111100");
    cout << sd.bitStuffing(sd.getOriginData()) << endl;
}

void test1()
{
    Sender sd("001111111100");
    cout << sd.framing() << endl;
}

void test2()
{
    Sender sd; 
    cout << sd.framing() << endl;
}

void test3()
{
    Sender sd("001111111100", 8);
    cout << sd.bitStuffing(sd.getOriginData()) << endl;
}

void test4()
{
    Sender sd("001111111100", 8);
    cout << sd.getOriginData() << endl;
    cout << sd.framing() << endl;
}


