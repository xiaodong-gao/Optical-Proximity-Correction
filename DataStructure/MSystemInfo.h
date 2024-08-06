#ifndef MSYSTEMINFO_H
#define MSYSTEMINFO_H

#include "DataStructure/MCameraFactory.h"
#include "DataStructure/MCameraHikFactory.h"
#include "DataStructure/MImageProcess.h"

class MSystemInfo{
public:
    explicit MSystemInfo();
    ~MSystemInfo();
    MSystemInfo(const MSystemInfo &rhs) = delete;
    MSystemInfo& operator=(const MSystemInfo &rhs) = delete;
    static MSystemInfo * Instance();
    std::unique_ptr<MCameraFactory> camera_factory_;
    std::unique_ptr<MImageProcess> image_process_;
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
