#include "ReportWidget.h"
#include "ui_reportwidget.h"

ReportWidget::ReportWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReportWidget)
{
    ui->setupUi(this);

    controls.push_back(ui->labelCoarseImage1);
    controls.push_back(ui->labelCoarseImage2);
    controls.push_back(ui->labelFineImage1);
    controls.push_back(ui->labelFineImage2);
    controls.push_back(ui->labelFocusImage1);
    controls.push_back(ui->labelFocusImage2);
    controls.push_back(ui->labelQualityImage1);
    controls.push_back(ui->labelQualityImage2);
    controls.push_back(ui->btnExportPDF);
    image_process_ = std::make_unique<MImageProcess>();
    export_pdf_ = std::make_unique<MExportPDF>();
    connect(ui->btnExportPDF, &QPushButton::clicked, this, &ReportWidget::export_pdf_file);
}

ReportWidget::~ReportWidget()
{
    delete ui;
}

void ReportWidget::show_result(QLabel *label, cv::Mat image){
    QImage q_image = image_process_->Mat2QImage(image);
    QPixmap q_pixmap = QPixmap::fromImage(q_image).scaled(label->size());
    QList<QWidget*> controls_ = controls;
    for (int i = 0; i < controls_.size(); ++i) {
        if (controls_[i] == label) {
            controls_.removeAt(i); // 移除控件
            break; // 如果你只需要移除第一个找到的控件，可以使用 break 退出循环
        }
    }


    ((ZoomableLabel *)label)->setControls(controls_);
    label->setPixmap(q_pixmap);
    /*
    if (!q_pixmap.isNull()) {
        QPixmap scaled_pixmap = q_pixmap.scaled(label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        label->setPixmap(scaled_pixmap);
        update();
    }
    */
}

void ReportWidget::set_work_done_result(const WorkDoneResult &work_done_result){
   work_done_result_ = work_done_result;
   show_result(ui->labelCoarseImage1, work_done_result.coarse_mat1);
   show_result(ui->labelCoarseImage2, work_done_result.coarse_mat2);

   show_result(ui->labelFocusImage1, work_done_result.roi_src_mat1);
   show_result(ui->labelFocusImage2, work_done_result.roi_src_mat2);

   show_result(ui->labelFineImage1, work_done_result.focus_mat1);
   show_result(ui->labelFineImage2, work_done_result.focus_mat2);

   if(work_done_result.src_mat1.empty())
   {
       show_result(ui->labelQualityImage1, work_done_result.roi_src_mat1);
       show_result(ui->labelQualityImage2, work_done_result.roi_src_mat2);
   }
   else{
       cv::Mat mat1, mat2;
       cv::applyColorMap(work_done_result.roi_src_mat1, mat1, cv::COLORMAP_JET);
       cv::applyColorMap(work_done_result.roi_src_mat2, mat2, cv::COLORMAP_JET);
       show_result(ui->labelQualityImage1, mat1);
       show_result(ui->labelQualityImage2, mat2);
   }
}


