#include "switchdetect.h"
#include "square.h"
#include "switchstatedetect.h"

switchDetect::switchDetect(Mat temp_H_, Mat temp_V_, Mat ref_, int switchNum_)
{
    ref = ref_;
    switchNum = switchNum_;
    inputList = new QList<Square>;
    loadList = new QList<Square>;
    Mat gref;


    gref = ref;
    gtempH = temp_H_;
    gtempV = temp_V_;

//    cvtColor(ref, gref, COLOR_BGR2GRAY);
//    cvtColor(temp_H_, gtempH, COLOR_BGR2GRAY);
//    cvtColor(temp_V_, gtempV, COLOR_BGR2GRAY);

//    threshold(gref, gref, 100, 255, CV_THRESH_TOZERO);
//    threshold(gtempH, gtempH, 100, 255, CV_THRESH_TOZERO);
//    threshold(gtempV, gtempV, 100, 255, CV_THRESH_TOZERO);

//    namedWindow("ref", WINDOW_FREERATIO);
//    imshow("ref", gref);
//    namedWindow("tempH", WINDOW_FREERATIO);
//    imshow("tempH", gtempH);
//    namedWindow("tempV", WINDOW_FREERATIO);
//    imshow("tempV", gtempV);

    //TemplateMatching...
    resV = Mat(ref.rows-temp_V_.rows+1, ref.cols-temp_V_.cols+1, CV_32FC1);
    matchTemplate(gref, gtempV, resV, CV_TM_CCOEFF_NORMED);
    threshold(resV, resV, 0.3, 1., CV_THRESH_TOZERO);

    resH = Mat(ref.rows-temp_H_.rows+1, ref.cols-temp_H_.cols+1, CV_32FC1);
    matchTemplate(gref, gtempH, resH, CV_TM_CCOEFF_NORMED);
    threshold(resH, resH, 0.3, 1., CV_THRESH_TOZERO);


    namedWindow("H", WINDOW_FREERATIO);
    imshow("H", resH);

    namedWindow("V", WINDOW_FREERATIO);
    imshow("V", resV);
}

void switchDetect::startDetect()
{
    int count = 0;
    //Start to cutting switch out of the sample picture
    while(1)
    {
        Mat cutV, cutH;
        double minvalV, maxvalV, minvalH, maxvalH;
        Point minlocV, maxlocV, minlocH, maxlocH;
        minMaxLoc(resV, &minvalV, &maxvalV, &minlocV, &maxlocV);
        Rect rectV(Point(maxlocV.x, maxlocV.y + (gtempV.rows/3)), Point(maxlocV.x + gtempV.cols, maxlocV.y + (gtempV.rows*0.6)));
        minMaxLoc(resH, &minvalH, &maxvalH, &minlocH, &maxlocH);
        Rect rectH(Point(maxlocH.x + (gtempH.cols/3), maxlocH.y), Point(maxlocH.x + (gtempH.cols*0.6), maxlocH.y + gtempH.rows));

        if (count < switchNum)
        {           
            if(maxvalV >= maxvalH)
            {
                if(!distanceAllowRange(maxlocV, 80))
                {
                    floodFill(resV, maxlocV, Scalar(0), 0, Scalar(.1), Scalar(1.));
                    continue;
                }
                inputList->append(Square(maxlocV));
                inputList->last().setCenterPoint(Point(maxlocV.x +  gtempV.cols/2, maxlocV.y + gtempV.rows/2));
                cutV = ref(rectV).clone();
                rectangle(ref, rectV, CV_RGB(255,255,0), 5);
                floodFill(resV, maxlocV, Scalar(0), 0, Scalar(.1), Scalar(1.));
                switchStateDetect::doSwitchStateDetect(cutV, switchStateDetect::VERTICAL, inputList);
            }else if(maxvalV <= maxvalH)
            {
                if(!distanceAllowRange(maxlocH, 80))
                {
                    floodFill(resH, maxlocH, Scalar(0), 0, Scalar(.1), Scalar(1.));
                    continue;
                }
                inputList->append(Square(maxlocH));
                inputList->last().setCenterPoint(Point(maxlocH.x +  gtempH.cols/2, maxlocH.y + gtempH.rows/2));
                cutH = ref(rectH).clone();
                rectangle(ref, rectH, CV_RGB(255,255,0), 5);
                floodFill(resH, maxlocH, Scalar(0), 0, Scalar(.1), Scalar(1.));
                switchStateDetect::doSwitchStateDetect(cutH, switchStateDetect::HORIZONTAL, inputList);
            }
            count++;

        }
        else
        {            
            break;
        }
    }
}

