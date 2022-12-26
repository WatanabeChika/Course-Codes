#include <iostream>

using namespace std;

template<class elemType>
void shellSort(elemType *a, int n)
{
    int step, i, j;
    elemType tmp;

    for (step = n/2; step > 0; step/=2) {
        for (i = step; i < n; ++i) {
            tmp = a[i];
            j = i;
            while (j-step >= 0 && a[j-step] > tmp) {
                a[j] = a[j-step];
                j-=step;
            }
            a[j] = tmp;
        }
    }
}

int main() 
{
    int n, onehash, count = 0;
    cin >> n;
    const int M = 20000;
    int a[n], b[n];
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }

    for (int i = 0; i < n ; ++i) {
        onehash = a[i];
        for(int j = 0; j < i; ++j) {
            if (b[j] == onehash) {
                a[i] = M;
                ++count;
                break;
            }
        }
        b[i] = onehash;
    }

    shellSort(a, n);

    cout << n-count << endl;

    for (int i = 0; i < n; ++i) {
        if (a[i] != M)
            cout << a[i] << " ";
    }

    return 0;
}