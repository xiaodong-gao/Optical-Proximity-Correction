#include "ROILabel.h"

ROILabel::ROILabel(QWidget* parent): QLabel(parent), drawing(false) {
    // Create actions for the context menu
    saveAction = new QAction("Save", this);
    cancelAction = new QAction("Cancel", this);
    clearAction = new QAction("Clear", this);
    // Connect actions to their respective slots
    connect(saveAction, &QAction::triggered, this, &ROILabel::saveROI);
    connect(cancelAction, &QAction::triggered, this, &ROILabel::cancelROI);
    connect(clearAction, &QAction::triggered, this, &ROILabel::clearROIs);
    //setMouseTracking(true);
    setScaledContents(true);
}


ROILabel::~ROILabel(){

}

void ROILabel::setQImage(const QImage &image){
    image_ = image;
    QPixmap q_pixmap = QPixmap::fromImage(image);
    setPixmap(q_pixmap.scaled(this->size(),  Qt::KeepAspectRatio, Qt::SmoothTransformation));
    update();
}

QVector<QRectF> ROILabel::getROIRects() const {
    return roiRects;
}


void ROILabel::mousePressEvent(QMouseEvent* event){
    if (event->button() == Qt::LeftButton) {
        drawing = true;
        roiStart = event->pos();
        roiEnd = roiStart;
        update();
    }
}

void ROILabel::mouseMoveEvent(QMouseEvent* event) {
    if (drawing) {
        roiEnd = event->pos();
        update();
    }
}

void ROILabel::mouseReleaseEvent(QMouseEvent* event){
    if (event->button() == Qt::LeftButton) {
        drawing = false;
        update();
    } else if (event->button() == Qt::RightButton) {
        if (!roiRect.isNull()) {
            showContextMenu(event->pos());
        }
    }
}

void ROILabel::resizeEvent(QResizeEvent* event) {
    // Optionally handle resize events
    QLabel::resizeEvent(event);
    // You can add code here to handle the resizing if needed
    QPixmap originalPixmap = QPixmap::fromImage(image_);
    if (!originalPixmap.isNull()) {
       QPixmap scaledPixmap = originalPixmap.scaled(event->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
       setPixmap(scaledPixmap);
       update();
    }
}

void ROILabel::paintEvent(QPaintEvent* event){
    QLabel::paintEvent(event);

    QPainter painter(this);
    painter.setPen(Qt::red);
    if (drawing) {
       roiRect = QRectF(roiStart, roiEnd).normalized();
       painter.drawRect(roiRect);
    }
    painter.setPen(Qt::blue);
    for (const QRectF &rect : roiRects) {
       float xScale = static_cast<float>(this->width()) / image_.width() ;
       float yScale = static_cast<float>(this->height())/ image_.height()  ;
       painter.drawRect(QRectF(rect.x() * xScale,rect.y() * yScale,rect.width() * xScale,rect.height() * yScale));
    }
}


void ROILabel::saveROI() {
    if (!roiRect.isNull()) {
        float xScale =  image_.width() / static_cast<float>(this->width()) ;
        float yScale =  image_.height()/static_cast<float>(this->height()) ;
        roiRects.append(QRectF(roiRect.x() * xScale,roiRect.y() * yScale,roiRect.width() * xScale,roiRect.height() * yScale));
        roiRect = QRect();
        update();
    }
}

void ROILabel::cancelROI() {
    roiRect = QRect();
    update();
}

void ROILabel::clearROIs() {
    roiRects.clear();
    roiRect = QRect();
    update();
}

void ROILabel::showContextMenu(const QPoint& pos) {
    QMenu contextMenu(this);
    contextMenu.addAction(saveAction);
    contextMenu.addAction(cancelAction);
    contextMenu.addAction(clearAction);
    contextMenu.exec(mapToGlobal(pos));
}

