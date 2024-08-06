#ifndef MCAMERAHIK_H
#define MCAMERAHIK_H

#include "DataStructure/MCameraBase.h"
#include "MvCameraControl.h"

class MCameraHik : public MCameraBase{
public:
    explicit MCameraHik();
    ~MCameraHik();
    virtual CameraStatusCode init(void *ptr ,int nIndex) override;
    virtual CameraStatusCode open() override;
    virtual CameraStatusCode is_opened() override;
    virtual CameraStatusCode close() override;
    virtual CameraStatusCode grab() override;
    virtual CameraStatusCode stop() override;
    virtual CameraStatusCode get_mat(cv::Mat &image) const override;
    virtual CameraStatusCode get_camera_frame_rate(float &frame_rate) const override;
    virtual CameraStatusCode set_camera_exposure(float exposure_value) override;
    virtual CameraStatusCode get_camera_exposure(float &exposure_value) const override;
    virtual CameraStatusCode set_camera_trigger(const std::string &trigger_mode) override;
    virtual CameraStatusCode get_camera_trigger(std::string &trigger_mode) const override;
    virtual CameraStatusCode import_file(const char *file_path) override;
    virtual CameraStatusCode export_file(const char *file_path) const override;
private:
    void* handle{nullptr};
    MV_CC_DEVICE_INFO_LIST* stDeviceList{nullptr};
    int camera_id_{-1};
};

#endif // MCAMERAHIK_H
