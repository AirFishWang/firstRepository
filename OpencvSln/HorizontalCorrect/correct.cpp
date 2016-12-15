/**************************************************************************
����任�������ڵ����⣺
(1): ֱ�߼�����ֵ�޷�ȷ������ͬ��ͼƬ����ͬ����ֵ���в�ͬ��Ч��(���������������)
     i:  ����ⲻ��ֱ�ߵ�ʱ�򣬿�����С��ֵ
     ii: ����⵽��ֱ�߹����ʱ�򣬿���������ֵ      
     ���뿼�������������Ϊ���������ֱ�߹���Ļ�����Ӱ�����ļ���ٶȺ���ȷ��
     ������2��100
(2): ���ڸ��������⣬�������߽϶��ʱ�򣬽���ʹƽ���Ƕȵļ���������
     (��������������������Ȼ������������ȶ����򣬿��Խ���������ڵĸ��Ӷ���O(n*n))
     ����Ҫ�Ľ�ΪO(n)��ʱ�临�Ӷ�,����ֱ�߹����ʱ���Ӱ������ٶ�
(3): ��ת��ͼƬ��ת��������֮�⣬����ͼƬ���и�(�ѽ��)
**************************************************************************/

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <algorithm>
#include <iostream>
#include <chrono>
#include <direct.h> 
#include <io.h>

using namespace cv;
using namespace std;

void GetFiles(string path, vector<string>& files)
{
    //�ļ����  
    long   hFile = 0;
    //�ļ���Ϣ  
    struct _finddata_t fileinfo;
    string p;
    if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
    {
        do
        {
            //�����Ŀ¼,����֮  
            //�������,�����б�  
            if ((fileinfo.attrib &  _A_SUBDIR))
            {
                if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
                    GetFiles(p.assign(path).append("\\").append(fileinfo.name), files);
            }
            else
            {
                files.push_back(p.assign(path).append("\\").append(fileinfo.name));
                //cout << p.assign(path).append("\\").append(fileinfo.name) << endl;
            }
        } while (_findnext(hFile, &fileinfo) == 0);   //�����ļ�ϵͳ���λ��˳Ѷ������һ��
        _findclose(hFile);
    }
}

Mat RotateImage(Mat img, int degree)              //˳ʱ����������ת
{
    degree = -degree;
    double angle = degree  * CV_PI / 180.;        //����  
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
    warpAffine(img, img_rotate, map_matrix, Size(width_rotate, height_rotate), 1, 0, Scalar(255, 255, 255));
    return img_rotate;
}

bool SortByAngle(Vec2f line1, Vec2f line2)      //����
{
    return line1[1] < line2[1];
}

//�Էǿ���������������ȶ�����  ����ĸ��Ӷ���O(n*n)
int GetMaxStableRegion(const vector<Vec2f> lines, int &begin, int &end, double threshold)
{
    begin = 0;
    end = 0;
    for (int i = 0; i < lines.size(); i++)
        for (int j = i; j < lines.size(); j++)
        {
            if (lines[j][1] - lines[i][1] < threshold)
            {
                if (j - i > end - begin)
                {
                    begin = i;
                    end = j;
                }

            }
            else
                break;                       //��Ϊ���������Ժ����һ��Ҳ���Ǵ�����ֵ
        }
    return 0;
}

int RotateCorrect(string filename)
{
    Mat srcImage = imread(filename);
    //imshow("ԭʼͼ", srcImage);
    //waitKey();
    Mat midImage, dstImage;
    Canny(srcImage, midImage, 50, 200, 3);
    Mat element = getStructuringElement(MORPH_RECT, Size(11, 11));
    dilate(midImage, midImage, element);       //��ɫ����
    Canny(midImage, midImage, 50, 200, 3);     //�ٴ���ȡ��Ե
    cvtColor(midImage, dstImage, COLOR_GRAY2BGR);   //COLOR_GRAY2BGR ��Ϊ����dstImage��ʹ�ò�ɫ������������ֱ��
    vector<Vec2f> lines;
    int lineThreshold = 100;
    while (1)     //��Ҫ���ݼ�⵽��ֱ��������������ֵ���������м��
    {
        HoughLines(midImage, lines, 1, CV_PI / 180, lineThreshold);
        if (lines.size() == 0)
        {
            if (lineThreshold < 20)           //С��20������¶�û�м�⵽ֱ�ߣ�����Ϊû��ֱ��
                break;
            else
                lineThreshold -= 20;          //���Լ�С��ֵ�����ٴμ��
        }
        else if (lines.size() > 100)
            lineThreshold += 20;              //����������ֵ�����ٴμ��
        else
            break;                            //��⵽��ֱ��������0��100֮�䣬��Ϊ��һ���������������������
        //��ֱ�ߵ�������0��100֮���ʱ��Ҳ�������sort��getMaxStableRegion��ʱ��
    };

    if (lines.size() == 0)
    {
        cout << "no line detected" << endl;
        return 0;
    }
    sort(lines.begin(), lines.end(), SortByAngle);      //����
    int begin;
    int end;
    GetMaxStableRegion(lines, begin, end, 5 * CV_PI / 180);
    double averageAngle;
    double angleSum = 0.0;                     //���Ǻ�
    for (size_t i = begin; i <= end; i++)
    {
        float rho = lines[i][0];               //��ֱ����   ���������Ǹ�ֵ
        float theta = lines[i][1];             //��x��������ĽǶȣ�0��ʾ��ֱ�ߣ�pai/2 ��ʾˮƽ��,  ���ǵķ�Χ��[0, pai]
        angleSum += theta;                     //�ۻ�����

        //����ֱ��
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        int detal = 1000;
        Point pt1(cvRound(x0 - detal * b), cvRound(y0 + detal * a));
        Point pt2(cvRound(x0 + detal * b), cvRound(y0 - detal * a));

        line(dstImage, pt1, pt2, Scalar(0, 0, 255), 1, 8);
    }
    averageAngle = angleSum / (end - begin + 1);    //ƽ������
    double angleThres = 5 * CV_PI / 180;            //ƫ�����ֵС����ֵ��������ת
    //angleThres = 0.087266462599716474;

    if (fabs(averageAngle - CV_PI / 2) > angleThres)  //ˮƽ�ߵļ�����CV_PI / 2  
    {
        Mat rotateImg = RotateImage(srcImage, 90 - averageAngle * 180 / CV_PI);
        int pos = filename.find_last_of("/\\");
        string outName = "./out/"+ filename.substr(pos + 1, filename.length() - pos - 1);    //�ļ���
        imwrite(outName, rotateImg);
    }

    return 0;
}

