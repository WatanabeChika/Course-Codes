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
        void pop() {
            if (isEmpty()) return;
            node *j = top;
            top = j->next;
            delete j;
        }
        void push(const int x) {
            node *j = new node(x, top);
            top = j;
        }
        void clear() {
            while (!isEmpty()) {
                pop();
            }
        }
};

int main() 
{
    mystack nums;
    char sign;
    int num = 0;
    int a, b;

    char oper;
    while (cin >> oper) {
        switch (oper) {
        case '@':
            cout << nums.topsee() << endl;
            nums.clear();
            break;
        case '+':
            a = nums.topsee();
            nums.pop();
            b = nums.topsee();
            nums.pop();
            nums.push(b+a);
            break;
        case '-':
            a = nums.topsee();
            nums.pop();
            b = nums.topsee();
            nums.pop();
            nums.push(b-a);
            break;
        case '*':
            a = nums.topsee();
            nums.pop();
            b = nums.topsee();
            nums.pop();
            nums.push(b*a);
            break;
        case '/':
            a = nums.topsee();
            nums.pop();
            b = nums.topsee();
            nums.pop();
            nums.push(b/a);
            break;
        case '.':
            nums.push(num);
            num = 0;
            break;
        default:
            num = num*10 + int(oper - '0');
            break;
        }
    }
    return 0;
}