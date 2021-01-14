/*
  ==============================================================================

    CommonFunctions.h
    Created: 17 Jun 2020 9:54:26pm
    Author:  Vincenzo Sicurella

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

const double PI = 3.14159265359f;

template <class T>
struct PointPair
{
	Point<T> x, y;

	PointPair() {}
	PointPair(Point<T> xIn, Point<T> yIn)
	{
		x = xIn;
		y = yIn;
	};
	PointPair(T x1, T y1, T x2, T y2)
	{
		x = Point<T>(x1, y1);
		y = Point<T>(x2, y2);
	}
};

static int modulo(int numIn, int mod)
{
	int val = 0;

	if (mod != 0)
		val = ((numIn % mod) + mod) % mod;

	return val;
}

static double roundToNDecimals(double numIn, int numDecimals)
{
	int f = pow(10, numDecimals);
	return round(numIn * f) / f;
}

static float roundToNDecimals(float numIn, int numDecimals)
{
	int f = pow(10, numDecimals);
	return roundf(numIn * f) / f;
}

static int getGCD(int num1, int num2)
{
	if (num1 == 0 || num2 == 0)
		return num1 + num2;
	else if (num1 == 1 || num2 == 1)
		return 1;
	else if (num1 == num2)
		return num1;

	return getGCD(num2, num1 % num2);
}

static int getLCM(int num1, int num2)
{
	if (num1 == 0 || num2 == 0)
		return 0;

	long gcd = getGCD(num1, num2);
	return (int)(jmax(num1, num2) / gcd * (jmin(num1, num2)));
}

static Array<int> getCoprimes(int numIn)
{
    Array<int> coprimes = {1};
    int m, d, t;
    for (int i = 2; i < numIn - 1; i++)
    {
        m = numIn;
        d = i;
        while (d > 1)
        {
            m %= d;
            t = d;
            d = m;
            m = t;
        }
        
		if (d > 0)
		{
			coprimes.add(i);
		}
    }
    
    coprimes.add(numIn-1);

    return coprimes;
}

static Array<int> getFactors(int numIn)
{
    Array<int> factors;
    
    for (int i = 1; i < numIn / 2; i++)
    {
        if (numIn % i == 0)
            factors.add(i);
    }
    
    return factors;
}

static Array<int> getContinuedFraction(double num, int maxDepth=20, double round0Thresh=10e-8)
{
    Array<int> cf;
    double f = num;
    double nextF;

    for (int i = 0; i < maxDepth; i++)
    {
        cf.add((int)f);
        nextF = f - cf[i];
        if (nextF > round0Thresh)
            f = 1.0 / nextF;
        else
            break;
    }

    return cf;
}

template <class T>
static String arrayToString(Array<T>& arrayIn, String name = "", char arrayStart=0, char arrayEnd=0)
{
	String strOut;

	if (name != "")
		strOut += name + ": ";
	if (arrayStart != 0)
		strOut += arrayStart;

	for (int i = 0; i < arrayIn.size(); i++)
	{
		strOut += String(arrayIn[i]);
		if (i < arrayIn.size() - 1)
			strOut += ',';
	}

	if (arrayEnd != 0)
		strOut += arrayEnd;

	return strOut;
}
