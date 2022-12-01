# include <iostream>

using namespace std;

struct HuffNode {
    int data, parent;
    HuffNode() {}
    HuffNode(int data, int par=0) 
    : data(data), parent(par) {}
};

int minIndex(HuffNode *tree, int m, int n) {
    long long minData = 3000000000;
    int index;
    for(m; m<n; ++m) {
        if (tree[m].parent == 0 && tree[m].data < minData) {
            index = m;
            minData = tree[m].data;
        }
    }
    return index;
}

long long sumOfHuffTree(int type, int *nums)
{
    HuffNode tree[2*type];
    int i, j, start;
    long long sum = 0;
    for(i = 0; i < type; ++i) {
        j = 2*type-1-i;
        tree[j] = HuffNode(nums[i]);
    }

    start = type-1;
    while (start != 0) {
        int firstMin = minIndex(tree, start+1, 2*type);
        tree[firstMin].parent = start;
        int secondMin = minIndex(tree, start+1, 2*type);
        tree[secondMin].parent = start;

        tree[start].data = tree[firstMin].data + tree[secondMin].data;
        tree[start].parent = 0;

        sum += tree[start].data;
        --start;
    }
    return sum;
}


int main() 
{
    int type;
    cin >> type;
    int nums[type];
    int num, i=0;
    while(cin >> num) {
        nums[i] = num;
        ++i;
    }
    cout << sumOfHuffTree(type, nums) << endl;
    return 0;
}