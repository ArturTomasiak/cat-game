#include "include/opencv2/core.hpp"
#include "include/opencv2/imgcodecs.hpp"
#include "include/opencv2/highgui.hpp"
#include "include/opencv2/imgproc.hpp"
#include <vector>
#include <algorithm>
#include <numeric>
#include <random>
#include <chrono>

cv::Mat putImage(const cv::Mat& srcMat, const cv::Mat& imgToPut, const std::vector<cv::Point>& destContour) {
    cv::Mat resultMat;
    cv::Mat dstMat(srcMat.size(), CV_8UC3, cv::Scalar::all(0));
    cv::Mat cutter(imgToPut.size(), imgToPut.type(), cv::Scalar(0));

    cv::bitwise_not(cutter, cutter);
    std::vector<cv::Point2f> src = {
        {0, 0},
        {0, static_cast<float>(dstMat.rows)},
        {static_cast<float>(dstMat.cols), static_cast<float>(dstMat.rows)},
        {static_cast<float>(dstMat.cols), 0}
    };
    std::vector<cv::Point2f> dst;
    dst.reserve(destContour.size());
    for (const auto& p : destContour) {
        dst.emplace_back(p.x, p.y);
    }
    auto wrap_mtx = cv::getPerspectiveTransform(src, dst);
    cv::warpPerspective(imgToPut, dstMat, wrap_mtx, dstMat.size());
    cv::warpPerspective(cutter, cutter, wrap_mtx, dstMat.size());
    cv::bitwise_not(cutter, cutter);

    cv::Mat dstMatClone;
    srcMat.copyTo(dstMatClone, cutter);

    resultMat = dstMat + dstMatClone;
    return resultMat;
}

int main() {

    cv::Mat wrappedCatImage = cv::imread("089.jpg", cv::IMREAD_COLOR);
    if (wrappedCatImage.empty()) {
        return -1;
    }

    cv::VideoCapture camera(0);
    if (!camera.isOpened()) {
        return -1;
    }

    int low_h = 35, high_h = 85;
    int low_s = 50, high_s = 255;
    int low_v = 50, high_v = 255;

    cv::Point2f catPosition = {100.0, 100.0};
    cv::Point2f catVelocity = {70.0, 30.0};
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> distX(0, 640);
    std::uniform_int_distribution<int> distY(0, 480);

    auto start = std::chrono::steady_clock::now();
    int score = 0;
    while (cv::waitKey(1) != 'q' && cv::waitKey(1) != 'Q') {
        auto end = std::chrono::steady_clock::now();
        double dt = std::chrono::duration<double>(end - start).count();
        start = end;

        cv::Mat frame;
        camera >> frame;
        if (frame.empty()) continue;
        cv::resize(frame, frame, {640, 480});
        cv::flip(frame, frame, 1);

        cv::Mat hsvFrame;
        cv::cvtColor(frame, hsvFrame, cv::COLOR_BGR2HSV);
        cv::Mat mask;
        cv::inRange(hsvFrame, cv::Scalar(low_h, low_s, low_v), cv::Scalar(high_h, high_s, high_v), mask);
        cv::morphologyEx(mask, mask, cv::MORPH_OPEN, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(7, 7)));
        cv::morphologyEx(mask, mask, cv::MORPH_CLOSE, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(7, 7)));

        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        cv::Point2f penPosition = {-1, -1};
        float penRadius = 0;

        for (const auto& contour : contours) {
            if (cv::contourArea(contour) > 500) {
                cv::minEnclosingCircle(contour, penPosition, penRadius);
                cv::circle(frame, penPosition, static_cast<int>(penRadius), cv::Scalar(0, 255, 0), 2);
                break;
            }
        }

        catPosition += catVelocity * dt;
        if (catPosition.x + 60 > 640) catVelocity.x = -std::abs(catVelocity.x);
        if (catPosition.x - 50 < 0) catVelocity.x = std::abs(catVelocity.x);
        if (catPosition.y + 35 > 480) catVelocity.y = -std::abs(catVelocity.y);
        if (catPosition.y - 50 < 0) catVelocity.y = std::abs(catVelocity.y);

        if (penRadius > 0 && norm(catPosition - penPosition) < penRadius + 50) {
            score++;
            catPosition = {static_cast<float>(distX(rng)), static_cast<float>(distY(rng))};
        }

        frame = putImage(frame, wrappedCatImage, {
            {(int)catPosition.x - 50, (int)catPosition.y - 50},
            {(int)catPosition.x - 50, (int)catPosition.y + 50},
            {(int)catPosition.x + 50, (int)catPosition.y + 50},
            {(int)catPosition.x + 50, (int)catPosition.y - 50}
        });

        std::string scorestr = "score: " + std::to_string(score);
        int baseline = 0;
        cv::Size textSize = cv::getTextSize(scorestr, cv::FONT_HERSHEY_DUPLEX, 1, 1, &baseline);
        cv::Point textOrg((frame.cols - textSize.width) / 2, textSize.height + 4);
        cv::putText(frame, scorestr, textOrg, cv::FONT_HERSHEY_DUPLEX, 1, cv::Scalar(255, 255, 255), 1);
        
        imshow("Frame with Cat", frame);
    }

    return 0;
}
