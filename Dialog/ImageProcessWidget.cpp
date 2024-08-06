#include "ImageProcessWidget.h"
#include "ui_imageprocesswidget.h"

ImageProcessWidget::ImageProcessWidget(int id, QWidget *parent) :
    id_{id},
    QWidget(parent),
    ui(new Ui::ImageProcessWidget)
{
    ui->setupUi(this);
    image_process_ = std::make_unique<MImageProcess>();
}

ImageProcessWidget::~ImageProcessWidget()
{
    delete ui;
}

void ImageProcessWidget::recv_work_done_from_thread_manager(const WorkDoneResult& work_done_result) {
    switch (id_) {
    case 1:
        process_coarse_location(work_done_result);
        break;
    case 2:
        process_auto_focus(work_done_result);
        break;
    case 3:
        process_fine_location(work_done_result);
        break;
    case 4:
        process_quality_analysis(work_done_result);
        break;
    }
}

void ImageProcessWidget::process_coarse_location(WorkDoneResult  work_done_result) {
    QImage q_roi_img1 = image_process_->Mat2QImage(work_done_result.coarse_mat1);
    QImage q_roi_img2 = image_process_->Mat2QImage(work_done_result.coarse_mat2);
    QPixmap q_pixmap_a = QPixmap::fromImage(q_roi_img1);
    if (!q_pixmap_a.isNull()) {
        //QPixmap scaled_pixmap_a = q_pixmap_a.scaled(ui->labelLeft->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QPixmap scaled_pixmap_a = q_pixmap_a.scaled(ui->labelLeft->size());
        ui->labelLeft->setPixmap(scaled_pixmap_a);
        update();
    }

    QPixmap q_pixmap_b = QPixmap::fromImage(q_roi_img2);
    if (!q_pixmap_b.isNull()) {
        //QPixmap scaled_pixmap_b = q_pixmap_b.scaled(ui->labelRight->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QPixmap scaled_pixmap_b = q_pixmap_b.scaled(ui->labelRight->size());
        ui->labelRight->setPixmap(scaled_pixmap_b);
        update();
    }

    cv::Mat result_mat;
    cv::absdiff(work_done_result.roi_src_mat1, work_done_result.roi_src_mat2, result_mat);       // |I1 - I2|
    cv::applyColorMap(result_mat, result_mat, cv::COLORMAP_JET);


    QImage q_img_result = image_process_->Mat2QImage(result_mat);
    QPixmap q_pixmap_result = QPixmap::fromImage(q_img_result);
    if (!q_pixmap_result.isNull()) {
        //QPixmap scaled_pixmap_result = q_pixmap_result.scaled(ui->labelResult->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QPixmap scaled_pixmap_result = q_pixmap_result.scaled(ui->labelResult->size());
        ui->labelResult->setPixmap(scaled_pixmap_result);
        update();
    }

    ui->labelInfo->setStyleSheet("QLabel { color : red; }");
    QString str = "调整相机的姿态Position(x,y,z,a,b,c)使右侧相机实时采集的图像ROI区域特征和左侧标准模板图像的区域特征一致\n";
    ui->labelInfo->setText(str);
    ui->labelInfo->update();
}

void ImageProcessWidget::process_auto_focus(WorkDoneResult  work_done_result) {

    QImage q_image_a = image_process_->Mat2QImage(work_done_result.focus_mat1);
    QImage q_image_b = image_process_->Mat2QImage(work_done_result.focus_mat2);

    QPixmap q_pixmap_a = QPixmap::fromImage(q_image_a);
    if (!q_pixmap_a.isNull()) {
        //QPixmap scaled_pixmap_a = q_pixmap_a.scaled(ui->labelLeft->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QPixmap scaled_pixmap_a = q_pixmap_a.scaled(ui->labelLeft->size());
        ui->labelLeft->setPixmap(scaled_pixmap_a);
        update();
    }

    QPixmap q_pixmap_b = QPixmap::fromImage(q_image_b);
    if (!q_pixmap_b.isNull()) {
        //QPixmap scaled_pixmap_b = q_pixmap_b.scaled(ui->labelRight->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QPixmap scaled_pixmap_b = q_pixmap_b.scaled(ui->labelRight->size());
        ui->labelRight->setPixmap(scaled_pixmap_b);
        update();
    }

    cv::Mat result_mat;
    cv::absdiff(work_done_result.roi_src_mat1, work_done_result.roi_src_mat2, result_mat);       // |I1 - I2|
    cv::applyColorMap(result_mat, result_mat, cv::COLORMAP_JET);

    QImage q_img_result = image_process_->Mat2QImage(result_mat);
    QPixmap q_pixmap_result = QPixmap::fromImage(q_img_result);
    if (!q_pixmap_result.isNull()) {
        //QPixmap scaled_pixmap_result = q_pixmap_result.scaled(ui->labelResult->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QPixmap scaled_pixmap_result = q_pixmap_result.scaled(ui->labelResult->size());
        ui->labelResult->setPixmap(scaled_pixmap_result);
        update();
    }

    ui->labelInfo->setStyleSheet("QLabel { color : red; }");
    QString str = "调整相机镜头光圈的焦距和光圈的曝光量，使右侧相机实时采集图像的轮廓和左侧标准模板图像的轮廓接近一致\n";
    str += "\n";
    str += "laplacianVariance1:" + QString::number(work_done_result.laplacianVariance1) + "\n";
    str += "\n";
    str += "laplacianVariance2:" + QString::number(work_done_result.laplacianVariance2) + "\n";
    str += "\n";
    str += "diff:" + QString::number(work_done_result.laplacianVariance1 - work_done_result.laplacianVariance2) + "\n";
    ui->labelInfo->setText(str);
    ui->labelInfo->update();

}

