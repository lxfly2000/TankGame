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
	//ע�ⷽ������Ƿ�����ţ����ǻ��Ƚǡ�
	void Init(IplImage**,IplImage**, int init_x = 0, int init_y = 0, int init_direction = 0, int init_speed = 1, float init_ap = 0);
	virtual ~CTank();
	int pos_x;
	int pos_y;
	float apparency;
	IplImage **Image;
	IplImage **FireList;
	int movestep;//�ƶ��ٶ�
	float direction;//���򣬻���
	int ImageNo;//̹�˷�������
	/*��ͼƬ������ָ�������У������߽�Ĳ��ֽ�������ʾ��
	src: Ҫ���Ƶ�ͼƬ����
	dst: �����������
	init_x, init_y: ָ�����Ƶ���ʼ���꣬�Ի���ͼƬ�����Ͻ�Ϊԭ��
	apparency: ͸���ȣ�0 Ϊ��͸����1 Ϊ͸��*/
	void Draw(IplImage*);
	float CalcDir(int,int);
	std::vector<CFire*> Fires;
	//����Ϊ�ڵ��б�ָ��
	void CreateFire();
};
class CEnemyTank :public CTank
{
public:
	int to_x;
	int to_y;
	int m_SpeedX;
	int m_SpeedY;
	void Move(IplImage*,unsigned,int speed=0);//����ͼƬ�����򣨻��ȣ�����Ϊ0�����ٶ�
	bool MoveTo(IplImage*, int toX = -1, int toY = -1, int speed = 0);//����ͼƬ����X����Y���ٶ�
	//size: ������Ϊ��߳���Բ��Ϊ�뾶��shape: 0=�����Σ�1=Բ��
	void ShowTargetPoint(IplImage*, int target_x, int target_y, int size = 8, int color = 0x00FF0000, int shape = 0);
	void CalcOri();
	// ����̹��
	void ResetTank(IplImage*);
	void ResetToPos(IplImage*);
	// �����ҵ�̹��
	bool ShootMyTank(void* MyTank);
};
class CMyTank :public CTank
{
public:
	CMyTank();
	bool Move(IplImage*,char);
	//shape: 0=ֱ�ߣ�1=��ͷ
	void ShowDirection(IplImage*, int dir = 1, int length = 8, int color = 0x00FF0000, int shape = 0, int distance = 16);
	// ����̹��
	bool ShootTank(CEnemyTank*);
};

