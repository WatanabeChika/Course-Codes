#include <iostream>
#include <string>

using namespace std;

// Reverse the string according to syllables.
string rev_syl(string str){
    if (str.length() == 0)
        return str;
    else {
        if (str[0] == 'a' || str[0] == 'e' || str[0] == 'i' || str[0] == 'o' || str[0] == 'u')
            return rev_syl(str.substr(1)) + str[0];
        else
            return rev_syl(str.substr(2)) + str.substr(0,2);
    }
}

// main function
int main(){
    string str;
    cin >> str;
    if (rev_syl(str) == str)
        cout << "YES" << endl;
    else
        cout << "NO" << endl;
    return 0;
}