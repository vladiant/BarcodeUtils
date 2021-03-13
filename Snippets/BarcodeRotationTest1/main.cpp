// https://www.learnopencv.com/barcode-and-qr-code-scanner-using-zbar-and-opencv/

#include <zbar.h>

#include <chrono>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

typedef struct {
  std::string type;
  std::string data;
  std::vector<cv::Point> location;
} decodedObject;

// Find and decode barcodes and QR codes
void decode(cv::Mat &im, std::vector<decodedObject> &decodedObjects) {
  // Create zbar scanner
  zbar::ImageScanner scanner;

  // Configure scanner
  scanner.set_config(zbar::ZBAR_NONE, zbar::ZBAR_CFG_ENABLE, 1);

  // Convert image to grayscale
  cv::Mat imGray;
  cvtColor(im, imGray, cv::COLOR_BGR2GRAY);

  // Wrap image data in a zbar image
  zbar::Image image(imGray.cols, imGray.rows, "Y800", (uchar *)imGray.data,
                    imGray.cols * imGray.rows);

  // Scan the image for barcodes and QRCodes
  int n = scanner.scan(image);

  // Print results
  for (zbar::Image::SymbolIterator symbol = image.symbol_begin();
       symbol != image.symbol_end(); ++symbol) {
    decodedObject obj;

    obj.type = symbol->get_type_name();
    obj.data = symbol->get_data();

    // Print type and data
    std::cout << "Type : " << obj.type << std::endl;
    std::cout << "Data : " << obj.data << std::endl << std::endl;

    // Obtain location
    for (int i = 0; i < symbol->get_location_size(); i++) {
      obj.location.push_back(
          cv::Point(symbol->get_location_x(i), symbol->get_location_y(i)));
    }

    decodedObjects.push_back(obj);
  }
}

// Display barcode and QR code location
void display(cv::Mat &im, std::vector<decodedObject> &decodedObjects) {
  // Loop over all decoded objects
  for (uint i = 0; i < decodedObjects.size(); i++) {
    std::vector<cv::Point> points = decodedObjects[i].location;
    std::vector<cv::Point> hull;

    // If the points do not form a quad, find convex hull
    if (points.size() > 4)
      convexHull(points, hull);
    else
      hull = points;

    // Number of points in the convex hull
    int n = hull.size();

    for (int j = 0; j < n; j++) {
      line(im, hull[j], hull[(j + 1) % n], cv::Scalar(255, 0, 0), 3);
    }
  }

  // Display results
  imshow("Results", im);
}

bool readImage(const std::string &imageName, cv::Mat &image) {
  image = imread(imageName, cv::IMREAD_COLOR);  // Read the file

  if (!image.data)  // Check for invalid input
  {
    std::cout << "Could not open or find the image" << std::endl;
    return false;
  }

  return true;
}
/*
 * Function: readImage
 * Usage:  displayImg(image, name);
 * ----------------------------------------
 * Displays the image with <code>name</code> as title
 */
void displayImg(const cv::Mat &image, const std::string &name) {
  image.convertTo(image, CV_8U);
  namedWindow(name, cv::WINDOW_AUTOSIZE);  // Create a window for display.
  imshow(name, image);                     // Show our image inside it.
  //  std::cout << image(cv::Range(0, 10), cv::Range(0, 10)) << std::endl;
  return;
}

cv::Mat rotate(cv::Mat src, double angle) {
  cv::Mat dst;
  cv::Point2f pt(src.cols / 2., src.rows / 2.);
  cv::Mat r = getRotationMatrix2D(pt, angle, 1.0);
  cv::warpAffine(src, dst, r, cv::Size(src.cols, src.rows));
  return dst;
}

int main(int argc, char *argv[]) {
  constexpr auto image_name = "barcode.jpg";

  std::cout << "OpenCV version: " << CV_VERSION << std::endl;

  cv::Mat image;
  if (!readImage(image_name, image)) {
    std::cout << "Error reading " << image_name << std::endl;
    return -1;
  }

  for (int angle = 0; angle < 180; angle += 1) {
    std::cout << "Rotation angle: " << angle << std::endl;

    const auto start_point = std::chrono::steady_clock::now();

    auto rotated_image = rotate(image, angle);

    const auto end_point = std::chrono::steady_clock::now();
    const auto elapsed_seconds =
        std::chrono::duration_cast<std::chrono::microseconds>(end_point -
                                                              start_point);

    std::cout << "Rotation time [us]: " << elapsed_seconds.count() << std::endl;

    displayImg(image, "Initial");

    std::vector<decodedObject> decodedObjects;

    decode(rotated_image, decodedObjects);

    display(rotated_image, decodedObjects);
    cv::waitKey(40);
  }

  std::cout << "Done." << std::endl;
  return 0;
}
