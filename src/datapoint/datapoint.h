#ifndef DATAPOINT_H 
#define DATAPOINT_H

#include <stdlib.h>

typedef struct {
    int     ndim;
    int    class;
    float *coord;
} DataPoint;

DataPoint *newDataPoint(int ndim, int class, float* coordinates);

void copyDataPoint(DataPoint *a, DataPoint *b);

void freeDataPoint(DataPoint *p); 

#endif