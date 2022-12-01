#include <iostream>
#include <vector>
#include <math.h>

using namespace std;

// This program would judge whether an integer is a palindrome number.
int main(){
    int num,orinum,revnum=0;
    vector<int> digit;
    cin>>num;
    orinum=num;
    while (num>10){
        digit.push_back(num%10);
        num/=10;
    }
    digit.push_back(num);
    for (int i=0; i!=int(digit.size()); ++i)
        revnum+=digit[i]*pow(10,digit.size()-1-i);
    if (orinum==revnum)
        cout<<"YES"<<endl;
    else 
        cout<<"NO"<<endl;
    return 0;
}