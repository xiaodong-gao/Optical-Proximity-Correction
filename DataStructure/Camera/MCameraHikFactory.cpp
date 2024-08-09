#include "MCameraHikFactory.h"

MCameraHikFactory::MCameraHikFactory(){
    // ch:初始化SDK | en:Initialize SDK
    int nRet = MV_CC_Initialize();
    if (MV_OK != nRet)
    {
        //printf("Initialize SDK fail! nRet [0x%x]\n", nRet);
        //break;
    }
}

MCameraHikFactory::~MCameraHikFactory(){
    // ch:反初始化SDK | en:Finalize SDK
    MV_CC_Finalize();
}

CameraStatusCode MCameraHikFactory::init(){
    // ch:枚举设备 | en:Enum device
    MV_CC_DEVICE_INFO_LIST stDeviceList;
    memset(&stDeviceList, 0, sizeof(MV_CC_DEVICE_INFO_LIST));
    int nRet = MV_CC_EnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE | MV_GENTL_CAMERALINK_DEVICE | MV_GENTL_CXP_DEVICE | MV_GENTL_XOF_DEVICE, &stDeviceList);
    if (MV_OK != nRet)
    {
        //printf("Enum Devices fail! nRet [0x%x]\n", nRet);
        //break;
    }

    if (stDeviceList.nDeviceNum > 0)
    {
        for (unsigned int i = 0; i < stDeviceList.nDeviceNum; i++)
        {
            //printf("[device %d]:\n", i);
            MV_CC_DEVICE_INFO* pDeviceInfo = stDeviceList.pDeviceInfo[i];
            if (NULL == pDeviceInfo)
            {
                //break;
            }

            if (pDeviceInfo->nTLayerType == MV_GIGE_DEVICE)
            {
                //int nIp1 = ((pDeviceInfo->SpecialInfo.stGigEInfo.nCurrentIp & 0xff000000) >> 24);
                //int nIp2 = ((pDeviceInfo->SpecialInfo.stGigEInfo.nCurrentIp & 0x00ff0000) >> 16);
                //int nIp3 = ((pDeviceInfo->SpecialInfo.stGigEInfo.nCurrentIp & 0x0000ff00) >> 8);
                //int nIp4 = (pDeviceInfo->SpecialInfo.stGigEInfo.nCurrentIp & 0x000000ff);
                //vec_camera_ip_.push_back(QString::number(nIp1) + "." + QString::number(nIp2) + "." + QString::number(nIp3) + "." + QString::number(nIp4));
                vec_camera_serial_number_.push_back(QString::fromUtf8(reinterpret_cast<char*>(pDeviceInfo->SpecialInfo.stGigEInfo.chSerialNumber)));


                // ch:打印当前相机ip和用户自定义名字 | en:print current ip and user defined name
                //printf("CurrentIp: %d.%d.%d.%d\n" , nIp1, nIp2, nIp3, nIp4);
                //printf("UserDefinedName: %s\n\n" , pDeviceInfo->SpecialInfo.stGigEInfo.chUserDefinedName);

            }
            else if (pDeviceInfo->nTLayerType == MV_USB_DEVICE)
            {
                vec_camera_serial_number_.push_back(QString::fromUtf8(reinterpret_cast<char*>(pDeviceInfo->SpecialInfo.stGigEInfo.chSerialNumber)));
                //printf("UserDefinedName: %s\n", pstMVDevInfo->SpecialInfo.stUsb3VInfo.chUserDefinedName);
                //printf("Serial Number: %s\n", pstMVDevInfo->SpecialInfo.stUsb3VInfo.chSerialNumber);
                //printf("Device Number: %d\n\n", pstMVDevInfo->SpecialInfo.stUsb3VInfo.nDeviceNumber);
            }
            else if (pDeviceInfo->nTLayerType == MV_GENTL_GIGE_DEVICE)
            {
                vec_camera_serial_number_.push_back(QString::fromUtf8(reinterpret_cast<char*>(pDeviceInfo->SpecialInfo.stGigEInfo.chSerialNumber)));
                //printf("UserDefinedName: %s\n", pstMVDevInfo->SpecialInfo.stGigEInfo.chUserDefinedName);
                //printf("Serial Number: %s\n", pstMVDevInfo->SpecialInfo.stGigEInfo.chSerialNumber);
                //printf("Model Name: %s\n\n", pstMVDevInfo->SpecialInfo.stGigEInfo.chModelName);
            }
            else if (pDeviceInfo->nTLayerType == MV_GENTL_CAMERALINK_DEVICE)
            {
                vec_camera_serial_number_.push_back(QString::fromUtf8(reinterpret_cast<char*>(pDeviceInfo->SpecialInfo.stGigEInfo.chSerialNumber)));
                //printf("UserDefinedName: %s\n", pstMVDevInfo->SpecialInfo.stCMLInfo.chUserDefinedName);
                //printf("Serial Number: %s\n", pstMVDevInfo->SpecialInfo.stCMLInfo.chSerialNumber);
                //printf("Model Name: %s\n\n", pstMVDevInfo->SpecialInfo.stCMLInfo.chModelName);
            }
            else if (pDeviceInfo->nTLayerType == MV_GENTL_CXP_DEVICE)
            {
                vec_camera_serial_number_.push_back(QString::fromUtf8(reinterpret_cast<char*>(pDeviceInfo->SpecialInfo.stGigEInfo.chSerialNumber)));
                //printf("UserDefinedName: %s\n", pstMVDevInfo->SpecialInfo.stCXPInfo.chUserDefinedName);
                //printf("Serial Number: %s\n", pstMVDevInfo->SpecialInfo.stCXPInfo.chSerialNumber);
                //printf("Model Name: %s\n\n", pstMVDevInfo->SpecialInfo.stCXPInfo.chModelName);
            }
            else if (pDeviceInfo->nTLayerType == MV_GENTL_XOF_DEVICE)
            {
                vec_camera_serial_number_.push_back(QString::fromUtf8(reinterpret_cast<char*>(pDeviceInfo->SpecialInfo.stGigEInfo.chSerialNumber)));
                //printf("UserDefinedName: %s\n", pstMVDevInfo->SpecialInfo.stXoFInfo.chUserDefinedName);
                //printf("Serial Number: %s\n", pstMVDevInfo->SpecialInfo.stXoFInfo.chSerialNumber);
                //printf("Model Name: %s\n\n", pstMVDevInfo->SpecialInfo.stXoFInfo.chModelName);
            }
            else
            {
                printf("Not support.\n");
            }
            //PrintDeviceInfo(pDeviceInfo);
        }
    }
    else
    {
        //printf("Find No Devices!\n");
        //break;
    }

    for(int i = 0 ; i < stDeviceList.nDeviceNum ; i++){
        vec_cameras_.push_back(std::make_unique<MCameraHik>());
        vec_cameras_[i]->init(static_cast<void *>(&stDeviceList), i);
    }

    return  CameraStatusCode::SUCCESS;
}

CameraStatusCode MCameraHikFactory::open() {

    qDebug() << "112" ;
    for (int i = 0; i < vec_cameras_.size(); i++) {
        vec_cameras_[i]->open();
    }
    return  CameraStatusCode::SUCCESS;
}


CameraStatusCode MCameraHikFactory::close(){
    for(int i = 0 ; i < vec_cameras_.size(); i++){
        vec_cameras_[i]->close();
    }
    return  CameraStatusCode::SUCCESS;
}
