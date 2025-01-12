4-4

#include <iostream>

#include<opencv\cv.h>
#include<opencv\highgui.h>
#include<opencv\cxcore.h>

#include<stdio.h>
#include<Windows.h>
#include<stdlib.h>
#include<time.h>

#define unsigned char uchar

double** sobelYMask, ** sobelXMask, ** PrewittMask, ** RobertsMak, ** Laaplace1Mask, ** Laplace2Mask, **embosMask;
int mask_size = 3;
int block_size = 3;
int embos_size = 3;


uchar** uc_alloc(int size_x, int size_y)
{

	uchar** m;
	int i;

	if ((m = (uchar * *)calloc(size_y, sizeof(uchar*))) == NULL)
	{
		printf("uc_alloc error 1\7\n");
		exit(0);
	}

	for (i = 0; i < size_y; i++)
		if ((m[i] = (uchar*)calloc(size_x, sizeof(uchar))) == NULL) {
			printf("uc_alloc error 2\7\n");
			exit(0);

		}
	return m;
}


double** d_alloc(int size_x, int size_y)
{

	double** m;
	int i;

	if ((m = (double**)calloc(size_y, sizeof(double*))) == NULL)
	{
		printf("uc_alloc error 1\7\n");
		exit(0);
	}

	for (i = 0; i < size_y; i++)
		if ((m[i] = (double*)calloc(size_x, sizeof(double))) == NULL) {
			printf("uc_alloc error 2\7\n");
			exit(0);

		}
	return m;
}


void read_ucmatrix(int size_x, int size_y, uchar** ucmatrix, char* filename)
{
	int i;
	FILE* f;
	if ((fopen_s(&f, filename, "rb")) != NULL)
	{
		printf("%s File open Error! \n", filename);
		exit(0);

	}
	for (i = 0; i < size_y; i++)
		if (fread(ucmatrix[i], sizeof(uchar), size_x, f) != size_x)
		{
			printf("Data Read Error! \n");
			exit(0);

		}
	fclose(f);
}
void write_ucmatrix(int size_x, int size_y, uchar** ucmatrix, char* filename)
{
	int i;
	FILE* f;

	if ((fopen_s(&f, filename, "wb")) != NULL)
	{
		printf("%s File open Error! \n", filename);
		exit(0);
	}

	for (i = 0; i < size_y; i++)
		if (fwrite(ucmatrix[i], sizeof(uchar), size_x, f) != size_x)
		{
			printf("Data Write Error! \n");
			exit(0);
		}
	fclose(f);
}

