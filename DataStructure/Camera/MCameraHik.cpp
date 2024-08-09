#include "MCameraHik.h"

MCameraHik::MCameraHik(){

}

MCameraHik::~MCameraHik(){
    // ch:销毁句柄 | Destroy handle
    int nRet = MV_CC_DestroyHandle(handle);
    if (MV_OK != nRet)
    {
        //printf("Destroy Handle fail! nRet [0x%x]\n", nRet);
        //break;
    }
    handle = NULL;

}

CameraStatusCode MCameraHik::init(void *ptr ,int nIndex) {
    // ch:选择设备并创建句柄 | en:Select device and create handle
    stDeviceList  = static_cast<MV_CC_DEVICE_INFO_LIST*>(ptr);
    camera_id_ = nIndex;
    int nRet = MV_CC_CreateHandle(&handle, stDeviceList->pDeviceInfo[nIndex]);
    if (MV_OK != nRet)
    {
        ////qDebug() << "Create Handle fail!" << "\n";
        //printf("Create Handle fail! nRet [0x%x]\n", nRet);
        //break;
    }
    return  CameraStatusCode::SUCCESS;
}

CameraStatusCode MCameraHik::open() {
    // ch:打开设备 | en:Open device
    int nRet = MV_CC_OpenDevice(handle);
    if (MV_OK != nRet)
    {
        ////qDebug() << "Open Device fail!" << "\n";
        //printf("Open Device fail! nRet [0x%x]\n", nRet);
        //break;
    }

    // ch:探测网络最佳包大小(只对GigE相机有效) | en:Detection network optimal package size(It only works for the GigE camera)
    if (stDeviceList->pDeviceInfo[camera_id_]->nTLayerType == MV_GIGE_DEVICE)
    {
        int nPacketSize = MV_CC_GetOptimalPacketSize(handle);
        if (nPacketSize > 0)
        {
            nRet = MV_CC_SetIntValueEx(handle,"GevSCPSPacketSize",nPacketSize);
            if(nRet != MV_OK)
            {
                ////qDebug() << "Warning: Set Packet Size fail!" << "\n";
                //printf("Warning: Set Packet Size fail nRet [0x%x]!", nRet);
            }
        }
        else
        {
            //printf("Warning: Get Packet Size fail nRet [0x%x]!", nPacketSize);
        }
    }
    return  CameraStatusCode::SUCCESS;
}

CameraStatusCode MCameraHik::is_opened(){
    bool bAccessible = MV_CC_IsDeviceAccessible(stDeviceList->pDeviceInfo[camera_id_], MV_ACCESS_Exclusive);
    if(bAccessible){
        return  CameraStatusCode::SUCCESS;
    }
    return  CameraStatusCode::SUCCESS;
}


CameraStatusCode MCameraHik::close(){
    // ch:关闭设备 | Close device
    int nRet = MV_CC_CloseDevice(handle);
    if (MV_OK != nRet)
    {
        ////qDebug() << "ClosDevice fail!" << "\n";
        //printf("ClosDevice fail! nRet [0x%x]\n", nRet);
        //break;
    }
    return  CameraStatusCode::SUCCESS;
}


CameraStatusCode MCameraHik::grab(){
    // ch:开始取流 | en:Start grab image
    int nRet = MV_CC_StartGrabbing(handle);
    if (MV_OK != nRet)
    {
        ////qDebug() << "Start Grabbing fail!" << "\n";
        //printf("Start Grabbing fail! nRet [0x%x]\n", nRet);
        //break;
    }
    return  CameraStatusCode::SUCCESS;
}

CameraStatusCode MCameraHik::stop(){
    // ch:停止取流 | en:Stop grab image
    int nRet = MV_CC_StopGrabbing(handle);
    if (MV_OK != nRet)
    {
        //printf("Stop Grabbing fail! nRet [0x%x]\n", nRet);
        //break;
    }
    return  CameraStatusCode::SUCCESS;
}