int main()
{
#if 0
    Mat srcImage = imread("1.jpg");
    //imshow("ԭʼͼ", srcImage);
    Mat midImage, dstImage;
    Canny(srcImage, midImage, 50, 200, 3);
    //imshow("��һ�α�Ե�����ͼ", midImage);
    //waitKey();
    //imwrite("canny.jpg", midImage);            //midImage�ǵ�ͨ����ֵͼ(ֻ��0��255)

    Mat element = getStructuringElement(MORPH_RECT, Size(11, 11)); 
    dilate(midImage, midImage, element);       //��ɫ����
    //imshow("����ͼ", midImage);
    //waitKey();
    //imwrite("dilate.jpg", midImage);
    Canny(midImage, midImage, 50, 200, 3);     //�ٴ���ȡ��Ե
    //imshow("�ڶ��α�Ե�����ͼ", midImage);
    //waitKey();
    cvtColor(midImage, dstImage, COLOR_GRAY2BGR);   //COLOR_GRAY2BGR ��Ϊ����dstImage��ʹ�ò�ɫ������������ֱ��

    vector<Vec2f> lines;

    int lineThreshold = 80;
    while (1)     //��Ҫ���ݼ�⵽��ֱ��������������ֵ���������м��
    {
        HoughLines(midImage, lines, 1, CV_PI / 180, lineThreshold);
        if (lines.size() == 0)
        {
            if (lineThreshold < 20)           //С��20������¶�û�м�⵽ֱ�ߣ�����Ϊû��ֱ��
                break;
            else
                lineThreshold -= 20;          //���Լ�С��ֵ�����ٴμ��
        }
        else if (lines.size() > 100)
            lineThreshold += 20;              //����������ֵ�����ٴμ��
        else
            break;                            //��⵽��ֱ��������0��100֮�䣬��Ϊ��һ���������������������
                                              //��ֱ�ߵ�������0��100֮���ʱ��Ҳ�������sort��getMaxStableRegion��ʱ��
    };

    if (lines.size() == 0)
    {
        cout << "no line detected" << endl;
        return 0;
    }

    auto nStart = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()).time_since_epoch().count();
    sort(lines.begin(), lines.end(), SortByAngle);      //����
    auto nEnd = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()).time_since_epoch().count();
    printf("sort Time: %d ms\n", nEnd - nStart);

    int begin;
    int end;

    nStart = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()).time_since_epoch().count();
    getMaxStableRegion(lines, begin, end, 5 * CV_PI / 180);
    nEnd = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()).time_since_epoch().count();
    printf("getMaxStableRegion Time: %d ms\n", nEnd - nStart);

    double averageAngle;
    double angleSum = 0.0;                     //���Ǻ�
    for (size_t i = begin; i <= end; i++)
    {
        float rho = lines[i][0];               //��ֱ����   ���������Ǹ�ֵ
        float theta = lines[i][1];             //��x��������ĽǶȣ�0��ʾ��ֱ�ߣ�pai/2 ��ʾˮƽ��,  ���ǵķ�Χ��[0, pai]
        angleSum += theta;                     //�ۻ�����

        //����ֱ��
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        int detal = 1000;
        Point pt1(cvRound(x0 - detal * b), cvRound(y0 + detal * a));
        Point pt2(cvRound(x0 + detal * b), cvRound(y0 - detal * a));

        line(dstImage, pt1, pt2, Scalar(0, 0, 255), 1, 8);
    }
    imshow("������Ч��ͼ", dstImage);
   // imwrite("hough.jpg", dstImage);
    waitKey();

    averageAngle = angleSum / (end - begin + 1);    //ƽ������
    double angleThres = 5 * CV_PI / 180;            //ƫ�����ֵС����ֵ��������ת
                                                    //angleThres = 0.087266462599716474;

    if (fabs(averageAngle - CV_PI / 2) > angleThres)  //ˮƽ�ߵļ�����CV_PI / 2  
    {
        Mat rotateImg = rotateImage(srcImage, 90 - averageAngle * 180  / CV_PI);
        imshow("��תͼ", rotateImg);
        waitKey();
        imwrite("rotateImage.jpg", rotateImg);
    }
#else              //������
    _mkdir("out");
    vector<string> files;
    GetFiles("ad", files);             //��ȡ��·���µ������ļ�  
    int count = files.size();           //�ܵ�ͼƬ����
    for (int i = 0; i < count; i++)
    {
        cout << files[i].c_str() << endl;
        RotateCorrect(files[i]);
    }



#endif

    return 0;
}