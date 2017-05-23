#include <QCoreApplication>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <QDebug>
#include <switchdetect.h>
#include <square.h>
using namespace cv;
using namespace std;

Mat ref1, tempV, tempH;
int switchNum;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if(argc < 3)
    {
        cout << "Not enough parameters" << endl;
        cout << "Usage:\n./MatchTemplate_Demo <image_name> <componentNum> <mode>" << endl;
        return 1;
    }

    ref1 = imread(argv[1], IMREAD_COLOR);
    tempV = imread("N1-O1_V.png", IMREAD_COLOR);
    tempH = imread("N1-O1_H.png", IMREAD_COLOR);

    QString num = argv[2];
    switchNum = num.toInt();

    QString mode = argv[3];

    switchDetect switchDetector(tempH, tempV, ref1, switchNum);

    switchDetector.startDetect();

    QString fileName;

    fileName = QCoreApplication::applicationDirPath() + "/setting.ini";

    if(mode.toLower() == "sample")
    {
        switchDetector.saveSettings(fileName);
    }else if(mode.toLower() == "detect")
    {
        switchDetector.loadSettings(fileName);
        switchDetector.resultMatching();
    }else
    {
        cout << "Parameter <mode> ERROR" << endl;
        return 1;
    }

    switchDetector.showResult();

    return a.exec();
}
