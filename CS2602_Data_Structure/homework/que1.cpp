#include <iostream>
#include <vector>
#include <math.h>

using namespace std;

int det(const vector<vector<int> >&m)
{
    if (m.size() == 1) return m[0][0];
    else {
        int n = m.size();
        int res = 0;
        vector<vector<int> >new_m(n-1, vector<int>(n-1));
        for (int i=0; i<n; ++i)
        {
            int row=0;
            for (int j=0; j<n; ++j)
            {
                if (i == j) continue;
                else {
                    for (int k=1; k<n; ++k)
                        new_m[row][k-1] = m[j][k];
                    ++row;
                }
            }
            res += m[i][0]*pow(-1,i)*det(new_m);
        }
        return res;
    }
}

int det2(int m[][10], int n)
{
    if (n == 1) return m[0][0];
    else {
        int res = 0;
        int new_m[n-1][10];
        for (int i=0; i<n; ++i)
        {
            int row=0;
            for (int j=0; j<n; ++j)
            {
                if (i == j) continue;
                else {
                    for (int k=1; k<n; ++k)
                        new_m[row][k-1] = m[j][k];
                    ++row;
                }
            }
            res += m[i][0]*pow(-1,i)*det2(new_m, n-1);
        }
        return res;
    }
}

long long cal_to_same(const vector<int>& a, const vector<int>& b)
{
    int len = a.size();
    long long res=0;
    bool sign;

    vector<int> seq;
    for (int i=0; i<len; ++i)
    {
        seq.push_back(a[i]-b[i]);
    }
    res += abs(seq[0]);

    if (seq[0] >= 0) sign = true;
    else sign = false;

    for (int j=1; j<len; ++j)
    {
        if (seq[j] >= 0) {
            if (sign) {
                if (seq[j] > seq[j-1]) res += abs(seq[j] - seq[j-1]);
            }
            else {
                res += abs(seq[j]);
                sign = true;
            }
        }
        else {
            if (!sign) {
                if (seq[j] < seq[j-1]) res += abs(seq[j] - seq[j-1]);
            }
            else {
                res += abs(seq[j]);
                sign = false;
            }
        }
    }
    return res;
}

long long cal_to_same2(const int a[], const int b[], int len)
{
    long long res=0;
    bool sign;

    int seq[100000];
    for (int i=0; i<len; ++i)
    {
        seq[i] = a[i]-b[i];
    }
    res += abs(seq[0]);

    if (seq[0] >= 0) sign = true;
    else sign = false;

    for (int j=1; j<len; ++j)
    {
        if (seq[j] >= 0) {
            if (sign) {
                if (seq[j] > seq[j-1]) res += abs(seq[j] - seq[j-1]);
            }
            else {
                res += abs(seq[j]);
                sign = true;
            }
        }
        else {
            if (!sign) {
                if (seq[j] < seq[j-1]) res += abs(seq[j] - seq[j-1]);
            }
            else {
                res += abs(seq[j]);
                sign = false;
            }
        }
    }
    return res;
}

int main(){
    int n, tmp;
    cin >> n;
    // int m[n][10];
    
    int a[100000], b[100000];
    for (int i=0; i<n; ++i)
    {
        cin >> tmp;
        a[i] = tmp;
    }
    for (int i=0; i<n; ++i)
    {
        cin >> tmp;
        b[i] = tmp;
    }
    cout << cal_to_same2(a,b,n) << endl;
    
    /*for (int i=0; i<n; ++i) {
        for (int j=0; j<n; ++j) {
            cin >> tmp;
            m[i][j] = tmp;
        }
    }   

    cout << det2(m,n) << endl;*/
    return 0;
}