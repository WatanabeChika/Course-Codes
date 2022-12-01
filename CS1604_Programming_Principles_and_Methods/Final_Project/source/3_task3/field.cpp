#include <string>
#include <cassert>
#include <iomanip>
#include "field.h"
#include "units.h"
#include "algorithms.h"
using namespace std;


/* Battle field */

// Constructor
Field::Field(int h, int w)
    : units(h, w), terrain(h, w, 1)
{
    assert (h > 0 && h <=20 && w > 0 && w <= 20);
}

// Get the height of the field
int Field::getHeight() const
{
    return units.numRows();
}

// Get the width of the field
int Field::getWidth() const
{
    return units.numCols();
}

// Get the costs of different types in field
int Field::getCosts(int row, int col) const
{
    assert (terrain.inBounds(row, col));

    return terrain[row][col];
}

// Set special terrains
bool Field::setCosts(int row, int col, int costs)
{
    assert (terrain.inBounds(row, col));

    terrain[row][col] = costs;
    return true;
}

// In bounds check
bool Field::inBounds(int row, int col) const
{
    return units.inBounds(row, col);
}

// Get the unit at row and col
Unit* Field::getUnit(int row, int col) const
{
    assert (units.inBounds(row, col));

    return units[row][col];
}

// Set a new unit at row and col
// Return false if the coordinate is already occupied
// or unreachable
bool Field::setUnit(int row, int col, Unit* u)
{
    assert (units.inBounds(row, col));
    if (terrain[row][col] > 990) return false;
    else {
    if (units[row][col] == NULL) {
        u->setCoord(row, col);
        units[row][col] = u;
        return true;
    }
    else
        return false;
    }
}

// Delete a unit at row and col
bool Field::deleteUnit(int row, int col)
{
    assert (units.inBounds(row, col));

    if (units[row][col] == NULL) return false;
    else {
        units[row][col] = NULL;
        return true;
    }
}
// Move a unit to a different coordinate
// Return false if the source does not exist or
// the target coordinate is already occupied or
// unreachable
bool Field::moveUnit(int srow, int scol, int trow, int tcol)
{
    assert (units.inBounds(srow, scol));
    assert (units.inBounds(trow, tcol));

    Unit* start = Field::getUnit(srow, scol);
    Grid<int> tempmove = terrain;
    for (int i = 0; i < terrain.numRows(); i++)
    for (int j = 0; j < terrain.numCols(); j++) {
        if (units[i][j] != NULL) {
            tempmove[i][j] = 1000;
        }
    }
    Grid<bool> can_reach = searchReachable(tempmove, srow, scol, start->getMovPoints());

    if (can_reach[trow][tcol]){
        return Field::deleteUnit(srow, scol);
    }
    return false;
}

// Reclaim all the units
Field::~Field()
{
    for (int i = 0; i < units.numRows(); i++)
    for (int j = 0; j < units.numCols(); j++)
        if (units[i][j] != NULL)
            delete units[i][j];
}
