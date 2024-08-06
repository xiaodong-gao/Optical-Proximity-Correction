#ifndef ZOOMABLELABEL_H
#define ZOOMABLELABEL_H

#include <QObject>
#include <QLabel>
#include <QPropertyAnimation>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QWidget>
#include <QPixmap>
#include <QList>

class ZoomableLabel : public QLabel {
    Q_OBJECT
public:
    explicit ZoomableLabel(QWidget* parent = nullptr);
    ~ZoomableLabel();
    void setImage(const QPixmap& image);
    void setControls(QList<QWidget*> controls);
protected:
    void mouseDoubleClickEvent(QMouseEvent* event) override ;
private:
    void animateGeometry(const QRect& targetGeometry);
    void hideControls();
    void showControls();
    QRect originalGeometry;
    bool zoomed;
    QPixmap image_;
    QList<QWidget*> controls;
};

#endif // ZOOMABLELABEL_H
