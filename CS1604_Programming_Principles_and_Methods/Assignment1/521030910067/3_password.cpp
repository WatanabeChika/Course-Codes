#include <iostream>
#include <vector>

using namespace std;

// This program would change a four-digit number into 4 letters using some rule.
int main(){
    vector<int> nums;
    vector<char> password;
    int dig;
    cin>>dig;
    for (int i=1000; i!=0; i/=10){
    nums.push_back(dig/i);
    dig%=i;
    };
    for (int i=0; i!=4; ++i)
        password.push_back(64+5+nums[i]);
    for (auto j: password)
        cout<<j;
    cout<<endl;
    return 0;
}