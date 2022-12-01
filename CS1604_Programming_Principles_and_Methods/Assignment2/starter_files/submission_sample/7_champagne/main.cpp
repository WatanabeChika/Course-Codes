/**
 * File: main.cpp
 * -------------------
 * This file contains a function to compute the amount of champagne
 * in the glass of a champagne tower after pouring from the top
 * 
 */

#include<iostream>

using namespace std;

/* Function prototypes */
double champagneTower(int poured, int query_row, int query_glass);

/* Main program */
int main(){
    int query_row, query_glass, poured;
    cin >> poured >> query_row >> query_glass;
    double result = champagneTower(poured, query_row, query_glass);
    cout << result << endl;
    return 0;
}


/*
 * Function: champagneTower(int poured, int query_row, int query_glass)
 * Usage: double result = champagneTower(poured, query_row, query_glass);
 * -----------------------------------------------------
 * return the amount of champagne that is left in the position
 * (query_row, query_glass) after pouring from the top `poured`
 * amount of champagne
 * 
 * The result will range from 0 to 1
 */
double champagneTower(int poured, int query_row, int query_glass) {
    /** FILL IN HERE */
    return 0;
}


/** TODO:
 * You should write another recursive function to calculate 
 * the flow of the champagne */