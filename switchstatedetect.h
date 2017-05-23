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

    static void doSwitchStateDetect(Mat cut, switchDirection direction, QList<Square> *inputList);

};

#endif // SWITCHSTATEDETECT_H
