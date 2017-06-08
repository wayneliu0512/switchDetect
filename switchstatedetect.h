#ifndef SWITCHSTATEDETECT_H
#define SWITCHSTATEDETECT_H
#include <square.h>
#include <QtCore>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

class switchStateDetect
{
public:

    switchStateDetect();

    enum switchDirection{  VERTICAL = 0, HORIZONTAL};

    //Vertical : right = 1, left = 0   Horizontal : right = 3, left = 2
    static QString rollMat(int direction , Mat ROI , Mat ROI1);

    static void doSwitchStateDetect(Mat cut, switchDirection direction, QList<Square> *inputList);

};

#endif // SWITCHSTATEDETECT_H
