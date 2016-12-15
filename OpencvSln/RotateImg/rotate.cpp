#include "cv.h"                             //  OpenCV �ļ�ͷ
#include "highgui.h"
#include "cvaux.h"
#include "cxcore.h"
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <string>

using namespace cv;
using namespace std;
Mat rotateImage(Mat img, int degree)
{
    degree = -degree;
    double angle = degree  * CV_PI / 180.; // ����  
    double a = sin(angle), b = cos(angle);
    int width = img.cols;
    int height = img.rows;
    int width_rotate = int(height * fabs(a) + width * fabs(b));
    int height_rotate = int(width * fabs(a) + height * fabs(b));

    //��ת����map
    // [ m0  m1  m2 ] ===>  [ A11  A12   b1 ]
    // [ m3  m4  m5 ] ===>  [ A21  A22   b2 ]

    float map[6];
    Mat map_matrix = Mat(2, 3, CV_32F, map);
    // ��ת����
    CvPoint2D32f center = cvPoint2D32f(width / 2, height / 2);
    CvMat map_matrix2 = map_matrix;
    cv2DRotationMatrix(center, degree, 1.0, &map_matrix2);
    map[2] += (width_rotate - width) / 2;
    map[5] += (height_rotate - height) / 2;
    Mat img_rotate;
    //��ͼ��������任
    //CV_WARP_FILL_OUTLIERS - ����������ͼ������ء�
    //�������������������ͼ��ı߽��⣬��ô���ǵ�ֵ�趨Ϊ fillval.
    //CV_WARP_INVERSE_MAP - ָ�� map_matrix �����ͼ������ͼ��ķ��任��
    warpAffine(img, img_rotate, map_matrix, Size(width_rotate, height_rotate), 1, 0, Scalar(255,255,255));
    return img_rotate;
}

int main(int argc, char *argv[])
{
    int degree;
    Mat srcImg;
    srcImg = imread("ad.jpg");
    //namedWindow("ԭͼ��", 1);
    //imshow("ԭͼ��", m_SrcImg);
    //cout << "��������ת�Ķ�����";
    //cin >> degree;
    //Mat m_ResImg = rotateImage(m_SrcImg, degree);
    //namedWindow("��ת��ͼ��", 1);
    //imshow("��ת��ͼ��", m_ResImg);
    //waitKey(0);

    for (int degree = 10; degree < 360; degree += 10)
    {
        Mat resultImg = rotateImage(srcImg, degree);
        stringstream ss;   //���ڽ�intתstring
        ss << degree;
        string outPath = ss.str() + ".jpg";
        imwrite(outPath, resultImg);
    }
}