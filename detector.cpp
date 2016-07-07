//
// Created by 黄金 on 16/7/7.
//

#include "detector.h"

void Detector::end()
{
    api->End();
    delete api;
}


bool comp(const vector<Point> key1,const vector<Point> key2)
{
    Rect rect1 = boundingRect(key1);
    Rect rect2 = boundingRect(key2);
    return rect1.y > rect2.y;
}

string Detector::decoder(Mat image) {
    char *outText;
    string out;
    api->SetImage((uchar*)image.data, image.size().width, image.size().height, image.channels(), (int)image.step1());
    outText = api->GetUTF8Text();
    out = outText;
    delete [] outText;
    api->Clear();
    return out;
}

void Detector::init(const char* datapath)
{
    api = new TessBaseAPI();
    if (api->Init(datapath, "chi_sim"))
    {
        printf("Could not initialize tesseract.\n");
    }
    api->SetVariable("tessedit_char_blacklist", "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
    api->Recognize(NULL);
}



string Detector::getIdentification(Mat src)
{
    cv::Mat gray, thr, med, last;
    string text;
    uint rows = src.rows;
    vector<Mat> splited;
    split(src, splited);
    gray = splited[0];

    adaptiveThreshold(gray, thr, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV, 13, 15);

    Mat opening = getStructuringElement(MORPH_RECT, Size(rows/800, rows/800));
    morphologyEx(thr, med, MORPH_OPEN, opening, Point(-1, -1), 2);

    Mat closeing = getStructuringElement(MORPH_RECT, Size(rows/20, rows/25));
    morphologyEx(med, med, MORPH_CLOSE, closeing, Point(-1, -1), 2);

    vector< vector<Point> > contours;
    findContours(med, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);
    sort(contours.begin(), contours.end(), comp);

    size_t szie = contours.size();
    Rect rect = boundingRect(contours[szie-1]);
    Mat tmp(gray, rect);
    adaptiveThreshold(tmp, tmp, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV, 57, 15);
    text = decoder(tmp);
    rect = boundingRect(contours[0]);
    Mat atmp(gray, rect);
    text += decoder(atmp);
    adaptiveThreshold(atmp, atmp, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV, 57, 15);
    return  text;
}
