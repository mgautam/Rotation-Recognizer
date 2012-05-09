#include "ModelAffine/testAffineFit.h"
#include "ModelAffine/fitAffineMatrix.h"

void testAffineModel (void) {
	COORDS initial,final;

	initial.x = new double[100];
	initial.y = new double[100];
	initial.Number_of_Coordinates = 100;

	final.x = new double[100];
	final.y = new double[100];
	final.Number_of_Coordinates = 100;

	for (int index = 0; index < 100; index++) {
		initial.x[index] = index;
		initial.y[index] = rand();
	
		cout << "(" << initial.x[index] << "," << initial.y[index] << ")" ;

		final.x[index] = 6*initial.x[index] + 7*initial.y[index] + 2;
		final.y[index] = 1*initial.x[index] + 13*initial.y[index] + 3;

		cout << " => (" << final.x[index] << "," << final.y[index] << ")" << endl;

	}

	cout << fitAffineMatrix ( initial, final ) << endl;
}