#include <opencv2\opencv.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>
#include "ArrayOfDrawablePoints.h"

using namespace std;
using namespace cv;

/* Uchwyt funkcji */
void myMethode(Mat frame);

/* Zmienne globalne */
String window_name = "Detekcja oczek na kostce do gry!";

vector<int> buffor;
ArrayOfDrawablePoints arrayOfPoints;

int minDist = 8; // NIE ZMNIEJSZAÆ
int upperThreshold = 200;
int centersThreshold = 14; // NIE ZMNIEJSZAÆ
int minRadius = 1;
int maxRadius = 10;

/* Main */
int main(void)
{
	VideoCapture capture;
	Mat frame;
	Mat frame_amount_color;

	//Czytaj wideo stream - domyœlna kamera
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
			break;
		}

		//Wywolanie uchwytu 
		//detectAndDisplay(detector, frame);
		cvtColor(frame, frame_amount_color, COLOR_BGR2HLS_FULL);
		
		int c = waitKey(100);
		if ((char)c == 27) { break; } // escape

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

		myMethode(frame);

		
		system("cls");
		cout << "H - odcien; L - jasnoœæ, S - nasycenie" << endl;
		cout << "Light: " << mean(frame_amount_color) << endl;
		cout << "Iloœæ oczek: " << arrayOfPoints.GetDrawablePoints().size() << endl;
		cout << "Minimalny dystans: " << minDist << endl;
		cout << "Próg dla Canny: " << upperThreshold << endl;
		cout << "Próg dla centrów: " << centersThreshold << endl;
		cout << "Minimalny promieñ: " << minRadius << endl;
		cout << "Maksymalny promieñ: " << maxRadius << endl;
		
	}
	return 0;
}

void myMethode(Mat frame)
{
	Mat frame_gray;
	/// Convert it to gray
	cvtColor(frame, frame_gray, CV_BGR2GRAY);

	Mat blurredGray;
	/// Reduce the noise so we avoid false circle detection
	GaussianBlur(frame_gray, blurredGray, Size(9, 9), 2, 2);

	//imshow(window_name, frame_gray);

	vector<Vec3f> circles;

	/// Apply the Hough Transform to find the circles
	HoughCircles(blurredGray, circles, CV_HOUGH_GRADIENT, 1, minDist, upperThreshold, centersThreshold, minRadius, maxRadius);

	arrayOfPoints.RefreshArray(circles);
	vector<Vec3f> pointsToDraw = arrayOfPoints.GetDrawablePoints();
	for (int i = 0; i < pointsToDraw.size(); i++)
	{
		Point center(cvRound(pointsToDraw[i][0]), cvRound(pointsToDraw[i][1]));
		int radius = cvRound(pointsToDraw[i][2]);
		// circle center
		circle(frame, center, 3, Scalar(0, 255, 0), -1, 8, 0);
		// circle outline
		circle(frame, center, radius, Scalar(0, 0, 255), 3, 8, 0);
	}


	cout << "Chwilowa iloœæ obiektów: " << circles.size() << endl;

	circles.clear();
	pointsToDraw.clear();

	imshow(window_name, frame);
	frame.release();
	frame_gray.release();
}
