#include <iostream>

using namespace std;

// This program would find out the digits in the hundreds, tens and ones place in a positive number.
int main(){
    int num;
    cin>>num;
    for (int i=100; i!=1; i/=10){
    cout<<num/i<<' ';
    num%=i;
    };
    cout<<num<<endl;
    return 0;
}