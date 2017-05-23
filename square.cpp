#include "square.h"

Square::Square()
{}


Square::Square(int location_x, int location_y, QString switchState_)
{
    x = location_x;
    y = location_y;
    switchState = switchState_;
    location = Point(location_x, location_y);
}

Square::Square(Point location_)
{
    location = location_;
    x = location_.x;
    y = location_.y;
}

QPoint Square::getQPoint() const
{
    QPoint point(x, y);
    return point;
}

void Square::setCenterPoint(Point point)
{
    centerPoint = point;
}

Point Square::getCenterPoint() const
{
    return centerPoint;
}
