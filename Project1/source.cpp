#include <opencv2\opencv.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>
#include "ArrayOfDrawablePoints.h"

using namespace std;
using namespace cv;

/* Uchwyt funkcji */
void detectAndDisplay(Ptr<SimpleBlobDetector> detector, Mat frame);
void myMethode(Mat frame);

/* Zmienne globalne */
String window_name = "Detekcja oczek na kostce do gry!";
SimpleBlobDetector::Params params;
int oczka;
int sizeOfBuffor = 5;
int current = 0;
vector<int> buffor;
ArrayOfDrawablePoints arrayOfPoints;
int temp = 0;
int maxSze = 10;

void addToBuffor(int element)
{
	current++;
	if (buffor.size() < sizeOfBuffor)
	{
		buffor.push_back(element);
	}
	else
	{
		buffor[current%sizeOfBuffor] = element;
	}
}

int averageOfBuffor()
{
	int result = 0;
	for (int i = 0; i < buffor.size(); i++)
	{
		result += buffor[i];
	}
	return result / buffor.size();
}

struct MinMaxCoordinates {
	int xMin; int yMin; int xMax; int yMax;
};

bool isInRange(int x, int y, int xMin, int yMin, int xMax, int yMax)
{
	return xMin <= x && x <= xMax && yMin <= y && y <= yMax;
}

int circlesInSpecifiedRange(int xMin, int yMin, int xMax, int yMax, vector<KeyPoint> circles)
{
	int result = 0;
	for (int i = 0; i < circles.size(); i++)
	{
		if (isInRange(circles[i].pt.x, circles[i].pt.y, xMin, yMin, xMax, yMax))
			result++;
	}
	return result;
}

int circlesInAllRanges(vector<KeyPoint> circles, vector<MinMaxCoordinates> ranges)
{
	int result = 0;

	for (int i = 0; i < ranges.size(); i++)
	{
		result += circlesInSpecifiedRange(ranges[i].xMin, ranges[i].yMin, ranges[i].xMax, ranges[i].yMax, circles);
	}

	return result;
}

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

	params.minDistBetweenBlobs = 1;

	// Filter by Area.
	params.filterByArea = true;
	params.minArea = 20;
	params.maxArea = 100;

	// Filter by Thresholds
	params.minThreshold = 0;

	// Filter by Circularity
	params.filterByCircularity = true;
	params.minCircularity = 0;

	// Filter by Convexity
	params.filterByConvexity = true;
	params.minConvexity = 0;

	// Filter by Inertia
	params.filterByInertia = true;
	params.minInertiaRatio = 0;

	while (capture.read(frame))
	{
		if (frame.empty())
		{
			printf("Brak przechwytywanego obrazu, break!");
			break;
		}

		Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);
		oczka = 0;

		//Wywolanie uchwytu 
		detectAndDisplay(detector, frame);
		cvtColor(frame, frame_amount_color, COLOR_BGR2HLS_FULL);

		int c = waitKey(100);
		if ((char)c == 27) { break; } // escape

		if ((char)c == 49) { params.minArea += 10; } //1
		if ((char)c == 113) { params.minArea -= 10; } //q

		if ((char)c == 51) { params.minThreshold += 8; } //3
		if ((char)c == 101) { params.minThreshold -= 8; } //e

		if ((char)c == 52) { params.minCircularity += 0.1; } //4
		if ((char)c == 114) { params.minCircularity -= 0.1; } //r

		if ((char)c == 53) { params.minInertiaRatio += 0.1; } //5
		if ((char)c == 116) { params.minInertiaRatio -= 0.1; } //t

		if ((char)c == 54) { params.minConvexity += 0.1; } //6
		if ((char)c == 121) { params.minConvexity -= 0.1; } //y

		if ((char)c == 'n') { params.maxArea += 0.1; }
		if ((char)c == 'b') { params.maxArea -= 0.1; }


		system("cls");
		cout << "H - odcien; L - jasnoœæ, S - nasycenie" << endl;
		cout << "Light: " << mean(frame_amount_color) << endl;
		cout << "Area: " << params.minArea << endl;
		cout << "Thre: " << params.minThreshold << endl;
		cout << "Cir: " << params.minCircularity << endl;
		cout << "Ine: " << params.minInertiaRatio << endl;
		cout << "Con: " << params.minConvexity << endl;
		cout << "LICZBA OCZEK: " << oczka << endl;
		cout << "Nowa metoda: " << arrayOfPoints.GetDrawablePoints().size() << endl;
		cout << temp<<endl;
		cout << params.maxArea;
		//myMethode(frame);
	}
	return 0;
}

void myMethode(Mat frame)
{
	Mat frame_gray;
	/// Convert it to gray
	cvtColor(frame, frame_gray, CV_BGR2GRAY);

	/// Reduce the noise so we avoid false circle detection
	GaussianBlur(frame_gray, frame_gray, Size(9, 9), 2, 2);

	//imshow(window_name, frame_gray);

	vector<Vec3f> circles;

	/// Apply the Hough Transform to find the circles
	HoughCircles(frame_gray, circles, CV_HOUGH_GRADIENT, 1, frame_gray.rows / 50, 200, 20, 0, maxSze);

	for (size_t i = 0; i < circles.size(); i++)
	{
		Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		int radius = cvRound(circles[i][2]);
		// circle center
		circle(frame, center, 3, Scalar(0, 255, 0), -1, 8, 0);
		// circle outline
		circle(frame, center, radius, Scalar(0, 0, 255), 3, 8, 0);
	}

	imshow(window_name, frame);

	temp = circles.size();
}

void detectAndDisplay(Ptr<SimpleBlobDetector> detector, Mat frame)
{
	Mat frame_gray;

	//Rozmycie obrazu za pomoc¹ filtru Gaussa w celu usuniêcia szumów 
	GaussianBlur(frame, frame, Size(5, 5), 1.5, -0.5, BORDER_DEFAULT);
	//imshow(window_name, frame);
	//Konwertuje obraz z jednej przestrzeni kolorów na inn¹
	cvtColor(frame, frame_gray, COLOR_BGR2GRAY);

	//Wyrównywanie histogramu w skali szaroœci
	equalizeHist(frame_gray, frame_gray);

	vector<KeyPoint> keypoints;
	detector->detect(frame_gray, keypoints);
	oczka = (int)keypoints.size();
	//addToBuffor(oczka);
	//arrayOfPoints.RefreshArray(keypoints);

	//Rysuje punkty kluczowe.
	//drawKeypoints(Obraz Ÿród³owy, punkty kluczowe, obraz wyjœciowy, kolor punktów bazowych, funkcje rysowania flag)
	drawKeypoints(frame, keypoints, frame, Scalar(0, 255, 255), 4);
	// DRAW_RICH_KEYPOINTS = 4 <Dla ka¿dego punktu kluczowego zostanie narysowany okr¹g wokó³ // punktu kluczowego o rozmiarze i orientacji klucza>

	// Wyœwietlenie obrazu
	imshow(window_name, frame);
}