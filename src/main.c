#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <assert.h>

#include "knn/knn.h"
#include "datapoint/datapoint.h"

int   randint(int max)     { return rand() % (max + 1); }
float randfloat(float max) { return (((float) rand()) * max) / ((float) RAND_MAX); }

DataPoint **generateRandomData(int nclass, int ndim, float max, int n) {
    srand(time(NULL));

    DataPoint **data = malloc(sizeof(DataPoint*)*n);

    for (int i = 0; i < n; i++) {
        int class = randint(nclass - 1);
        float coord[ndim];
        for (int j = 0; j < ndim; j++) { coord[j] = randfloat(max); }
        data[i] = newDataPoint(ndim, class, coord);
    }

    return data;
}

int main(int argc, char **argv) {
    int len = 10, nclass = 5;

    DataPoint **pts = generateRandomData(nclass, 2, 10.f, len);

    KNNClassifier *knn = newKNNClassifier(10, nclass);
    fit(knn, pts, len);

    float coord[2] = {randfloat(10.f), randfloat(10.f)};
    DataPoint *p = newDataPoint(2, 0, coord);

    int class = classify(knn, p);

    printf("Point (%.2f, %.2f) classified as: %d.\n", p->coord[0], p->coord[1], class);

    for (int i = 0; i < knn->size; i++) { freeDataPoint(pts[i]); }
    free(pts);
    freeDataPoint(p);
    freeKNNClassifier(knn);
}