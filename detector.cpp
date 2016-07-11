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
    api->SetImage(image.data, image.size().width, image.size().height, image.channels(), (int)image.step1());
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
    api->SetVariable("tessedit_char_blacklist", "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ[");
    api->SetPageSegMode(PSM_SINGLE_LINE);
    api->Recognize(NULL);
}



string Detector::getIdentification(Mat src)
{
    cv::Mat gray, thr, med, last;
    string text;
    uint rows = (uint)src.rows;
    vector<Mat> channels;
    split(src, channels);

    gray = channels[0];

    threshold(gray, thr, 80, 255, THRESH_BINARY_INV);

    Mat closing = getStructuringElement(MORPH_RECT, Size(rows/50, rows/50));
    morphologyEx(thr, med, MORPH_CLOSE, closing, Point(-1, -1), 4);

    Mat opening = getStructuringElement(MORPH_RECT, Size(2, 2));
    morphologyEx(med, last, MORPH_OPEN, opening, Point(-1, -1), 4);

    vector< vector<Point> > contours;
    findContours(last, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);
    sort(contours.begin(), contours.end(), comp);

    size_t szie = contours.size();
    Rect rect = boundingRect(contours[szie-1]);
    if (rect.x > 10) {
        rect.x -= 10;
        rect.width += 10;
    }
    if (rect.y > 10) {
        rect.y -= 10;
        rect.height += 10;
    }

    Mat tmp(thr, rect);
    text = decoder(tmp);
    rect = boundingRect(contours[0]);

    if (rect.x > 10) {
        rect.x -= 10;
        rect.width += 20;
    }
    if (rect.y > 10) {
        rect.y -= 10;
        rect.height += 10;
    }

    Mat atmp(thr, rect);
    text += decoder(atmp);
    return  text;
}
