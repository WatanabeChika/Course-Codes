#include "algorithms.h"
#include <Vector.h>
#include <Grid.h>
#include <cassert>

using namespace std;

// Data structure for storing squares during path finding
struct SearchSquare {
    SearchSquare(int r, int c, int p)
        : row(r), col(c), pts(p)
    {}

    int row;
    int col;
    int pts;

    //friend std::ostream& operator<< (std::ostream& out, const SearchSquare& sqr);
};

/*
ostream& operator<<(ostream& os, const SearchSquare& sqr)
{
    os << "[row = " << sqr.row << ", col = " << sqr.col << ", mvpt = " << sqr.pts << "]";
    return os;
}
*/

// Find the square at (row, col)
SearchSquare* findSquare(Vector<SearchSquare>& squares, int row, int col)
{
    for (int i = 0; i < squares.size(); i++) {
        SearchSquare& sqr = squares[i];
        if (sqr.row == row && sqr.col == col)
            return &sqr;
    }
    return NULL;
}

// Find the square with the maximal movement points
int findMaxSquare(const Vector<SearchSquare>& squares)
{
    assert (squares.size() > 0);

    int maxIndex = 0;
    for (int i = 1; i < squares.size(); i++) {
        if (squares[i].pts > squares[maxIndex].pts)
            maxIndex = i;
    }
    return maxIndex;
}

// Update a reachable square if possible
void updateSquare(Grid<bool>& reachable, const Grid<int>& costs,
                  int row, int col, int mvPts,
                  Vector<SearchSquare>& unvisited)
{
    // No need to check a unattainable position
    // or a square already reachable
    if (!costs.inBounds(row, col)
        || reachable[row][col])
        return;

    // Adjust movement points
    int cost = costs[row][col];
    mvPts -= cost;

    // Update the movement point of unvisited squares
    if (mvPts >= 0) {
        SearchSquare* sqr = findSquare(unvisited, row, col);
        // Update the movement points for unvisited squares
        if (sqr == NULL)
            unvisited.add(SearchSquare(row, col, mvPts));
        else if (mvPts > sqr->pts)
            sqr->pts = mvPts;
    }
}

// Search for reachable points
Grid<bool> searchReachable(const Grid<int>& costs, int row, int col, int mvPts)
{
    assert(costs.inBounds(row,col));

    Grid<bool> reachable(costs.numRows(), costs.numCols());

    Vector<SearchSquare> unvisited;
    if (mvPts >= 0)
        unvisited.add(SearchSquare(row, col, mvPts));

    while (!unvisited.isEmpty()) {
        int maxIndex = findMaxSquare(unvisited);
        SearchSquare pt = unvisited[maxIndex];
        unvisited.remove(maxIndex);


        int rMvPts = pt.pts;
        int curRow = pt.row;
        int curCol = pt.col;
        reachable[curRow][curCol] = true;

        // Search four directions
        updateSquare(reachable, costs, curRow-1, curCol, rMvPts, unvisited); // North
        updateSquare(reachable, costs, curRow+1, curCol, rMvPts, unvisited); // South
        updateSquare(reachable, costs, curRow, curCol+1, rMvPts, unvisited); // East
        updateSquare(reachable, costs, curRow, curCol-1, rMvPts, unvisited); // West
    }

    return reachable;
}
