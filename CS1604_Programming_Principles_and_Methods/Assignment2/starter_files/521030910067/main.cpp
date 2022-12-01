#include <iostream>
#include "queue.h"
#include "map.h"
#include "set.h"
#include "grid.h"
#include "vector.h"
#include "Stack.h"
#include "string"
using namespace std;

void test(bool b){
    if(b)cout<<"PASS";
    else cout<<"FAILED";
    cout<<endl;
}


int main()
{
    Queue<int> que;
    que.enqueue(3);
    cout<< "Test Queue: ";
    test(que.size()==1&&que.dequeue()==3&&que.size()==0);

    cout<< "Test Stack: ";
    Stack<int> stk;
    stk.push(1);
    test(stk.pop()==1&&stk.size()==0);

    cout<< "Test Map: ";
    Map<string,int> m;
    m.put("a",1);
    test(m.get("a")==1);

    cout<<"Test Set: ";
    Set<string> s;
    s.add("hana");
    test(s.contains("hana"));

    cout<<"Test Vector: ";
    Vector<int> v;
    v.add(1);
    v.insert(1,2);
    test(v[0]==1&&v[1]==2);

    cout<<"Test Grid: ";
    Grid<int> g(2,3);
    test(g.numRows()==2&&g.numCols()==3);
    return 0;
}