void make_Mask(int mask_size, double** Mask, int checkMask) {
	int i, j;
	double sobelYMask[3][3] = { 1,0,-1,2,0,-2,1,0,-1 };
	double sobelXMAK[3][3] = { -1,-2,-1,0,0,0,1,2,1 };
	double PrewittYMask[3][3] = { 1,0,-1,1,0,-1,1,0,-1 };
	double PrewittXMaks[3][3] = { -1,-1,-1,0,0,0,1,1,1 };
	double RobertsYMask[3][3] = { 0,0,-1,0,1,0,0,0,0 };
	double RobertsXMask[3][3] = { -1,0,0,0,1,0,0,0,0 };
	double Laplace1Mask[3][3] = { 0,-1,0,-1,4,-1,0,-1,0 };
	double Laplace2Mask[3][3] = { -1,-1,-1,-1,8,-1,-1,-1,-1 };
	double Laplace3Mask[3][3] = { 0,-1,0,-1,5,-1,0,-1,0 };
	double Laplace4Mask[3][3] = { -1,-1,-1,-1,9,-1,-1,-1,-1 };
	double emboMask[3][3] = { -1,0,0,0,0,0,0,0,1 };

	switch (checkMask) {
	case 0:
		for (i = 0; i < mask_size; i++)
			for (j = 0; j < mask_size; j++)
				Mask[i][j] = sobelYMask[i][j];
		break;
	case 1:
		for (i = 0; i < mask_size; i++)
			for (j = 0; j < mask_size; j++)
				Mask[i][j] = sobelXMAK[i][j];
		break;
	case 2:
		for (i = 0; i < mask_size; i++)
			for (j = 0; j < mask_size; j++)
				Mask[i][j] = PrewittYMask[i][j];
		break;
	case 3:
		for (i = 0; i < mask_size; i++)
			for (j = 0; j < mask_size; j++)
				Mask[i][j] = PrewittXMaks[i][j];
		break;
	case 4:
		for (i = 0; i < mask_size; i++)
			for (j = 0; j < mask_size; j++)
				Mask[i][j] = RobertsYMask[i][j];
		break;
	case 5:
		for (i = 0; i < mask_size; i++)
			for (j = 0; j < mask_size; j++)
				Mask[i][j] = RobertsXMask[i][j];
		break;
	case 6:
		for (i = 0; i < mask_size; i++)
			for (j = 0; j < mask_size; j++)
				Mask[i][j] = Laplace1Mask[i][j];
		break;
	case 7:
		for (i = 0; i < mask_size; i++)
			for (j = 0; j < mask_size; j++)
				Mask[i][j] = Laplace2Mask[i][j];
		break;
	case 8:
		for (i = 0; i < mask_size; i++)
			for (j = 0; j < mask_size; j++)
				Mask[i][j] = Laplace3Mask[i][j];
		break;

	case 9:
		for (i = 0; i < mask_size; i++)
			for (j = 0; j < mask_size; j++)
				Mask[i][j] = Laplace4Mask[i][j];
		break;
	case 10:
		for (i = 0; i < mask_size; i++)
			for (j = 0; j < mask_size; j++)
				Mask[i][j] = emboMask[i][j];
		break;
	default:
		printf("error\n");
		exit(1);

	}


}

void convolutionD2(double** h, int F_length, int size_x, int size_y, uchar** image1, uchar** image2) {
	int i, j, x, y;
	int margin, indexX, indexY;
	double sum, coeff;

	margin = (int)(F_length / 2);


	for (i = 0; i < size_y; i++) {
		for (j = 0; j < size_x; j++) {
			sum = 0;

			for (y = 0; y < F_length; y++) {
				indexY = i - margin + y;
				if (indexY < 0)indexY = -indexY;
				else if (indexY >= size_y) indexY = (2 * size_y - indexY - 1);

				for (x = 0; x < F_length; x++) {
					indexX = j - margin + x;
					if (indexX < 0)indexX = -indexX;
					else if (indexX >= size_x)indexX = (2 * size_x - indexX - 1);

					sum += h[y][x] * (double)image1[indexY][indexX];

				}
			}

			sum += 100;
			if (sum < 0)sum = 0.;
			else if (sum > 255)sum = 255.;
			image2[i][j] = (uchar)sum;

		}
	}
}




int main(int argc, char* argv[])
{

	int i, j,value;
	double** Mask;
	IplImage* cvImg;
	CvSize imgSize;
	uchar** img,** result_img;

	if (argc != 4)
	{
		printf("Exe imgData x_size y_size \n");
		exit(0);
	}

	imgSize.width = atoi(argv[2]);
	imgSize.height = atoi(argv[3]);
	img = uc_alloc(imgSize.width, imgSize.height);
	result_img = uc_alloc(imgSize.width, imgSize.height);
	read_ucmatrix(imgSize.width, imgSize.height, img, argv[1]);

	Mask = d_alloc(mask_size, mask_size);
	cvImg = cvCreateImage(imgSize, 8, 1);
	

	make_Mask(mask_size, Mask, 10); //엠보싱
	convolutionD2(Mask, block_size, imgSize.width, imgSize.height, img, result_img);


	for (i = 0; i < imgSize.height; i++)
		for (j = 0; j < imgSize.width; j++)
		{

			((uchar*)(cvImg->imageData + cvImg->widthStep * i))[j] = result_img[i][j];

		}
	cvNamedWindow(argv[1], 1);
	cvShowImage(argv[1], cvImg);

	cvWaitKey(0);

	cvDestroyWindow("image");
	cvReleaseImage(&cvImg);

	getchar();

	return 0;

}
