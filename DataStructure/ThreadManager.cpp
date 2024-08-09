#include "ThreadManager.h"

ThreadManager::ThreadManager(QObject *parent) :
    QThread(parent),
    m_running(false),
    m_stop(false) {

}

ThreadManager::~ThreadManager(){
    m_running = false;
    m_stop = false;
}

void ThreadManager::start() {
    if (!m_running.load()) {
        m_stop.store(false);
        QThread::start();
        //qDebug() << "Thread start...\n";
    }
}

void ThreadManager::stop() {
    if (m_running.load()) {
        m_stop.store(true);
    }
    //qDebug() << "Thread stop...\n";
}

bool ThreadManager::isRunning() const {
    return m_running.load();
}

void ThreadManager::run(){
    m_running.store(true);
    //cv::Mat src_mat1, src_mat2;
    //std::vector<cv::Rect> selection_rois_;
    //int camera_id;
    //MSystemInfo::Instance()->get_configuration_param(src_mat1, src_mat2, selection_rois_, camera_id);
    RecipeParam recipe_param_;
    MSystemInfo::Instance()->get_recipe_param(recipe_param_);
    MSystemInfo::Instance()->camera_factory_->vec_cameras_[recipe_param_.camera_id_]->grab();

    while (!m_stop.load()) {
        //QThread::msleep(100); // Sleep for 100 milliseconds
        // 这里是线程要执行的任务

        //camera grab image
        CameraStatusCode camera_code = MSystemInfo::Instance()->camera_factory_->vec_cameras_[recipe_param_.camera_id_]->get_mat(recipe_param_.src_mat2_);
        if (camera_code == CameraStatusCode::ERROR)
            continue;

        work_done_result.src_mat1 = recipe_param_.src_mat1_;
        work_done_result.src_mat2 = recipe_param_.src_mat2_;
        work_done_result.coarse_mat1 = recipe_param_.src_mat1_;
        work_done_result.coarse_mat2 = recipe_param_.src_mat2_;

        //cv::cvtColor(work_done_result.src_mat1, work_done_result.src_mat1, cv::COLOR_GRAY2BGR);
        //cv::cvtColor(work_done_result.src_mat2, work_done_result.src_mat2, cv::COLOR_GRAY2BGR);
        //cv::cvtColor(work_done_result.coarse_mat1, work_done_result.coarse_mat1, cv::COLOR_GRAY2BGR);
        //cv::cvtColor(work_done_result.coarse_mat2, work_done_result.coarse_mat2, cv::COLOR_GRAY2BGR);
        //first widget
        //cv::Mat corase_mat1, coarse_mat2;
        for (const auto& roi : recipe_param_.selection_rois_) {
            cv::rectangle(work_done_result.coarse_mat1, roi, cv::Scalar(0, 0, 255), 10);  // 绿色边框，线宽为 2
            cv::rectangle(work_done_result.coarse_mat2, roi, cv::Scalar(0, 0, 255), 10);  // 绿色边框，线宽为 2
        }
        
        if((!recipe_param_.src_mat1_.empty()) && (!recipe_param_.src_mat2_.empty()) && (recipe_param_.selection_rois_.size() != 0))
            emit send_work_status_to_optial_tool_dlg(1, true);
        else
            emit send_work_status_to_optial_tool_dlg(1, false);
        //second widget
        cv::Mat roi_src_mat1 = MSystemInfo::Instance()->image_process_->extractRois(recipe_param_.src_mat1_, recipe_param_.selection_rois_);
        cv::Mat roi_src_mat2 = MSystemInfo::Instance()->image_process_->extractRois(recipe_param_.src_mat2_, recipe_param_.selection_rois_);
        
        int width =  recipe_param_.src_mat1_.cols;
        int height = recipe_param_.src_mat1_.rows;

        cv::resize(roi_src_mat1, roi_src_mat1, cv::Size(width/4, height/4));
        cv::resize(roi_src_mat2, roi_src_mat2, cv::Size(width/4, height/4));

        cv::Mat focus_mat1, focus_mat2;
        double laplacianVariance1 = MSystemInfo::Instance()->image_process_->getLaplacian(roi_src_mat1, focus_mat1);
        double laplacianVariance2 = MSystemInfo::Instance()->image_process_->getLaplacian(roi_src_mat2, focus_mat2);

        work_done_result.focus_mat1 = focus_mat1;
        work_done_result.laplacianVariance1 = laplacianVariance1;
        work_done_result.focus_mat2 = focus_mat2;
        work_done_result.laplacianVariance2 = laplacianVariance2;
        work_done_result.roi_src_mat1 = roi_src_mat1;
        work_done_result.roi_src_mat2 = roi_src_mat2;

        emit send_work_status_to_optial_tool_dlg(2, std::fabs(laplacianVariance1 - laplacianVariance2) - recipe_param_.focus_length_value < 1e-9);

        //third widget
        cv::Point2d pt = MSystemInfo::Instance()->image_process_->getPhaseCorr(roi_src_mat1, roi_src_mat2);

        cv::Mat translation_matrix = (cv::Mat_<double>(2, 3) << 1, 0, -pt.x, 0, 1, -pt.y);
        cv::warpAffine(work_done_result.roi_src_mat2, work_done_result.roi_src_mat2, translation_matrix, work_done_result.roi_src_mat2.size());


        work_done_result.fine_pt = pt;
        if((std::fabs(pt.x ) - recipe_param_.fine_offset_x < 1e-9) && (std::fabs(pt.y ) - recipe_param_.fine_offset_y < 1e-9))
            emit send_work_status_to_optial_tool_dlg(3, true);
        else
            emit send_work_status_to_optial_tool_dlg(3, false);
        
        //fourth widget
        double ssim_value{ 0.0 };
        cv::Mat ssim_mat = MSystemInfo::Instance()->image_process_->getMSSIM(roi_src_mat1, roi_src_mat2, ssim_value);
        work_done_result.ssim_value = ssim_value;
        work_done_result.ssim_mat = ssim_mat;
        cv::applyColorMap(work_done_result.roi_src_mat1, work_done_result.ssim_mat1, cv::COLORMAP_JET);
        cv::applyColorMap(work_done_result.roi_src_mat2, work_done_result.ssim_mat2, cv::COLORMAP_JET);

        if(m_running.load() == true)
            send_work_done_to_image_process_widget(work_done_result);


        if((ssim_value >= recipe_param_.ssim_min_value) && (ssim_value <= recipe_param_.ssim_max_value)){
            emit send_work_status_to_optial_tool_dlg(4, true);
        } else {
            emit send_work_status_to_optial_tool_dlg(4, false);
        }

        QThread::msleep(100); // Sleep for 100 milliseconds

    }
    MSystemInfo::Instance()->camera_factory_->vec_cameras_[recipe_param_.camera_id_]->stop();
    m_running.store(false);
}

WorkDoneResult ThreadManager::get_work_done_result() const{
    return work_done_result;
}


