#include"Fire.h"

void CFire::Init(IplImage **ImageList,int x,int y,int speed)
{
	Image = ImageList;
	ImageNo = 0;
	IsDead = 0;
	movestep = 10;
	pos_x = x;
	pos_y = y;
}

void CFire::Move(IplImage *dst)
{
	if (pos_x > 0 && pos_x < dst->width - Image[0]->width)
		pos_x = (unsigned)(pos_x + movestep*cos(direction));
	if (pos_y>0 && pos_y < dst->width - Image[0]->width)
		pos_y = (unsigned)(pos_y + movestep*sin(direction));
}

void CFire::Draw(IplImage *dst)
{
	CvPoint src_p = { 0, 0 };
	for (src_p.y = 0; src_p.y < Image[ImageNo]->height; src_p.y++)
	{
		for (src_p.x = 0; src_p.x < Image[ImageNo]->width; src_p.x++)
			if (pos_x >= 0 && pos_x < dst->width-src_p.x && pos_y >= 0 && pos_y < dst->height-src_p.y&&
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