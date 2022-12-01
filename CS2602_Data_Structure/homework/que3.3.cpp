# include <iostream>

using namespace std;

struct node {
    int data;
    node *next;
    node(const int d, node *n = nullptr) : data(d), next(n) {};
};

class mystack {
    private:
        node *top;
    public:
        mystack() {
            top = nullptr;
        }
        bool isEmpty() const {
            return (!top);
        }
        int topsee() const {
            if (isEmpty()) return -1;
            return top->data;
        }
        void pop(int& m) {
            if (isEmpty()) return;
            node *j = top;
            m = j->data;
            top = j->next;
            delete j;
        }
        void push(const int x) {
            node *j = new node(x, top);
            top = j;
        }
        ~mystack() {
            while (!isEmpty()) {
                int a;
                pop(a);
            }
        }
};

bool canPop(int n)
{
    mystack numlist;
    int m, num;
    int len = 0;
    cin >> m; ++len;

    for (int i = 1; i <= n; ++i) { 
        numlist.push(i);
        while (!numlist.isEmpty() && numlist.topsee() == m) {
                numlist.pop(num);
                if (len < n) {
                    cin >> m; ++len;
                }
        }
    }
    if (numlist.isEmpty() && len == n) return true;
    
    while (len < n) {
        cin >> m;
        ++len;
    }
    return false;
}


int main()
{
    int T, n;
    cin >> T;
    while (T>0) {
        cin >> n;
        if (canPop(n)) cout << "Yes" << endl;
        else cout << "No" << endl;
        --T;
    }
    return 0;
}