#include <iostream>
#include <string>
#include "Date.h"

string Mon_from_Int(int m);
int judge_month_days(int month, int year);
int more_than(Date d, int year, int month, int day);

/*
Default Constructor
*/
Date::Date()
    : year(1900), month(1), day(1)
{}

/*
Constructor using date
*/
Date::Date(int y,int m,int d)
    : year(y), month(m), day(d)
{
   Date::error();
}

/*
Getter: xxxxxxxxxxxx
*/

// Get the year
int Date::getYear() const
{
    return year;
}
// Get the month
int Date::getMonth() const
{
    return month;
}
// Get the day
int Date::getDay() const
{
    return day;
}

//Return as a string
string Date::toString() const
{
    string mon = Mon_from_Int(month);
    string yea = to_string(year);
    string datee = to_string(day);
    return datee + '-' + mon + '-' + yea;
}

/*
Setter: xxxxxxxxxxxx
*/

// add n days to the date
void Date::add(int n)
{
    int month_days;
    while (n > 0) {
        if (Date::error()) return;
        month_days = judge_month_days(month, year);
        if (day < month_days ) {
            ++day; --n;
        }
        else {
            if (month < 12){
                ++month; --n;
                day = 1;
            }
            else {
                ++year; --n;
                day = 1; month = 1;
            }
        }
    }
}

// sub n days to the date
void Date::sub(int n)
{
    int month_days;
    while (n > 0) {
        if (Date::error()) return;
        month_days = judge_month_days(month-1, year);
        if (day > 1 ) {
            --day; --n;
        }
        else {
            if (month > 1){
                --month; --n;
                day = month_days;
            }
            else {
                --year; --n;
                day = 31; month = 12;
            }
        }
    }
}

// operation: calculate how many days d and self differs.
int Date::diff(Date d) const
{
    int n = 0;
    if (year == d.getYear() && month == d.getMonth() && day == d.getDay()) return 0;
    int more = more_than(d, year, month, day);
    Date temp = d;
    if (more == 1) {
        while (more == 1) {
            ++n;
            temp.add(1);
            if (year == temp.getYear() && month == temp.getMonth() && day == temp.getDay()) return n;
        }
    }
    else {
        while (more == 0) {
            ++n;
            temp.sub(1);
            if (year == temp.getYear() && month == temp.getMonth() && day == temp.getDay()) return n;
        }
    }
    return 0;
}

// situations to output error
bool Date::error() 
{
    if (year < 1900 || year > 2900) {
        std::cout << "Error" << std::endl;
        year = 1900; month = 1; day = 1;
        return true;
    }
    int maxdays = judge_month_days(month, year);
    if (day > maxdays) {
        std::cout << "Error" << std::endl;
        year = 1900; month = 1; day = 1;
        return true;
    }
    return false;
}

/* a helper function which may be useful to you :) */
string Mon_from_Int(int m) {
    switch (m){
    case 1:
        return "Jan";
        break;
    case 2:
        return "Feb";
        break;
    case 3:
        return "Mar";
        break;
    case 4:
        return "Apr";
        break;
    case 5:
        return "May";
        break;
    case 6:
        return "Jun";
        break;
    case 7:
        return "Jul";
        break;
    case 8:
        return "Aug";
        break;
    case 9:
        return "Sep";
        break;
    case 10:
        return "Oct";
        break;
    case 11:
        return "Nov";
        break;
    case 12:
        return "Dec";
        break;
    default:
        return "";
    }
}

// judge how many days in this month
int judge_month_days(int month, int year) {
    bool leap = false;
    if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) leap = true;
    switch (month) {
        case 2: 
            if(leap) return 29;
            else return 28;
            break;
        case 4: case 6: case 9: case 11:
            return 30;
            break;
        default:
            return 31;
    }
}

// judge d and self which is later in time
int more_than(Date d, int year, int month, int day) {
    if (year > d.getYear()) return 1;
    else if (year < d.getYear()) return 0;
    else {
        if (month > d.getMonth()) return 1;
        else if (month < d.getMonth()) return 0;
        else {
            if (day > d.getDay()) return 1;
            else if (day < d.getDay()) return 0;
            else return -1;
        }
    }
}

