#ifndef MEXPORTPDF_H
#define MEXPORTPDF_H

#include <QObject>
#include <opencv2/opencv.hpp>
#include <hpdf.h>

constexpr float A4_WIDTH = 210 * 72 / 25.4; // A4 width in points (210mm)
constexpr float A4_HEIGHT = 297 * 72 / 25.4; // A4 height in points (297mm)

class MExportPDF : public QObject{
    Q_OBJECT
public:
    explicit MExportPDF(QObject *parent = nullptr);
    ~MExportPDF();
    void drawText(std::string& text, float x, float y, float fontSize, int page);
    void drawImage(const cv::Mat& rgb_image, float x, float y, float width, float height,int page, bool isHeatmap);
    void savePDF(const std::string &str);
    cv::Mat createHeatmap(const cv::Mat& src);
private:
    HPDF_Doc pdf;
    HPDF_Page page1, page2;
};

#endif // MEXPORTPDF_H
