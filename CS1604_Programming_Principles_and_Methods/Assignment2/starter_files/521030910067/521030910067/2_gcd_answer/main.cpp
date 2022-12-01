/**
 * File: main.cpp
 * ------------------------
 * This file contains an iterative greatest common divisor implementation
 * and asks for its loop invariant
 */ 

#include<iostream>
using namespace std;

/* Function prototypes */
void gcdAnswer();
int gcd(int x, int y);

/* Main program */
int main(){
    gcdAnswer();
    return 0;
}

/*
 * Function: gcd_answer()
 * Usage: gcd_answer();
 * -----------------------------------------------------
 * print the correct loop invariant to iterative gcd implementation
 */
void gcdAnswer(){
    
    // cout << "a > 0 && b > 0 && GCD(b,r) = GCD(a,r)" << endl;
    // cout << "a > 0 && b > 0 && GCD(x,y) = GCD(a,r)" << endl;
    // cout << "a > 0 && b > 0 && GCD(x,y) = GCD(a,b)" << endl;
    cout << "a > 0 && b > 0 && GCD(x,y) = GCD(a,b) && r = a % b" << endl;
    // cout << "a > 0 && b > 0 && GCD(x,y) = GCD(b,r) && r = a % b" << endl;
    // cout << "a > 0 && b > 0 && GCD(a,b) = GCD(b,r) && r = a % b" << endl;
    // cout << "a > 0 && b > 0 && GCD(x,y) = GCD(b,r) && a = b + r" << endl;
    
    return;
}

/*
 * Function: gcd(int x, int y)
 * Usage: int result = gcd(x,y);
 * -----------------------------------------------------
 * return the greatest common divisor of x and y
 */
int gcd(int x, int y) { 
   int a = x;
   int b = y;
   int r = a % b;
   while (r != 0) {
       // Loop invariant here
       a = b;
       b = r;
       r = a % b; 
   } 
   return b;
} 