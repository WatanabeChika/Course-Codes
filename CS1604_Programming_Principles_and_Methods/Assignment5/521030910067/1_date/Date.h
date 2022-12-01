
#ifndef DATE_H_INCLUDED
#define DATE_H_INCLUDED

#include <string>
using namespace std;

class Date{
public:
    //Constructor
    Date();
    Date(int y,int m,int d);
    //getter
    int getDay() const;
    int getMonth() const;
    int getYear() const;
    string toString() const;
    //setter
    void add(int n);
    void sub(int n);
    //operation
    int diff(Date d) const;
private:
    // Choose your implementation of internal store for ADT "Date"
    int year, month, day;
    //Error judger
    bool error();
};

/*
If you want to test a helper function defined in Date.cpp in main.cpp,
You can add the signature of the function here.
*/

#endif // DATE_H_INCLUDED
