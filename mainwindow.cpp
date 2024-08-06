#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTranslator>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qRegisterMetaType<WorkDoneResult>("WorkDoneResult");
    qRegisterMetaType<RecipeParam>("RecipeParam");
}

MainWindow::~MainWindow()
{
    MSystemInfo::Instance()->close();
    delete ui;
}

void MainWindow::on_btnOPC_clicked()
{
    OpticalToolDlg dlg(this);
    dlg.exec();
}
