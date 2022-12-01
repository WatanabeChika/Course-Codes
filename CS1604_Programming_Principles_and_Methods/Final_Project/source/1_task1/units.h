#ifndef UNITS_H_INCLUDED
#define UNITS_H_INCLUDED

#include <string>
#include "field.h"

/* Type of Units */
enum UnitType {FOOTMAN, KNIGHT};

/* Class for units */
class Unit {
public:
    Unit(UnitType u, bool sd, int row, int col);

    // Check which side the unit belongs to
    bool getSide() const;

    // Get the coordinate of the current unit
    int getRow() const;
    int getCol() const;

    // Set the coordinates
    void setCoord(int row, int col);

    // Get your movement point
    int getMovPoints() const;

    // Return a displayable character
    // Depending on the player you may get different results
    std::string getSymbol() const;

private:
    UnitType type;
    bool side;
    int urow, ucol;
};

#endif // UNITS_H_INCLUDED
