#ifndef KNN_H
#define KNN_H

#include "../datapoint/datapoint.h"

typedef struct {
    int            k;
    int         size;
    DataPoint **data;
    int       nclass;
} KNNClassifier;

typedef struct {
    DataPoint *point;
    float       dist;
} DataPointDistPair;

KNNClassifier *newKNNClassifier(int, int);

void freeKNNClassifier(KNNClassifier*);

void fit(KNNClassifier*, DataPoint**, int);

float euclidean(DataPoint*, DataPoint*);

void sortPointsByDistance(DataPointDistPair*, int, int); 

int classify(KNNClassifier*, DataPoint*);

#endif