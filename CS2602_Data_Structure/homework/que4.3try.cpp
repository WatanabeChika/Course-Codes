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

struct treenode {
    double data;
    int down, up;
    treenode *left, *right;

    treenode(const int& down, const int& up, treenode *l = NULL, treenode *r = NULL)
        : data(double(up)/double(down)), down(down), up(up), left(l), right(r) {};
};

class Ftree {
    private:
        treenode *root;
        treenode *array[100000];
        int pos = 1;
        void inorder(treenode* p, int k) {
            if(!p) return;
            inorder(p->left, k);
            if (pos > k) return;
            array[pos] = p;
            if (pos == k) {
                ++pos;
                return;
            }
            ++pos;
            inorder(p->right, k);
        }
    public:
        Ftree() {root = NULL;}

        void insert(const int& down, const int& up) {
            if (!root) root = new treenode(down, up);
            else {
                double thisdata = double(up)/double(down);
                treenode *p, *q;

                if (thisdata > root->data) {
                    p = root->left;
                    q = root;
                }
                else {
                    p = root->right;
                    q = root;
                }

                while (p) {
                    if (thisdata > p->data) {
                        q = p;
                        p = p->left; 
                    }
                    else {
                        q = p; 
                        p = p->right;
                    }
                }

                p = new treenode(down, up);
                if (thisdata > q->data) q->left = p;
                else q->right = p;
            } 
        }

        void getK(const int& k) {
            inorder(root, k);
            cout << array[pos-1]->up << '/' << array[pos-1]->down << endl;
        }
};


int main() {
    int N, K;
    cin >> N >> K;
    Ftree tree;

    int min_i = 2;
    int min_j = 1;

    if (N > 1000) {
        min_i = N - K/N - 1;
        min_j = min_i/2;
    }

    for (int i = min_i; i <= N; ++i) {
        for (int j = min_j; j < i; ++j) {
            if (gcd(i, j) == 1) {
                tree.insert(i, j);
            }
        }
    }

    tree.getK(K);
    
    return 0;
}