void ImageProcessWidget::process_fine_location(WorkDoneResult  work_done_result) {
    QImage q_image_a = image_process_->Mat2QImage(work_done_result.roi_src_mat1);
    QImage q_image_b = image_process_->Mat2QImage(work_done_result.roi_src_mat2);

    QPixmap q_pixmap_a = QPixmap::fromImage(q_image_a);
    if (!q_pixmap_a.isNull()) {
        //QPixmap scaled_pixmap_a = q_pixmap_a.scaled(ui->labelLeft->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QPixmap scaled_pixmap_a = q_pixmap_a.scaled(ui->labelLeft->size());
        ui->labelLeft->setPixmap(scaled_pixmap_a);
        update();
    }

    QPixmap q_pixmap_b = QPixmap::fromImage(q_image_b);
    if (!q_pixmap_b.isNull()) {
        //QPixmap scaled_pixmap_b = q_pixmap_b.scaled(ui->labelRight->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QPixmap scaled_pixmap_b = q_pixmap_b.scaled(ui->labelRight->size());
        ui->labelRight->setPixmap(scaled_pixmap_b);
        update();
    }

    cv::Mat result_mat;
    cv::absdiff(work_done_result.roi_src_mat1, work_done_result.roi_src_mat2, result_mat);       // |I1 - I2|
    cv::applyColorMap(result_mat, result_mat, cv::COLORMAP_JET);

    QImage q_img_result = image_process_->Mat2QImage(result_mat);
    QPixmap q_pixmap_result = QPixmap::fromImage(q_img_result);
    if (!q_pixmap_result.isNull()) {
        //QPixmap scaled_pixmap_result = q_pixmap_result.scaled(ui->labelResult->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QPixmap scaled_pixmap_result = q_pixmap_result.scaled(ui->labelResult->size());
        ui->labelResult->setPixmap(scaled_pixmap_result);
        update();
    }

    ui->labelInfo->setStyleSheet("QLabel { color : red; }");
    QString str = "通过计算位置的偏移量，移动产品，使左右偏移量接近一致\n";
    str += "\n";
    str += "offset x:" + QString::number(work_done_result.fine_pt.x) + "\n";
    str += "offset y:" + QString::number(work_done_result.fine_pt.y) + "\n";
    ui->labelInfo->setText(str);
    ui->labelInfo->update();
}

void ImageProcessWidget::process_quality_analysis(WorkDoneResult  work_done_result) {
    
    //double ssim_value{0.0};
    //cv::Mat ssim_img = image_process_->getMSSIM(mat1, mat2, ssim_value);
    //cv::Mat mat1, mat2, ssim_img;
    //cv::applyColorMap(work_done_result.roi_src_mat1, mat1, cv::COLORMAP_JET);
    //cv::applyColorMap(work_done_result.roi_src_mat2, mat2, cv::COLORMAP_JET);
    //cv::applyColorMap(work_done_result.ssim_mat, ssim_img, cv::COLORMAP_JET);

    QImage q_roi_img1 = image_process_->Mat2QImage(work_done_result.ssim_mat1);
    QImage q_roi_img2 = image_process_->Mat2QImage(work_done_result.ssim_mat2);

    QPixmap q_pixmap_a = QPixmap::fromImage(q_roi_img1);
    if (!q_pixmap_a.isNull()) {
        //QPixmap scaled_pixmap_a = q_pixmap_a.scaled(ui->labelLeft->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QPixmap scaled_pixmap_a = q_pixmap_a.scaled(ui->labelLeft->size());
        ui->labelLeft->setPixmap(scaled_pixmap_a);
        update();
    }

    QPixmap q_pixmap_b = QPixmap::fromImage(q_roi_img2);
    if (!q_pixmap_b.isNull()) {
        //QPixmap scaled_pixmap_b = q_pixmap_b.scaled(ui->labelRight->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QPixmap scaled_pixmap_b = q_pixmap_b.scaled(ui->labelRight->size());
        ui->labelRight->setPixmap(scaled_pixmap_b);
        update();
    }

    QImage q_img_result = image_process_->Mat2QImage(work_done_result.ssim_mat);
    QPixmap q_pixmap_result = QPixmap::fromImage(q_img_result);
    if (!q_pixmap_result.isNull()) {
        //QPixmap scaled_pixmap_result = q_pixmap_result.scaled(ui->labelResult->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QPixmap scaled_pixmap_result = q_pixmap_result.scaled(ui->labelResult->size());
        ui->labelResult->setPixmap(scaled_pixmap_result);
        update();
    }

    ui->labelInfo->setStyleSheet("QLabel { color : red; }");
    QString str = "图像的质量分析结果，通过移动光源的位置Position（X，Y，Z，A，B，C）和光源的亮度数值，使右侧实时采集的图像的热力图左侧图像接近一致。\n";
    str += "\n";
    str += "ssim value:" + QString::number(work_done_result.ssim_value);
    ui->labelInfo->setText(str);
    ui->labelInfo->update();
}
