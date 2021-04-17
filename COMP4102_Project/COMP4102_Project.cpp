#ifdef _CH_
#pragma package <opencv>
#endif

#include "opencv\cv.h"
#include "opencv\highgui.h"

// Load the source image. HighGUI use.
IplImage* image1 = 0, * image2 = 0, *image3 = 0, *outputImage = 0;
int x, y, averageB, averageG, averageR;

int main(int argc, char** argv)
{
	char* filename1 = (argc == 4) ? argv[1] : (char*)"test1.jpg";
	char* filename2 = (argc == 4) ? argv[2] : (char*)"test2.jpg";
	char* filename3 = (argc == 4) ? argv[3] : (char*)"test3.jpg";

	if ((image1 = cvLoadImage(filename1, 1)) == 0)
		return -1;
	if ((image2 = cvLoadImage(filename2, 1)) == 0)
		return -1;
	if ((image3 = cvLoadImage(filename3, 1)) == 0)
		return -1;

	printf("New Image width %d image height %d channels %d \n",
		image1->width, image1->height, image1->nChannels);
	// create output image
	outputImage = cvCreateImage(cvSize(image1->width, image1->height), IPL_DEPTH_8U, 3);
	
	// average out three images for superresolution
	for (x = 0; x < image1->height; x++) {
		for (y = 0; y < image1->width; y++) {
			averageB = 0;
			averageG = 0;
			averageR = 0;
			averageB = averageB + (image1->imageData + image1->widthStep * x)[y*3];
			averageG = averageG + (image1->imageData + image1->widthStep * x)[y * 3+1];
			averageR = averageR + (image1->imageData + image1->widthStep * x)[y * 3+2];
			averageB = averageB + (image2->imageData + image2->widthStep * x)[y*3];
			averageG = averageG + (image2->imageData + image2->widthStep * x)[y * 3+1];
			averageR = averageR + (image2->imageData + image2->widthStep * x)[y * 3+2];
			averageB = averageB + (image3->imageData + image3->widthStep * x)[y*3];
			averageG = averageG + (image3->imageData + image3->widthStep * x)[y * 3+1];
			averageR = averageR + (image3->imageData + image3->widthStep * x)[y * 3+2];

			averageB = averageB / 3;
			averageG = averageG / 3;
			averageR = averageR / 3;

			((uchar*)(outputImage->imageData + outputImage->widthStep * x))[y*3] = averageB;
			((uchar*)(outputImage->imageData + outputImage->widthStep * x))[y * 3+1] = averageG;
			((uchar*)(outputImage->imageData + outputImage->widthStep * x))[y * 3+2] = averageR;
		}
	}

	// Create windows.
	cvNamedWindow("Source1", 1);
	cvNamedWindow("Source2", 1);
	cvNamedWindow("Source3", 1);
	cvNamedWindow("Result", 1);

	// Show the images.
	cvShowImage("Source1", image1);
	cvShowImage("Source2", image2);
	cvShowImage("Source3", image3);
	cvShowImage("Result", outputImage);

	// Wait for a key stroke
	cvWaitKey(0);
	cvReleaseImage(&image1);
	cvReleaseImage(&image2);
	cvReleaseImage(&image3);
	cvReleaseImage(&outputImage);

	cvDestroyWindow("Source1");
	cvDestroyWindow("Source2");
	cvDestroyWindow("Source3");
	cvDestroyWindow("Result");

	return 0;
}