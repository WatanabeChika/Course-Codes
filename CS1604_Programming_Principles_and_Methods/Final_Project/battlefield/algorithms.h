#ifndef ALGORITHMS_H_INCLUDED
#define ALGORITHMS_H_INCLUDED

/**** Algorithms for the game ****/
#include <Grid.h>
#include "field.h"

/** Path finding algorithm **/

// Given movement points (pts), calculate
// which squares can be reached starting from (row, col)
Grid<bool> searchReachable(const Grid<int>& costs, int row, int col, int pts);

#endif // ALGORITHMS_H_INCLUDED
