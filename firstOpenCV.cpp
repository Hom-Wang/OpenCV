#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

#define cvPixel(_FRAME, _X, _Y, _INDEX)    (_FRAME.ptr<uint8_t>(_X))[j * 3 + _INDEX]

static int mpos[2] = {0};
static int hsv_val[3][2] = {{0, 10}, {100, 255}, {100, 255}};

void mouse(int event, int x, int y, int flags, void* param)
{
    mpos[0] = x;
    mpos[1] = y;
    printf("x: %d, y: %d\n", mpos[0], mpos[1]);
}

int main()
{
    VideoCapture cap(0);
    if (!cap.isOpened())
        return -1;

    Mat frame;
    cap.read(frame);

    cout << "frame size: " << frame.rows << ", " << frame.cols << endl;
    cout << "channels: "   << frame.channels() << endl;

    namedWindow("cam", WINDOW_AUTOSIZE);
    namedWindow("hsv", WINDOW_AUTOSIZE);
    namedWindow("threshold bar", WINDOW_NORMAL);

    cvSetMouseCallback("cam", mouse);
    cvCreateTrackbar("H min", "threshold bar", &hsv_val[0][0], 255, NULL);
    cvCreateTrackbar("H max", "threshold bar", &hsv_val[0][1], 255, NULL);
    cvCreateTrackbar("S min", "threshold bar", &hsv_val[1][0], 255, NULL);
    cvCreateTrackbar("S max", "threshold bar", &hsv_val[1][1], 255, NULL);
    cvCreateTrackbar("V min", "threshold bar", &hsv_val[2][0], 255, NULL);
    cvCreateTrackbar("V max", "threshold bar", &hsv_val[2][1], 255, NULL);

    for (;;) {
        cap.read(frame);

        Mat recFrame = frame;

        rectangle(frame, Point(mpos[0]-50, mpos[1]-50), Point(mpos[0]+50, mpos[1]+50), Scalar(0, 255, 0), 4, 0);
        imshow("cam", recFrame);

        Mat imgHSV;
        cvtColor(frame, imgHSV, COLOR_BGR2HSV);
        inRange(imgHSV, Scalar(hsv_val[0][0], hsv_val[1][0], hsv_val[2][0]), Scalar(hsv_val[0][1], hsv_val[1][1], hsv_val[2][1]), imgHSV);
        imshow("hsv", imgHSV);

        if (waitKey(30) >= 0)
            break;
    }

    return 0;
}
