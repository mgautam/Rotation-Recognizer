#include "bmpAccess/testImageEngine.h"
#include "DSP/testDSP.h"
#include "CriticalPoints/testCriticalPoints.h"
#include "Matrix/testMatrix.h"
#include "AffineTransformer/testFrames.h"
#include "ModelAffine/testAffineFit.h"
#include "MotionExtract/testMotionExtract.h"

int main (int argc, char **argv) {
	//testImageEngine ();
	//testDSP ();
	//testCriticalPoints ();
	//testMatrix ();
	//testFrames ();
	//testAffineModel ();
	testMotionExtract ();

	return 0;
}