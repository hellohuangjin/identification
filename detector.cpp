#include "detector.h"

void show(string name, Mat img) {
  namedWindow(name);
  imshow(name, img);
  waitKey();
  destroyWindow(name);
}

void Detector::end() {
  api->End();
  delete api;
}

string Detector::recoginze(Mat image) {
  char *outText;
  string out = "NO";
  api->SetImage(image.data, image.size().width, image.size().height,
                image.channels(), (int)image.step1());
  outText = api->GetUTF8Text();
  if (!outText) {
    goto finally;
  }
  out = outText;
finally:
  delete[] outText;
  api->Clear();
  return out;
}

void Detector::init(const char *datapath) {
  api = new TessBaseAPI();

  if (api->Init(datapath, "eng")) {
    cout << "Could not initialize tesseract." << endl;
    exit(0);
  }

  api->SetVariable("tessedit_char_blacklist", ". -");
  api->SetVariable("tessedit_char_whitelist", "0123456789X");
  api->SetPageSegMode(PSM_SINGLE_LINE);
}

string Detector::getIdentification(Mat src) {

  cv::Mat gray, blr, thr, med, last;
  string text;
  unsigned int cols = src.cols;
  unsigned int rows = src.rows;

  vector<Mat> chanel;
  split(src, chanel);
  gray = chanel[0];

  adaptiveThreshold(gray, thr, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY,
                    91, 40);
  blur(thr, blr, Size(7, 7));

  threshold(blr, thr, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);

  Mat closing = getStructuringElement(MORPH_RECT, Size(cols / 80, cols / 100));
  morphologyEx(thr, med, MORPH_CLOSE, closing, Point(-1, -1), 2);

  Mat opening = getStructuringElement(MORPH_RECT, Size(2, 2));
  morphologyEx(med, last, MORPH_OPEN, opening, Point(-1, -1), 4);

  Mat dilation = getStructuringElement(MORPH_RECT, Size(4, 2));
  dilate(last, last, dilation, Point(-1, -1), 3);

  vector< vector<Point> > contours;
  findContours(last, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

  size_t size = contours.size();

  if (size == 0) {
    return "NO";
  }

  size_t maxWidth = 0;
  Rect id_rect;
  for (int i = 0; i < size; i++) {

    Rect rect = boundingRect(contours[i]);

    // 区域过滤
    if (rect.width < cols / 3 || rect.x > cols * 3 / 5 ||
        rows - (rect.y + rect.height) < 10) {
      continue;
    }

    // 扩大截取区域
    if (rect.x > 10) {
      rect.x -= 10;
      rect.width += 10;
    }

    if (rect.y > 100) {
      rect.y -= 10;
      rect.height += 10;
    } else {
      continue;
    }

    if (rect.width > maxWidth) {
      maxWidth = rect.width;
      id_rect = rect;
    }
  }

  if (maxWidth == 0) {
    return "NO";
  }

  Mat ids(gray, id_rect);

  adaptiveThreshold(ids, ids, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV,
                    51, 40);
  string idString = recoginze(ids);

  if (idString.length() == 20) {
    return idString;
  } else {
    return "NO";
  }
}
