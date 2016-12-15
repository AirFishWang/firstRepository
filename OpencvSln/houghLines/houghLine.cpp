#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

int main()
{
    Mat srcImage = imread("3.jpg");
    imshow("ԭʼͼ", srcImage);

    Mat midImage, dstImage;
    Canny(srcImage, midImage, 50, 200, 3);
    imshow("��Ե�����ͼ", midImage);
    imwrite("canny.jpg", midImage);

    //Mat tmp(midImage.rows, midImage.cols, CV_8UC1, Scalar(255));
    //midImage = tmp - midImage;

    cvtColor(midImage, dstImage, COLOR_GRAY2BGR);   //COLOR_GRAY2BGR ��Ϊ����dstImage��ʹ�ò�ɫ������������ֱ��
    imshow("COLOR_GRAY2BGR", dstImage);
    imwrite("COLOR_GRAY2BGR.jpg", dstImage);

    vector<Vec4i> lines;
    //midImage = imread("afterThres.png");         //���ֶ��Դ���HoughLinesP���������ǵ�ͨ���Ķ�ֵͼ����������λͼ
    
    //    @param image 8 - bit, single - channel binary source image.The image may be modified by the function.
    //    @param lines Output vector of lines.Each line is represented by a 4 - element vector
    //    \f$(x_1, y_1, x_2, y_2)\f$, where \f$(x_1, y_1)\f$ and \f$(x_2, y_2)\f$ are the ending points of each detected
    //    line segment.
    //    @param rho Distance resolution of the accumulator in pixels.
    //    @param theta Angle resolution of the accumulator in radians.
    //    @param threshold Accumulator threshold parameter.Only those lines are returned that get enough
    //    votes(\f$ > \texttt{ threshold }\f$).
    //    @param minLineLength Minimum line length.Line segments shorter than that are rejected.
    //    @param maxLineGap Maximum allowed gap between points on the same line to link them.

    //����任�Ƕ԰�ɫ���ص���м���(����ƽ���ϵ������ۻ�)  
    HoughLinesP(midImage, lines, 1, CV_PI / 180, 80, 50, 20);
    for (size_t i = 0; i < lines.size(); i++)
    {
        Vec4i l = lines[i];
        line(dstImage, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 1, LINE_AA);
    }
    imshow("������Ч��ͼ", dstImage);
    imwrite("hough.jpg", dstImage);
    waitKey();
    return 0;
}