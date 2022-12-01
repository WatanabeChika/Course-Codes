/**
 * File: main.cpp
 * ------------------------
 * This file contains an **incorrect** implementation of a function 
 * int countPlans. Your task is to figure out what the error is and correct it.
 */ 

#include <iostream>

using namespace std;

/*
 * Function: countPlans(int m, int n)
 * Usage: int result = countPlans(int m, int n);
 * -----------------------------------------------------
 * return the total number of possible dividing plans
 */
int countPlans(int m, int n){
    if(m == 0||n == 1)
        return 1;
    if(m < n)
        return countPlans(m, m);
    return countPlans(m - n, n) + countPlans(m, n - 1);
}

/* main function */
int main() {
    int m, n;
    cin >> m >> n;
    cout << countPlans(m, n) << endl;
    return 0;
}
