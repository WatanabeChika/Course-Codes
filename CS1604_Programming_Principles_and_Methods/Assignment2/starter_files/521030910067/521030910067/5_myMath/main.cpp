/** File: main.cpp
 * ----------------
 * This file demonstrates the library of GCD and LCM functions.
 * 
 * Organize the project yourself. The project should have the following structure:
 * 
 *  - main.cpp
 *    + `void gcd_lcm(int m, int n)` that calls the `gcd` and `lcm` in the math library
 *      and computes the GCD and LCM of m and n
 *    + main function to process input/output, and call functions `gcd_lcm` and `factorial`
 * 
 * Note that input/output should be handled by the main function instead of gcd_lcm
 * and that computation should be done in the gcd_lcm function. 
 * Find a way to pass the computation result in gcd_lcm to the main function
 * 
 *  - myMath.h: interfaces for
 *    + gcd
 *    + lcm
 *    + factorial
 * 
 *  - myMath.cpp: implementations for
 *    + gcd in a tail recursive way
 *    + lcm
 *    + factorial in a tail recursive way
 * 
 *  The interface should **NOT** expose any implementation details, in particular,
 *  the tail recursive implementation of factorial.
 * 
 */

#include <iostream>
#include "myMath.h"

using namespace std;

/*
 * Function: gcd_lcm(int & m, int & n)
 * Usage: gcd_lcm(m, n);
 * -----------------------------------------------------
 * Compute GCD(m, n) and LCM(m, n) and give results to m and n respectively.
 */
void gcd_lcm(int & m, int & n){
    int a = m;
    int b = n;
    m = gcd(a, b);
    n = lcm(a, b);
}

/* main function */
int main(){
    int m, n, k;
    cin >> m >> n >> k;
    gcd_lcm(m, n);
    cout << m << endl << n <<endl;
    cout << factorial(k) << endl;
    return 0;
}