#ifndef MAGNOR_H
#define MAGNOR_H

#include<Extreme.h>
#include<math.h>

struct siftKey {
	int x, y;	
	char KeyOrientation;
	GTYPE Descriptor[128];
};

int magoriCalc(GIMAGE *GaussPix, IMAGE *ExPix, int octave, int blur, char *Project_Folder);
void siftKeyCalc(IMAGE *ExPix, int numKeys, int octave, int blur, char* Project_Folder);
void writeAllSift(GTYPE sigma, int numOctaves,int numBlurs,char *PROJECT_FOLDER);

#endif
