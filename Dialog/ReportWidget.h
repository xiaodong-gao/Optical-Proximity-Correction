#ifndef REPORTWIDGET_H
#define REPORTWIDGET_H

#include <QWidget>
#include <QLabel>
#include "DataStructure/MImageProcess.h"
#include "DataStructure/MCommonBase.h"
#include "DataStructure/MExportPDF.h"
#include <QFileDialog>

namespace Ui {
class ReportWidget;
}

class ReportWidget : public QWidget{
    Q_OBJECT
public:
    explicit ReportWidget(QWidget *parent = nullptr);
    ~ReportWidget();
    void set_work_done_result(const WorkDoneResult &work_done_result);
    void show_result(QLabel *label, cv::Mat image);
private slots:
    void export_pdf_file();
private:
    Ui::ReportWidget *ui;
    std::unique_ptr<MImageProcess> image_process_;
    std::unique_ptr<MExportPDF> export_pdf_;
    WorkDoneResult work_done_result_;
    QList<QWidget*> controls;
};

#endif // REPORTWIDGET_H
