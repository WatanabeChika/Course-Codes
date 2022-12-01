/** File: main.cpp
 * ----------------
 * This file computes the plans for three scenarios of dividing balls into buckets.
 * 
 * Organize the project yourself. The project should have the following structures:
 * 
 *  - main.cpp
 *    + main function to process input/output
 *    + countPlanA/countPlanB/countPlanC functions
 * 
 *  - combinatorics.h: interfaces for
 *    + permutations
 *    + combinations
 * 
 *  - combinatorics.cpp: implementations for
 *    + permutations that does not use factorial
 *    + combinations that does not use factorial
 * 
 * 
 */

#include <iostream>
#include "combinatorics.h"

using namespace std;

/*
 * Function: countPlanA(int n, int k)
 * Usage: int result = countPlanA(n, k);
 * -----------------------------------------------------
 * Distribute n distinguishable balls into k buckets of different colors, and each bucket should at most contain one ball.
 * return the number of different distributions
 */
int countPlanA(int n, int k){
    return permutations(k, n);
}

/*
 * Function: countPlanB(int n, int k)
 * Usage: int result = countPlanB(n, k);
 * -----------------------------------------------------
 * Distribute n indistinguishable balls into k buckets of different colors, and each bucket should contain at least one ball.
 * return the number of different distributions
 */
int countPlanB(int n, int k){
    return combinations(n - 1, k - 1);
}

/*
 * Function: countPlanC(int n, int k)
 * Usage: long long result = countPlanC(n, k);
 * -----------------------------------------------------
 * Distribute n indistinguishable balls into k buckets of different colors, and buckets can be empty.
 * return the number of different distributions
 */
int countPlanC(int n, int k){
    return combinations(n + k - 1, k - 1);
}

/* main function */
int main(){
    char plan;
    int n, k;
    cin >> plan >> n >> k;
    switch (plan)
    {
    case 'A':
        cout << countPlanA(n, k) << endl;
        break;
    case 'B':
        cout << countPlanB(n, k) << endl;
        break;
    case 'C':
        cout << countPlanC(n, k) << endl;
        break;
    }
    return 0;
}