#include <iostream>

using namespace std;

unsigned long long hashfuc(char *s) {
    int i = 0;
    unsigned long long hash = 0;
    while (s[i] != '\0') {
        hash = (hash*17 + s[i]);
        ++i;
    }
    hash %= 1000000000;
    return hash;
}

unsigned long long arr[10020] = {0};

int main() 
{
    int n, sum = 1;
    bool pos;
    unsigned long long index = 0;
    cin >> n;
    char s[1600] = "";

    for (int i = 0; i < n; ++i) {
        cin >> s;
        // cout << s << " ";
        index = hashfuc(s);
        arr[i] = index;
        // cout << index << " ";
        for (int j = 0; j < i; ++j) {
            pos = true;
            if (arr[j] == index) {
                pos = false;
                break;
            }
        }
        if (pos) ++sum;
    }

    cout << sum << endl;

    return 0;
}

