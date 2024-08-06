#include "MExportPDF.h"

MExportPDF::MExportPDF(QObject *parent)
    : QObject(parent){

    // Initialize libharu PDF document
    pdf = HPDF_New(NULL, NULL);
    if (!pdf) {
        //std::cerr << "Failed to create PDF object" << std::endl;
        //return;
    }
    // Add a new page to the PDF document
    page1 = HPDF_AddPage(pdf);
    // Set the page size to A4
    HPDF_Page_SetSize(page1, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);

    // Add a new page to the PDF document
    page2 = HPDF_AddPage(pdf);
    // Set the page size to A4
    HPDF_Page_SetSize(page2, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);
}

MExportPDF::~MExportPDF(){
    // Clean up
    HPDF_Free(pdf);
}


void MExportPDF::drawText(std::string& text, float x, float y, float fontSize, int page){

    // 设置字体
    //const char* font_path = "path/to/your/simsun.ttc"; // 替换为你的字体文件路径
    //HPDF_UseUTFEncodings(pdf);
    //HPDF_SetCurrentEncoder(pdf, "UTF-8");
    //HPDF_Font font = HPDF_LoadTTFontFromFile(pdf, font_path, HPDF_TRUE);

    if (page == 1) {
        HPDF_Page_BeginText(page1);
        HPDF_Font font = HPDF_GetFont(pdf, "Helvetica", NULL);
        HPDF_Page_SetFontAndSize(page1, font, fontSize);
        HPDF_Page_TextOut(page1, x, y, text.c_str());
        HPDF_Page_EndText(page1);
    }
    else if (page == 2) {
        HPDF_Page_BeginText(page2);
        HPDF_Font font = HPDF_GetFont(pdf, "Helvetica", NULL);
        HPDF_Page_SetFontAndSize(page2, font, fontSize);
        HPDF_Page_TextOut(page2, x, y, text.c_str());
        HPDF_Page_EndText(page2);
    }
}

void MExportPDF::drawImage(const cv::Mat& image, float x, float y, float width, float height, int page, bool isHeatmap) {
    cv::Mat rgb_image;
    if (isHeatmap == false) {
        if (rgb_image.channels() == 1) {
            cv::cvtColor(image, rgb_image, cv::COLOR_GRAY2RGB);
        }
        else if (rgb_image.channels() == 3) {
            cv::cvtColor(image, rgb_image, cv::COLOR_BGR2RGB);
        }
    }
    else {
        rgb_image = createHeatmap(image);
        if (rgb_image.channels() == 1) {
            cv::cvtColor(image, rgb_image, cv::COLOR_GRAY2RGB);
        }
        else if (rgb_image.channels() == 3) {
            cv::cvtColor(image, rgb_image, cv::COLOR_BGR2RGB);
        }
    }


    int img_width = rgb_image.cols;
    int img_height = rgb_image.rows;

    HPDF_Image pdf_image = HPDF_LoadRawImageFromMem(pdf, rgb_image.data, img_width, img_height, HPDF_CS_DEVICE_RGB, 8);
    if (!pdf_image) {
        throw std::runtime_error("Failed to create PDF image object");
    }
    if (page == 1) 
        HPDF_Page_DrawImage(page1, pdf_image, x, y, width, height);
    else if(page == 2)
        HPDF_Page_DrawImage(page2, pdf_image, x, y, width, height);
}

void MExportPDF::savePDF(const std::string &str){
    // Save the PDF document to file
    HPDF_SaveToFile(pdf, str.c_str());
}

cv::Mat MExportPDF::createHeatmap(const cv::Mat& src) {
    cv::Mat heatmap;
    // Normalize the source image to the range [0, 255]
    cv::normalize(src, heatmap, 0, 255, cv::NORM_MINMAX);
    // Convert to 8-bit
    heatmap.convertTo(heatmap, CV_8U);
    // Apply the colormap
    cv::applyColorMap(heatmap, heatmap, cv::COLORMAP_JET);
    return heatmap;
}
