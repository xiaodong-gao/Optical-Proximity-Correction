#ifndef MCAMERAFACTORY_H
#define MCAMERAFACTORY_H

#include "DataStructure/MCommonBase.h"
#include "DataStructure/Camera/MCameraBase.h"
#include <vector>
#include <memory>
#include <QString>
#include <QVector>

class MCameraFactory{
public:   
    virtual ~MCameraFactory();
    virtual CameraStatusCode init() = 0 ;
    virtual CameraStatusCode open() = 0;
    virtual CameraStatusCode close() = 0 ;
    std::vector<std::unique_ptr<MCameraBase>> vec_cameras_{};
    QVector<QString> vec_camera_serial_number_{};
protected:
    explicit MCameraFactory();
};

#endif // MCAMERAFACTORY_H
