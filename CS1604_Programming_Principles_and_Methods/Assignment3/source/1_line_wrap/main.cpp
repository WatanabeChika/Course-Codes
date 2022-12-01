#include <iostream>
#include <string>
#include <cctype>

using namespace std;

// Turn many lines of words into one string with whitespaces separated.
void get_str (string& ori_str){
    string lines;
    while (cin >> lines) {
        for (int i=0; i < int(lines.length()); ++i){
            if (!isspace(lines[i])){
                lines += ' ';
                ori_str += lines;
                break;
            }  
        }
    }
}



// Insert "/n" into lines with given width and delete whitespaces on the head of lines.
string div_str (int w, string ori){
    while (ori[0] == ' ')
        ori.erase(0,1);
    
    auto index = ori.find(' ');
    if (index == string::npos || int(ori.length()) <= w)
        return ori;
    if (int(index) >= w){
        ori.insert(index, 1, '\n');
        return ori.substr(0, index+1) + div_str(w, ori.substr(index+1));
    }
    else {
        if (ori[w] == ' '){
            ori.insert(w, 1, '\n');
            return ori.substr(0, w+1) + div_str (w, ori.substr(w+1));
        }
        else {
            int cws = w-1;
            while (ori[cws] != ' ') {--cws;}
            ori.insert(cws, 1, '\n');
            return ori.substr(0, cws+1) + div_str (w, ori.substr(cws+1));
        }
    }
}


// main function
int main(){
    int W;
    cin >> W;
    string ori_str = "";
    get_str (ori_str);
    cout << div_str (W, ori_str) << endl;
    return 0;
}