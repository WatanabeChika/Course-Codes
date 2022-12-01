#include "units.h"
#include "field.h"
#include <cassert>
using namespace std;

/* Unit */

Unit::Unit(UnitType t, bool sd, int row, int col)
    : type(t), side(sd), urow(row), ucol(col), moval(false)
    {}

// Check which side the unit belongs to
bool Unit::getSide() const
{
    return side;
}

// Get the type of the unit
UnitType Unit::getType() const
{
    return type;
}

// Get your movement point
int Unit::getMovPoints() const
{
    if (type == FOOTMAN || type == ARCHER) return 3;
    else if (type == KNIGHT) return 5;
    return 0;
}

// Get and set the unit moval condition
bool Unit::getMoval() const
{
    return moval;
}

void Unit::setMoval()
{
    moval = !moval;
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

// Function for archer symbols
string getSymbolArcher(bool side)
{
    if (side)
        return "AR";
    else
        return "ar";
}

// Get the symbol of the unit
string Unit::getSymbol() const
{
    switch (type) {
    case FOOTMAN:
        return getSymbolFootman(side);
    case KNIGHT:
        return getSymbolKnight(side);
    case ARCHER:
        return getSymbolArcher(side);
    default:
    return "  ";
    }
    assert (false);
}


