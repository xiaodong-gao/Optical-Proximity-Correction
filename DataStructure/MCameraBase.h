#ifndef MCAMERABASE_H
#define MCAMERABASE_H

#include "DataStructure/MCommonBase.h"
#include <opencv2/opencv.hpp>
#include <string>

class MCameraBase{
public:
    virtual ~MCameraBase();
    virtual CameraStatusCode init(void *ptr ,int nIndex) = 0;
    virtual CameraStatusCode open() = 0;
    virtual CameraStatusCode is_opened() = 0;
    virtual CameraStatusCode close() = 0;
    virtual CameraStatusCode grab() = 0;
    virtual CameraStatusCode stop() = 0;
    virtual CameraStatusCode get_mat(cv::Mat &image) const = 0;
    virtual CameraStatusCode get_camera_frame_rate(float &frame_rate) const= 0;
    virtual CameraStatusCode set_camera_exposure(float exposure_value) = 0;
    virtual CameraStatusCode get_camera_exposure(float &exposure_value) const= 0;
    virtual CameraStatusCode set_camera_trigger(const std::string &trigger_mode) = 0;
    virtual CameraStatusCode get_camera_trigger(std::string &trigger_mode) const= 0;
    virtual CameraStatusCode import_file(const char *file_path) = 0;
    virtual CameraStatusCode export_file(const char *file_path) const = 0;
protected:
    explicit MCameraBase();
};

#endif // MCAMERABASE_H
