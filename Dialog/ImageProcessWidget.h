#ifndef IMAGEPROCESSWIDGET_H
#define IMAGEPROCESSWIDGET_H

#include <QWidget>
#include "DataStructure/MImageProcess.h"
#include "DataStructure/MCommonBase.h"

namespace Ui {
class ImageProcessWidget;
}

class ImageProcessWidget : public QWidget{
    Q_OBJECT
public:
    explicit ImageProcessWidget(int id, QWidget *parent = nullptr);
    ~ImageProcessWidget();
    void process_coarse_location(WorkDoneResult  work_done_result);
    void process_auto_focus(WorkDoneResult  work_done_result);
    void process_fine_location(WorkDoneResult  work_done_result);
    void process_quality_analysis(WorkDoneResult  work_done_result);
public slots:
    void recv_work_done_from_thread_manager(const WorkDoneResult& work_done_result);
private:
    Ui::ImageProcessWidget *ui;
    std::unique_ptr<MImageProcess> image_process_;
    int id_;
};

#endif // IMAGEPROCESSWIDGET_H
