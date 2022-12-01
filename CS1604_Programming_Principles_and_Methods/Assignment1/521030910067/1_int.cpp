#include <iostream>
#include <limits.h>

using namespace std;

// This program would add two int-type numbers into an int-type addition.
int main(){
    long long a,b;
    int sum;
    int max=INT_MAX, min=INT_MIN;
    cin>>a>>b;
    if (a<=max && a>=min && b<=max && b>=min){
        sum=a+b;
        if ((a>=0 && b>=0) || (a<0 && b<0)){
            if ((a>=0 && sum>=0) || (a<0 && sum<0))
                cout<<sum<<endl;
            else
                cout<<"ERROR"<<endl;
        } else
            cout<<sum<<endl;
    } else 
        cout<<"ERROR"<<endl;
    return 0;
}