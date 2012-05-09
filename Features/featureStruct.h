#ifndef FEATURESTRUCT_H
#define FEATURESTRUCT_H

typedef struct {
	int x,y;
	double *FeatureVector;
} FEATURE;

typedef struct {
	int FeatureVectorLength;
	int Number_of_Features;
	FEATURE *features;
} FEATURES;

#endif