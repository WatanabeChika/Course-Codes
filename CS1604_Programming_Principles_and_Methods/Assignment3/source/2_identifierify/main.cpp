#include <iostream>
#include <string>
#include <cctype>

using namespace std;

// Get all words with single whitespace separated, and there's a whitespace on the head.
void get_str(string& sente){
    string word;
    while (cin >> word){
        sente += ' ';
        sente += word;
    }
}

// Turn a string into PascalCase.
string Pascal(string str){
    while (!isalnum(str[1]) && str != " ")
        str.erase(1,1);
    if (str == " ")
        return "";
    else {
        for (int i = 0; i < int(str.length()); ++i){
            if (!isalnum(str[i]))
                str[i+1] = toupper(str[i+1]);
        }
        for (int j = int(str.length()); j >= 0; --j){
            if (!isalnum(str[j]))
                str.erase(j,1);
        }
        return str;
    }
}

// Turn a string into camelCase.
string camel(string str){
    while (!isalnum(str[1]) && str != " ")
        str.erase(1,1);
    if (str == " ")
        return str;
    else {
        for (int i = 0; i < int(str.length()); ++i)
            str[i] = tolower(str[i]);
        return str.substr(0,2) + Pascal(str.substr(2));
    }
}

// Turn a string into snake_case.
string snake_(string str){
    while (!isalnum(str[1]) && str != " ")
        str.erase(1,1);
    if (str == " ")
        return "";
    else {
        while (!isalnum(str[str.length()-1]))
            str.erase(str.length()-1,1);
        for (int i = 0; i < int(str.length()); ++i)
            str[i] = tolower(str[i]);
        for (int j = 0; j < int(str.length()); ++j){
            if (!isalnum(str[j]) && j != 0 && j != int(str.length())-1 && str[j-1] != '_')
                str[j]='_';
        }
        for (int j = int(str.length()); j >= 0; --j){
            if (!isalnum(str[j]) && str[j] != '_')
                str.erase(j,1);
        }
        for (int j = int(str.length()); j > 0; --j){
            if (str[j]=='_' && str[j-1]=='_')
                str.erase(j,1);
        }
        return str;
    }
}

// main function
int main(){
    string sente = "";
    get_str(sente);
    cout << "PascalCase: " << Pascal(sente) << endl;
    cout << "camelCase: " << camel(sente) << endl;
    cout << "snake_case: " << snake_(sente) << endl;
    return 0;
}