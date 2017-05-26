#ifndef SWITCHDETECT_H
#define SWITCHDETECT_H

#include <square.h>
#include <QtCore>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

class switchDetect
{
public:

    switchDetect(Mat tempH_, Mat tempV_, Mat ref_, int switchNum_);

    void startDetect();

    void showResult();

    void loadSettings(QString fileName);

    void saveSettings(QString fileName);

    void resultMatching();

    bool distanceAllowRange(const Point &point, int distance);

    QList<Square> *getOutputList();

private:

    QList<Square> *inputList, *loadList;

    Mat ref, gtempH, gtempV, resH, resV;

    int switchNum;
};

#endif // SWITCHDETECT_H
