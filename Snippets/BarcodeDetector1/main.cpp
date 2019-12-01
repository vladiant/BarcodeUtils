// https://www.pyimagesearch.com/2014/11/24/detecting-barcodes-images-python-opencv/

#include <cv.hpp>
#include <iostream>

int main(int argc, char *argv[]) {
  constexpr char frame_name[] = "Frame";

  cv::VideoCapture cap;

  if (!cap.open(0)) {
    return -1;
  }

  while (true) {
    cv::Mat frame;
    cap >> frame;
    if (frame.empty()) {
      break;
    }

    cv::namedWindow(frame_name, cv::WINDOW_AUTOSIZE);
    cv::imshow(frame_name, frame);

    if (cv::waitKey(10) == 27) {
      break;
    }
  }
  cap.release();

  std::cout << "Done." << std::endl;
  return 0;
}
