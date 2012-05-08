#include "bmpAccess/testImageEngine.h"
#include "DSP/testDSP.h"
#include "CriticalPoints/testCriticalPoints.h"


int main (int argc, char **argv) {
	testImageEngine ();
	//testDSP ();
	testCriticalPoints ();
}