#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

#define cvPixel(_FRAME, _X, _Y, _INDEX)    (_FRAME.ptr<uint8_t>(_X))[_Y * 3 + _INDEX]
#define cvPixel8(_FRAME, _X, _Y)           (_FRAME.ptr<uint8_t>(_X))[_Y]

static int mPos[2] = {0};
void mouse(int event, int x, int y, int flags, void* param)
{
    mPos[0] = x;
    mPos[1] = y;
    printf("x: %d, y: %d\n", mPos[0], mPos[1]);
}

int hsv_val[3][2] = {{100, 110}, {90, 255}, {90, 255}};

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

        rectangle(frame, Point(mPos[0]-50, mPos[1]-50), Point(mPos[0]+50, mPos[1]+50), Scalar(0, 255, 0), 4, 0);
        imshow("cam", recFrame);

        Mat imgRGB = frame;
        Mat imgHSV, imgHSV_R, imgHSV_B;
        cvtColor(frame, imgHSV, COLOR_BGR2HSV);
//        inRange(imgHSV, Scalar(hsv_val[0][0], hsv_val[1][0], hsv_val[2][0]), Scalar(hsv_val[0][1], hsv_val[1][1], hsv_val[2][1]), imgHSV);
        inRange(imgHSV, Scalar(  0, hsv_val[1][0], hsv_val[2][0]), Scalar( 10, hsv_val[1][1], hsv_val[2][1]), imgHSV_R);
        inRange(imgHSV, Scalar(100, hsv_val[1][0], hsv_val[2][0]), Scalar(120, hsv_val[1][1], hsv_val[2][1]), imgHSV_B);
        for (int i = 0; i < imgHSV.rows; i++) {
            for (int j = 0; j < imgHSV.cols; j++) {
                if (cvPixel8(imgHSV_R, i, j) > 128) {
                    cvPixel(imgRGB, i, j, 0) = 0;
                    cvPixel(imgRGB, i, j, 1) = 0;
                    cvPixel(imgRGB, i, j, 2) = 255;
                }
                else if (cvPixel8(imgHSV_B, i, j) > 128) {
                    cvPixel(imgRGB, i, j, 0) = 255;
                    cvPixel(imgRGB, i, j, 1) = 0;
                    cvPixel(imgRGB, i, j, 2) = 0;
                }
                else {
                    cvPixel(imgRGB, i, j, 0) = 0;
                    cvPixel(imgRGB, i, j, 1) = 0;
                    cvPixel(imgRGB, i, j, 2) = 0;
                }
            }
        }
        imshow("hsv", imgRGB);

        if (waitKey(30) >= 0)
            break;
    }

    return 0;
}
