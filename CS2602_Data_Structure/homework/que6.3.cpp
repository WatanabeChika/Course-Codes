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
    int n, t, k, num = 0;
    cin >> n >> t >> k;
    int a[n], b[n];
    double point = double(100)/double(n);
    double score;

    for (int i = 0; i < n; ++i) {
        cin >> a[i];
        b[i] = 0;
    }
    shellSort(a,n);

    while (t < k) {
        t += a[num];
        if (t < k) {
            b[num] = 1;
        }
        ++num;
    }

    for (int i = 0; i < n; ++i) {
        if (b[i] == 1) 
            score += point;
        else
            score += 0.1 * point;
    }

    cout << int(score) << endl;

    return 0;
}