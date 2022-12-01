#include <iostream>
using namespace std;

void fun(int n)
{
    int x=n;
    int* circ = new int[n];
    for (int i = 0; i < n; ++i)
        circ[i] = i+1;
    int count = 1;
    while (x>1) {
        for (int i = 0; i < n; ++i){
            if (circ[i] == 0) continue;
            else {
                if (count%3 == 0){
                    circ[i] = 0;
                    --x;
                }
                ++count;
            }
        }
    }
    for (int i = 0; i < n; ++i){
        if (circ[i] != 0)
            cout << circ[i] << " ";
    }
    delete circ;
}

int main()
{    
    for (int n=0; n<101; ++n)
        fun(n);
    return 0;
}