#pragma once
#include<opencv2/imgproc/imgproc_c.h>

class CFire
{
public:
	void Init(IplImage**,int,int,int speed=10);//�ڵ�ͼƬ����ʼX����ʼY���ƶ��ٶ�
	IplImage **Image;
	float apparency;
	int pos_x;
	int pos_y;
	int to_x;
	int to_y;
	int ImageNo;//�ڵ�ͼƬ���
	int movestep;
	bool IsDead;
	float direction;
	//����Ϊ����ͼƬָ�롣
	void Move(IplImage*);
	//����Ϊ����ͼƬָ�롣
	void Draw(IplImage*);
};