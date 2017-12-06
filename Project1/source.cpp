#include <opencv2\opencv.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>
#include "ArrayOfDrawablePoints.h"

using namespace std;
using namespace cv;

void DetectCircles(Mat frame);
void ChangeParameters(int c);
void PrintResultAndParameters(Mat frame_amount_color);
void DrawCircles(Mat frame, vector<Vec3f> circlesToDraw, String windowName);

ArrayOfDrawablePoints arrayOfPoints;

/// Parameters for Houghs transform
int minDist = 8;
int upperThreshold = 200;
int centersThreshold = 14;
int minRadius = 1;
int maxRadius = 10;

int main(void)
{
	VideoCapture capture;
	Mat frame;
	Mat frame_amount_color;

	String windowName = "Detekcja oczek na kostce do gry!";

	capture.open(0);
	if (!capture.isOpened()) 
	{ 
		printf("B³ad podczas otwierania przechwytywania wideo!\n"); 
		return -1; 
	}

	while (capture.read(frame))
	{
		if (frame.empty())
		{
			printf("Brak przechwytywanego obrazu, break!");
			return -2;
		}

		int c = waitKey(100);
		
		if ((char)c == 27) 
			return 0; 

		ChangeParameters(c);

		cvtColor(frame, frame_amount_color, COLOR_BGR2HLS_FULL);
		
		DetectCircles(frame);

		DrawCircles(frame, arrayOfPoints.GetDrawablePoints(), windowName);

		PrintResultAndParameters(frame_amount_color);

		frame_amount_color.release();
		frame.release();
	}
	return 0;
}

void DetectCircles(Mat frame)
{
	Mat frame_gray;
	
	cvtColor(frame, frame_gray, CV_BGR2GRAY);

	Mat blurredGray;
	
	GaussianBlur(frame_gray, blurredGray, Size(9, 9), 2, 2);

	vector<Vec3f> circles;

	HoughCircles(blurredGray, circles, CV_HOUGH_GRADIENT, 1, minDist, upperThreshold, centersThreshold, minRadius, maxRadius);

	arrayOfPoints.RefreshArray(circles);

	circles.clear();
	frame.release();
	frame_gray.release();
	blurredGray.release();
}

void DrawCircles(Mat frame, vector<Vec3f> circlesToDraw, String windowName)
{
	for (int i = 0; i < circlesToDraw.size(); i++)
	{
		Point center(cvRound(circlesToDraw[i][0]), cvRound(circlesToDraw[i][1]));
		int radius = cvRound(circlesToDraw[i][2]);
		circle(frame, center, 3, Scalar(0, 255, 0), -1, 8, 0);
		circle(frame, center, radius, Scalar(0, 0, 255), 3, 8, 0);
	}
	imshow(windowName, frame);
	frame.release();
	circlesToDraw.clear();
}

void ChangeParameters(int c)
{
	if ((char)c == '1')
		minDist++;
	if ((char)c == 'q')
		minDist--;

	if ((char)c == '2')
		upperThreshold++;
	if ((char)c == 'w')
		upperThreshold--;

	if ((char)c == '3')
		centersThreshold++;
	if ((char)c == 'e')
		centersThreshold--;

	if ((char)c == '4')
		minRadius++;
	if ((char)c == 'r')
		minRadius--;

	if ((char)c == '5')
		maxRadius++;
	if ((char)c == 't')
		maxRadius--;
}

void PrintResultAndParameters(Mat frame_amount_color)
{
	system("cls");
	cout << "H - odcien; L - jasnoœæ, S - nasycenie" << endl;
	cout << "Light: " << mean(frame_amount_color) << endl;
	cout << "Iloœæ oczek: " << arrayOfPoints.GetDrawablePoints().size() << endl;
	cout << "Minimalny dystans: " << minDist << endl;
	cout << "Próg dla Canny: " << upperThreshold << endl;
	cout << "Próg dla centrów: " << centersThreshold << endl;
	cout << "Minimalny promieñ: " << minRadius << endl;
	cout << "Maksymalny promieñ: " << maxRadius << endl;
	frame_amount_color.release();
}