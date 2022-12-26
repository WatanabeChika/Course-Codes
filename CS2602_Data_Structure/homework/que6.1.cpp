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

int main() {
    long long res = 0;
    int n;
    cin >> n;
    long long a1[n];
    long long a2[n];
    for (int i = 0; i < n; ++i) {
        cin >> a1[i];
    }
    for (int i = 0; i < n; ++i) {
        cin >> a2[i];
    }
    shellSort(a1, n);
    shellSort(a2, n);
    for (int i = 0; i < n; ++i) {
        res += a1[i] * a2[i];
    }
    cout << res << " ";
    res = 0;
    for (int i = 0; i < n; ++i) {
        res += a1[i] * a2[n-1-i];
    }
    cout << res << endl;
    return 0;
}