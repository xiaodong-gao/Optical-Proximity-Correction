#ifndef MCOMMONBASE_H
#define MCOMMONBASE_H

#include "opencv2/opencv.hpp"

enum class CameraStatusCode {
    SUCCESS,          // 操作成功
    ERROR,            // 通用错误
    NOT_CONNECTED,    // 相机未连接
    TIMEOUT,          // 操作超时
    INVALID_PARAMETER // 无效的参数
};

typedef struct WorkDoneResult {
    cv::Mat src_mat1{};
    cv::Mat src_mat2{};
    cv::Mat coarse_mat1{};
    cv::Mat coarse_mat2{};
    cv::Mat focus_mat1{};
    cv::Mat focus_mat2{};
    double laplacianVariance1{};
    double laplacianVariance2{};
    cv::Mat roi_src_mat1{};
    cv::Mat roi_src_mat2{};
    cv::Point2d fine_pt{0.0,0.0};
    cv::Mat ssim_mat1{};
    cv::Mat ssim_mat2{};
    cv::Mat ssim_mat{};
    double ssim_value{0.0};
}WorkDoneResult;

enum class CameraType {
    FRONT_CAMERA,
    BACK_CAMERA,
    LEFT_CAMERA,
    RIGHT_CAMERA
};

typedef struct RecipeParam{
    cv::Mat src_mat1_{};
    cv::Mat src_mat2_{};
    std::vector<cv::Rect> selection_rois_{};
    int camera_id_{-1};
    double camera_exposure{0.0};
    std::string camera_trigger{""};
    float focus_length_value{0.0};
    float fine_offset_x{0.0};
    float fine_offset_y{0.0};
    float ssim_min_value{0.0};
    float ssim_max_value{0.0};
}RecipeParam;


#endif // MCOMMONBASE_H
