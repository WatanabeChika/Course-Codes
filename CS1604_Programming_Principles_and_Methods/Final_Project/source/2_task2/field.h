#ifndef FIELD_H_INCLUDED
#define FIELD_H_INCLUDED

#include <ostream>
#include <Grid.h>

// Forward declaration of the class of units
class Unit;


/* Battle field */
class Field {
public:
    // Constructor
    Field(int h, int w);

    // Get the height and width of the field
    int getHeight() const;
    int getWidth() const;
    
    // Get the costs of different types in field
    int getCosts(int row, int col) const;

    // Set special terrains
    bool setCosts(int row, int col, int costs);

    // Check if the row and column is in bounds
    bool inBounds(int row, int col) const;

    // Display the field on the out stream os
    void display(std::ostream& os, const Grid<bool>& grd = Grid<bool>()) const;

    // Get the unit at row and col
    Unit* getUnit(int row, int col) const;

    // Set a new unit at row and col
    // Return false if the coordinate is already occupied
    bool setUnit(int row, int col, Unit* u);

    // Delete a unit at row and col
    bool deleteUnit(int row, int col);

    // Move a unit to a different coordinate
    // Return false if the target coordinate is already occupied
    bool moveUnit(int srow, int scol, int trow, int tcol);

    // Destructor
    ~Field();

private:
    // Store the units
    Grid<Unit*> units;
    // Store the terrains
    Grid<int> terrain;
};

#endif // FIELD_H_INCLUDED
