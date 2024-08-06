#include "OpticalToolDlg.h"
#include "ui_opticaltooldlg.h"

OpticalToolDlg::OpticalToolDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OpticalToolDlg)
{
    ui->setupUi(this);

    setWindowTitle("Optical Proximity Correction");
    setWindowIcon(QIcon("./Icon/OPC.png"));
    setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);

    ui->btnMeasurement->setText(tr("Measurement"));
    ui->btnMeasurement->setIcon(QIcon("./Icon/measurement.png"));

    ui->btnROIRectangle->setText(tr("Rectangle"));
    ui->btnROIRectangle->setIcon(QIcon("./Icon/rectangle.png"));

    ui->btnROIPolygon->setText(tr("polygon"));
    ui->btnROIPolygon->setIcon(QIcon("./Icon/polygon.png"));

    configuration_widget_ = std::make_unique<ConfigurationWidget>(this);
    ui->tabWidget->insertTab(0, configuration_widget_.get(), QIcon("./Icon/configuration.png"), "0:configuration");
    coarse_location_widget_ = std::make_unique<ImageProcessWidget>(1,this);
    ui->tabWidget->insertTab(1, coarse_location_widget_.get(), QIcon("./Icon/coarse_location.png"), "1:coarse location");
    auto_focus_widget_ = std::make_unique<ImageProcessWidget>(2,this);
    ui->tabWidget->insertTab(2, auto_focus_widget_.get(), QIcon("./Icon/auto_focus.png"), "2:auto focus");
    fine_location_widget_ = std::make_unique<ImageProcessWidget>(3,this);
    ui->tabWidget->insertTab(3, fine_location_widget_.get(), QIcon("./Icon/fine_location.png"), "3:fine location");
    quality_analysis_widget_ = std::make_unique<ImageProcessWidget>(4,this);
    ui->tabWidget->insertTab(4, quality_analysis_widget_.get(), QIcon("./Icon/quality_analysis.png"), "4:quality analysis");
    report_widget_ = std::make_unique<ReportWidget>(this);
    ui->tabWidget->insertTab(5, report_widget_.get(), QIcon("./Icon/report.png"), "5:report");

    thread_manager_ = std::make_unique<ThreadManager>(this);

    ui->labelCaorseLocation->setFixedSize(40, 40);   // 设置标签大小
    ui->labelAutoFocus->setFixedSize(40, 40);        // 设置标签大小
    ui->labelFineLocation->setFixedSize(40, 40);     // 设置标签大小
    ui->labelQualityAnalysis->setFixedSize(40, 40);  // 设置标签大小
    
    setLabelColor(ui->labelCaorseLocation, Qt::red);
    setLabelColor(ui->labelAutoFocus, Qt::red);
    setLabelColor(ui->labelFineLocation, Qt::red);
    setLabelColor(ui->labelQualityAnalysis, Qt::red);
    
    QObject::connect(ui->tabWidget, &QTabWidget::currentChanged, this, &OpticalToolDlg::change_tab_widget);
    
    QObject::connect(thread_manager_.get(), &ThreadManager::send_work_done_to_image_process_widget, coarse_location_widget_.get(), &ImageProcessWidget::recv_work_done_from_thread_manager);
    QObject::connect(thread_manager_.get(), &ThreadManager::send_work_done_to_image_process_widget, auto_focus_widget_.get(),      &ImageProcessWidget::recv_work_done_from_thread_manager);
    QObject::connect(thread_manager_.get(), &ThreadManager::send_work_done_to_image_process_widget, fine_location_widget_.get(),   &ImageProcessWidget::recv_work_done_from_thread_manager);
    QObject::connect(thread_manager_.get(), &ThreadManager::send_work_done_to_image_process_widget, quality_analysis_widget_.get(),&ImageProcessWidget::recv_work_done_from_thread_manager);
    QObject::connect(thread_manager_.get(), &ThreadManager::send_work_status_to_optial_tool_dlg, this,&OpticalToolDlg::recv_work_status_form_thread_manager);
}

OpticalToolDlg::~OpticalToolDlg(){
    if (thread_manager_->isRunning() == true)
        thread_manager_->stop();
    QThread::msleep(1000);
    delete ui;
}

void OpticalToolDlg::change_tab_widget(int index) {
    if (0 != index) {
        bool flag = configuration_widget_->isTimerActive();
        if (flag == true) 
            configuration_widget_->stopTimer();
        if (thread_manager_->isRunning() == false)
            thread_manager_->start();
    }
    else {
        if (thread_manager_->isRunning() == true)
            thread_manager_->stop();
    } 

    if(5 == index){
        report_widget_->set_work_done_result(thread_manager_->get_work_done_result());
    }
}


void OpticalToolDlg::setLabelColor(QLabel* label, const QColor& color) {
    QString qss = QString("background-color: %1; border-radius: %2px;")
                  .arg(color.name())
                  .arg(label->width() / 2);  // 使标签成为圆形
    label->setStyleSheet(qss);
}

void OpticalToolDlg::recv_work_status_form_thread_manager(int widget_index, bool status){
    if(widget_index == 1){
        if(status == true)
            setLabelColor(ui->labelCaorseLocation, Qt::green);
        else
            setLabelColor(ui->labelCaorseLocation, Qt::red);
    }

    if(widget_index == 2){
        if(status == true)
            setLabelColor(ui->labelAutoFocus, Qt::green);
        else
            setLabelColor(ui->labelAutoFocus, Qt::red);
    }

    if(widget_index == 3){
        if(status == true)
            setLabelColor(ui->labelFineLocation, Qt::green);
        else
            setLabelColor(ui->labelFineLocation, Qt::red);
    }

    if(widget_index == 4){
        if(status == true)
            setLabelColor(ui->labelQualityAnalysis, Qt::green);
        else
            setLabelColor(ui->labelQualityAnalysis, Qt::red);
    }


}

