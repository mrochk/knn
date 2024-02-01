#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <assert.h>

typedef struct {
    int     ndim;
    int    class;
    float *coord;
} DataPoint;

DataPoint *newDataPoint(int ndim, int class, float* coordinates) {
    DataPoint *p = (DataPoint*) malloc(sizeof(DataPoint));
    float *coord = (float*)     malloc(sizeof(float)*ndim);

    for (int i = 0; i < ndim; i++) { coord[i] = coordinates[i]; }

    p->ndim  = ndim;
    p->class = class;
    p->coord = coord;

    return p;
}

/* Copy a into b. */
void copyDataPoint(DataPoint *a, DataPoint *b) {
    b->class = a->class;
    b->ndim  = a->ndim;

    float *coord = (float*) malloc(sizeof(float) * a->ndim);
    for (int i = 0; i < a->ndim; i++) { coord[i] = a->coord[i]; }
    b->coord = coord;
}

void freeDataPoint(DataPoint *p) {
    free(p->coord);
    free(p);
} 

typedef struct {
    int            k;
    int         size;
    DataPoint **data;
    int       nclass;
} KNNClassifier;

KNNClassifier *newKNNClassifier(int k, int nclass) {
    KNNClassifier *knn = (KNNClassifier*) malloc(sizeof(KNNClassifier));
    knn->nclass = nclass;
    knn->data   = NULL;
    knn->k = k;
    return knn;
}

void fit(KNNClassifier* knn, DataPoint** data, int size) {
    DataPoint **d = (DataPoint**) malloc(sizeof(DataPoint*)*size);
    for (int i = 0; i < size; i++) { 
        d[i] = (DataPoint*) malloc(sizeof(DataPoint)); 
        copyDataPoint(data[i], d[i]);
    }
    knn->data = d;
    knn->size = size;
}

void freeKNNClassifier(KNNClassifier *knn) {
    for (int i = 0; i < knn->size; i++) { freeDataPoint(knn->data[i]); }
    free(knn->data);
    free(knn);
}

int main(int argc, char **argv) {
    float c[2] = {2.f, 3.f};
    DataPoint *p1 = newDataPoint(2, 0, c);
    DataPoint *p2 = newDataPoint(2, 0, c);
    DataPoint *p3 = newDataPoint(2, 0, c);

    DataPoint *pts[] = {p1, p2, p3};

    KNNClassifier *knn = newKNNClassifier(5, 1);

    fit(knn, pts, 3);

    //for (int i = 0; i < knn->size; i++) {
        //printf("%d, %d\n", knn->data[i]->ndim, knn->data[i]->ndim);
    //}

    freeDataPoint(p1);
    freeDataPoint(p2);
    freeDataPoint(p3);

    freeKNNClassifier(knn);
}