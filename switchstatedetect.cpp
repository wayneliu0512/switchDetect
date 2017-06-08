#include "switchstatedetect.h"
#include "square.h"

switchStateDetect::switchStateDetect()
{}

void switchStateDetect::doSwitchStateDetect(Mat cut, switchDirection direction, QList<Square> *inputList)
{
    Mat ROI, ROI1, ref, ref_gray, ref_gray_thresh;
    Rect re1, re;
    QString right, left;
    int threshR = 200, threshL = 200 , thresh = 0;
    ref = cut;
    cvtColor(ref, ref_gray, COLOR_BGR2GRAY);
    blur(ref_gray, ref_gray, Size(3,3));

    threshold(ref_gray, ref_gray_thresh, thresh, 255, THRESH_BINARY | THRESH_OTSU);

    vector<vector<Point> > contours;

    findContours(ref_gray_thresh, contours, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

    vector<Rect> boundRect( contours.size() );

    if(direction == VERTICAL)
    {
        for(int i=0;i<contours.size();i++)
        {
            boundRect[i] = boundingRect(contours[i]);//enclose in Rect
            Rect rex;
            if(boundRect[i].width>14 && boundRect[i].height>30)//ignore noise rects
            {
                rex = boundRect[i];
                if( rex.x < ref_gray_thresh.cols/2)
                {
                    re1 = boundRect[i];
                    ROI1=ref_gray(boundRect[i]);
                }else
                {
                    re = boundRect[i];
                    ROI=ref_gray(boundRect[i]);
                }
            }
            if((!ROI.empty()) && (!ROI1.empty())){  break;}
        }

        if(ROI.empty() || ROI1.empty() )
        {
            cout << "error1" << endl;
            return exit(1);
        }

        boundRect.clear();
        contours.clear();

        threshR = (ref_gray.at<uchar>(re.y+(re.height/4*3), re.x+(re.width/2)) + ref_gray.at<uchar>(re.y+(re.height/4), re.x+(re.width/2)))/2;
        threshL = (ref_gray.at<uchar>(re1.y+(re1.height/4*3), re1.x+(re.width/2)) + ref_gray.at<uchar>(re1.y+(re1.height/4), re1.x+(re1.width/2)))/2;

        threshold(ROI, ROI, threshR, 255, THRESH_BINARY);
        threshold(ROI1, ROI1, threshL, 255, THRESH_BINARY);

        right = rollMat(1, ROI, ROI1);
        left = rollMat(0, ROI, ROI1);
    }else
    {
        for(int i=0;i<contours.size();i++)
        {
            boundRect[i] = boundingRect(contours[i]);//enclose in Rect
            Rect rex;
            if(boundRect[i].width>30 && boundRect[i].height>14)//ignore noise rects
            {
                rex = boundRect[i];
                if( rex.y > ref_gray_thresh.rows/2)
                {
                    re1 = boundRect[i];
                    ROI1=ref_gray(boundRect[i]);
                }else
                {
                    re = boundRect[i];
                    ROI=ref_gray(boundRect[i]);
                }
            }
            if((!ROI.empty()) && (!ROI1.empty())){  break;}
        }

        if(ROI.empty() || ROI1.empty() )
        {
            cout << "error1" << endl;
            return exit(1);
        }

        boundRect.clear();
        contours.clear();

        threshR = (ref_gray.at<uchar>(re.y+(re.height/2), re.x+(re.width/4*3)) + ref_gray.at<uchar>(re.y+(re.height/2), re.x+(re.width/4)))/2;
        threshL = (ref_gray.at<uchar>(re1.y+(re1.height/2), re1.x+(re.width/4*3)) + ref_gray.at<uchar>(re1.y+(re1.height/2), re1.x+(re1.width/4)))/2;

        threshold(ROI, ROI, threshR, 255, THRESH_BINARY);
        threshold(ROI1, ROI1, threshL, 255, THRESH_BINARY);

        right = rollMat(3, ROI, ROI1);
        left = rollMat(2, ROI, ROI1);
    }


    if(right == "up" && left == "up")
    {
        inputList->last().switchState = "^^";
    }
    else if(right == "up" && left == "down")
    {
        inputList->last().switchState = "v^";
    }
    else if(right == "down" && left == "up")
    {
        inputList->last().switchState = "^v";
    }
    else if(right == "down" && left == "down")
    {
        inputList->last().switchState = "vv";
    }

    namedWindow("Right", WINDOW_FREERATIO);
    imshow("Right",ROI);

    namedWindow("Left", WINDOW_FREERATIO);
    imshow("Left",ROI1);
}

QString switchStateDetect::rollMat(int direction, Mat ROI, Mat ROI1)
{
    Mat_<uchar>::iterator it;
    Mat_<uchar>::iterator itend;
    int Ucount = 0 , Dcount =0;

    if(direction == 1 || direction == 3)
    {
        it = ROI.begin<uchar>();
        itend = ROI.end<uchar>();
    }else
    {
        it = ROI1.begin<uchar>();
        itend = ROI1.end<uchar>();
    }

    if(direction == 0 || direction == 1)
    {
        for(;it!=itend;it++){
            if(*it == 255 && (it.pos().y > ROI.rows/2))
            {
                Dcount++;
            }else if(*it == 255 && (it.pos().y < ROI.rows/2))
            {
                Ucount++;
            }
        }
    }else
    {
        for(;it!=itend;it++){
            if(*it == 255 && (it.pos().x > ROI.cols/2))
            {
                Dcount++;
            }else if(*it == 255 && (it.pos().x < ROI.cols/2))
            {
                Ucount++;
            }
        }
    }


    if(Ucount > Dcount)
    {
        return "up";
    }else{
        return "down";
    }
}
