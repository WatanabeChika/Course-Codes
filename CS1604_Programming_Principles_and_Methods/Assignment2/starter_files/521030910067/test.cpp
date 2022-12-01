#include <iostream>
#include <string>
#include "Vector.h"

using namespace std;

bool is_post(string ori, string exa){
    if (ori.length() < exa.length())
        return false;
    else {
        for (int i=0; i < exa.length(); ++i){
            if (ori[ori.length() - 1 - i] != exa[exa.length() - 1 - i])
                return false;
        }
        return true;
    }
}

int fb(int n){
    if (n==1)
        return 1;
    else if (n==0)
        return 0;
    else
        return fb(n-1)+fb(n-2);
}

int main(){
    Vector<int> vec;
    vec.add(34);
    cout << vec << endl;
    return 0;
}