#include <iostream>

using namespace std;

int maxlen;
int adv = 0;

int add_two_num(int pnum, int qnum, int& adv)
{
    int ans;
    if (pnum + qnum + adv >= 10) {
            ans = (adv + pnum + qnum)%10;
            adv = 1;
            return ans;
        }
        else {
            ans = adv + pnum + qnum;
            adv = 0;
            return ans;
        }
}

char* add_two_string(char *p, char *q)
{
    int pnum, qnum;
    char resnum;
    char *res = new char[maxlen];
    int i = 0;

    while (p[i] != '\0' && q[i] != '\0') {
        pnum = p[i] - 48;
        qnum = q[i] - 48;
        resnum = add_two_num(pnum, qnum, adv);
        res[i] = resnum + 48;
        ++i;
    }

    if (p[i] != '\0' && q[i] == '\0') {
        while (p[i] != '\0') {
            pnum = p[i] - 48;
            resnum = add_two_num(pnum, 0, adv);
            res[i] = resnum + 48;
            ++i;
        }
    }
    else if (p[i] == '\0' && q[i] != '\0') {
        while (q[i] != '\0') {
            qnum = q[i] - 48;
            resnum = add_two_num(0, qnum, adv);
            res[i] = resnum + 48;
            ++i;
        }
    }
    return res;
}

int main()
{
    int len1, len2;
    cin >> len1 >> len2;
    maxlen = max(len1, len2);

    char str1[len1*2] = "";
    char str2[len2*2] = "";
    char charnum;

    for (int i = 0; i < len1-1; ++i) {
        cin >> charnum;
        str1[i] = charnum;
    }
    for (int i = 0; i < len2-1; ++i) {
        cin >> charnum;
        str2[i] = charnum;
    }

    char* res = add_two_string(str1, str2);

    for (int j = 0; j < maxlen-1; ++j) {
        cout << res[j];
    }
    if (adv == 0) {
        cout << endl;
    }
    else {
        cout << '1' << endl;
    }
    delete []res;

    return 0;
}