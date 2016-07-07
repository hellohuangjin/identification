//
// Created by 黄金 on 16/7/7.
//

#ifndef IDENTIFICATION_DETECTOR_H
#define IDENTIFICATION_DETECTOR_H
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <tesseract/baseapi.h>

using namespace std;
using namespace cv;
using namespace tesseract;

class Detector{

private:
    TessBaseAPI *api;
    string decoder(Mat image);
public:
    void init(const char* datapath);
    string getIdentification(Mat src);
    void end();
};

#endif //IDENTIFICATION_DETECTOR_H
