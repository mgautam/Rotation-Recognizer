#include <MotionExtract/MotionExtract.h>
#include <math.h>

void showMotion (MATRIX AffineMatrix, FILE *output) {
	
	MATRIX_CONTENT_TYPE** AffineMatrixData = AffineMatrix.getDataPtr();
	
	double rawCosX = (AffineMatrixData[0][0]+AffineMatrixData[1][1]);
	double rawSinX = (AffineMatrixData[1][0]-AffineMatrixData[0][1]);	
	double Normalizer = sqrt ( pow (rawCosX,2) + pow (rawSinX,2) );

	double cosX = rawCosX / Normalizer;
	double sinX = rawSinX / Normalizer;

	double theta = acos (cosX);

	printf ("Rotation: %6.3lf = %6.3lf Translation: ( x: %8.3lf , y: %8.3lf )\n", theta, asin (sinX), AffineMatrixData[2][0], AffineMatrixData[2][1]);
	
	if (output) 
		fwrite (&theta, sizeof (double), 1, output);

}