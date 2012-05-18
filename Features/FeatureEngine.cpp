#include <bmpAccess/testImageEngine.h>
#include <DSP/testDSP.h>
#include <CriticalPoints/testCriticalPoints.h>
#include <Matrix/testMatrix.h>
#include <AffineTransformer/testFrames.h>
#include <ModelAffine/testAffineFit.h>
#include <MotionExtract/testMotionExtract.h>
#include <Features/testMatchFeatures.h>

#include <API/videoProcessor.h>

#include <ModelAffine/coordinates.h>
#include <ModelAffine/fitAffineMatrix.h>
#include <MotionExtract/MotionExtract.h>
#include <iostream>
using namespace std;

int main (int argc, char **argv) {
	//testImageEngine ();
	//testDSP ();
	//testCriticalPoints ();
	//testMatrix ();
	// testFrames ();
	//testAffineModel ();
	//testMotionExtract ();
	//testMatchFeatures ();
/*
	COORDS initial,final;
	
	FILE *matchFile = fopen ("C:\\Users\\Gautam\\Desktop\\Project\\Matlab Code\\siftmatches.bin","rb");
	
	fread ((void *) &initial.Number_of_Coordinates,sizeof (int),1,matchFile);
	final.Number_of_Coordinates = initial.Number_of_Coordinates;
	
	initial.x = new double[initial.Number_of_Coordinates];
	fread ((void *) initial.x,sizeof (double),initial.Number_of_Coordinates,matchFile);
	initial.y = new double[initial.Number_of_Coordinates];
	fread ((void *) initial.y,sizeof (double),initial.Number_of_Coordinates,matchFile);

	final.x = new double[final.Number_of_Coordinates];
	fread ((void *) final.x,sizeof (double),final.Number_of_Coordinates,matchFile);
	final.y = new double[final.Number_of_Coordinates];
	fread ((void *) final.y,sizeof (double),final.Number_of_Coordinates,matchFile);

	fclose (matchFile);


	cout << "length:" << (int)final.Number_of_Coordinates << endl;
	cout << "Matches:" << endl;
	for (int matchIndex = 0; matchIndex < final.Number_of_Coordinates; matchIndex++) {
		cout << "(" << final.x[matchIndex] << "," << final.y[matchIndex] << ")";
		cout << " => ";
		cout << "(" << initial.x[matchIndex] << "," << initial.y[matchIndex] << ")";
		cout << endl;
	}

	cout << endl << endl << "AffineFit:" << endl;
	MATRIX affine = fitAffineMatrix ( initial, final );
	cout << affine << endl;

	showMotion (affine);
*/


	FILE *outputRotation;
	char FileName[100];
	for (int Threshold = 51; Threshold < 900; Threshold+=10) {
		sprintf (FileName, "C:\\Users\\Gautam\\Desktop\\Project\\02.Test\\Rotation\\Threshold(%d).bin",Threshold);
		outputRotation = fopen (FileName, "wb");
		processFrames (Threshold,outputRotation);
		fclose (outputRotation);
	}
	
	
	return 0;
}

void BuildSift (char* srcFile, char* ProjectFolder) {
	IMAGE *visual;
	GIMAGE *inImage,*lowImage,*highImage;
	IMAGE *extremeImage;
	char fileName[100];//44
	int numExtrema=0,totKeys =0,numKeys;

	
	inImage = createImage(1024,1024,1);
	visual = readGrey(srcFile);
	resample(Gtype(visual),2,1,inImage);
	//inImage = Gtype(readGrey(imageName));//"C:\\Users\\Gautam\\Desktop\\Project\\00.Test_Images\\testSquare.bmp"));
	GIMAGE**** Pyramid = LaplacianPyramid(inImage, sigma,  numOctaves, numBlurs, ProjectFolder);
	releaseImage(inImage);


	for(int i = 0; i < numOctaves; i++)
		for(int j = 1; j < numBlurs-2; j++){

			cout << "\t   Octave " << i+1 << "/" << numOctaves << "\t Blur " << 1 << "/" << numBlurs << endl;

			highImage = Pyramid[1][i][j-1];

			lowImage = Pyramid[1][i][j+1];

			extremeImage = createimage( Pyramid[1][i][j]->width, Pyramid[1][i][j]->height , 1 );
			numExtrema += findExtrema(Pyramid[1][i][j],lowImage,highImage,extremeImage);
			sprintf(fileName,"%s\\03.Extreme_Pyramid\\Image_Xterm(%d%d).bmp",ProjectFolder,i,j);
			writeImage(fileName,extremeImage);

			inImage = Pyramid[0][i][j];//j-1?
			numKeys = magoriCalc(inImage,extremeImage,i,j,ProjectFolder);
			siftKeyCalc(extremeImage, numKeys, i,j,ProjectFolder);		
			
		
			visualExtreme(extremeImage, i,j,ProjectFolder);
			visual2(inImage->width, inImage->height, i,j,ProjectFolder);	
			releaseImage(extremeImage);
			totKeys += numKeys;
	}
		writeAllSift(sigma,numOctaves,numBlurs,ProjectFolder);
		visual3(*visual,ProjectFolder);
		cout << "Total Number of Keys: "<< totKeys << " Extrema: " << numExtrema << endl;
		
}