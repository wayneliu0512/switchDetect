#ifndef SQUARE_H
#define SQUARE_H

#include <QtCore>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

class Square
{
public:
    Square();

    Square(int location_x, int location_y, QString switchState_);

    Square(Point location_);

    QPoint getQPoint() const;

    void setCenterPoint(Point point);

    Point getCenterPoint() const;

    Point location;
    int x;
    int y;
    QString switchState;

private:

    Point centerPoint;
};

#endif // SQUARE_H
