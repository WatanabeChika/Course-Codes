#include <sstream>
#include <iomanip>
#include <cassert>
#include "engine.h"
#include "actions.h"
#include "units.h"
#include "algorithms.h"
using namespace std;

// Add units
bool addFootman(Field& field, bool sd, int row, int col)
{
    if (field.getUnit(row, col) != NULL)
        return false;

    Unit* ft = new Unit(FOOTMAN, sd, row, col);
    return field.setUnit(row, col, ft);
}


bool checkBounds(ostream& os, const Field& field, int row, int col);
bool readRowCol(const string& str, int& row, int& col);
bool queryRowCol(istream& is, ostream& os, int& row, int& col);
bool performActions(ostream& os, istream&is, Field& field, Unit* u);
bool checkWinning(ostream& os, Field& field, bool side);

// Main loop for playing the game
void play(Field& field, istream& is, ostream& os)
{
    bool side = true;
    int numTurns = 1;
    while (is)
    {
        os << endl;
        // Print the new map
        displayField(os, field);

        // Winning check
        if (checkWinning(os, field, side))
            break;

        string player;
        if (side)
            player = "Player A";
        else
            player = "Player B";


        os << "Turn " << numTurns << " (" << player << ")" << endl;

        // Choose the source coordination
        os << "Please select a unit: ";
        int srow, scol;
        if (!queryRowCol(is, os, srow, scol))
            continue;
        if (!checkBounds(os, field, srow, scol))
            continue;

        // Select a unit
        Unit* u;
        SelectUResult sres = selectUnit(field, side, srow, scol, u);
        if (sres == SELECTU_NO_UNIT) {
            os << "No unit at (" << srow << ", " << scol << ")!" << endl;
            continue;
        } else if (sres == SELECTU_WRONG_SIDE) {
            os << "Unit at (" << srow << ", " << scol << ") is an enemy!" << endl;
            continue;
        }

        // Perform actions
        if (!performActions(os, is, field, u))
            continue;


        side = !side;
        numTurns++;
    }
}

// Check if (row, col) is in bounds
bool checkBounds(ostream& os, const Field& field, int row, int col)
{
    if (!field.inBounds(row, col)) {
        os << "(" << row << ", " << col << ") is out of bound" << endl;
        return false;
    }
    return true;
}

// Read row and column from the string
bool readRowCol(const string& str, int& row, int& col)
{
    stringstream ss(str);
    ss >> row >> col;
    if (ss)
        return true;
    else
        return false;
}

// Query row and column from input stream
bool queryRowCol(istream& is, ostream& os, int& row, int& col)
{
    string line;
    if (!getline(is, line)) {
        os << "Read line failed!" << endl;
        return false;
    }

    if (!readRowCol(line, row, col)) {
        os << "Incorrect input! "<< endl;
        return false;
    }

    return true;
}

// Display actions
void displayActions(ostream& os, const Vector<Action>& acts)
{
    for (int i = 0; i < acts.size(); i++) {
        os << i+1 << ".";
        switch (acts[i]) {
        case MOVE:
            os << "Move ";
            break;

        case ATTACK:
            os << "Attack ";
            break;

        case SKIP:
            os << "Skip ";
            break;

        default:
            assert(false);
        }
    }
    os << endl;
}

// Display a menu and query for the actions
bool queryAction(ostream& os, istream& is, Vector<Action>& acts, Action& res)
{
    int i;
    string line;
    getline(is, line);
    stringstream ss(line);
    if (!ss) return false;
    ss >> i;
    i--;
    if (i < 0 || i >= acts.size())
        return false;
    else {
        res = acts[i];
        return true;
    }
}

// Collection actions that can be performed by a unit
Vector<Action> collectActions(Unit* u)
{
    Vector<Action> acts = getActions(u);

    return acts;
}

// Check if there is only skip action
bool isAllSkip(const Vector<Action>& acts)
{
    for (int i = 0; i < acts.size(); i++)
        if (acts[i] != SKIP)
            return false;
    return true;
}

// Convert field to costs
Grid<int> getFieldCosts(const Field& field)
{
    int h = field.getHeight();
    int w = field.getWidth();
    Grid<int> costs(h, w);

    for (int i = 0; i < h; i++)
    for (int j = 0; j < w; j++) {
        costs[i][j] = field.getCosts(i,j);
        if (field.getUnit(i,j) != NULL) {
            costs[i][j] = 1000;
        }
    }

    return costs;
}

// Perform the move action
bool performMove(ostream& os, istream& is, Field& field, Unit* u)
{
    // Display the reachable points
    Grid<bool> grd =
        searchReachable(getFieldCosts(field), u->getRow(), u->getCol(), u->getMovPoints());
    os << endl;
    displayField(os, field, grd);

    // Choose the target coordinate
    os << "Please enter your destination: ";
    int trow, tcol;
    if (!queryRowCol(is, os, trow, tcol))
        return false;

    // Move the target
    if (!checkBounds(os, field, trow, tcol)) return false;
    return moveUnit(field, u, trow, tcol);
}

