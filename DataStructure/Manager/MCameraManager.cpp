#include "MCameraManager.h"

MCameraManager::MCameraManager()
{

}

MCameraManager::~MCameraManager()
{

}

void MCameraManager::addCamera(CameraType type, int deviceID){


}


void MCameraManager::releaseAllCameras(){

}

std::vector<MCameraBase>* MCameraManager::getCamerasByType(CameraType type){
    if (cameraMap.find(type) != cameraMap.end()) {
        //return &cameraMap[type];
    }
    return nullptr;
}