void switchDetect::showResult(){

    namedWindow("ref", WINDOW_NORMAL);
    imshow("ref", ref);

    qDebug() << "Result------------------------";

    QListIterator<Square> outputList(*inputList);

    while(outputList.hasNext())
    {
        Square temp = outputList.next();

        cout << "X:" << temp.x << ", Y:" << temp.y << endl << "SwitchState:" << temp.switchState.toStdString() << endl;
    }
}

QList<Square> *switchDetect::getOutputList()
{
    return inputList;
}

void switchDetect::loadSettings(QString fileName)
{
    QFile file(fileName);

    if(!file.exists())
    {
        cout << "ERROR: fileName doesn't exist." << endl;
        return exit(1);
    }

    QSettings settings(fileName, QSettings::NativeFormat);

    int count = settings.value("SwitchNum").toInt();
    count = count - 1;

    while(count >= 0)
    {
        Square temp;
        temp.x = settings.value(QString::number(count) + "/X").toInt();
        temp.y = settings.value(QString::number(count) + "/Y").toInt();
        temp.switchState = settings.value(QString::number(count) + "/SwitchState").toString();

        loadList->append(temp);

        count--;
    }
}

void switchDetect::saveSettings(QString fileName)
{
    QFile file(fileName);

    if(file.exists())
    {
        cout << "ERROR: fileName already exists." << endl;
        return exit(1);
    }

    QSettings settings(fileName, QSettings::IniFormat);


    QListIterator<Square> outputList(*inputList);

    int count = 0;

    settings.setValue("SwitchNum", inputList->length());

    while(outputList.hasNext())
    {
        Square temp = outputList.next();
        settings.setValue(QString::number(count) + "/X", temp.x);
        settings.setValue(QString::number(count) + "/Y", temp.y);
        settings.setValue(QString::number(count) + "/SwitchState", temp.switchState);

        count++;
    }
}

void switchDetect::resultMatching()
{
    if(loadList->isEmpty())
    {
        cout << "ERROR: loadList is empty" << endl;
        return exit(1);
    }else if(inputList->isEmpty())
    {
        cout << "ERROR: inputList is empty" << endl;
        return exit(1);
    }


    for(int i = 0; i < loadList->length(); i++)
    {
        int min = 0;
        Square nearestIn, nearestLoad;
        nearestLoad = loadList->at(i);

        for(int j = 0; j < inputList->length(); j++)
        {
            QPoint dis = nearestLoad.getQPoint() - inputList->at(j).getQPoint();
            int res = dis.manhattanLength();
            if(res < min || j == 0)
            {
                nearestIn = inputList->at(j);
                min = res;
            }
        }

        //PASS or FAIL
        if(nearestIn.switchState == nearestLoad.switchState)
        {
            //PASS
            circle(ref, nearestIn.getCenterPoint(), 100, Scalar(0, 255, 0), 10, 8, 0);
            qDebug() << "Sample:" << nearestLoad.switchState << " <=> " << "Result:" << nearestIn.switchState << "____PASS";
        }else
        {
            //FAIL
            circle(ref, nearestIn.getCenterPoint(), 100, Scalar(0, 0, 255), 10, 8, 0);
            qDebug() << "Sample:" << nearestLoad.switchState << " <=> " << "Result:" << nearestIn.switchState << "____FAIL";
        }
    }
}

bool switchDetect::distanceAllowRange(const Point &point, int distance)
{
    if(inputList->empty())
    {
        return true;
    }

    for(int i = 0 ; i < inputList->length(); i++)
    {
        QPoint dis = inputList->at(i).getQPoint() - QPoint(point.x, point.y);
        int range = dis.manhattanLength();
        if(range <= distance)
        {
            return false;
        }
    }
    return true;
}
