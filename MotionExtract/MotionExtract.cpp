#include "MotionExtract/MotionExtract.h"
#include <math.h>

void showMotion (MATRIX AffineMatrix) {
	
	MATRIX_CONTENT_TYPE** AffineMatrixData = AffineMatrix.getDataPtr();
	
	double rawCosX = (AffineMatrixData[0][0]+AffineMatrixData[1][1]);
	double rawSinX = (AffineMatrixData[1][0]-AffineMatrixData[0][1]);	
	double Normalizer = sqrt ( pow (rawCosX,2) + pow (rawSinX,2) );

	double cosX = rawCosX / Normalizer;
	double sinX = rawSinX / Normalizer;

	cout << "Rotation: " << acos (cosX) << " = " << asin (sinX) << "   " ;
	cout << "Translation: " << "( x: " << AffineMatrixData[2][0] << " , y: " << AffineMatrixData[2][1] << " )" << endl;
}