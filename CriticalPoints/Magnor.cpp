#include <Magnor.h>

using namespace std;

int magoriCalc(GIMAGE *GaussPix, IMAGE *ExPix, int octave, int blur, char *Project_Folder) {
	FILE *magFile,*oriFile;
	char filename[100];
	sprintf(filename,"%s\\04.Gradient_Pyramid\\%d%d.bin",Project_Folder,octave,blur);
	magFile = fopen(filename,"wb");
	sprintf(filename,"%s\\05.Orientation_Pyramid\\%d%d.bin",Project_Folder,octave,blur);
	oriFile = fopen(filename,"wb");
		
	GTYPE *Magnitudes = new GTYPE[GaussPix->width*GaussPix->height];
	GTYPE maxMag = 0, minMag = 255;
	GTYPE *orientations = new GTYPE[GaussPix->width*GaussPix->height];
	for(int n1 = 0; n1 < GaussPix->height-1; n1++)
	{
		for(int n2 = 0; n2 < GaussPix->width-1; n2++)
		{
			Magnitudes[n1*GaussPix->width+n2] = sqrt(pow(GTYPE(GaussPix->imageData[n1*GaussPix->width+n2]-GaussPix->imageData[(n1+1)*GaussPix->width+n2]),2) 
				+ pow(GTYPE(GaussPix->imageData[n1*GaussPix->width+n2]-GaussPix->imageData[n1*GaussPix->width+n2+1]),2));
			if(Magnitudes[n1*GaussPix->width+n2] > maxMag) maxMag = Magnitudes[n1*GaussPix->width+n2];
			if(Magnitudes[n1*GaussPix->width+n2] < minMag) minMag = Magnitudes[n1*GaussPix->width+n2];			

			orientations[n1*GaussPix->width+n2] = atan2(GTYPE(GaussPix->imageData[n1*GaussPix->width+n2]-GaussPix->imageData[(n1+1)*GaussPix->width+n2]), 
				GTYPE(GaussPix->imageData[n1*GaussPix->width+n2]-GaussPix->imageData[n1*GaussPix->width+n2+1]));			
		}
		Magnitudes[n1*GaussPix->width+GaussPix->width-1] = 0;
	}
	for(int n2 = 0; n2 < GaussPix->width; n2++)
		Magnitudes[(GaussPix->height-1)*GaussPix->width+GaussPix->width-1] = 0;
			
	fwrite(&maxMag,sizeof(GTYPE),1,magFile);
	fwrite(&minMag,sizeof(GTYPE),1,magFile);
	fwrite(Magnitudes,sizeof(GTYPE),GaussPix->width*GaussPix->height,magFile);
	fwrite(orientations,sizeof(GTYPE),GaussPix->width*GaussPix->height,oriFile);
	//cout << index << ". MaxMag = " << maxMag << " , MinMag = " << minMag << endl;
	fclose(magFile);
	fclose(oriFile);
	
	delete orientations;
	
	int numKeys = 0;
	GTYPE threshold = maxMag/10.0;	
	for(int n1 = 0; n1 < ExPix->height-1; n1++)
		for(int n2 = 0; n2 < ExPix->width-1; n2++)
			if(ExPix->imageData[n1*ExPix->width+n2] == 255 && Magnitudes[n1*ExPix->width+n2] > threshold )//==255
				numKeys++;

	delete Magnitudes;

	cout << "\t\tStable Keypoints: " << numKeys << endl;
	return numKeys;	
}



