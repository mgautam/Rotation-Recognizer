#include "bmpAccess/testImageEngine.h"
#include "DSP/testDSP.h"
#include "CriticalPoints/testCriticalPoints.h"
#include "Matrix/testMatrix.h"
#include "AffineTransformer/testFrames.h"
#include "ModelAffine/testAffineFit.h"

int main (int argc, char **argv) {
	//testImageEngine ();
	//testDSP ();
	//testCriticalPoints ();
	//testMatrix ();
	//testFrames ();
	testAffineModel ();

	return 0;
}