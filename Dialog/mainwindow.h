#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include "DataStructure/MCommonBase.h"
#include <QICon>
#include <QLabel>
#include "Dialog/ConfigurationWidget.h"
#include "Dialog/ImageProcessWidget.h"
#include "Dialog/ReportWidget.h"
#include "DataStructure/ThreadManager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void centerOnScreen();
    void setHalfScreenSize();
    void setLabelColor(QLabel* label, const QColor& color);
public slots:
    void change_tab_widget(int index);
    void recv_work_status_form_thread_manager(int widget_index, bool status);
private:
    Ui::MainWindow *ui;
    std::unique_ptr<ConfigurationWidget> configuration_widget_;
    std::unique_ptr<ImageProcessWidget> coarse_location_widget_,auto_focus_widget_,fine_location_widget_,quality_analysis_widget_;
    std::unique_ptr<ReportWidget> report_widget_;
    std::unique_ptr<ThreadManager> thread_manager_;
};
#endif // MAINWINDOW_H
