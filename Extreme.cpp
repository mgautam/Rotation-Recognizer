#include <Extreme.h>

using namespace std;

 int findExtrema(GIMAGE *inImage, GIMAGE *lowImage, GIMAGE *highImage, IMAGE *extremeImage){
	int numExtreme = 0;

	for(int n1 = 0; n1 < extremeImage->height; n1++)
		for(int n2 = 0; n2 < extremeImage->width; n2++)
			extremeImage->imageData[n1*inImage->width + n2] = 0;

	for(int n1 = 1; n1 < inImage->height - 1 ; n1++)
		for(int n2 = 1; n2 < inImage->width - 1; n2++)
		{
			int i = 8;
			for(int x = n1-1; x <= n1+1; x++)
				for(int y = n2-1; y <= n2+1; y++)
				{
					if(x == n1 && y == n2) continue;
					if(inImage->imageData[n1*inImage->width+n2] > inImage->imageData[x*inImage->width+y])	i++;
					else if(inImage->imageData[n1*inImage->width+n2] < inImage->imageData[x*inImage->width+y]) i--;
				}
			
			if(!(i%16))
			{
				int j = 9;
				for(int x = n1-1; x <= n1+1; x++)
					for(int y = n2-1; y <= n2+1; y++)
						if(x >= 0 && x < lowImage->width && y >= 0 && y < lowImage->height)
						{
							if(inImage->imageData[n1*inImage->width+n2] > lowImage->imageData[x*lowImage->width+y])	j++;
							else if(inImage->imageData[n1*inImage->width+n2] < lowImage->imageData[x*lowImage->width+y]) j--;
						}				
			/*
				if(i == 16 && j == 18)
					//if(inImage->imageData[n1*inImage->width+n2] > lowImage->imageData[((int) (n1/1.5))*lowImage->width+((int) (n2/1.5))])
						extremeImage->imageData[n1*extremeImage->width + n2] = 255;
				else if(!i && !j)
					//if(inImage->imageData[n1*inImage->width+n2] < lowImage->imageData[((int) (n1/1.5))*lowImage->width+((int) (n2/1.5))])
						extremeImage->imageData[n1*extremeImage->width + n2] = 255;*/
				if(!(j%18))
				{
					
					int k = 9;
					for(int x = n1-1; x <= n1+1; x++)
						for(int y = n2-1; y <= n2+1; y++)
							if(x >= 0 && x < highImage->width && y >= 0 && y < highImage->height)
							{
								if(inImage->imageData[n1*inImage->width+n2] > highImage->imageData[x*highImage->width+y])	k++;
								else if(inImage->imageData[n1*inImage->width+n2] < highImage->imageData[x*highImage->width+y]) k--;
							}				
			
					if(i == 16 && j == 18 && k == 18){
							extremeImage->imageData[n1*extremeImage->width + n2] = 255;
							numExtreme++;
					}
					else if(!i && !j && !k){
							extremeImage->imageData[n1*extremeImage->width + n2] = 255;
							numExtreme++;
					}
					//if(!(k%18))
						//numExtreme++;
				}
			}
		}
		cout << "\t\tNumber of Extrema Points:" << numExtreme << endl;
		return numExtreme;
}