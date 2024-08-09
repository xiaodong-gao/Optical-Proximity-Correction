#ifndef MCAMERAMANAGER_H
#define MCAMERAMANAGER_H

#include <map>
#include "DataStructure/MCommonBase.h"
#include "DataStructure/MCameraFactory.h"

class MCameraManager
{
public:
    explicit MCameraManager();
    ~MCameraManager();
    MCameraManager(const MCameraManager& rhs) = delete;
    MCameraManager& operator=(const MCameraManager& rhs) = delete;

    void addCamera(CameraType type, int deviceID);
    void releaseAllCameras();
    std::vector<MCameraBase>* getCamerasByType(CameraType type);

private:
    std::map<CameraType, std::vector<MCameraFactory>> cameraMap;
};

#endif // MCAMERAMANAGER_H
