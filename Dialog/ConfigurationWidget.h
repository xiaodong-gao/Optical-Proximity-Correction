#ifndef CONFIGURATIONWIDGET_H
#define CONFIGURATIONWIDGET_H

#include <QWidget>
#include <QFileDialog>
#include <QPixmap>
#include <memory>
#include <QDebug>
#include <QTimer>
#include <QDesktopWidget>
#include <QTimer>
#include "ROILabel.h"
#include "DataStructure/MSystemInfo.h"

namespace Ui {
class ConfigurationWidget;
}

class ConfigurationWidget : public QWidget{
    Q_OBJECT
public:
    explicit ConfigurationWidget(QWidget *parent = nullptr);
    ~ConfigurationWidget();
    void startTimer();
    void stopTimer();
    bool isTimerActive() const;
private:
    Ui::ConfigurationWidget *ui;
    QImage q_image_a_, q_image_b_;
    RecipeParam recipe_param_;
    QTimer* timer;
    bool timerActive;
};

#endif // CONFIGURATIONWIDGET_H
