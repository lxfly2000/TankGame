#include "Tank.h"

const float PI = 3.141592654;

CTank::CTank()
{
}

CTank::~CTank()
{
}

void CTank::Init(IplImage **src,IplImage **fires,int x,int y,int dir,int speed,float ap)
{
	pos_x = x;
	pos_y = y;
	movestep = speed;
	apparency = ap;
	Image = src;
	ImageNo =  dir;
	direction = PI*1.5;
	FireList = fires;
}

void CEnemyTank::ResetToPos(IplImage *dst)
{
	to_x = rand() % (dst->width - Image[ImageNo]->width);
	to_y = rand() % (dst->height - Image[ImageNo]->height);
}

void CTank::Draw(IplImage *dst)
{
	CvPoint src_p = { 0, 0 };
	for (src_p.y = 0; src_p.y<Image[ImageNo]->height; src_p.y++)
	{
		for (src_p.x = 0; src_p.x<Image[ImageNo]->width; src_p.x++)
			if ((pos_x + src_p.x) >= 0 && (pos_x + src_p.x) < dst->width && (pos_y + src_p.y) >= 0 && (pos_y + src_p.y) < dst->height&&
				!((&CV_IMAGE_ELEM(Image[ImageNo], uchar, src_p.y, src_p.x*Image[ImageNo]->nChannels))[0] == 0 &&
				(&CV_IMAGE_ELEM(Image[ImageNo], uchar, src_p.y, src_p.x*Image[ImageNo]->nChannels))[1] == 255 &&
				(&CV_IMAGE_ELEM(Image[ImageNo], uchar, src_p.y, src_p.x*Image[ImageNo]->nChannels))[2] == 0))
			{
				(&CV_IMAGE_ELEM(dst, uchar, pos_y + src_p.y, (pos_x + src_p.x)*dst->nChannels))[0] =
					(&CV_IMAGE_ELEM(dst, uchar, pos_y + src_p.y, (pos_x + src_p.x)*dst->nChannels))[0] * apparency +
					(&CV_IMAGE_ELEM(Image[ImageNo], uchar, src_p.y, src_p.x*Image[ImageNo]->nChannels))[0] * (1 - apparency);
				(&CV_IMAGE_ELEM(dst, uchar, pos_y + src_p.y, (pos_x + src_p.x)*dst->nChannels))[1] =
					(&CV_IMAGE_ELEM(dst, uchar, pos_y + src_p.y, (pos_x + src_p.x)*dst->nChannels))[1] * apparency +
					(&CV_IMAGE_ELEM(Image[ImageNo], uchar, src_p.y, src_p.x*Image[ImageNo]->nChannels))[1] * (1 - apparency);
				(&CV_IMAGE_ELEM(dst, uchar, pos_y + src_p.y, (pos_x + src_p.x)*dst->nChannels))[2] =
					(&CV_IMAGE_ELEM(dst, uchar, pos_y + src_p.y, (pos_x + src_p.x)*dst->nChannels))[2] * apparency +
					(&CV_IMAGE_ELEM(Image[ImageNo], uchar, src_p.y, src_p.x*Image[ImageNo]->nChannels))[2] * (1 - apparency);
			}
	}
}

//��tan�󻡶Ƚ�
float CTank::CalcDir(int to_X, int to_Y)
{
	if (to_X - pos_x == 0)
		direction = to_Y < pos_y ? PI * 1.5 : PI / 2;
	else
	{
		direction = atan((float)(to_Y - pos_y) / (to_X - pos_x));
		if (to_X < pos_x)
			direction += PI;
		if (direction < 0)
			direction += PI * 2;
	}
	return direction;
}

CMyTank::CMyTank()
{

}

bool CMyTank::Move(IplImage *dst,char key)
{
	switch (key)
	{
	case 'u':
		ImageNo = 1;
		direction = 1.5 * PI;
		if (pos_y >= movestep)
			pos_y -= movestep;
		else
			pos_y = 0;
		break;
	case 'j':
		ImageNo = 3;
		direction = 0.5*PI;
		if (pos_y <= dst->height - Image[ImageNo]->height - movestep)
			pos_y += movestep;
		else
			pos_y = dst->height - Image[ImageNo]->height;
		break;
	case 'h':
		ImageNo = 0;
		direction = PI;
		if (pos_x >= movestep)
			pos_x -= movestep;
		else
			pos_x = 0;
		break;
	case 'k':
		ImageNo = 2;
		direction = 0;
		if (pos_x <= dst->width - Image[ImageNo]->width - movestep)
			pos_x += movestep;
		else
			pos_x = dst->width - Image[ImageNo]->width;
		break;
	case 'z':
		CreateFire();
		return 1;
	default:
		break;
	}
	return 0;
}

void CMyTank::ShowDirection(IplImage *dst, int dir, int size, int color, int shape,int dis)
{
	//��ʾ�Լ�̹���ƶ��ķ���
}

