#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "mine.h"

using namespace std;
using namespace cv;
 
int main()
{
	Mat img = imread("./reamp.bmp",1);
	Mat result = img.clone();
		circle(result,Point(20,20),10,Scalar(0,0,255),-1);
	imwrite("./test.bmp",result);

	return 0;
}