CameraStatusCode MCameraHik::get_mat(cv::Mat &image_) const{
    MV_FRAME_OUT stOutFrame = { 0 };
    int nRet = MV_CC_GetImageBuffer(handle, &stOutFrame, 1000);
    if (nRet == MV_OK)
    {
        int width = stOutFrame.stFrameInfo.nWidth;
        int height = stOutFrame.stFrameInfo.nHeight;
        unsigned char * buffer = static_cast<unsigned char*>(stOutFrame.pBufAddr);

        if (stOutFrame.stFrameInfo.enPixelType == PixelType_Gvsp_Mono8) {
            image_ = cv::Mat(height, width, CV_8UC1, buffer);
        }
        else if (stOutFrame.stFrameInfo.enPixelType == PixelType_Gvsp_RGB8_Packed || stOutFrame.stFrameInfo.enPixelType == PixelType_Gvsp_BGR8_Packed) {
            image_ = cv::Mat(height, width, CV_8UC3, buffer);
        }
        else {
            image_ = cv::Mat();
        }

        nRet = MV_CC_FreeImageBuffer(handle, &stOutFrame);
        if (nRet != MV_OK)
        {
            //printf("Free Image Buffer fail! nRet [0x%x]\n", nRet);
        }
    }
    else
    {
        CameraStatusCode::ERROR;
        //printf("Get Image fail! nRet [0x%x]\n", nRet);
    }

    return  CameraStatusCode::SUCCESS;
}

CameraStatusCode MCameraHik::get_camera_frame_rate(float &frame_rate) const{
    MVCC_FLOATVALUE frameRateValue = {0};

    int nRet = MV_CC_GetFloatValue(handle, "AcquisitionFrameRate", &frameRateValue);
    if (MV_OK != nRet) {
        //std::cerr << "Get AcquisitionFrameRate fail! nRet [" << nRet << "]" << std::endl;
        //return false;
    }

    frame_rate = frameRateValue.fCurValue;
    return  CameraStatusCode::SUCCESS;
}

CameraStatusCode MCameraHik::set_camera_exposure(float exposure_value) {
    int nRet = MV_CC_SetEnumValue(handle, "ExposureAuto", MV_EXPOSURE_AUTO_MODE_OFF);
    if (MV_OK != nRet)
    {
        //printf("Set Trigger Mode fail! nRet [0x%x]\n", nRet);
        //break;
    }

    nRet = MV_CC_SetFloatValue(handle, "ExposureTime", exposure_value);
    if (MV_OK != nRet)
    {
        //printf("Set Trigger Mode fail! nRet [0x%x]\n", nRet);
        //break;
    }
    return  CameraStatusCode::SUCCESS;
}

CameraStatusCode MCameraHik::get_camera_exposure(float &exposure_value) const{
    MVCC_FLOATVALUE stFloatValue = {0};
    int nRet = MV_CC_GetFloatValue(handle, "ExposureTime", &stFloatValue);
    if (MV_OK != nRet)
    {
        //return nRet;
    }
    exposure_value = stFloatValue.fCurValue;
    return  CameraStatusCode::SUCCESS;
}

CameraStatusCode MCameraHik::set_camera_trigger(const std::string &trigger_mode){
    // ch:设置触发模式为off | en:Set trigger mode as off
    if(trigger_mode == "off"){
        int nRet = MV_CC_SetEnumValue(handle, "TriggerMode", 0);
        if (MV_OK != nRet)
        {
            //printf("Set Trigger Mode fail! nRet [0x%x]\n", nRet);
            //break;
        }
    }
    else if(trigger_mode == "on"){
        int nRet = MV_CC_SetEnumValue(handle, "TriggerMode", 1);
        if (MV_OK != nRet)
        {
            //printf("Set Trigger Mode fail! nRet [0x%x]\n", nRet);
            //break;
        }
    }
    return  CameraStatusCode::SUCCESS;
}

CameraStatusCode MCameraHik::get_camera_trigger(std::string &trigger_mode) const{
    MVCC_ENUMVALUE stEnumValue = {0};
    int nRet = MV_CC_GetEnumValue(handle, "TriggerMode", &stEnumValue);
    if (MV_OK != nRet)
    {
        //printf("Set Trigger Mode fail! nRet [0x%x]\n", nRet);
        //break;
    }
    if (MV_TRIGGER_MODE_ON ==  stEnumValue.nCurValue)
    {
        trigger_mode = "on";
    }
    else
    {
        trigger_mode = "off";
    }
    return  CameraStatusCode::SUCCESS;
}

CameraStatusCode MCameraHik::import_file(const char *file_path){
    int nRet = MV_CC_FeatureLoad(handle, file_path);
    if (MV_OK != nRet)
    {
        //printf("Load Feature fail! nRet [0x%x]\n", nRet);
        //break;
    }
    return  CameraStatusCode::SUCCESS;
}

CameraStatusCode MCameraHik::export_file(const char *file_path) const{
    int nRet = MV_CC_FeatureSave(handle, file_path);
    if (MV_OK != nRet)
    {
        //printf("Save Feature fail! nRet [0x%x]\n", nRet);
        //break;
    }
    return  CameraStatusCode::SUCCESS;

}


