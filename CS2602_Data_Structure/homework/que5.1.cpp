#include <iostream>

using namespace std;

struct Huffnode {
    int weight;
    int parent, left, right;
};

int minIndex(Huffnode *bt, int k, int m)
{
    int i, min;
    long long minweight = 10000000000;

    for (i = m-1; i>k; --i) {
        if (bt[i].parent == 0 && bt[i].weight < minweight) {
            min = i; minweight = bt[i].weight;
        }
    }

    return min;
}

Huffnode *huff_tree(Huffnode *BBtree, int m, int *w, int n)
{
    int first_min, second_min;
    int i, j;

    for (i = 0; i < n; ++i) {
        BBtree[m-1-i].weight = w[i];
        BBtree[m-1-i].parent = 0;
        BBtree[m-1-i].left = 0;
        BBtree[m-1-i].right = 0;

    }

    j = n-1;
    while (j > 0) {
        first_min = minIndex(BBtree, j, m);
        BBtree[first_min].parent = j;
        BBtree[j].left = first_min;

        second_min = minIndex(BBtree, j, m);
        BBtree[second_min].parent = j;
        BBtree[j].right = second_min;

        BBtree[j].weight = BBtree[first_min].weight + BBtree[second_min].weight;
        BBtree[j].parent = 0;
        --j;
    }
    return BBtree;
} // Huffman Tree


int main() 
{
    int n, m;
    cin >> n;
    m = n*2;
    int w[n+1];

    for (int i = 0; i < n; ++i) {
        cin >> w[i];
    }

    Huffnode BBtree[m];
    Huffnode *tree = huff_tree(BBtree, m, w, n);

    int path, par;
    long long sum = 0;
    for (int j = 0; j < n; ++j) {
        path = 0;
        par = tree[m-1-j].parent;
        while (par != 0) {
            ++path;
            par = tree[par].parent;
        }
        sum += path * tree[m-1-j].weight;
    }

    cout << sum << endl;

    return 0;
}

