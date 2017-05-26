#include "switchstatedetect.h"
#include "square.h"

switchStateDetect::switchStateDetect()
{}

void switchStateDetect::doSwitchStateDetect(Mat cut, switchDirection direction, QList<Square> *inputList)
{
    static int count = 0;
    count++;

    Mat cut_gray;
    cvtColor(cut, cut_gray, COLOR_BGR2GRAY);
    blur(cut_gray, cut_gray, Size(3,3));

    Mat threshold_output;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    //I got some problem here , So thresh value can't too high.
    int thresh = 195;

    threshold(cut_gray, threshold_output, thresh, 255, THRESH_BINARY);
    namedWindow(QString::number(count).toStdString() + "thresh", WINDOW_FREERATIO);
    imshow(QString::number(count).toStdString() + "thresh", threshold_output);
    findContours( threshold_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0) );

    //ERROR
    if(contours.size() < 2)
    {
        inputList->last().switchState = "ERROR: contours less than two.(Try to adjust threshold)";
        return;
    }

    vector<vector<Point> > contours_poly( contours.size() );
    vector<Rect> boundRect( contours.size() );
    vector<Point2f>center( contours.size() );
    vector<float>radius( contours.size() );

    double maxContourArea[2] = {0, 0};
    int maxContour[2];
    //Find max 2 contour & contour area
    for(size_t i = 0; i < contours.size(); i++)
    {
        approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
        double temp = contourArea(contours_poly[i]);

        if(temp > maxContourArea[0])
        {
            maxContourArea[0] = temp;
            maxContour[0] = i;
        }
    }
    for(size_t i = 0; i < contours.size(); i++)
    {
        double temp = contourArea(contours_poly[i]);
        if(i == maxContour[0])
        {
            temp = 0;
        }else if(temp > maxContourArea[1])
        {
            maxContourArea[1] = temp;
            maxContour[1] = i;
        }
    }

    //ERROR
    for(size_t i = 0; i < 2; i++)
    {
        if(maxContourArea[i] <= 35 )
        {
            inputList->last().switchState = "ERROR: ContourArea too small.(Try to adjust threshold)";
            return;
        }else if(maxContourArea[i] >= 200)
        {
            inputList->last().switchState = "ERROR: ContourArea too big.(Try to adjust threshold)";
            return;
        }
    }

//    cout << count << "Area:" << maxContourArea[0] << endl;
//    cout << count << "Area1:" << maxContourArea[1] << endl;

//    minEnclosingCircle( contours_poly[maxContour[0]], center[maxContour[0]], radius[maxContour[0]] );
//    minEnclosingCircle( contours_poly[maxContour[1]], center[maxContour[1]], radius[maxContour[1]] );

    Point loc1 = center[maxContour[0]];
    Point loc2 = center[maxContour[1]];


    if(direction == VERTICAL)
    {
        if(loc1.x > loc2.x)
        {
            if(loc1.y - loc2.y >= 10){ inputList->last().switchState = "NY-O1";}
            else if(loc2.y - loc1.y >= 10){ inputList->last().switchState = "N1-OY";}
            else if((loc1.y + loc2.y) >= threshold_output.rows){inputList->last().switchState = "N1-O1";}
            else if((loc1.y + loc2.y) <= threshold_output.rows){ inputList->last().switchState = "NY-OY";}
            else{ inputList->last().switchState = "Error";}
        }else{
            if(loc2.y - loc1.y >= 10){ inputList->last().switchState = "NY-O1";}
            else if(loc1.y - loc2.y >= 10){ inputList->last().switchState = "N1-OY";}
            else if((loc1.y + loc2.y) >= threshold_output.rows){ inputList->last().switchState = "N1-O1";}
            else if((loc1.y + loc2.y) <= threshold_output.rows){ inputList->last().switchState = "NY-OY";}
            else{ inputList->last().switchState = "Error";}
        }
    }else if(direction == HORIZONTAL)
    {
        if(loc1.y > loc2.y)
        {
            if(loc2.x - loc1.x >= 10){ inputList->last().switchState = "NY-O1";}
            else if(loc1.x - loc2.x >= 10){ inputList->last().switchState = "N1-OY";}
            else if((loc1.x + loc2.x) >= threshold_output.cols){ inputList->last().switchState = "N1-O1";}
            else if((loc1.x + loc2.x) <= threshold_output.cols){ inputList->last().switchState = "NY-OY";}
            else{ inputList->last().switchState = "Error";}
        }else{
            if(loc1.x - loc2.x >= 10){ inputList->last().switchState = "NY-O1";}
            else if(loc2.x - loc1.x >= 10){ inputList->last().switchState = "N1-OY";}
            else if((loc1.x + loc2.x) >= threshold_output.cols){ inputList->last().switchState = "N1-O1";}
            else if((loc1.x + loc2.x) <= threshold_output.cols){ inputList->last().switchState = "NY-OY";}
            else{ inputList->last().switchState = "Error";}
        }
    }

}
