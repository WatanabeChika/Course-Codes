#include <iostream>
#include <Stack.h>
#include <string>
#include <sstream>
#include <Map.h>
#include <Vector.h>

using namespace std;


int pc = 0;
string stm;
Stack<int> mystack;
Map<string, int> mystate;

// 将栈顶两个元素出栈，两者相加，并将结果放入栈顶.
void add(){
    if (mystack.size() < 2){
        stm = "Error";
        return;
    }
    int a = mystack.pop();
    int b = mystack.pop();
    mystack.push(a + b);
}

// 将栈顶两个元素出栈，第二个出栈的元素减去第一个出栈的元素，并将结果放入栈顶.
void sub(){
    if (mystack.size() < 2){
        stm = "Error";
        return;
    }
    int a = mystack.pop();
    int b = mystack.pop();
    mystack.push(b - a);
}

// 乘法，与 add 类似.
void mul(){
    if (mystack.size() < 2){
        stm = "Error";
        return;
    }
    int a = mystack.pop();
    int b = mystack.pop();
    mystack.push(a * b);
}

// 整数除法，与 sub 类似.
void div(){
    if (mystack.size() < 2){
        stm = "Error";
        return;
    }
    int a = mystack.pop();
    int b = mystack.pop();
    if (a == 0){
        stm = "Error";
        return;
    }
    mystack.push(b / a);
}

// 将栈顶元素a出栈，并更新mystate将变量x映射到a，其中x是一个类型为string的变量名.
void setvar(const string& key){
    if (mystack.isEmpty()){
        stm = "Error";
        return;
    }
    int a = mystack.pop();
    mystate[key] = a;
}

// 将mystate中变量x的值放入栈顶.
void var(const string& key){
    if (mystate.containsKey(key))
        mystack.push(mystate[key]);
    else {
        stm = "Error";
        return;
    }
}

// 跳转到pc=n处.
void jmp(int n, const int& ALLstates){
    if (n >= ALLstates){
        stm = "Error";
        return;
    }
    pc = n - 1; 
}

// 将栈顶两个元素出栈，若两者相等，则跳转到pc=n处.
void jmpeq(int n, const int& ALLstates){
    if (mystack.size() < 2){
        stm = "Error";
        return;
    }
    int a = mystack.pop();
    int b = mystack.pop();
    if (a == b){
        if (n >= ALLstates){
            stm = "Error";
            return;
        }
        pc = n - 1; 
    }
}

// 将栈顶两个元素出栈，若第二个出栈的元素大于第一个出栈的元素，则跳转到pc=n处.
void jmpgt(int n, const int& ALLstates){
    if (mystack.size() < 2){
        stm = "Error";
        return;
    }
    int a = mystack.pop();
    int b = mystack.pop();
    if (a < b){
        if (n >= ALLstates){
            stm = "Error";
            return;
        }
        pc = n - 1; 
    }
}

// 小于关系，执行方式与 jmpGt 类似.
void jmplt(int n, const int& ALLstates){
    if (mystack.size() < 2){
        stm = "Error";
        return;
    }
    int a = mystack.pop();
    int b = mystack.pop();
    if (a > b){
        if (n >= ALLstates){
            stm = "Error";
            return;
        }
        pc = n - 1; 
    }
}

// 将整数n放入栈顶.
void stackconst(int n){
    mystack.push(n);
}

// 打印变量x的值并换行.
void stackprint(const string& key){
    if (mystate.containsKey(key))
        cout << mystate[key] << endl;
    else {
        stm = "Error";
        return;
    }
}

// 判断此时该执行什么命令.
void judge_stms(string& stm, string& subs, const int& ALLstates){
    if (stm == "Add") {add();} 
    else if (stm == "Sub") {sub();}
    else if (stm == "Mul") {mul();} 
    else if (stm == "Div") {div();} 
    
    else if (stm == "SetVar"){
        setvar(subs); 
    } 
    else if (stm == "Var"){
        var(subs);
    }
    
    else if (stm == "Jmp"){
        jmp(stoi(subs),ALLstates);
    }
    else if (stm == "JmpEq"){
        jmpeq(stoi(subs),ALLstates);
    }
    else if (stm == "JmpGt"){
        jmpgt(stoi(subs),ALLstates);
    }
    else if (stm == "JmpLt"){
        jmplt(stoi(subs),ALLstates);
    }
    
    else if (stm == "Const"){
        stackconst(stoi(subs));
    }
    else if (stm == "Print"){
        stackprint(subs);
    }
}

// main function
int main(){
    int ALLstates;
    string instru;
    string subs;
    Vector<string> cmd;
    istringstream iss;
    cin >> ALLstates;

    // get all commands
    while (getline (cin, instru)){
        if (instru!="")
            cmd.add(instru);
    }
    while (pc < ALLstates){
        iss.str(cmd[pc]);
        iss >> stm;
        if (iss) iss >> subs;
        judge_stms(stm, subs, ALLstates);
        if (stm != "Halt" && pc == ALLstates - 1)
            stm = "Error";
        if (stm == "Error"){
            cout << stm << endl;
            break;
        }
        if (stm == "Halt")
            break;
        ++pc;
        iss.clear();
    }
    return 0;
}