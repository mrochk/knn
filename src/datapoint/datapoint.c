#ifndef DATAPOINT 
#define DATAPOINT 

#include <stdlib.h>
#include "datapoint.h"

DataPoint *newDataPoint(int ndim, int class, float* coordinates) {
    DataPoint *p = (DataPoint*) malloc(sizeof(DataPoint));
    float *coord = (float*) malloc(sizeof(float)*ndim);

    for (int i = 0; i < ndim; i++) { coord[i] = coordinates[i]; }
    p->class = class, p->coord = coord, p->ndim = ndim;

    return p;
}

void copyDataPoint(DataPoint *a, DataPoint *b) {
    float *coord = (float*) malloc(sizeof(float) * a->ndim);

    for (int i = 0; i < a->ndim; i++) { coord[i] = a->coord[i]; }
    b->coord = coord;

    b->class = a->class, b->ndim = a->ndim;
}

void freeDataPoint(DataPoint *p) {
    free(p->coord);
    free(p);
} 

#endif