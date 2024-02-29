#ifndef MISC_H
#define MISC_H

#include "math.h"
#include "float.h"

float max(float num1, float num2)
{
    return (num1 > num2 ) ? num1 : num2;
}

float maxn(float* numbers, int n)
{
    if (n <= 0) {
        return -1;
    }

    float largest = -FLT_MAX;
    for (int i=0; i<n; ++i) {
        largest = max(largest, numbers[i]);
    }

    return largest;
}

float min(float num1, float num2) 
{
    return (num1 > num2 ) ? num2 : num1;
}

float minn(float* numbers, int n)
{
    if (n <= 0) {
        return -1;
    }

    float smallest = FLT_MAX;
    for (int i=0; i<n; ++i) {
        smallest = min(smallest, numbers[i]);
    }

    return smallest;
}

#endif