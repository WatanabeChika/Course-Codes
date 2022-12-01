#include <iostream>

using namespace std;

/*
 * Function: permutations(int n, int k)
 * Usage: long long result = permutations(n, k);
 * -----------------------------------------------------
 * return the number of permutations of m items taken k at a time
 */
int permutations(int n, int k){
    long long res = n;
    for (int i = k; i != 1; --i){
        res *= (n - 1);
        --n;
    }
    return res;
}

/*
 * Function: combinations(int n, int k)
 * Usage: long long result = combinations(n, k);
 * -----------------------------------------------------
 * return the number of combinations of m items taken k at a time
 */
int combinations(int n, int k){
    long long res = n;
    for (int i = 1; i != k; ++i){
        res *= (n - 1);
        res /= (i + 1);
        --n;
    }
    return res;
}