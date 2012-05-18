#include <Features/matchFeatures.h>
#include <math.h>

COORDS* findNearestNeighbor (FEATURES TrainFeature,FEATURES TestFeature) {
	int FeatureVectorLength = TrainFeature.FeatureVectorLength; // == TestFeature.FeatureVectorLength
	int numTrainFeatures = TrainFeature.Number_of_Features;
	int numTestFeatures = TestFeature.Number_of_Features;

	COORDS *coordinates = new COORDS[2];
	coordinates[0].Number_of_Coordinates = numTestFeatures; // Number of Corresponding Training object Positions
	coordinates[0].x = new double [numTestFeatures];
	coordinates[0].y = new double [numTestFeatures];
	coordinates[1].Number_of_Coordinates = numTestFeatures; // Number of Testing object Positions
	coordinates[1].x = new double [numTestFeatures];
	coordinates[1].y = new double [numTestFeatures];


	double Distance,minDistance;
	for ( int TestIndex = 0; TestIndex < numTestFeatures; TestIndex++) {

		coordinates[1].x[TestIndex] = TestFeature.features[TestIndex].x;
		coordinates[1].y[TestIndex] = TestFeature.features[TestIndex].y;
		minDistance = 99999999999;
		
		for ( int TrainIndex = 0; TrainIndex < numTrainFeatures; TrainIndex++) {
			
			Distance = 0;
			for (int vectorIndex = 0; vectorIndex < FeatureVectorLength; vectorIndex++)
				Distance += pow (TrainFeature.features[TrainIndex].FeatureVector[vectorIndex] - TestFeature.features[TestIndex].FeatureVector[vectorIndex] , 2);
			Distance = sqrt (Distance);
			
			if (Distance < minDistance) {
				coordinates[0].x[TestIndex] = TrainFeature.features[TrainIndex].x;
				coordinates[0].y[TestIndex] = TrainFeature.features[TrainIndex].y;
				minDistance = Distance;
			}
		}
	}

	return coordinates;
}