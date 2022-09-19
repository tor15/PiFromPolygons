#pragma once
#include "resource.h"   // needed for global variables

// polygon function is in wingdi.h
// https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-polygon

#include <objidl.h>
#include <stdio.h>
#include <math.h>       
#include <string>
#include <iostream>
#include <fstream>

//#include <gdiplusgraphics.h>    // for graphic.DrawPolygon()
// https://learn.microsoft.com/en-us/windows/win32/api/gdiplusgraphics/nf-gdiplusgraphics-graphics-drawpolygon(constpen_constpointf_int)

#define PI 3.14159265
#define RADIUS 100

// Global variables
int TEXTPOSITION{ -10 };

// declarations of functions for piGraphicallyWin2022.cpp

void plotThePolygon(HDC hdc, double numSides);      // plots a polygon of numSides in window hdc
void displayDouble(HDC hdc, double D);              // takes a double d and displays it in the window hdc
double findSideLength(double numSides);             // returns the length of a side of a polygon with numSides
double calculatePi(double numSides);                // return the value of pi using a polygon of sides numSides
void displayPiValue(HDC hdc, double numSides);      // display value of pi in window
void displayNumSides(HDC hdc, double numSides);     // display statement in window telling using the number of sides in current polygon

//
// functions
//

// display statement in window telling using the number of sides in current polygon
void displayNumSides(HDC hdc, double numSides)
{
    // create string statement to be printed to window
    // will print numSides in the statement
    std::string strStatement = "Value of pi from polygon with ";
    strStatement += std::to_string(int(numSides));          // add num of sides to the string
    strStatement += " sides is: ";                              // add comment to string
    std::wstring tempStr2;                                      // temp wide string
    tempStr2 = std::wstring(strStatement.begin(), strStatement.end());  // copy string to wide string
    LPCWSTR wideString2;                    // create temp LPCWSTR for conversion
    wideString2 = tempStr2.c_str();         // copying strStatement to LPCWSTR for TextOut function

    // output the text to the window using wideString2
    TextOut(hdc, 10, TEXTPOSITION += 20, wideString2, _tcslen(wideString2));
}

void displayPiValue(HDC hdc, double numSides)
{
    // calculate the value of pi for number of sides in the polygon
    double piValue;
    piValue = calculatePi(numSides);

    // convert piValue double into LPCWSTR format for TextOut function
    // create temp wide string and copy strStatement to it
    std::wstring tempStr;
    std::string str = std::to_string(piValue);      // convert pi number to string - only 6 dec places
    tempStr = std::wstring(str.begin(), str.end());
    LPCWSTR wideString;                 // create temp LPCWSTR for conversion
    wideString = tempStr.c_str();       // copying piValue to LPCWSTR for TextOut function

    TextOut(hdc, 280, TEXTPOSITION, wideString, _tcslen(wideString));   // display pi to window
}



// converts double d to string and displays in window
void displayDouble(HDC hdc, double d)
{
    std::string str;        // create string
    std::wstring temp;      // create wide string
    LPCWSTR wideString;     // create LPCWSTR for TextOut
    str = std::to_string(d);      // convert number to string - only 6 dec places
    temp = std::wstring(str.begin(), str.end());
    wideString = temp.c_str();          // convert to TextOut form

    TextOut(hdc, 100, 25, wideString, _tcslen(wideString));
}

/// plot polygon of d sides in windwon HDC
void plotThePolygon(HDC hdc, double numSides)
{
    // centre of polygon is (400,300)
    struct doublePoints {
        double x;
        double y;
    };

    doublePoints centrePoint{ 600, 300 };

    // create the point to hold all the poinst
    // cannot create dynamically - must be const - set to 100
    POINT pt[100];

    // Set the first point to the top point above the centre
    // exactly RADIUS above the centre
    pt[0].x = centrePoint.x;
    pt[0].y = centrePoint.y - RADIUS;

    // how many angles the point is from the first point
    // angle number is 1 less than point number
    double angleNum = 1;    

    // angle from first point - from horizontal going down
    // this is the exterior angle of a polygon
    // 360 (2pi) divided by numSides
    double extAngle = 2 * PI / numSides;
    double angleAtCentre = 2 * PI / numSides;

    // double angle = (PI/2) - (PI - angleAtCentre)/2
    // the above simplifies to this
    double angle = angleAtCentre / 2.0;
    double sideLength = findSideLength(numSides);   // get the length of the side of the polygon

    // finding coordinates for points on the right side only
    // left side points will be found from right side points
    // iterations is the number of right side points to be found
    // if even number of sides, find the bottom point independently
    int iterations = 0;
    if (int(numSides) % 2 == 0)
        iterations = numSides / 2;          // if even
    else
        iterations = (numSides + 1) / 2;    // if odd

    for (int i = 1; i < iterations; i++)
    {
        // x and y distance from last point
        double y = sin(angle + (i - 1) * extAngle) * sideLength;
        double x = cos(angle + (i - 1) * extAngle) * sideLength;

        // set points on the right side
        pt[i].x = pt[i - 1].x + x;
        pt[i].y = pt[i - 1].y + y;

        // symmetrical, so set points on the left side
        pt[int(numSides) - i].x = centrePoint.x - (pt[i].x - centrePoint.x);

        // both y values the same
        pt[int(numSides) - i].y = pt[i].y;
    }
    
    // if even set the vertically opposite bottom point
    // this will not have been set yet
    if (int(numSides) % 2 == 0)
    {
        pt[int(numSides) / 2].x = pt[0].x;                      // x coord some as first point
        pt[int(numSides) / 2].y = pt[0].y + 2 * RADIUS;         // y coord 2 times radius above first point
    }
    
    Polygon(hdc, pt, numSides);             // draw the polygon in the window
}

// return the value of pi using a polygon of sides numSides
double calculatePi(double numSides)
{
    // perimeter of polygon is side length of polygon times the number of sides
    double perimeter = findSideLength(numSides) * numSides;

    // pi is circumference divided by diameter (2 times radius)
    // return value of pi
    return perimeter / (2 * RADIUS);
}

// return the length of a side of a polygon of numSides
double findSideLength(double numSides)
{
    // double angle = 360 / numSides;
    // using cosine rule to find the length of the side of the polygon
    // angle has to be converted to radians - angle is 360 degrees (2pi) divided by number of sides
    double lengthSquared = RADIUS * RADIUS + RADIUS * RADIUS - 2 * RADIUS * RADIUS * cos((2 * PI) / numSides);
    return std::sqrt(lengthSquared);
}