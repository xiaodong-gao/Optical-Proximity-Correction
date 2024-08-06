#include "MImageProcess.h"

MImageProcess::MImageProcess(){

}

MImageProcess::~MImageProcess(){

}

cv::Mat MImageProcess::QImage2Mat(const QImage &image)
{
    cv::Mat mat;
    switch(image.format())
    {
    case QImage::Format_Indexed8: // 灰度图，每个像素点1个字节（8位）
    case QImage::Format_Grayscale8: // 灰度图，每个像素点1个字节（8位）
        // Mat构造：行数，列数，存储结构，数据，step每行多少字节
        mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
        break;
    case QImage::Format_ARGB32: // uint32存储0xAARRGGBB，pc一般小端存储低位在前，所以字节顺序就成了BGRA
    case QImage::Format_RGB32: // Alpha为FF
    case QImage::Format_ARGB32_Premultiplied:
        mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
        break;
    case QImage::Format_RGB888: // RR,GG,BB字节顺序存储
        mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
        // opencv需要转为BGR的字节顺序
        cv::cvtColor(mat, mat, cv::COLOR_RGB2BGR);
        break;
    case QImage::Format_RGBA64: // uint64存储，顺序和Format_ARGB32相反，RGBA
        mat = cv::Mat(image.height(), image.width(), CV_16UC4, (void*)image.constBits(), image.bytesPerLine());
        // opencv需要转为BGRA的字节顺序
        cv::cvtColor(mat, mat, cv::COLOR_RGBA2BGRA);
        break;
    }
    return mat;
}

QImage MImageProcess::Mat2QImage(const cv::Mat &mat)
{
    QImage image;
    switch(mat.type())
    {
    case CV_8UC1:
        // QImage构造：数据，宽度，高度，每行多少字节，存储结构
        image = QImage((const unsigned char*)mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Grayscale8);
        break;
    case CV_8UC3:
        image = QImage((const unsigned char*)mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        image = image.rgbSwapped(); // BRG转为RGB
        // Qt5.14增加了Format_BGR888
        // image = QImage((const unsigned char*)mat.data, mat.cols, mat.rows, mat.cols * 3, QImage::Format_BGR888);
        break;
    case CV_8UC4:
        image = QImage((const unsigned char*)mat.data, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
        break;
    case CV_16UC4:
        image = QImage((const unsigned char*)mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGBA64);
        image = image.rgbSwapped(); // BRG转为RGB
        break;
    }
    return image;
}

double MImageProcess::getLaplacian(const cv::Mat& img,cv::Mat & laplacian) {
    cv::Mat gray;
    if (img.channels() == 3) {
        cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
    }
    else {
        gray = img;
    }

    //cv::Mat laplacian;
    cv::Laplacian(gray, laplacian, CV_64F);
    laplacian.convertTo(laplacian,CV_8UC1);
    //cv::applyColorMap(laplacian, laplacian, cv::COLORMAP_JET);
    cv::Scalar mean, stddev;
    cv::meanStdDev(laplacian, mean, stddev);

    double laplacianVariance = stddev[0] * stddev[0];
    return laplacianVariance;
}

cv::Mat MImageProcess::getMSSIM( const cv::Mat& i1, const cv::Mat& i2,double &ssim_value)
{
    const double C1 = 6.5025, C2 = 58.5225;
    /***************************** INITS **********************************/
    int d     = CV_32F;

    cv::Mat I1, I2;
    i1.convertTo(I1, d);           // cannot calculate on one byte large values
    i2.convertTo(I2, d);

    cv::Mat I2_2   = I2.mul(I2);        // I2^2
    cv::Mat I1_2   = I1.mul(I1);        // I1^2
    cv::Mat I1_I2  = I1.mul(I2);        // I1 * I2

    /*************************** END INITS **********************************/

    cv::Mat mu1, mu2;   // PRELIMINARY COMPUTING
    GaussianBlur(I1, mu1, cv::Size(11, 11), 1.5);
    GaussianBlur(I2, mu2, cv::Size(11, 11), 1.5);

    cv::Mat mu1_2   =   mu1.mul(mu1);
    cv::Mat mu2_2   =   mu2.mul(mu2);
    cv::Mat mu1_mu2 =   mu1.mul(mu2);

    cv::Mat sigma1_2, sigma2_2, sigma12;

    GaussianBlur(I1_2, sigma1_2, cv::Size(11, 11), 1.5);
    sigma1_2 -= mu1_2;

    GaussianBlur(I2_2, sigma2_2, cv::Size(11, 11), 1.5);
    sigma2_2 -= mu2_2;

    GaussianBlur(I1_I2, sigma12, cv::Size(11, 11), 1.5);
    sigma12 -= mu1_mu2;

    cv::Mat t1, t2, t3;

    t1 = 2 * mu1_mu2 + C1;
    t2 = 2 * sigma12 + C2;
    t3 = t1.mul(t2);              // t3 = ((2*mu1_mu2 + C1).*(2*sigma12 + C2))

    t1 = mu1_2 + mu2_2 + C1;
    t2 = sigma1_2 + sigma2_2 + C2;
    t1 = t1.mul(t2);               // t1 =((mu1_2 + mu2_2 + C1).*(sigma1_2 + sigma2_2 + C2))

    cv::Mat ssim_map;
    divide(t3, t1, ssim_map);      // ssim_map =  t3./t1;

    cv::Scalar mssim = mean( ssim_map ); // mssim = average of ssim map
    if (ssim_map.channels() == 1) {
        ssim_value = mssim[0];
    }
    else if (ssim_map.channels() == 3) { 
        ssim_value = (mssim[0] + mssim[1] + mssim[2]) / 3;
    }   
    ssim_map.convertTo(ssim_map, CV_8UC1,255.0);
    return ssim_map;
}

cv::Point2d MImageProcess::getPhaseCorr(const cv::Mat& img1, const cv::Mat& img2){
    cv::Mat gray_img1, gray_img2,hann, img164f,img264f;

    if (img1.channels() == 3) {
        cv::cvtColor(img1, gray_img1, cv::COLOR_BGR2GRAY);
    }
    else {
        gray_img1 = img1;
    }
    if (img2.channels() == 3) {
        cv::cvtColor(img2, gray_img2, cv::COLOR_BGR2GRAY);
    }
    else {
        gray_img2 = img2;
    }

    gray_img1.convertTo(img164f, CV_64F);
    gray_img2.convertTo(img264f, CV_64F);

    cv::Point2d shift = phaseCorrelate(img164f, img264f, hann);
    return shift;
}

cv::Mat MImageProcess::extractRois(cv::Mat image, std::vector<cv::Rect> rois){
    std::vector<cv::Mat> roiImages;
    for (size_t i = 0; i < rois.size(); ++i) {
        // 提取ROI区域
        cv::Mat roiImage = image(rois[i]);
        roiImages.push_back(roiImage);
    }
    // 创建显示窗口
    cv::Mat displayImage(image.rows, image.cols, image.type(), cv::Scalar(0, 0, 0));

    // 将每个ROI区域放置在显示窗口中

    for (size_t i = 0; i < roiImages.size(); ++i) {
        int x_offset = rois[i].x;
        int y_offset = rois[i].y;
        cv::Mat roiImage = roiImages[i];
        roiImage.copyTo(displayImage(cv::Rect(x_offset, y_offset, roiImage.cols, roiImage.rows)));

        // 更新偏移量
        x_offset += roiImage.cols;
        if (x_offset >= displayImage.cols) {
            x_offset = 0;
            y_offset += roiImage.rows;
        }
    }
    return displayImage;
}




