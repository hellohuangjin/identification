//
// Created by 黄金 on 16/7/7.
//

#ifndef DETECTOR_H
#define DETECTOR_H
#include <iostream>
#include <map>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <tesseract/baseapi.h>

using namespace std;
using namespace cv;
using namespace tesseract;

class Detector{

private:
    TessBaseAPI *api;
    string recoginze(Mat image, bool isNum = false);
public:
    void init(const char* datapath);
    string getIdentification(Mat src);
    void end();
};

#endif //DETECTOR_H
