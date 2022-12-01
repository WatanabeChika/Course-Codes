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
double champagneFlow(double to_pour, int row, int col);

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
    double flow = champagneFlow(poured, query_row, query_glass);
    if (query_row < query_glass || query_glass < 0)
        return -1;
    if (flow >= 1)
        return 1;
    else if (flow <= 0)
        return 0;
    else 
        return flow;
}

/*
 * Function: champagneFlow(double to_pour, int row, int col)
 * Usage: double champagneFlow(to_pour, row, col);
 * -----------------------------------------------------
 * return the amount of champagne that has flowed through the position
 * (row, col) after pouring from the top `to_pour`
 * amount of champagne
 */
double champagneFlow(double to_pour, int row, int col){
     if (to_pour <= 0)
        return 0;
    else if (row == 0)
        return to_pour;
    else if (col == 0){
        double flow = champagneFlow(to_pour, row-1, col)-1;
        if (flow < 0) return 0;
        else return flow / 2;
    }
    else if (col == row){
        double flow = champagneFlow(to_pour, row-1, col-1)-1;
        if (flow < 0) return 0;
        else return flow / 2;
    }
    else {
        double flow1 = champagneFlow(to_pour, row-1, col)-1, flow2 = champagneFlow(to_pour, row-1, col-1)-1;
        if (flow1 < 0) flow1 = 0;
        if (flow2 < 0) flow2 = 0;
        return flow1 / 2 + flow2 / 2;
    }
}