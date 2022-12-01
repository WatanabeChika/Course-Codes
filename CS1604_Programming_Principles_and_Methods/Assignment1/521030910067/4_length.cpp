#include <iostream>

using namespace std;

// This program would calculate the length the man walked in n days.
int main(){
    long n,length;
    int x;
    cin>>x>>n;
    if (n==0){
        length=0;
    }else{
        for (int i=0; i<=n; ++i){
            switch ((i+x)%7){
                case 0:
                case 6: break;
                default: length+=250; break;
            };
        };
    }

    cout<<length<<endl;
    return 0;
}