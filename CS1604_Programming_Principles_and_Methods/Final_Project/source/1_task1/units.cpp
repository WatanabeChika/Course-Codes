#include "units.h"
#include "field.h"
#include <cassert>
using namespace std;

/* Unit */

Unit::Unit(UnitType t, bool sd, int row, int col)
    : type(t), side(sd), urow(row), ucol(col)
    {}

bool Unit::getSide() const
{
    return side;
}

int Unit::getMovPoints() const
{
    if (type == FOOTMAN) return 3;
    else if (type == KNIGHT) return 5;
    return 0;
}

// Get the coordinate of the current unit
int Unit::getRow() const
{
    return urow;
}

int Unit::getCol() const
{
    return ucol;
}

// Set the coordinates
void Unit::setCoord(int row, int col)
{
    urow = row;
    ucol = col;
}

// Function for footman symbols
string getSymbolFootman(bool side)
{
    if (side)
        return "FT";
    else
        return "ft";
}

// Function for knight symbols
string getSymbolKnight(bool side)
{
    if (side)
        return "KN";
    else
        return "kn";
}

// Get the symbol of the unit
string Unit::getSymbol() const
{
    switch (type) {
    case FOOTMAN:
        return getSymbolFootman(side);
        break;
    case KNIGHT:
        return getSymbolKnight(side);
        break;
    default:
    return "  ";
    }
    assert (false);
}


