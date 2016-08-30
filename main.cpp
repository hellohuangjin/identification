#include "detector.h"

using namespace std;

int main() {
    Detector *detector = new Detector();
    Mat id_img = imread("/Users/lambda/Downloads/n.jpg");

    if (!id_img.data) {
        cout<< "can not find img" <<endl;
        exit(0);
    }

    detector->init(NULL);
    int i = 100;
    while(i) {
        cout<< detector->getIdentification(id_img) <<endl;
        i--;
    }
    detector->end();

}