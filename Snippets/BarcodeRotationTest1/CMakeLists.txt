project(BarcodeRotationTest1)
cmake_minimum_required(VERSION 3.10)
find_package(OpenCV REQUIRED)
#find_package(zbar)

configure_file(barcode.jpg . COPYONLY)

aux_source_directory(. SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS} zbar )

