## Description

The dates for certain holidays are described in the form of "the `n`th weekday (or weekend day) of a certain month". For example, the Mother's Day is the second Sunday of May every year.

Given positive integers `a` (`1 <= a <= 12`), `b`, `c` (`1 <= c <= 7`, representing Monday-Sunday), and `y1,y2 (1850 ≤ y1, y2 ≤ 2050)`, we would like to find out "the `b`th weekday or weekend day represented by `c` in the `a`th month", for every year starting from `y1` to `y2 ` (both included).

Tip: You will need to determine if a certain year is a leap year. The rules are as follows: a year that is divisible by 400 is a leap year. Otherwise, if a year is divisible by 4 and NOT divisible by 100, it is also a leap year. For example, 1900 is not a leap year, while 2000 is.

For your convenience, January 1, 1850 is a Tuesday.

## Input Format

One line, five integers, `a, b, c, y1, y2`.

`c`=1, 2, …, 6, 7.

## Output Format

For each year between `y1` and `y2` (including `y1` and `y2`), output the desired dates in a sequential order, separated by new lines.

For a given year, if "the `b`th weekday or weekend day represented by `c` in the `a`th month" exists, the output should be in the format "yyyy / mm / dd", that is, four digit year, two digit month and two digit date, separated by a forward slash `"/"` (Note that zero should be filled in if there are not enough digits). Other wise, output `none`.

## Sample Input

```
5 2 7 2014 2015
```

## Sample Output

```
2014/05/11
2015/05/10
```

## Input Range

1 ≤ `a` ≤ 12，1 ≤ `b` ≤ 5，1 ≤ `c `≤ 7，1850 ≤ `y1, y2` ≤ 2050。