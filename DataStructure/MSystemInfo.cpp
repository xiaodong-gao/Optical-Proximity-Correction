#include "MSystemInfo.h"

MSystemInfo * MSystemInfo::instance_ = nullptr;

MSystemInfo::MSystemInfo(){

}

MSystemInfo::~MSystemInfo(){
    if (nullptr == instance_){
        delete instance_;
        instance_ = nullptr;
    }
}

MSystemInfo * MSystemInfo::Instance(){
    if(nullptr == instance_)
        instance_ = new MSystemInfo();
    return instance_;
}

void MSystemInfo::init(){
    camera_factory_ = std::make_unique<MCameraHikFactory>();
    image_process_ = std::make_unique<MImageProcess>();
    camera_factory_->init();
}

void MSystemInfo::open(){
    camera_factory_->open();
}

void MSystemInfo::close(){
    camera_factory_->close();
}


void MSystemInfo::set_recipe_param(const RecipeParam & recipe_param){
    recipe_param_ = recipe_param;
}

void MSystemInfo::get_recipe_param(RecipeParam &recipe_param) const{
    recipe_param = recipe_param_;
}

