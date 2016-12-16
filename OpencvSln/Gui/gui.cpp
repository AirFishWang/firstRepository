#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

#define WINDOW_NAME "�����Ի��ʾ����"

const int g_nMaxAlphaValue = 100;    //Alphaֵ�����ֵ
int       g_nAlphaValueSlider;       //��������Ӧ�ı���
double    g_dAlphaValue;
double    g_dBetaValue;

//�����洢ͼ��ı���
Mat g_srcImage1;
Mat g_srcImage2;
Mat g_dstImage;


void on_Trackbar(int position, void*)       //��Ӧ�������Ļص�����
{
    g_dAlphaValue = (double)g_nAlphaValueSlider / g_nMaxAlphaValue;
    g_dBetaValue = 1.0 - g_dAlphaValue;
    
    //����alpha��betaֵ�������Ի��
    addWeighted(g_srcImage1, g_dAlphaValue, g_srcImage2, g_dBetaValue, 0.0, g_dstImage);
    imshow(WINDOW_NAME, g_dstImage);
}

int main(int argc, char** agrv)
{
    g_srcImage1 = imread("1.jpg");
    g_srcImage2 = imread("2.jpg");

    g_nAlphaValueSlider = 70;            //���û������ĳ�ֵΪ70

    namedWindow(WINDOW_NAME, 1);         //��������

    char TrackbarName[50];
    sprintf(TrackbarName, "͸��ֵ %d", g_nMaxAlphaValue);

    createTrackbar(TrackbarName, WINDOW_NAME, &g_nAlphaValueSlider, g_nMaxAlphaValue, on_Trackbar);

    on_Trackbar(g_nAlphaValueSlider, 0);
    waitKey();
    return 0;
}