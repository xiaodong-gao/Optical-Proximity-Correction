#include "ConfigurationWidget.h"
#include "ui_configurationwidget.h"

ConfigurationWidget::ConfigurationWidget(QWidget* parent) :
    timerActive(false),
    QWidget(parent),
    ui(new Ui::ConfigurationWidget)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    QObject::connect(timer, &QTimer::timeout, [&]() {
        MSystemInfo::Instance()->camera_factory_->vec_cameras_[recipe_param_.camera_id_]->get_mat(recipe_param_.src_mat2_);
        q_image_b_ = MSystemInfo::Instance()->image_process_->Mat2QImage(recipe_param_.src_mat2_);
        ui->labelImage->setQImage(q_image_b_);
        //ui->labelImage->show();
    });

    QObject::connect(ui->lineEditCameraExposure, &QLineEdit::textChanged, [&](const QString& text) {
        if (ui->comboBoxMode->currentText() == tr("A Image  -> B Camera")) {
            recipe_param_.camera_exposure = text.toDouble();
            MSystemInfo::Instance()->camera_factory_->vec_cameras_[recipe_param_.camera_id_]->set_camera_exposure(recipe_param_.camera_exposure);
        }
        else if (ui->comboBoxMode->currentText() == tr("A Image  -> B Image")) {

        }
    });

    QObject::connect(ui->lineEditCameraTrigger, &QLineEdit::textChanged, [&](const QString& text) {
        if (ui->comboBoxMode->currentText() == tr("A Image  -> B Camera") && MSystemInfo::Instance()->camera_factory_->vec_camera_serial_number_.size() > 0) {
            recipe_param_.camera_trigger = text.toStdString();
            MSystemInfo::Instance()->camera_factory_->vec_cameras_[recipe_param_.camera_id_]->set_camera_trigger(recipe_param_.camera_trigger);
        }
        else if (ui->comboBoxMode->currentText() == tr("A Image  -> B Image")) {

        }
    });

    connect(ui->comboBoxCameraSerialNumber, &QComboBox::currentTextChanged, this, [=](const QString& text) {
        auto it = std::find(MSystemInfo::Instance()->camera_factory_->vec_camera_serial_number_.begin(), MSystemInfo::Instance()->camera_factory_->vec_camera_serial_number_.end(), text);
        if (it != MSystemInfo::Instance()->camera_factory_->vec_camera_serial_number_.end()) {
            recipe_param_.camera_id_ = std::distance(MSystemInfo::Instance()->camera_factory_->vec_camera_serial_number_.begin(), it);
            float camera_exposure;
            MSystemInfo::Instance()->camera_factory_->vec_cameras_[recipe_param_.camera_id_]->get_camera_exposure(camera_exposure);
            ui->lineEditCameraExposure->setText(QString::number(camera_exposure));
            std::string camera_trigger;
            MSystemInfo::Instance()->camera_factory_->vec_cameras_[recipe_param_.camera_id_]->get_camera_trigger(camera_trigger);
            ui->lineEditCameraTrigger->setText(QString::fromStdString(camera_trigger));
        }
    });

    connect(ui->comboBoxMode, &QComboBox::currentTextChanged, this, [=](const QString &text) {
        ui->comboBoxCameraSerialNumber->clear();
        if(text == tr("A Image  -> B Camera")){
            ui->labelBPath->hide();
            ui->lineEditBPath->hide();
            ui->btnSelectBPath->hide();
            for (auto &camera_serial_number : MSystemInfo::Instance()->camera_factory_->vec_camera_serial_number_) {
                 ui->comboBoxCameraSerialNumber->addItem(camera_serial_number)  ;
            }    
        }
        else if(text == tr("A Image  -> B Image")){
            ui->labelBPath->show();
            ui->lineEditBPath->show();
            ui->btnSelectBPath->show();
            ui->comboBoxCameraSerialNumber->addItem(tr("B folder"));
            ui->lineEditCameraTrigger->clear();
            ui->lineEditCameraExposure->clear();

        }
    });
    ui->comboBoxMode->addItem(tr("A Image  -> B Camera"));
    ui->comboBoxMode->addItem(tr("A Image  -> B Image"));
    ui->comboBoxMode->setCurrentText(tr("A Image  -> B Camera"));

    
    QObject::connect(ui->btnSelectAPath, &QPushButton::clicked, [&]() {
        QString fileName = QFileDialog::getOpenFileName(nullptr, "Open Image File", "", "Images (*.png *.jpg *.jpeg *.bmp)");
        if (!fileName.isEmpty()) {
            ui->lineEditAPath->setText(fileName);
            q_image_a_ = QImage(fileName);
        }
        else{
            ui->lineEditAPath->setText("");
            q_image_a_ = QImage();
        }
        recipe_param_.src_mat1_ = MSystemInfo::Instance()->image_process_->QImage2Mat(q_image_a_);
        ui->labelImage->setQImage(q_image_a_);
        ui->labelImage->show();
    });

    QObject::connect(ui->btnSelectBPath, &QPushButton::clicked, [&]() {
        QString folderPath = QFileDialog::getExistingDirectory(nullptr, "select images folder", "");
        if (!folderPath.isEmpty()) {
            ui->lineEditBPath->setText(folderPath);
        }
        else{
            ui->lineEditBPath->setText("");
        }
    });

    QObject::connect(ui->btnGrab, &QPushButton::clicked,[&]() {
        ui->btnGrab->setEnabled(false);
        ui->btnSnap->setEnabled(false);
        ui->btnStop->setEnabled(true);

        if(ui->comboBoxMode->currentText() == tr("A Image  -> B Camera")){
            startTimer();
        }
        else if(ui->comboBoxMode->currentText() == tr("A Image  -> B Image")){

        }
    });

    QObject::connect(ui->btnStop, &QPushButton::clicked,[&]() {
        ui->btnGrab->setEnabled(true);
        ui->btnSnap->setEnabled(true);
        ui->btnStop->setEnabled(false);

        if(ui->comboBoxMode->currentText() == tr("A Image  -> B Camera")){
            stopTimer();
        }
        else if(ui->comboBoxMode->currentText() == tr("A Image  -> B Image")){

        }
    });

    QObject::connect(ui->btnSnap, &QPushButton::clicked,[&]() {
        if(ui->comboBoxMode->currentText() == tr("A Image  -> B Camera")){
            MSystemInfo::Instance()->camera_factory_->vec_cameras_[recipe_param_.camera_id_]->grab();
            cv::Mat image;
            MSystemInfo::Instance()->camera_factory_->vec_cameras_[recipe_param_.camera_id_]->get_mat(image);
            QImage q_image = MSystemInfo::Instance()->image_process_->Mat2QImage(image);
            ui->labelImage->setQImage(q_image);
            ui->labelImage->show();
            MSystemInfo::Instance()->camera_factory_->vec_cameras_[recipe_param_.camera_id_]->stop();

        }
        else if(ui->comboBoxMode->currentText() == tr("A Image  -> B Image")){

        }
    }); 

    QObject::connect(ui->btnSettings, &QPushButton::clicked,[&]() {
        std::vector<cv::Rect> rois;
        QVector<QRectF> selection_rois_ = ui->labelImage->getROIRects();
        for (int i = 0; i < selection_rois_.size(); i++) {
            recipe_param_.selection_rois_.push_back (cv::Rect(selection_rois_[i].x(), selection_rois_[i].y(), selection_rois_[i].width(), selection_rois_[i].height()));
        }
        MSystemInfo::Instance()->set_recipe_param(recipe_param_);
    });


    QObject::connect(ui->lineEditFocusLengthDiff, &QLineEdit::textChanged, [&](const QString& text) {
        if (ui->comboBoxMode->currentText() == tr("A Image  -> B Camera") ) {
            recipe_param_.focus_length_value = text.toFloat();
        }
        else if (ui->comboBoxMode->currentText() == tr("A Image  -> B Image")) {
        }
    });

    QObject::connect(ui->lineEditFineOffsetXTolerance, &QLineEdit::textChanged, [&](const QString& text) {
        if (ui->comboBoxMode->currentText() == tr("A Image  -> B Camera")) {
            recipe_param_.fine_offset_x = text.toFloat();
        }
        else if (ui->comboBoxMode->currentText() == tr("A Image  -> B Image")) {
        }
    });

    QObject::connect(ui->lineEditFineOffsetYTolerance, &QLineEdit::textChanged, [&](const QString& text) {
        if (ui->comboBoxMode->currentText() == tr("A Image  -> B Camera")) {
            recipe_param_.fine_offset_y = text.toFloat();
        }
        else if (ui->comboBoxMode->currentText() == tr("A Image  -> B Image")) {
        }
    });

    QObject::connect(ui->lineEditSSIMMinTolerance, &QLineEdit::textChanged, [&](const QString& text) {
        if (ui->comboBoxMode->currentText() == tr("A Image  -> B Camera")) {
            recipe_param_.ssim_min_value = text.toFloat();
        }
        else if (ui->comboBoxMode->currentText() == tr("A Image  -> B Image")) {
        }
    });

    QObject::connect(ui->lineEditSSIMMaxTolerance, &QLineEdit::textChanged, [&](const QString& text) {
        if (ui->comboBoxMode->currentText() == tr("A Image  -> B Camera")) {
            recipe_param_.ssim_max_value = text.toFloat();
        }
        else if (ui->comboBoxMode->currentText() == tr("A Image  -> B Image")) {
        }
    });
    ui->lineEditFocusLengthDiff->setText(tr("50"));
    ui->lineEditFineOffsetXTolerance->setText(tr("10"));
    ui->lineEditFineOffsetYTolerance->setText(tr("10"));
    ui->lineEditSSIMMinTolerance->setText(tr("0.5"));
    ui->lineEditSSIMMaxTolerance->setText(tr("0.8"));
}

ConfigurationWidget::~ConfigurationWidget()
{
    delete ui;
}

void ConfigurationWidget::startTimer() {
    MSystemInfo::Instance()->camera_factory_->vec_cameras_[recipe_param_.camera_id_]->grab();
    timer->start(200);
    timerActive = true;
}

void ConfigurationWidget::stopTimer() {
    timerActive = false;
    timer->stop();
    MSystemInfo::Instance()->camera_factory_->vec_cameras_[recipe_param_.camera_id_]->stop();
}

bool ConfigurationWidget::isTimerActive() const {
    return timerActive;
}
