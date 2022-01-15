project(BarcodeRotationTest1)
cmake_minimum_required(VERSION 3.10)
find_package(OpenCV REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(PC_ZBAR zbar REQUIRED)

find_library(ZBAR_LIBRARIES NAMES zbar HINTS ${PC_ZBAR_LIBDIR} ${PC_ZBAR_LIBRARY_DIRS})

configure_file(barcode.jpg . COPYONLY)

add_executable(
    ${PROJECT_NAME} 
    main.cpp
)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
        ${PC_ZBAR_INCLUDEDIR}
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
        ${OpenCV_LIBS} 
        ${ZBAR_LIBRARIES}
)