void ReportWidget::export_pdf_file(){
        QString fileName = QFileDialog::getSaveFileName(nullptr,
                                                        "Save PDF",
                                                        "",
                                                        "PDF Files (*.pdf);;All Files (*)");
        if (!fileName.isEmpty()) {
            if (!fileName.endsWith(".pdf", Qt::CaseInsensitive)) {
                            fileName += ".pdf";
            }
            else{
                // 获取图像的宽度和高度
                int   image_width = work_done_result_.src_mat1.cols;
                int   image_height = work_done_result_.src_mat1.rows;
                float width = A4_WIDTH / 2 - 20; // Leave some margin
                float height = width * image_height / image_width;

                //src image
                float x1 = 10;
                float y1 = A4_HEIGHT - height - 10;
                float offset_y = 20;
                export_pdf_->drawImage(work_done_result_.src_mat1, x1, y1, width, height, 1,false);
                std::string str1 = "This is the src tempalte image.";
                export_pdf_->drawText(str1, x1, y1 - offset_y, 12,1);

                float x2 = width + 30;
                float y2 = A4_HEIGHT - height - 10;
                export_pdf_->drawImage(work_done_result_.src_mat2, x2, y2, width, height,1,false);
                std::string str2 = "This is the src camera image.";
                export_pdf_->drawText(str2, x2, y2 - offset_y, 12,1);

                //coarse image
                float x3 = 10;
                float y3 = y1- offset_y - height - 10;
                export_pdf_->drawImage(work_done_result_.roi_src_mat1, x3, y3, width, height,1,false);
                std::string str3 = "This is the coarse location image1.";
                export_pdf_->drawText(str3, x3, y3 - offset_y, 12,1);

                float x4 = width + 30;
                float y4 = y2 - offset_y - height - 10;
                export_pdf_->drawImage(work_done_result_.roi_src_mat2, x4, y4, width, height,1,false);
                std::string str4 = "This is the coarse location image2.";
                export_pdf_->drawText(str4, x4, y4 - offset_y, 12,1);

                //focus image
                float x5 = 10;
                float y5 = y3 - offset_y - height - 10;
                export_pdf_->drawImage(work_done_result_.focus_mat1, x5, y5, width, height,1,false);
                std::string str5 = "This is the focus length image1.";
                export_pdf_->drawText(str5, x5, y5 - offset_y, 12,1);

                float x6 = width + 30;
                float y6 = y4 - offset_y - height - 10;
                export_pdf_->drawImage(work_done_result_.focus_mat2, x6, y6, width, height,1,false);
                std::string str6 = "This is the focus length image2.";
                export_pdf_->drawText(str6, x6, y6 - offset_y, 12,1);

                float Deviation1 = work_done_result_.laplacianVariance1;
                float Deviation2 = work_done_result_.laplacianVariance2;
                std::string str5_result = "Deviation1 : " + std::to_string(Deviation1);
                export_pdf_->drawText(str5_result, x5, y5 - offset_y - offset_y, 12,1);
                std::string str6_result = "Deviation2 : " + std::to_string(Deviation2);
                export_pdf_->drawText(str6_result, x5, y5 - offset_y - offset_y - offset_y, 12,1);
                std::string str56_diff = "diff : " + std::to_string(std::fabs(Deviation2- Deviation1));
                export_pdf_->drawText(str56_diff, x5, y5 - offset_y - offset_y - offset_y - offset_y, 12,1);
               

                //fine location image
                export_pdf_->drawImage(work_done_result_.roi_src_mat1, x1, y1, width, height,2,false);
                std::string str7 = "This is the fine location image1.";
                export_pdf_->drawText(str7, x1, y1 - offset_y, 12,2);

                export_pdf_->drawImage(work_done_result_.roi_src_mat2, x2, y2, width, height,2,false);
                std::string str8 = "This is the fine location image2.";
                export_pdf_->drawText(str8, x2, y2 - offset_y, 12,2);

                std::string str78_result = "This is the fine location result : offset x:" + std::to_string(work_done_result_.fine_pt.x) + "offset y : " + std::to_string(work_done_result_.fine_pt.y);
                export_pdf_->drawText(str78_result, x1, y1 - offset_y * 2, 12, 2);
                
                
                //quality analysis image
                //coarse image
                float x9 = 10;
                float y9 = y1 - offset_y * 2 - height - 10;
                // 将图像转换为 RGB 格式
                //cv::Mat rgb_heatmap;
                //cv::normalize(work_done_result_.ssim_mat1, work_done_result_.ssim_mat1, 0, 255, cv::NORM_MINMAX);
                //cv::cvtColor(work_done_result_.ssim_mat1, rgb_heatmap, cv::COLOR_BGR2RGB);
                //cv::cvtColor(work_done_result_.ssim_mat1, work_done_result_.ssim_mat1, cv::COLOR_BGR2RGB);
                export_pdf_->drawImage(work_done_result_.ssim_mat1, x9, y9, width, height, 2,true);
                std::string str9 = "This is the quality analysis image1.";
                export_pdf_->drawText(str9, x9, y9 - offset_y, 12, 2);

                float x10 = width + 30;
                float y10 = y2 - offset_y * 2 - height - 10;
                //cv::cvtColor(work_done_result_.ssim_mat2, work_done_result_.ssim_mat2, cv::COLOR_BGR2RGB);
                export_pdf_->drawImage(work_done_result_.ssim_mat2, x10, y10, width, height, 2,true);
                std::string str10 = "This is the quality analysis image2.";
                export_pdf_->drawText(str10, x10, y10 - offset_y, 12, 2);

                export_pdf_->savePDF(fileName.toStdString());
                //export_pdf_->drawImage(work_done_result_.src_mat1,)
                //work_done_result_.src_mat1
                //qDebug() << "Selected file:" << fileName;
                // 这里可以添加保存文件的逻辑
            }
    }
}
