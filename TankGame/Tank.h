#pragma once
#include<opencv2/opencv.hpp>
#include<opencv2/imgproc/imgproc_c.h>
#include<opencv2/highgui/highgui_c.h>
#include<vector>
#include"Fire.h"

class CTank
{
public:
	CTank();
	//注意方向参数是方向序号，不是弧度角。
	void Init(IplImage**,IplImage**, int init_x = 0, int init_y = 0, int init_direction = 0, int init_speed = 1, float init_ap = 0);
	virtual ~CTank();
	int pos_x;
	int pos_y;
	float apparency;
	IplImage **Image;
	IplImage **FireList;
	int movestep;//移动速度
	float direction;//方向，弧度
	int ImageNo;//坦克方向的序号
	/*将图片绘制在指定对象中，超出边界的部分将不被显示。
	src: 要绘制的图片对象
	dst: 背景区域对象
	init_x, init_y: 指定绘制的起始坐标，以绘制图片的左上角为原点
	apparency: 透明度，0 为不透明，1 为透明*/
	void Draw(IplImage*);
	float CalcDir(int,int);
	std::vector<CFire*> Fires;
	//参数为炮弹列表指针
	void CreateFire();
};
class CEnemyTank :public CTank
{
public:
	int to_x;
	int to_y;
	int m_SpeedX;
	int m_SpeedY;
	void Move(IplImage*,unsigned,int speed=0);//背景图片，方向（弧度，向右为0），速度
	bool MoveTo(IplImage*, int toX = -1, int toY = -1, int speed = 0);//背景图片，到X，到Y，速度
	//size: 正方形为半边长，圆形为半径；shape: 0=正方形，1=圆形
	void ShowTargetPoint(IplImage*, int target_x, int target_y, int size = 8, int color = 0x00FF0000, int shape = 0);
	void CalcOri();
	// 重置坦克
	void ResetTank(IplImage*);
	void ResetToPos(IplImage*);
	// 攻击我的坦克
	bool ShootMyTank(void* MyTank);
};
class CMyTank :public CTank
{
public:
	CMyTank();
	bool Move(IplImage*,char);
	//shape: 0=直线，1=箭头
	void ShowDirection(IplImage*, int dir = 1, int length = 8, int color = 0x00FF0000, int shape = 0, int distance = 16);
	// 攻击坦克
	bool ShootTank(CEnemyTank*);
};

