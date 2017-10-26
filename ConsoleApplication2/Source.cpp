#include "stdafx.h"
#include "opencv2/objdetect.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <stdio.h>
using namespace std;
using namespace cv;
bool isSleepy();
class Detection {
protected:

	String face_cascade_name, eyes_cascade_name;
	CascadeClassifier face_cascade;
	CascadeClassifier eyes_cascade;
	String window_name = "Capture - Face detection";
	static size_t time, n;

	virtual void detect(Mat frame)
	{
		cout << "Mains Detect" << endl;
	}

	friend bool isSleepy();

};
size_t Detection::time = 0;
size_t Detection::n = 1;
bool isSleepy() {
	double sleepIndex = (double)Detection::time / Detection::n;
	//cout<<Detection::time<<" "<<Detection::n<<" " << sleepIndex << endl;
	//cout << sleepIndex << endl;
	if (sleepIndex < 1.6)
	{
		cout << "YOU ARE SLEEPY !!YOU NEED SLEEP" << endl;

		return true;
	}
	else
	{
		//cout << "YOU ARE NOT SLEEPY !! " << endl;
		return false;
	}

}
class FaceDetection :virtual protected Detection {

protected:
	void detectF(Mat frame)
	{
		//n++;
		std::vector<Rect> faces;
		Mat frame_gray;


		cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
		equalizeHist(frame_gray, frame_gray);

		//-- Detect faces
		face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

		for (size_t i = 0; i < faces.size(); i++)
		{
			Point center(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 2);
			ellipse(frame, center, Size(faces[i].width / 2, faces[i].height / 2), 0, 0, 360, Scalar(255, 0, 255), 4, 8, 0);
			rectangle(frame, Point(faces[i].x, faces[i].y), Point(faces[i].x + faces[i].width, faces[i].y + faces[i].height), Scalar(0, 255, 255), 8, 8);
			Mat faceROI = frame_gray(faces[i]);
			std::vector<Rect> eyes;

			//-- In each face, detect eyes
			eyes_cascade.detectMultiScale(faceROI, eyes, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));
			//time += eyes.size();


		}
		//-- Show what you got
		imshow(window_name, frame);

	}

};

class EyeDetection :virtual protected Detection {
protected:
	void detectE(Mat frame)
	{
		//n++;
		std::vector<Rect> faces;
		Mat frame_gray;


		cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
		equalizeHist(frame_gray, frame_gray);

		//-- Detect faces
		face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

		for (size_t i = 0; i < faces.size(); i++)
		{
			Point center(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 2);
			ellipse(frame, center, Size(faces[i].width / 2, faces[i].height / 2), 0, 0, 360, Scalar(255, 0, 255), 4, 8, 0);
			rectangle(frame, Point(faces[i].x, faces[i].y), Point(faces[i].x + faces[i].width, faces[i].y + faces[i].height), Scalar(0, 255, 255), 8, 8);
			Mat faceROI = frame_gray(faces[i]);
			std::vector<Rect> eyes;

			//-- In each face, detect eyes
			eyes_cascade.detectMultiScale(faceROI, eyes, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));
			//cout << eyes.size() << endl;
			//time += eyes.size();
			//cout << eyes.size() << endl;
			for (size_t j = 0; j < eyes.size(); j++)
			{

				Point eye_center(faces[i].x + eyes[j].x + eyes[j].width / 2, faces[i].y + eyes[j].y + eyes[j].height / 2);
				int radius = cvRound((eyes[j].width + eyes[j].height)*0.25);
				circle(frame, eye_center, radius, Scalar(255, 0, 0), 4, 8, 0);
			}
		}
		//-- Show what you got
		imshow(window_name, frame);

	}

	void detectES(Mat frame)
	{

		std::vector<Rect> faces;
		Mat frame_gray;

		n++;
		cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
		equalizeHist(frame_gray, frame_gray);

		//-- Detect faces

		face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

		for (size_t i = 0; i < faces.size(); i++)
		{
			Point center(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 2);
			ellipse(frame, center, Size(faces[i].width / 2, faces[i].height / 2), 0, 0, 360, Scalar(255, 0, 255), 4, 8, 0);
			rectangle(frame, Point(faces[i].x, faces[i].y), Point(faces[i].x + faces[i].width, faces[i].y + faces[i].height), Scalar(0, 255, 255), 8, 8);
			Mat faceROI = frame_gray(faces[i]);
			std::vector<Rect> eyes;

			//-- In each face, detect eyes
			eyes_cascade.detectMultiScale(faceROI, eyes, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));
			//cout << eyes.size() << endl;
			time += eyes.size();

			//cout << eyes.size() << endl;
			for (size_t j = 0; j < eyes.size(); j++)
			{

				Point eye_center(faces[i].x + eyes[j].x + eyes[j].width / 2, faces[i].y + eyes[j].y + eyes[j].height / 2);
				int radius = cvRound((eyes[j].width + eyes[j].height)*0.25);
				circle(frame, eye_center, radius, Scalar(255, 0, 0), 4, 8, 0);
			}
		}
		//-- Show what you got
		imshow(window_name, frame);

	}

};

