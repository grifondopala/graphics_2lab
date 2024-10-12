#ifndef KMEAN_H 
#define KMEAN_H

#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <limits>

double distance(const cv::Vec3b& a, const cv::Vec3b& b);

std::vector<cv::Vec3b> kMeans(const std::vector<cv::Vec3b>& palette, int k);

cv::Vec3b getNearColor(const std::vector<cv::Vec3b>& palette, const cv::Vec3b& color);

#endif
