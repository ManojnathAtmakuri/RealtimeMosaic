#include<stdio.h>
#include<iostream>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp> 


using namespace cv;
using namespace std;

Mat frame;
Point p, p1;
Rect r;
int onMove;
int bd = 5;
vector<Rect> rect;
void noBlur(Mat &frame)
{
	bd = 1;
}
void incBlur(Mat &frame) {

	if (bd < 5) {
		bd = bd + 1;

	}
	else {

		bd = bd + 5;
	}

}
void decBlur(Mat &frame) {

	if (bd <= 5) {
		bd = bd - 1;

	}
	else {

		bd = bd - 5;
	}

}
void mosaicBlock(Mat &frame) {
	for (int n = 0; n < rect.size();n++)
	{
		Rect r = rect[n];
		Point point0 = Point(r.x, r.y);
		Point point1 = Point(r.x + r.width, r.y);
		Point point2 = Point(r.x + r.width, r.y + r.height);
		Point point3 = Point(r.x, r.y + r.height);
		double red = 0, green = 0, blue = 0;

		int total = bd * bd;

		int rows = (point2.x - point0.x) / bd;
		int cols = (point2.y - point0.y) / bd;
		for (int i = 0; i <= rows; i = i + 1)
		{
			for (int j = 0; j <= cols; j = j + 1) {

				int x1 = point0.x + i * (bd);
				int y1 = point0.y + j * (bd);
				for (int y = y1; y < y1 + bd;y++)
				{
					for (int x = x1; x < x1 + bd; x++) {

						red += frame.ptr<uchar>(y)[3 * x + 0];
						green += frame.ptr<uchar>(y)[3 * x + 1];
						blue += frame.ptr<uchar>(y)[3 * x + 2];

					}
				}


				double	r = red / total;
				double	g = green / total;
				double	b = blue / total;


				red = 0;
				green = 0;
				blue = 0;





				for (int y = y1; y < y1 + (bd) && y < point2.y; y++)
				{
					for (int x = x1; x < x1 + (bd) && x < point2.y; x++) {

						(frame.ptr<uchar>(y)[3 * x + 0]) = r;
						(frame.ptr<uchar>(y)[3 * x + 1]) = g;
						(frame.ptr<uchar>(y)[3 * x + 2]) = b;


					}
				}
			}

		}


	}

}

void myMouse(int event, int x, int y, int flags, void* param)
{
	if (event == CV_EVENT_LBUTTONDOWN)
	{
		onMove = 1;
		p.x = x;
		p.y = y;
		r.x = x;
		r.y = y;
		r.height = 0;
		r.width = 0;

	}
	else if (event == CV_EVENT_MOUSEMOVE)
	{
		if (onMove == 0)
			return;
		int dx = abs(p.x - x);
		int dy = abs(p.y - y);

		if (x < p.x) {
			r.x = x;
			r.width = abs(x - p.x);

		}
		else {
			r.width = dx;
		}

		if (y < p.y) {
			r.y = y;
			r.height = abs(y - p.y);
		}
		else {
			r.height = dy;

		}

	}
	else if (event == CV_EVENT_LBUTTONUP) {
		onMove = 0;
		rect.push_back(r);

	}
}
int main(int argc, char* argv[])
{
	VideoCapture camera(0);
	if (!camera.isOpened())
	{
		cout << "change the cam" << endl;
		return -1;

	}
	namedWindow("camera");
	setMouseCallback("camera", myMouse);
	while (1)
	{

		camera.read(frame);
		for (int m = 0; m < rect.size(); m++)
		{
			Rect rx = rect[m];
			rectangle(frame, rx, Scalar(0, 255, 0));
			//cout << rect.size();
		}
		mosaicBlock(frame);
		imshow("camera", frame);
		char c = waitKey(1);
		if (c == 32)
		{
			break;
		}
		else if (c == 'i' || c == 'I') {

			incBlur(frame);
		}
		else if (c == 'd' || c == 'D') {
			decBlur(frame);
		}
		else if (c == 's' || c == 'S') {

			imwrite("C:/BlurMosaic.bmp", frame);
		}
		else if (c == 'r' || c == 'R')
		{
			noBlur(frame);
		}
	}


}