class callingclass :public FaceDetection, public EyeDetection {
public:
	int  caller(String s1, String s2)
	{
		face_cascade_name = s1;
		eyes_cascade_name = s2;

		VideoCapture capture;
		Mat frame;

		//-- 1. Load the cascades
		if (!face_cascade.load(face_cascade_name)) { printf("--(!)Error loading face cascade\n"); return -1; };
		if (!eyes_cascade.load(eyes_cascade_name)) { printf("--(!)Error loading eyes cascade\n"); return -1; };

		//-- 2. Read the video stream
		capture.open(0);
		if (!capture.isOpened()) { printf("--(!)Error opening video capture\n"); return -1; }
		cout << "DETECTION MENU" << endl;
		cout << endl << "PLEASE ENTER YOUR CHOICE" << endl;
		cout << endl << "1.FACE DETECTION" << endl;
		cout << endl << "2.BOTH FACE AND EYE DETECTION " << endl;
		cout << endl << "3.TO KNOW WHETHER OR NOT YOU ARE SLEEPY" << endl;
		cout << endl << "PRESS 0 TO EXIT FROM PROGRAM" << endl;
		cout << endl << "PLEASE PRESS ESCAPE KEY IN ORDER TO EXIT FROM THE CURRENT WEBCAM WINDOW" << endl;
		int ch;

		do {
			cout << endl << "PLEASE ENTER YOUR CHOICE" << endl;
			cin >> ch;
			switch (ch)
			{
			case 1: {

				while (capture.read(frame))
				{
					if (frame.empty())
					{
						printf(" --(!) No captured frame -- Break!");
						break;
					}

					//-- 3. Apply the classifier to the frame
					detectF(frame);

					char c = (char)waitKey(1);
					if (c == 27) { destroyWindow(window_name); break; } // escape
				}

				break;
			}
			case 2: {
				while (capture.read(frame))
				{
					if (frame.empty())
					{
						printf(" --(!) No captured frame -- Break!");
						break;
					}

					//-- 3. Apply the classifier to the frame
					detectE(frame);

					char c = (char)waitKey(1);
					if (c == 27) { destroyWindow(window_name); break; } // escape
				}
				break;
			}
			case 3: {
				Detection::time = 5;
				Detection::n = 1;
				while (capture.read(frame))
				{
					if (frame.empty())
					{
						printf(" --(!) No captured frame -- Break!");
						break;
					}

					//-- 3. Apply the classifier to the frame
					detectES(frame);

					char c = (char)waitKey(1);
					if (c == 27 || isSleepy()) { destroyWindow(window_name); break; } // escape
				}
				break;
			}

			default:break;
			}
		} while (ch != 0);
		return 0;

	}
};

/** @function main */
int main(int argc, const char** argv)
{
	CommandLineParser parser(argc, argv,
		"{help h||}"
		"{face_cascade|../../data/haarcascades/haarcascade_frontalface_alt.xml|}"
		"{eyes_cascade|../../data/haarcascades/haarcascade_eye_tree_eyeglasses.xml|}");

	//cout << "\nThsifier class to detect objects (Face + eyes) in a video stream.\n");
	//parser.printMessage();

	String f1, e1;
	f1 = parser.get<string>("face_cascade");
	e1 = parser.get<string>("eyes_cascade");
	callingclass c1;
	c1.caller(f1, e1);
	//destroyWindow(window_name);
	//cout << "HI";


	return 0;
}