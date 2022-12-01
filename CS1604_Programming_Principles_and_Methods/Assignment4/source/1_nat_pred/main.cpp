#include <Vector.h>
#include <iostream>
#include <string>

using namespace std;

// Turn a string to a vector.
Vector<int> str_to_vec(string num){
    while (num[0] == '0' && num != "0")
        num.erase(0,1);
    Vector<int> res;
    for (int i = num.size()-1; i >= 0; --i){
        res.add(num[i] - '0');
    }
    return res;
}

// Turn a vector to a string.
string vec_to_str(Vector<int> num){
    string res = "";
    for (int i = num.size()-1; i >= 0; --i)
        res += char('0' + num[i]);
     while (res[0] == '0' && res != "0")
        res.erase(0,1);
    return res;
}

// get the perdessor of a vectorized number.
void get_pred(Vector<int>& num){
    while (num[int(num.size())-1] == 0 && !(num.size() == 1 && num[0] == 0))
        num.remove(int(num.size())-1);
    if (num.size() == 1 && num[0] == 0)
        return;
    for (int i = 0; i < num.size(); ++i){
        if (num[i] == 0)
            num[i] = 9;
        else {
            num[i] = num[i] - 1;
            break;
        }
    }
}

// main function
int main(){
    string str_n;
    int m;
    cin >> str_n >> m;
    Vector<int> vec_n = str_to_vec(str_n);
    for (int i = 0; i < m; ++i)
        get_pred(vec_n);
    cout << vec_to_str(vec_n) << endl;
    return 0;
}