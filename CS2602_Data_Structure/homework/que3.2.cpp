# include <iostream>

using namespace std;

class myqueue {
    private:
        int *array;
        int front, rear;
        int maxsize, len;
    public:
        myqueue(int s) {
            array = new int[s];
            front = rear = 0;
            maxsize = s;
            len = 0;
        }
        bool isEmpty() const {
            return (len == 0);
        }
        bool isFull() const {
            return (len == maxsize-1);
        }
        void enqueue(const int x) {
            if (isFull()) return;
            array[rear] = x;
            if (rear == maxsize-1) rear = 0;
            else ++rear;
            ++len;
            cout << rear << ' ' << len << endl;
        }
        void dequeue() {
            if (isEmpty()) {
                cout << front << ' ' << 0 << endl;
                return;
            }
            if (front == maxsize-1) front = 0;
            else ++front;
            --len;
            cout << front << ' ' << len << endl;
        }
        ~myqueue() {
            delete []array;
        }
};

int main() 
{
    int s, n, oper, x;
    cin >> s >> n;
    myqueue thlis(s+1);
    while (n>0) {
        --n;
        cin >> oper;
        switch (oper) {
        case 0:
            cin >> x;
            thlis.enqueue(x);
            break;
        case 1:
            thlis.dequeue();
            break;
        default:
            break;
        }
    }
    return 0;
}