void siftKeyCalc(IMAGE *ExPix, int numKeys, int octave, int blur, char* Project_Folder) {
	char filename[100];	
	//sprintf(filename,"%s\\01.Gaussian_Pyramid\\Image_Gauss(%d).bmp",Project_Folder,index);
	//IplImage *scaleImage = cvLoadImage(filename,CV_LOAD_IMAGE_GRAYSCALE);

	FILE *magFile,*oriFile;
	GTYPE *Magnitudes = new GTYPE[ExPix->width*ExPix->height];
	GTYPE maxMag, minMag;
	GTYPE *orientations = new GTYPE[ExPix->width*ExPix->height];

	sprintf(filename,"%s\\04.Gradient_Pyramid\\%d%d.bin",Project_Folder,octave,blur);
	magFile = fopen(filename,"rb");
	fread(&maxMag,sizeof(GTYPE),1,magFile);
	fread(&minMag,sizeof(GTYPE),1,magFile);
	fread(Magnitudes,sizeof(GTYPE),ExPix->width*ExPix->height,magFile);
	fclose(magFile);

	sprintf(filename,"%s\\05.Orientation_Pyramid\\%d%d.bin",Project_Folder,octave,blur);
	oriFile = fopen(filename,"rb");
	fread(orientations,sizeof(GTYPE),ExPix->width*ExPix->height,oriFile);
	fclose(oriFile);
	

	GTYPE gaussFilter[81];
	GaussianFilter2(0, 1.5*1.5*blur, -4, 9, gaussFilter);
	siftKey *keyDescriptors = new siftKey[numKeys];
	int keyIndex = 0;
	GTYPE oriBins[36];
	
	GTYPE threshold = maxMag/10;
	GTYPE Norm;
	for(int n1 = 0; n1 < ExPix->height-1; n1++)
		for(int n2 = 0; n2 < ExPix->width-1; n2++)
			if(ExPix->imageData[n1*ExPix->width+n2] == 255 && Magnitudes[n1*ExPix->width+n2] > threshold )//==255
			{
				for(int i=0; i < 36; i++) 
					oriBins[i] = 0;

				for(int k1=-4; k1 <= 4; k1++)
					for(int k2=-4; k2 <= 4; k2++)
						if((n1+k1) > 0 && (n1+k1) < ExPix->height && (n2+k2) > 0 && (n2+k2) < ExPix->width)
							for(int i=0; i < 36; i++)
								if(orientations[(n1+k1)*ExPix->width+(n2+k2)] > pi*(-1.0+(GTYPE)i/18.0) 
									&& orientations[(n1+k1)*ExPix->width+(n2+k2)] < pi*(-1.0+((GTYPE)i+1.0)/18.0))
									oriBins[i] += Magnitudes[(n1+k1)*ExPix->width+(n2+k2)]*gaussFilter[(k1+4)*9+(k2+4)];
				
				keyDescriptors[keyIndex].KeyOrientation = 0;
				for(int i=1; i < 36; i++)
					if(oriBins[keyDescriptors[keyIndex].KeyOrientation] < oriBins[i])
						keyDescriptors[keyIndex].KeyOrientation = i;
						
			
				for(int m1=-2; m1 < 2; m1++)
					for(int m2=-2; m2 < 2; m2++)					
					{
						for(int i=0; i < 8; i++) 
							keyDescriptors[keyIndex].Descriptor[((m1+2)*4+(m2+2))*8+i] = 0;
						
						for(int k1=m1*4; k1 < (m1+1)*4; k1++)
							for(int k2=m2*4; k2 < (m2+1)*4; k2++)
								if((n1+k1) > 0 && (n1+k1) < ExPix->height && (n2+k2) > 0 && (n2+k2) < ExPix->width)
									for(int i=0; i < 8; i++)
										if(orientations[(n1+k1)*ExPix->width+(n2+k2)] > pi*(-1.0+(GTYPE)i/4.0) && orientations[(n1+k1)*ExPix->width+(n2+k2)] < pi*(-1.0+((GTYPE)i+1.0)/4.0))										
											keyDescriptors[keyIndex].Descriptor[((m1+2)*4+(m2+2))*8+i] += Magnitudes[(n1+k1)*ExPix->width+(n2+k2)];//(maxMag-minMag)//*gaussFilter[(k1+3)*7+(k2+3)];
					}

				Norm = 0;
				for(int i=0; i < 128; i++)
					Norm+=pow(keyDescriptors[keyIndex].Descriptor[i],2);
				Norm = sqrt(Norm);
				if( Norm != 0) {
					for(int i=0; i < 128; i++)
						keyDescriptors[keyIndex].Descriptor[i] /= Norm;					
				}
				
				//Threshold to 0.2
				for(int i=0; i < 128; i++)
					if(keyDescriptors[keyIndex].Descriptor[i] > 0.2) 
						keyDescriptors[keyIndex].Descriptor[i] = 0.2;

				// Renormalize
				Norm = 0;
				for(int i=0; i < 128; i++)
					Norm+=pow(keyDescriptors[keyIndex].Descriptor[i],2);
				Norm = sqrt(Norm);
				for(int i=0; i < 128; i++) {
					keyDescriptors[keyIndex].Descriptor[i] /= Norm;
					if(keyDescriptors[keyIndex].Descriptor[i] > 1 || keyDescriptors[keyIndex].Descriptor[i]  < 0)
						keyDescriptors[keyIndex].Descriptor[i] = 0;
				}

				keyDescriptors[keyIndex].y = n1;
				keyDescriptors[keyIndex].x = n2;
				keyIndex++;
			}
	//cout << "NumKeys = " << keyIndex << endl;
	FILE *keyFile;
	sprintf(filename,"%s\\07.SIFT_Keys\\%d%d.bin",Project_Folder,octave,blur);
	keyFile = fopen(filename,"wb");
	fwrite(&keyIndex,sizeof(int),1,keyFile);
	fwrite(keyDescriptors,sizeof(siftKey),keyIndex,keyFile);
	fclose(keyFile);
	delete keyDescriptors;
}

void writeAllSift(GTYPE sigma, int numOctaves,int numBlurs,char *PROJECT_FOLDER){
	FILE *keyFile;
	int numKeys,totalKeys = 0;
	siftKey *keyDescriptors;
	GTYPE scale;
	char filename[100];

	sprintf(filename,"%s\\07.SIFT_Keys\\ALL.bin",PROJECT_FOLDER);
	FILE *AllKeyFile = fopen(filename,"wb");

	//fwrite(&totalKeys,sizeof(int),1,AllKeyFile);
	fseek(AllKeyFile,sizeof(int),0);
	fwrite(&numOctaves,sizeof(int),1,AllKeyFile);
	fwrite(&numBlurs,sizeof(int),1,AllKeyFile);

	for(int i = 0; i < numOctaves; i++)
		for(int j = 1; j < numBlurs-2; j++){
			sprintf(filename,"%s\\07.SIFT_Keys\\%d%d.bin",PROJECT_FOLDER,i,j);
			keyFile = fopen(filename,"rb");
			fread(&numKeys,sizeof(int),1,keyFile);
			keyDescriptors = new siftKey[numKeys];
			fread(keyDescriptors,sizeof(siftKey),numKeys,keyFile);
			fclose(keyFile);

			totalKeys += numKeys;
			scale = sigma*pow(2.0,(GTYPE)i+(j+1)/(GTYPE)(numBlurs-3));
			fwrite(&numKeys,sizeof(int),1,AllKeyFile);
			fwrite(&scale,sizeof(GTYPE),1,AllKeyFile);
			fwrite(keyDescriptors,sizeof(siftKey),numKeys,AllKeyFile);
			delete keyDescriptors;
		}
	fseek(AllKeyFile,0,0);
	fwrite(&totalKeys,sizeof(int),1,AllKeyFile);
	fclose(AllKeyFile);	
}
