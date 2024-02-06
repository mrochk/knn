#ifndef KNN
#define KNN

#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include "../datapoint/datapoint.h"
#include "knn.h"

KNNClassifier *newKNNClassifier(int k, int nclass) {
    KNNClassifier *knn = (KNNClassifier*) malloc(sizeof(KNNClassifier));
    knn->nclass = nclass, knn->data = NULL, knn->k = k;
    return knn;
}

void freeKNNClassifier(KNNClassifier *knn) {
    for (int i = 0; i < knn->size; i++) { freeDataPoint(knn->data[i]); }
    free(knn->data);
    free(knn);
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

float euclidean(DataPoint *a, DataPoint *b) {
    assert(a->ndim == b->ndim);

    float sum = 0.f;
    
    for (int i = 0; i < a->ndim; i++) {
        float diff = a->coord[i] - b->coord[i];
        sum += diff * diff;
    }

    return sqrt(sum);
}

/* Quicksort */
void sortPointsByDistance(DataPointDistPair *arr, int low, int high) {
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

        if (low  < j) { sortPointsByDistance(arr, low, j); }
        if (high > i) { sortPointsByDistance(arr, i, high); }
    }
}

int classify(KNNClassifier *knn, DataPoint *point) {
    DataPointDistPair distances[knn->size];

    for (int i = 0; i < knn->size; i++) {
        float dist = euclidean(knn->data[i], point);
        DataPointDistPair pair = { .point = knn->data[i], .dist = dist };
        distances[i] = pair;
    }

    sortPointsByDistance(distances, 0, knn->size-1);

    int count[knn->nclass];
    for (int i = 0; i < knn->nclass; i++) { count[i] = 0; }

    for (int k = 0; k < knn->k; k++) { count[distances[k].point->class]++; }

    int max, index;
    max = index = -1;
    for (int k = 0; k < knn->nclass; k++) {
        if (count[k] > max) { max = count[k], index = k; }
    }

    return index;
}

#endif