void CEnemyTank::Move(IplImage *dst,unsigned dir,int v)
{
	if (v != 0)
		movestep = v;
	pos_x = (int)(pos_x + movestep*cos(dir)) % (dst->width - Image[ImageNo]->width);
	pos_y = (int)(pos_y + movestep*sin(dir)) % (dst->height - Image[ImageNo]->height);
}
bool CEnemyTank::MoveTo(IplImage *dst, int toX, int toY, int v)
{
	if (v != 0)
		movestep = v;
	if (toX != -1 && toY != -1)
	{
		to_x = toX;
		to_y = toY;
	}
	pos_x = (unsigned)(pos_x + movestep*(to_x - pos_x) / sqrt((pos_x - to_x)*(pos_x - to_x) + (pos_y - to_y)*(pos_y - to_y))) % (dst->width - Image[ImageNo]->width);
	pos_y = (unsigned)(pos_y + movestep*(to_y - pos_y) / sqrt((pos_x - to_x)*(pos_x - to_x) + (pos_y - to_y)*(pos_y - to_y))) % (dst->height - Image[ImageNo]->height);
	if (abs(to_x - pos_x) < movestep&&abs(to_y - pos_y) < movestep)
		return 1;
	else
		return 0;
}

void CEnemyTank::ShowTargetPoint(IplImage *dst, int x, int y, int size, int color, int shape)
{
	for (int j = -size; j <= size; j++)
		for (int i = -size; i <= size; i++)
			if (shape == 0 || shape == 1 && sqrt(i*i + j*j) <= size + 0.5)
				if (x + i >= 0 && x + i < dst->width&&y + j >= 0 && y + j < dst->height)
				{
					(&CV_IMAGE_ELEM(dst, uchar, y + j, (x + i) *dst->nChannels))[0] = color % 0x100;
					(&CV_IMAGE_ELEM(dst, uchar, y + j, (x + i) *dst->nChannels))[1] = color / 0x100 % 0x100;
					(&CV_IMAGE_ELEM(dst, uchar, y + j, (x + i) *dst->nChannels))[2] = color / 0x10000 % 0x100;
				}
}

void CTank::CreateFire()
{
	CFire *Fire = new CFire();
	Fire->Init(FireList, pos_x, pos_y);
	Fire->direction = direction;
	Fires.push_back(Fire);
}

void CEnemyTank::CalcOri()
{
	const int step = 4;
	m_SpeedX = step * cos(direction);
	m_SpeedY = step * sin(direction);

	ImageNo = ((int)((direction*180/PI+7.5)/15)+12)%24;
}

// ����̹��
bool CMyTank::ShootTank(CEnemyTank* tank)
{
	int minX = MAX(pos_x, tank->pos_x);
	int minY = MAX(pos_y, tank->pos_y);
	int maxX = MIN(pos_x + Image[ImageNo]->width, tank->pos_x + tank->Image[ImageNo]->width);
	int maxY = MIN(pos_y + Image[ImageNo]->height, tank->pos_y + tank->Image[ImageNo]->height);
	if (!(minX > maxX || minY > maxY))
		return true; //̹����̹����ײ
	for (int i = 0; i < Fires.size();)
	{
		minX = MAX(Fires[i]->pos_x, tank->pos_x);
		minY = MAX(Fires[i]->pos_y, tank->pos_y);
		maxX = MIN(Fires[i]->pos_x + Fires[i]->Image[Fires[i]->ImageNo]->width, tank->pos_x + tank->Image[ImageNo]->width);
		maxY = MIN(Fires[i]->pos_y + Fires[i]->Image[Fires[i]->ImageNo]->height, tank->pos_y + tank->Image[ImageNo]->height);
		if (!(minX>maxX || minY > maxY))
		{
			delete Fires[i];
			Fires.erase(Fires.begin() + i);
			return true;//�ڵ���̹����ײ
		}
		else
		{
			i++;
		}
	}
	return false;
}

// ����̹��
void CEnemyTank::ResetTank(IplImage *dst)
{
	int NewDirection = rand() % 4;//0Ϊ���ϣ�1Ϊ���ϣ�2Ϊ���£�3Ϊ����
	switch (NewDirection)
	{
	case 0:pos_x = 0; pos_y = 0; break;
	case 1:pos_x = dst->width - Image[ImageNo]->width; pos_y = 0; break;
	case 2:pos_x = 0; pos_y = dst->height - Image[ImageNo]->height; break;
	case 3:pos_x = dst->width - Image[ImageNo]->width;
		pos_y = dst->height - Image[ImageNo]->height; break;
	default:
		break;
	}
	to_x = rand() % (dst->width - Image[ImageNo]->width);
	to_y = rand() % (dst->height - Image[ImageNo]->height);
}


// �����ҵ�̹��
bool CEnemyTank::ShootMyTank(void* tank)
{
	CMyTank *MyTank = (CMyTank*)tank;
	for (auto f : Fires)
	{
		int px[4] = { f->pos_x,f->pos_x,f->pos_x + f->Image[f->ImageNo]->width,f->pos_x + f->Image[f->ImageNo]->width };
		int py[4] = { f->pos_y,f->pos_y + f->Image[f->ImageNo]->height,f->pos_y,f->pos_y + f->Image[f->ImageNo]->height };
		for (int i = 0; i < 4; i++)
		{
			if (px[i] > MyTank->pos_x&&px[i]<MyTank->pos_x + MyTank->Image[MyTank->ImageNo]->width&&
				py[i]>MyTank->pos_y&&py[i] < MyTank->pos_y + MyTank->Image[MyTank->ImageNo]->height)
				return true;
		}
	}
	return false;
}
