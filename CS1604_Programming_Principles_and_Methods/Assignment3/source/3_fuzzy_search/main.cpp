#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Find maximum correlation and their positions respectively.
void fuz_sear(const string t, const string s, int& best, vector<int>& index){
    int len = s.length();
    for (int i = 0; i < int(t.length()) - len + 1; ++i){
        string com = t.substr(i, i + len);
        int match = 0;
        for (int j = 0; j < int(s.length()); ++j){
            if (com[j] == s[j])
                match+=1;
        }
        if (match > best){
            index.erase(index.begin(),index.end());
            index.push_back(i);
            best = match;
        }
        else if (match == best){
            index.push_back(i);
        }
        match = 0;
    } 
}

// Print every maximum correlations according to the given format.
void format_print(const string t, const string s, int best, vector<int> pos){
    for (auto i : pos){
        cout << t << endl;
        string align(i, ' ');
        cout << align;
        for (int j = 0; j < int(s.length()); ++j){
            if (t[j+i] == s[j]) {cout << '+';}
            else {cout << '-';}
        }
        cout << ' ' << best << '/' << s.length() << endl;
        cout << align << s << endl;
        cout << align << "^ col " << i+1 << endl;
        cout << "---" << endl;
    }
}

// main function
int main(){
    string T, S;
    getline(cin, T);
    getline(cin, S);
    int best_match = 0;
    vector<int> head_pos;
    fuz_sear(T, S, best_match, head_pos);
    format_print(T, S, best_match, head_pos);
    cout << "Found " << head_pos.size() << " best match(es)" << endl;
    return 0;
}