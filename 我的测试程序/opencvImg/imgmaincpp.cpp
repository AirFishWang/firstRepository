#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdint.h>
#include <direct.h>
#include <io.h>

using namespace cv;
using namespace std;

void getFiles(string path, vector<string>& files)
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
					getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);   //�����ļ�ϵͳ���λ��˳Ѷ������һ��
		_findclose(hFile);
	}
}

void main()
{
/*	Mat img = imread("1.bmp");
	resize(img, img, Size(24, 24));   //��һ��Ϊ24��24��ͼ��
	cvtColor(img, img, CV_RGB2GRAY);  //ת��Ϊ�Ҷ�ͼ
	equalizeHist(img, img);           //ֱ��ͼ���⴦����ǿ�Աȶ�
	for (int i = 0; i < 10; i++){
		for (int j = 0; j < 10; j++)
			printf("%d\t", (uint8_t)(img.at<uchar>(i, j)));
			//cout << (uint8_t)(img.at<uchar>(i, j)) << '\t';
		cout << endl;
	}
	
	uint8_t x = 255;
	cout << x << endl;
	printf("%d\n", x);

	imwrite("24.jpg", img);
	waitKey();
*/
	string foldername = "ZEX";
	vector<string> files;
	getFiles(foldername, files);		////��ȡ��·���µ������ļ�  
	int count = files.size();           //�ܵ�ͼƬ����

	string dstFoldername = foldername + "bmp";
	_mkdir(dstFoldername.c_str());   //�½�Ŀ���ļ���,���ڴ洢Ԥ�����bmpͼ��

	for (int i = 0; i < count; i++)
	{
		string path = files[i].c_str();
		Mat srcImage = imread(path), dstImage;
		resize(srcImage, dstImage, Size(32, 32));   //��һ��Ϊ80��80��ͼ��
		cvtColor(dstImage, dstImage, CV_RGB2GRAY);  //ת��Ϊ�Ҷ�ͼ
		equalizeHist(dstImage, dstImage);      //ֱ��ͼ���⴦����ǿ�Աȶ�

		stringstream ss;   //���ڽ�intתstring
		ss << i + 1;
		path = dstFoldername + "\\" + ss.str() + ".bmp";   //д��·��
	
		imwrite(path, dstImage);                           //��������ͼƬд�뵽dstFoldername�ļ�����

	}
}