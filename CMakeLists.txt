cmake_minimum_required(VERSION 3.5)
project(identification)

include_directories("/usr/local/Cellar/opencv3/3.1.0_3/include/" "/usr/local/Cellar/tesseract/3.04.01_1/include/")
link_directories("/usr/local/Cellar/opencv3/3.1.0_3/lib/" "/usr/local/Cellar/tesseract/3.04.01_1/lib/")


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lopencv_highgui -lopencv_imgcodecs -lopencv_core -lopencv_imgproc -ltesseract")

set(SOURCE_FILES main.cpp detector.h detector.cpp)
add_executable(identification ${SOURCE_FILES})