#include <iostream>

using namespace std;

int gcd (int a, int b) {
    while (b > 0) {
        int tmp = a % b;
        a = b;
        b = tmp;
    }
    return a;
}

struct rat {
    double data;
    int down, up;

    rat(const int& down = 1, const int& up = 1) : data(double(up)/double(down)), down(down), up(up) {};
    void reload(const int& anodown, const int& anoup) {
        data = double(anoup)/double(anodown);
        down = anodown; up = anoup;
    }
};



class myqueue{
    rat a[3900000];
    int size = 0;

    public:
    void enqueue(const rat &x) {
        ++size;
        int p = size; 
        a[p] = x;
        while (a[p].data > a[p/2].data) {
            swap(a[p], a[p/2]);
            p /= 2;
        }
    }

    bool isEmpty() { return size == 0;}

    rat top() { return a[1];}

    void dequeue() {
        a[1] = a[size];
        --size;

        for (int p=1, q; p<=size; p=q) {
            int l = p*2, r = p*2+1;
            if (a[l].data > a[r].data) q = l;
            else q = r;
            if (q>size || a[q].data <= a[p].data) break;
            swap(a[q], a[p]);
        }
    }
};

myqueue que;

rat getK(const int& N, const int& K) {
    int i = N, flag = K;

    que.enqueue(rat(N, N-1));
    rat res;
    
    while (flag > 0) {
        res = que.top();
        que.dequeue();

        int down = res.down;
        int up = res.up;
        if (down < i) que.enqueue(rat(down+1, up));
        if (up != 1 && up == down-1) que.enqueue(rat(down-1, up-1));
        if (gcd(up, down) != 1) ++flag;
        --flag;
    }
    return res;

    // rat que_num;
    // rat sign_num(i, i-1);
    // --flag;
    // if (flag == 0) return sign_num;
    // while (flag > 0) {
    //     --i;
    //     sign_num.reload(i, i-1);
    //     while (!que.isEmpty()) {
    //         if (que.top().data > sign_num.data) {
    //             que_num.reload(que.top().down, que.top().up);
    //             que.dequeue();
    //             --flag;
    //             if (flag == 0) return que_num;
    //         }
    //         else break;
    //     }
    //     --flag;
    //     if (flag == 0) return sign_num;
    //     for (int j = i+1; j <= N; ++j) {
    //         if (gcd(j, i-1) == 1) {
    //             que_num = rat(j, i-1);
    //             que.enqueue(que_num);
    //         }
    //     }
    // }
}


int main() {
    int N, K;
    cin >> N >> K;

    rat ans = getK(N, K);
    cout << ans.up << '/' << ans.down << endl;    
    
    return 0;
}