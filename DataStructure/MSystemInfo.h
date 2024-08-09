#ifndef MSYSTEMINFO_H
#define MSYSTEMINFO_H

#include <QObject>
#include <QDebug>
#include "DataStructure/Camera/MCameraFactory.h"
#include "DataStructure/Camera/MCameraHikFactory.h"
#include "DataStructure/MImageProcess.h"

class MSystemInfo {
public:
    explicit MSystemInfo();
    ~MSystemInfo();
    MSystemInfo(const MSystemInfo &rhs) = delete;
    MSystemInfo& operator=(const MSystemInfo &rhs) = delete;
    static MSystemInfo * Instance();
    std::unique_ptr<MCameraFactory> camera_factory_{nullptr};
    std::unique_ptr<MImageProcess> image_process_{nullptr};
    void init();
    void open();
    void close();

    void set_recipe_param(const RecipeParam & recipe_param);
    void get_recipe_param(RecipeParam &recipe_param) const;

private:
    static MSystemInfo * instance_;
    RecipeParam recipe_param_;
};

#endif // MSYSTEMINFO_H
