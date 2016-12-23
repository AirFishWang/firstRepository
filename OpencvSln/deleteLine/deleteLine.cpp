#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
    bool deleteH = true;                     //�Ƿ�ȥ��ˮƽ��
    bool deleteV = true;                     //�Ƿ�ȥ����ֱ��
    Mat srcImage = imread(argv[1], IMREAD_GRAYSCALE);
    if (srcImage.data == NULL)
    {
        cout << "Can't find " << argv[1] << endl;
        return 0;
    }
    imshow("src", srcImage);
    int erosion_size = 35;
    Mat elementH = getStructuringElement(MORPH_RECT, Size(2 * erosion_size + 1, 1));      //ˮƽ��ģ
    Mat elementV = getStructuringElement(MORPH_RECT, Size(1, 2 * erosion_size + 1));      //��ֱ��ģ
    Mat lineImage;                           //��ͼ
    if (deleteH == true && deleteV == false)
    {
        Mat dilateDstH;
        dilate(srcImage, dilateDstH, elementH);     //���ֵ�������ɫ����
        //imwrite("dilateDstH.png", dilateDstH);
        //imshow("dilateDstH", dilateDstH);
        erode(dilateDstH, lineImage, elementH);     //��Сֵ�������ɫ����(��ԭ����)
        //imwrite("lineImageH.png", lineImage);
        imshow("lineImageH", lineImage);            //ˮƽ��ͼ
    }
    else if (deleteH == false && deleteV == true)
    {
        Mat dilateDstV;
        dilate(srcImage, dilateDstV, elementV);     //���ֵ�������ɫ����
        //imwrite("dilateDstV.png", dilateDstH);
        //imshow("dilateDstV", dilateDstH);
        erode(dilateDstV, lineImage, elementV);     //��Сֵ�������ɫ����(��ԭ����)
        //imwrite("lineImageV.png", lineImage);
        imshow("lineImageV", lineImage);            //��ֱ��ͼ
    }
    else if (deleteH == true && deleteV == true)
    {
        Mat dilateDstH, erodeDstH;
        dilate(srcImage, dilateDstH, elementH);
        erode(dilateDstH, erodeDstH, elementH);     //erodeDstHˮƽ��ͼ

        Mat dilateDstV, erodeDstV;
        dilate(srcImage, dilateDstV, elementV);
        erode(dilateDstV, erodeDstV, elementV);     //erodeDstV��ֱ��ͼ

        bitwise_and(erodeDstH, erodeDstV, lineImage);
        //imwrite("lineImageHV.png", lineImage);
        imshow("lineImageHV", lineImage);
    }
    else
    {
        cout << "The deleteH and deleteV both are false\n";
        return 0;
    }


    Mat deleteLine;                                 
    bitwise_xor(srcImage, lineImage, deleteLine);     //���
    bitwise_not(deleteLine, deleteLine);              //ȡ��

    imwrite("deleteLine.png", deleteLine);
    imshow("deleteLine", deleteLine);
    waitKey(0);
    return 0;
}