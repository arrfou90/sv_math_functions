#include <opencv2/core/core.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <iostream>
#include <ctime>
#include <sys/time.h>
 

#ifndef BILATERAL_GPU_H_
#define BILATERAL_GPU_H_

#include <stdint.h>
#include <opencv2/opencv.hpp>
 


   //void median_filter_wrapper(const cv::Mat& input, cv::Mat& output);
   void bilateral_filter_wrapper(const cv::Mat& input, cv::Mat& output);
 #endif  
