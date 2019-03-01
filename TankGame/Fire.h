#pragma once
#include<opencv2/imgproc/imgproc_c.h>

class CFire
{
public:
	void Init(IplImage**,int,int,int speed=10);//炮弹图片，初始X，初始Y，移动速度
	IplImage **Image;
	float apparency;
	int pos_x;
	int pos_y;
	int to_x;
	int to_y;
	int ImageNo;//炮弹图片序号
	int movestep;
	bool IsDead;
	float direction;
	//参数为背景图片指针。
	void Move(IplImage*);
	//参数为背景图片指针。
	void Draw(IplImage*);
};