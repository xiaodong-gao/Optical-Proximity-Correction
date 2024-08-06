#ifndef ROILABEL_H
#define ROILABEL_H

#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include <QMenu>
#include <QAction>
#include <QVector>
#include <QDebug>
//#include <QMessageBox>

class ROILabel : public QLabel {
    Q_OBJECT
public:
    explicit ROILabel(QWidget* parent = nullptr);
    ~ROILabel();
    void setQImage(const QImage &image);
    QVector<QRectF> getROIRects() const;
protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override ;
    void resizeEvent(QResizeEvent* event) override;
private slots:
    void saveROI();
    void cancelROI();
    void clearROIs();
private:
    bool drawing;
    QPointF roiStart;
    QPointF roiEnd;
    QRectF roiRect;
    QVector<QRectF> roiRects; // Store the list of saved ROI rectangles
    QAction* saveAction;
    QAction* cancelAction;
    QAction* clearAction;
    void showContextMenu(const QPoint& pos);
    QImage image_;
};

#endif // ROILABEL_H
