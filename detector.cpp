#include "detector.h"

void show(string name, Mat img) {
    namedWindow(name);
    imshow(name, img);
    waitKey();
    destroyWindow(name);
}

void Detector::end()
{
    api->End();
    delete api;
}

static bool comp(const vector<Point> key1,const vector<Point> key2)
{
    Rect rect1 = boundingRect(key1);
    Rect rect2 = boundingRect(key2);
    return rect1.y > rect2.y;
}

string Detector::recoginze(Mat image, bool isNum) {
    char *outText;
    string out;
    if (isNum) {
        api->SetVariable("tessedit_char_whitelist", "1234567890X");
    } 
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
        cout<< "Could not initialize tesseract." <<endl;
        exit(0);
    }
    api->SetVariable("tessedit_char_blacklist", ":,\".-"); 
}


string Detector::getIdentification(Mat src)
{
    cv::Mat gray, blr, thr, med, last, edge;
    string text;
    unsigned int rows = src.rows;
   
    vector<Mat> channels;
    split(src, channels);
    gray = channels[0];

    // 中值滤波
    blur(gray, blr, Size(5, 5));

    threshold(blr, thr, 80, 255, THRESH_BINARY_INV);

    Mat closing = getStructuringElement(MORPH_RECT, Size(rows/20, rows/100));
    morphologyEx(thr, med, MORPH_CLOSE, closing, Point(-1, -1), 2);

    Mat opening = getStructuringElement(MORPH_RECT, Size(2, 2));
    morphologyEx(med, last, MORPH_OPEN, opening, Point(-1, -1), 4);


    Mat dilation = getStructuringElement(MORPH_RECT, Size(4, 6));
    dilate(last, last, dilation, Point(-1, 0), 3);

    vector< vector<Point> > contours;
    findContours(last, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);
    sort(contours.begin(), contours.end(), comp);

    size_t size = contours.size();
    Rect rect = boundingRect(contours[size-1]);
    unsigned int pos = 0;
    map<int, Rect> text_pos;
    for(int i=0; i<size; i++) {
        rect = boundingRect(contours[i]);
        if (rect.x > rows*4/5) {
            continue;
        }
        rect.x -= 10;
        rect.width += 10;
        text_pos[pos] = rect;
        pos++;
        
    }
    
    Mat name(gray, text_pos[pos-1]);
    adaptiveThreshold(name, name, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV, 111, 30);
    show("name", name);
    cout<< "name " << recoginze(name) <<endl;
    Mat ids(gray, text_pos[0]);
    adaptiveThreshold(ids, ids, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV, 111, 30);
    show("id", ids);
    cout<< "id " << recoginze(ids, true) <<endl;
    
    return  text;
}
