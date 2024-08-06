#include "ZoomableLabel.h"

ZoomableLabel::ZoomableLabel(QWidget* parent )
       : QLabel(parent),
         zoomed(false)
{
    setScaledContents(true);
    setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    setAlignment(Qt::AlignCenter);
}

ZoomableLabel::~ZoomableLabel(){

}

void ZoomableLabel::setControls(QList<QWidget*> controls) {
    this->controls = controls;
}

void ZoomableLabel::hideControls() {
    for (auto control : controls) {
        control->hide();
    }
}

void ZoomableLabel::showControls() {
    for (auto control : controls) {
        control->show();
    }
}

void ZoomableLabel::setImage(const QPixmap& image) {
    this->image_ = image;
    setPixmap(image);
}


void ZoomableLabel::mouseDoubleClickEvent(QMouseEvent* event) {
   if (event->button() == Qt::LeftButton) {
       if (zoomed) {
           // 还原窗口大小
           animateGeometry(originalGeometry);
           showControls();
       } else {
           // 放大显示
           originalGeometry = geometry();
           QRect newGeometry = parentWidget()->geometry();
           animateGeometry(newGeometry);
           hideControls();
       }
       zoomed = !zoomed;
   }
   QLabel::mouseDoubleClickEvent(event);
}

void ZoomableLabel::animateGeometry(const QRect& targetGeometry){
    QPropertyAnimation* animation = new QPropertyAnimation(this, "geometry");
        animation->setDuration(300);
        animation->setStartValue(geometry());
        animation->setEndValue(targetGeometry);
        animation->start(QAbstractAnimation::DeleteWhenStopped);
}
