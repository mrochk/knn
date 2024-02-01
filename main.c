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
    float *coord = (float*) malloc(sizeof(float)*ndim);

    for (int i = 0; i < ndim; i++) { coord[i] = coordinates[i]; }
    p->class = class, p->coord = coord, p->ndim = ndim;

    return p;
}

/* Copy a into b. */
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

typedef struct {
    int            k;
    int         size;
    DataPoint **data;
    int       nclass;
} KNNClassifier;

KNNClassifier *newKNNClassifier(int k, int nclass) {
    KNNClassifier *knn = (KNNClassifier*) malloc(sizeof(KNNClassifier));
    knn->nclass = nclass, knn->data = NULL, knn->k = k;
    return knn;
}

void fit(KNNClassifier* knn, DataPoint** data, int size) {
    assert(knn->k >= size);

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

int randint(int max) { return rand() % (max + 1); }

float randfloat(float max) { 
    return (((float) rand()) * max) / ((float) RAND_MAX); 
}

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

float euclidean(DataPoint *a, DataPoint *b) {
    assert(a->ndim == b->ndim);

    float sum = 0.f;
    
    for (int i = 0; i < a->ndim; i++) {
        float diff = a->coord[i] - b->coord[i];
        sum += diff * diff;
    }

    return sqrt(sum);
}

typedef struct {
    DataPoint *point;
    float dist;
} DataPointDistPair;

void quicksort(DataPointDistPair *arr, int low, int high) {
    if (low < high) {
        float pivot = arr[(low + high) / 2].dist;
        int i = low, j = high;
        DataPointDistPair temp;

        while (i <= j) {
            while (arr[i].dist < pivot) { i++; } 
            while (arr[j].dist > pivot) { j--; } 

            if (i <= j) {
                temp = arr[i];     
                arr[i] = arr[j];
                arr[j] = temp;
                i++;
                j--;
            }
        }

        if (low  < j) { quicksort(arr, low, j); }
        if (high > i) { quicksort(arr, i, high); }
    }
}

int classify(KNNClassifier *knn, DataPoint *point) {
    DataPointDistPair distances[knn->size];

    for (int i = 0; i < knn->size; i++) {
        float dist = euclidean(knn->data[i], point);
        DataPointDistPair pair = {.point = knn->data[i], .dist = dist};
        distances[i] = pair;
    }

    quicksort(distances, 0, knn->size-1);

/*
    for (int i = 0; i < knn->size; i++) {
        printf("%f\n", distances[i].dist);
    }
*/

    int count[knn->nclass];
    for (int i = 0; i < knn->nclass; i++) { count[i] = 0; }

    for (int k = 0; k < knn->k; k++) {
        count[distances[k].point->class]++;
    }

    int max, index;
    max = index = -1;

    for (int k = 0; k < knn->nclass; k++) {
        //printf("%d -> %d\n", k, count[k]);
        if (count[k] > max) {
            max = count[k];
            index = k;
        }
    }

    //printf("\n%.2f\%\n\n", ((float) max) / ((float) knn->k) * 100.f);

    return index;
}

int main(int argc, char **argv) {
    int len = 10;
    int nclass = 5;

    DataPoint **pts = generateRandomData(nclass, 2, 10.f, len);

    KNNClassifier *knn = newKNNClassifier(10, nclass);
    fit(knn, pts, len);

/*
    for (int i = 0; i < knn->size; i++) {
        printf("%d, %d, %f, %f\n", knn->data[i]->ndim, knn->data[i]->class, knn->data[i]->coord[0], knn->data[i]->coord[1]);
    }
*/

    float coord[2] = {0.0, 0.0};
    DataPoint *p = newDataPoint(2, 0, coord);

    int res = classify(knn, p);

    printf("Point classified as: %d.\n", res);

    free(p);

    for (int i = 0; i < knn->size; i++) { freeDataPoint(pts[i]); }
    free(pts);

    freeKNNClassifier(knn);
}