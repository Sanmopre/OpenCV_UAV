# CMake generated Testfile for 
# Source directory: /home/sanmopre/OpenCV_UAV/opencv-4.9.0/modules/highgui
# Build directory: /home/sanmopre/OpenCV_UAV/opencv-4.9.0/build/modules/highgui
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(opencv_test_highgui "/home/sanmopre/OpenCV_UAV/opencv-4.9.0/build/bin/opencv_test_highgui" "--gtest_output=xml:opencv_test_highgui.xml")
set_tests_properties(opencv_test_highgui PROPERTIES  LABELS "Main;opencv_highgui;Accuracy" WORKING_DIRECTORY "/home/sanmopre/OpenCV_UAV/opencv-4.9.0/build/test-reports/accuracy" _BACKTRACE_TRIPLES "/home/sanmopre/OpenCV_UAV/opencv-4.9.0/cmake/OpenCVUtils.cmake;1795;add_test;/home/sanmopre/OpenCV_UAV/opencv-4.9.0/cmake/OpenCVModule.cmake;1375;ocv_add_test_from_target;/home/sanmopre/OpenCV_UAV/opencv-4.9.0/modules/highgui/CMakeLists.txt;309;ocv_add_accuracy_tests;/home/sanmopre/OpenCV_UAV/opencv-4.9.0/modules/highgui/CMakeLists.txt;0;")
