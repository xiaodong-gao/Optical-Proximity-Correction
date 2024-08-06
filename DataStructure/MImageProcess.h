#ifndef MIMAGEPROCESS_H
#define MIMAGEPROCESS_H

#include <opencv2/opencv.hpp>
#include <QImage>

class MImageProcess{
public:
    explicit MImageProcess();
    ~MImageProcess();

    cv::Mat QImage2Mat(const QImage &image);
    QImage Mat2QImage(const cv::Mat &mat);

    //process rois
    cv::Mat extractRois(cv::Mat image, std::vector<cv::Rect> rois);

    //Laplacian
    double getLaplacian(const cv::Mat& img, cv::Mat& result_img);

    //fine location
    cv::Point2d getPhaseCorr(const cv::Mat& img1, const cv::Mat& img2);

    //quality analysis
    cv::Mat getMSSIM(const cv::Mat& img1, const cv::Mat& img2,double & ssim_value);

};

#endif // MIMAGEPROCESS_H
