/**
 * File: main.cpp
 * ----------------
 * This file implements the main program for the permutation and combination
 * programs.
 * 
 * Organize this file yourself. You should have the following functions:
 * 
 *  - int permutations(int n, int k)
 *  - int combinations(int n, int k)
 * 
 * and a main function that calls these two functions.
 * 
 */
#include <iostream>

using namespace std;

/*
 * Function: permutations(int n, int k)
 * Usage: long long result = permutations(n, k);
 * -----------------------------------------------------
 * return the number of permutations of m items taken k at a time
 */
int permutations(int n, int k){
    if (k==0) {return 1;}
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
    if (k==0) {return 1;}
    long long res = n;
    for (int i = 1; i != k; ++i){
        res *= (n - 1);
        res /= (i + 1);
        --n;
    }
    return res;
}

/* main function */
int main(){
    int m, k;
    cin >> m >> k;
    cout << permutations (m, k) << endl;
    cout << combinations (m, k) << endl;
    return 0;
}