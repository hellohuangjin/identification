#include "detector.h"

using namespace std;

int main() {
    Detector *detector = new Detector();
    Mat id_img = imread("/Users/lambda/Downloads/me.jpg");
    if (!id_img.data) {
        cout<< "can not find img" <<endl;
        exit(0);
    }

    detector->init(NULL);
    detector->getIdentification(id_img);
    detector->end();
}