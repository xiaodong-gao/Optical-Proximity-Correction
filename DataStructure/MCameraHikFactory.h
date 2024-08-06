#ifndef MCAMERAHIKFACTORY_H
#define MCAMERAHIKFACTORY_H

#include "DataStructure/MCameraFactory.h"
#include "DataStructure/MCameraHik.h"

class MCameraHikFactory : public MCameraFactory{
public:
    explicit MCameraHikFactory();
    ~MCameraHikFactory();
    virtual CameraStatusCode init() override;
    virtual CameraStatusCode open() override;
    virtual CameraStatusCode close() override ;
private:
    MV_CC_DEVICE_INFO_LIST stDeviceList;
};

#endif // MCAMERAHIKFACTORY_H
