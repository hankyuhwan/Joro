#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <opencv2/opencv.hpp>

//define map size
#define SIZE 256
using namespace cv;

void myremap(void)
{
	int fp1,fp2;
	fp1 = open("mymap.pgm", O_RDONLY);
	fp2 = open("mymap.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);

	//map save array
	unsigned int arr[SIZE][SIZE];

	int i = 0, j = 0;
	int min_x = 0, min_y = 0, max_x = 0, max_y = 0;
	unsigned char temp;

	//file open test
	if (fp1 == -1) 
		printf("image open fail\n");
	
	if(fp2==-1)
		printf("txt open fial\n");

	//pgm setting byte except
	for (i = 0; i < 50; i++) {
		read(fp1, &temp, 1);
	}

	//pgm file define array
	for (i = 0; i < SIZE; i++)
	{
		for (j = 0; j < SIZE; j++)
		{
			read(fp1, &temp, 1);

			//black - wall
			if (temp == 0) {
				arr[i][j] = 1;
			}
			//white - ground
			else if (temp == 254) {
				arr[i][j] = 2;
			}
			else {
				arr[i][j] = 0;
			}
		}
	}

	//file hole
	for (i = 0; i < SIZE; i++)
	{
		for (j = 0; j < SIZE; j++)
		{
			if (arr[i][j] == 0)
			{
				if (((arr[i - 1][j] == 1) || (arr[i - 1][j] == 2)) && ((arr[i + 1][j] == 1) || (arr[i + 1][j] == 2)) && ((arr[i][j - 1] == 1) || (arr[i][j - 1] == 2)) && ((arr[i][j + 1] == 1) || (arr[i][j + 1] == 2)))
					arr[i][j] = 2;
			}
		}
	}

	//cut array
	for (i = 0; i < SIZE; i++)
	{
		for (j = 0; j < SIZE; j++)
		{
			if (arr[i][j] == 1) {
				min_x = i;
				break;
			}
		}
		if (min_x != 0)
			break;
	}

	for (i = SIZE - 1; i > 0; i--)
	{
		for (j = 0; j < SIZE; j++)
		{
			if (arr[i][j] == 1) {
				max_x = i + 1;
				break;
			}
		}
		if (max_x != 0)
			break;
	}

	for (j = 0; j < SIZE; j++)
	{
		for (i = 0; i < SIZE; i++)
		{
			if (arr[i][j] == 1) {
				min_y = j;
				break;
			}
		}
		if (min_y != 0)
			break;
	}

	for (j = SIZE - 1; j > 0; j--)
	{
		for (i = 0; i < SIZE; i++)
		{
			if (arr[i][j] == 1) {
				max_y = j + 1;
				break;
			}
		}
		if (max_y != 0)
			break;
	}

	//print array to display
	for (i = min_x; i < max_x; i++)
	{
		for (j = min_y; j < max_y; j++) {
			//print to console
			//printf("%d", arr[i][j]);		
			
			//print to file - number ver.
			//fprintf(fp2, "%d", arr[i][j]);

			//print to char
			if (arr[i][j] == 0)
				write(fp2, "0",1);

			else if (arr[i][j] == 1)
				write(fp2, "1",1);

			else if (arr[i][j] == 2)
				write(fp2,"2",1);
		}
		//printf("\n");
		write(fp2, "\n",1);
	}
	int w, h;
	w = max_x-min_x;
	h = max_y-min_y;
	Mat img(h,w,CV_8UC3);
	for(int row=0;row<img.rows;row++)
		for(int col=0;col<img.cols;col++)
		{
			if(arr[col][row] == 0)
				for(int i=0;i<3;i++)
					img.at <Vec3b>(row,col)[i] = 127;
			else if(arr[col][row] == 1)
				for(int i=0; i<3; i++)
					img.at <Vec3b>(row,col)[i] = 0;
			else if(arr[col][row] == 2)
				for(int i = 0; i<3; i++)
					img.at <Vec3b>(row,col)[i] = 255;
			else
				for(int i=0; i<3; i++)
					img.at <Vec3b>(row,col)[i] = 127;
		}
	imwrite("./remap.bmp",img);
	
	close(fp1);
	close(fp2);
}
