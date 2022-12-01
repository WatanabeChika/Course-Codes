/**
 * File: main.cpp
 * ------------------------
 * This file contains a tail recursive function to compute the greatest common divisor and a tail recursive function to compute the factorial.
 * 
 */ 

#include<iostream>
using namespace std;

/* Function prototypes */
int gcd(int x, int y);
int factorial(int n);

/* Main program */
int main(){
    int x, y, n;
    cin >> x >> y >> n;
    cout << gcd (x, y) << endl;
    cout << factorial (n) <<endl;
    return 0;
}

/*
 * Function: gcd(int x, int y)
 * Usage: int result = gcd(x,y);
 * -----------------------------------------------------
 * return the greatest common divisor of x and y
 */
int gcd(int x, int y) {
    if (y == 0)
        return x;
    else {
        int a = y;
        int b = x % y;
        return gcd(a, b);
    }
}



/*
 * Function: factorial_aux(int n, int y)
 * Usage: int result = factorial_aux(n,y);
 * -----------------------------------------------------
 * Auxiliary function to compute the factorial of n
 * implemented in a tail recursive way
 * 
 * n: the number to compute the factorial of
 * y: the result of the factorial computation for the previous recursive call
 * 
 */
int factorial_aux(int n, int y) {
    if (n == 0)
        return 1;
    if (n == 1)
        return y;
    else {
        y *= n;
        --n;
        return factorial_aux(n, y);
    }
}

/*
 * Function: factorial(int n)
 * Usage: int result = factorial(n);
 * -----------------------------------------------------
 * Compute the factorial of n
 * 
 */
int factorial(int n) {
    return factorial_aux(n, 1);
}