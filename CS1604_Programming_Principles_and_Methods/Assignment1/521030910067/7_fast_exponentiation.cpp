#include <iostream>
#include <vector>

using namespace std;

// This program would calculate (a^n)%2019 in a fast way.
int main(){
    long long a,n;
    vector<int> num;
    int ult=1;
    cin>>a>>n;
    while (n>=1){
        num.push_back(n%2);
        n/=2;
    }
    for (int i=0; i!=int(num.size()); ++i){
        int res=a;
        if (num[i]==1){
            for (int j=0; j!=i; ++j){
                res*=res;
                res%=2019;
            };
            ult*=res;
            ult%=2019;
        };
    };
    cout<<ult<<endl;
    return 0;
}