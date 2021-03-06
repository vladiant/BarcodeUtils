// https://www.pyimagesearch.com/2014/11/24/detecting-barcodes-images-python-opencv/

#include <iostream>
#include <opencv/cv.hpp>

int main(int argc, char *argv[]) {
  constexpr char initial_frame_name[] = "Initial Frame";
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

    cv::namedWindow(initial_frame_name, cv::WINDOW_AUTOSIZE);
    cv::imshow(initial_frame_name, frame);

    //  Convert the image to grayscale
    cv::Mat gray;
    cv::cvtColor(frame, gray, cv::ColorConversionCodes::COLOR_BGR2GRAY);

    // Compute the Scharr gradient magnitude representation of the images
    // in both the x and y direction
    cv::Mat gradX, gradY;
    cv::Sobel(gray, gradX, CV_8U, 1, 0, -1);
    cv::Sobel(gray, gradY, CV_8U, 0, 1, -1);

    // Subtract the y-gradient from the x-gradient
    cv::Mat gradient = gradX - gradY;
    cv::convertScaleAbs(gradient, gradient);

    // Blur and threshold the image
    cv::Mat blurred;
    cv::blur(gradient, blurred, cv::Size(9, 9));
    cv::Mat thresh;
    cv::threshold(blurred, thresh, 125, 255, cv::THRESH_BINARY);

    // Construct a closing kernel and apply it to the thresholded image
    cv::Mat kernel =
        cv::getStructuringElement(cv::MORPH_RECT, cv::Size(21, 21));
    cv::Mat closed;
    cv::morphologyEx(thresh, closed, cv::MORPH_CLOSE, kernel);

    // Perform a series of erosions and dilations
    cv::Mat temp;
    cv::erode(closed, temp, cv::Mat(), cv::Point(-1, -1), 4);
    cv::dilate(temp, closed, cv::Mat(), cv::Point(-1, -1), 4);

    cv::namedWindow(frame_name, cv::WINDOW_AUTOSIZE);
    cv::imshow(frame_name, closed);

    if (cv::waitKey(10) == 27) {
      break;
    }
  }
  cap.release();

  std::cout << "Done." << std::endl;
  return 0;
}
