#ifndef OPTICALTOOLDLG_H
#define OPTICALTOOLDLG_H

#include <QDialog>
#include <QICon>
#include <QLabel>
#include "Dialog/ConfigurationWidget.h"
#include "Dialog/ImageProcessWidget.h"
#include "Dialog/ReportWidget.h"
#include "DataStructure/ThreadManager.h"

namespace Ui {
class OpticalToolDlg;
}

class OpticalToolDlg : public QDialog{
    Q_OBJECT
public:
    explicit OpticalToolDlg(QWidget *parent = nullptr);
    ~OpticalToolDlg();
    void setLabelColor(QLabel* label, const QColor& color);
public slots:
    void change_tab_widget(int index);
    void recv_work_status_form_thread_manager(int widget_index, bool status);
private:
    Ui::OpticalToolDlg *ui;
    std::unique_ptr<ConfigurationWidget> configuration_widget_;
    std::unique_ptr<ImageProcessWidget> coarse_location_widget_,auto_focus_widget_,fine_location_widget_,quality_analysis_widget_;
    std::unique_ptr<ReportWidget> report_widget_;
    std::unique_ptr<ThreadManager> thread_manager_;
};

#endif // OPTICALTOOLDLG_H