// Display the menu and query for the attack command
bool queryAttack(ostream& os, istream& is, int& choice) 
{
    os << "1.UP 2.DOWN 3.LEFT 4.RIGHT" << endl
       << "Please enter the direction: " ;

    int i;
    string line;
    getline(is, line);
    stringstream ss(line);
    ss >> i;
    i--;
    if (i < 0 || i >= 4) {
        os << "Incorrect input! "<< endl;  
        return false;
    }
    else {
        choice = i;
        return true;
    }
}

// Perform the attack action
bool performAttack(ostream& os, Field& field, Unit* u, int& choice)
{
    int trow, tcol;
    switch(choice){
    case 0:
        trow = u->getRow()-1;
        tcol = u->getCol();
        break;
    case 1:
        trow = u->getRow()+1;
        tcol = u->getCol();
        break;
    case 2:
        trow = u->getRow();
        tcol = u->getCol()-1;
        break;
    case 3:
        trow = u->getRow();
        tcol = u->getCol()+1;
        break;
    default:
        return false;
    }
    if (!checkBounds(os, field, trow, tcol)) return false;
    return attackUnit(field, u, trow, tcol);
}

// Perform actions
bool performActions(ostream& os, istream& is, Field& field, Unit* u)
{
    Vector<Action> acts = collectActions(u);
    if (isAllSkip(acts))
        return true;

    displayActions(os, acts);
    os << "Select your action: ";
    Action act;
    if (!queryAction(os, is, acts, act)) {
        os << "Invalid action!" << endl;
        return false;
    }

    switch(act) {
    case MOVE:
        if (!performMove(os, is, field, u)) return false;
        else {
            os << endl;
            u->setMoval();
            displayField(os, field);
            if (!performActions(os, is, field, u)) return false;
            else {
                u->setMoval();
                return true;
            }
        }

    case SKIP:
        return true;

    case ATTACK:
        int choice;
        if (!queryAttack(os, is, choice)) 
            return performActions(os, is, field, u);
        else 
            return performAttack(os, field, u, choice);
            
    default:
        os << "(Action not implemented yet)" << endl;
        return false;
    }

    //assert (false);
}

// Print the horizontal line
void printHLine(ostream& os, int n)
{
    os << "  ";
    for (int i = 0; i < n; i++)
        os << "+--";
    os << "+" << endl;
}

// Display the field on the out stream os
void displayField(ostream& os, const Field& field, const Grid<bool>& grd)
{
    int height = field.getHeight();
    int width = field.getWidth();

    // Print the x coordinates
    os << "  ";
    for (int i = 0; i < width; i++)
        os << ' ' << setw(2) << i;
    os << endl;

    printHLine(os, width);
    for (int i = 0; i < height; i++) {
        os << setw(2) << i;
        for (int j = 0; j < width; j++) {
            os << '|';
            Unit* u = field.getUnit(i,j);
            int spe_terrain = getFieldCosts(field)[i][j];
            if (u != NULL)
                os << u->getSymbol();
            else if (grd.inBounds(i,j) && grd[i][j])
                os << ". ";
            else if (spe_terrain == 2) 
                os << "$ ";
            else if (spe_terrain == 999) 
                os << "~~";
            else if (spe_terrain == 998) 
                os << "/\\";
            else {
                os << "  ";
            }
        }
        os << '|' << endl;
        printHLine(os, width);
    }
    os << endl;
}

// Check which player is winning
bool checkWinning(ostream& os, Field& field, bool side)
{
    int h = field.getHeight(), w = field.getWidth();
    for (int i = 0; i < h; ++i){
    for (int j = 0; j < w; ++j){
        Unit* temp = field.getUnit(i, j);
        if (temp != NULL && temp->getSide() == side)
            return false;
    }
    }

    if (!side)
        os << "Congratulations! Player A has won!" << endl;
    else
        os << "Congratulations! Player B has won!" << endl;
    return true;
}

// Load a map
Field* loadMap(std::istream& is)
{
    int M, N, NT, NU;
    int row, col;
    string T, S, U;
    is >> M >> N >> NT >> NU;
    Field* map= new Field(M, N);

    for (int i = 0; i < NT; ++i) {
        is >> row >> col >> T;
        if (T == "M") map->setCosts(row, col, 998);
        else if (T == "W") map->setCosts(row, col, 999);
        else if (T == "F") map->setCosts(row, col, 2);
    }
    for (int j = 0; j < NU; ++j) {
        is >> row >> col >> S >> U;
        if (S == "A"){
            if (U == "FT"){
                Unit* temp = new Unit(FOOTMAN, true, row, col);
                map->setUnit(row, col, temp); 
            }
            else if (U == "KN"){
                Unit* temp = new Unit(KNIGHT, true, row, col);
                map->setUnit(row, col, temp); 
            }
        }
        else if (S == "B"){
            if (U == "FT"){
                Unit* temp = new Unit(FOOTMAN, false, row, col);
                map->setUnit(row, col, temp); 
            }
            else if (U == "KN"){
                Unit* temp = new Unit(KNIGHT, false, row, col);
                map->setUnit(row, col, temp); 
            }
        }
    }
    getline(is,T); 
    return map;
}