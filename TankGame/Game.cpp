#include<opencv2/imgcodecs/legacy/constants_c.h>
#include "Game.h"

IplImage* cvLoadImage(const char* filename, int iscolor=CV_LOAD_IMAGE_COLOR)
{
	return cvCloneImage(&IplImage(cv::imread(filename, iscolor)));
}


CGame::CGame()
	: Score(0)
{
}


CGame::~CGame()
{
}

void CGame::ProgramInit(const char *wn)
{
	TankNum = 5;
	Time = 0;
	TimeScore = 0;
	FireFrequency = 16;
	invincible = 0;
	ShowTargetPoint = 0;
	//图片
	InitImage();
	WindowName = wn;
	GameStatus = 1;
	//打开窗口
	cvNamedWindow(WindowName, 1);//创建窗口
	cvInitFont(&ScoreFont, CV_FONT_HERSHEY_COMPLEX, 1, 1);
	cvInitFont(&GameOverFont, CV_FONT_HERSHEY_COMPLEX, 1, 1);
	cvInitFont(&PauseFont, CV_FONT_HERSHEY_COMPLEX, 1, 1);
	cvInitFont(&LevelFont, CV_FONT_HERSHEY_COMPLEX, 1, 1);
	TitleBGM = NULL,
	GameBGM = NULL,
	OverBGM = NULL,
	PauseSE = NULL,
	HitSE = NULL,
	StartSE = NULL,
	MoveSE = NULL,
	FireSE = NULL;
	channel[0] = NULL;
	channel[1] = NULL;
	FMOD::System_Create(&system);
	system->init(32, FMOD_INIT_NORMAL, NULL);
	system->createStream("bgm\\Normal Galaxy Man.ogg", FMOD_LOOP_NORMAL, NULL, &TitleBGM);
	system->createStream("bgm\\forest.ogg", FMOD_LOOP_NORMAL, NULL, &GameBGM);
	system->createStream("bgm\\Death2.ogg", FMOD_DEFAULT, NULL, &OverBGM);
	system->createStream("se\\Pause.mp3", FMOD_DEFAULT, NULL, &PauseSE);
	system->createStream("se\\sndBossHit.wav", FMOD_DEFAULT, NULL, &HitSE);
	system->createStream("se\\sndClick.wav", FMOD_DEFAULT, NULL, &StartSE);
	system->createStream("se\\sndJump.wav", FMOD_DEFAULT, NULL, &MoveSE);
	system->createStream("se\\sndShoot.wav", FMOD_DEFAULT, NULL, &FireSE);
}

void CGame::GameInit()
{
	//坦克
	InitTank();
	Score = 0;
}

int CGame::Run()
{
	Time = TimeScore = 0;
	char ScoreText[35] = "Score: ",
		input = 0;
	channel[0]->stop();
	system->playSound(GameBGM, 0, false, &channel[0]);
	while (1)
	{
		//敌人坦克运动
		//我的坦克运动
		//绘制
		//等待按键
		//退出，break
		if (!invincible&&Time == 5)
		{
			TimeScore += 0.1;
			Time = 0;
		}
		for (int i = 0; i < ET.size();i++)
		{
			if(ShowTargetPoint)
				ET[i]->ShowTargetPoint(scene, ET[i]->to_x, ET[i]->to_y, 20, 0x00FF0000, 1);
			if (Obj.ShootTank(ET[i]))
			{
				system->playSound(HitSE, 0, false, &channel[1]);
				ET[i]->ResetTank(scene);
				Score += 1;
			}
		}
		if (invincible)
			sprintf_s(ScoreText, 35, "Inv. Tanks:%2d Score:%3d", TankNum, Score);
		else
			sprintf_s(ScoreText, 35, "T.:%2d F.:%2d Time:%4.1f Sc.:%2d", TankNum, 100 - FireFrequency, TimeScore, Score);
		for (int i = 0; i < ET.size(); i++)
		{
			ET[i]->CalcOri();
			ET[i]->Draw(scene);
		}
		for (int j = 0; j < ET.size(); j++)
		{
			if (ET[j]->MoveTo(scene))
				ET[j]->ResetToPos(scene);
			ET[j]->CalcDir(ET[j]->to_x, ET[j]->to_y);
			if (rand() % FireFrequency == 0)
				ET[j]->CreateFire();
			if (ET[j]->ShootMyTank(&Obj) && !invincible)
			{
				for (int i = 0; i < ET[j]->Fires.size();)
				{
					if (ET[j]->Fires[i]->ImageNo < 14)
					{
						ET[j]->Fires[i]->Draw(scene);
						ET[j]->Fires[i]->Move(scene);
						ET[j]->Fires[i]->ImageNo++;
						i++;
					}
					else
					{
						delete ET[j]->Fires[i];
						ET[j]->Fires.erase(ET[j]->Fires.begin() + i);
					}
				}
				cvPutText(scene, ScoreText, { 4, scene->height - 8 }, &ScoreFont, CV_RGB(255, 255, 0));
				cvShowImage(WindowName, scene);
				return 3;
			}
			for (int i = 0; i < ET[j]->Fires.size();)
			{
				if (ET[j]->Fires[i]->ImageNo < 14)
				{
					ET[j]->Fires[i]->Draw(scene);
					ET[j]->Fires[i]->Move(scene);
					ET[j]->Fires[i]->ImageNo++;
					i++;
				}
				else
				{
					delete ET[j]->Fires[i];
					ET[j]->Fires.erase(ET[j]->Fires.begin() + i);
				}
			}
		}
		Obj.Draw(scene);
		for (int i = 0; i < Obj.Fires.size(); i++)
			if (Obj.Fires[i]->ImageNo < 14)
			{
				Obj.Fires[i]->Draw(scene);
				Obj.Fires[i]->Move(scene);
				Obj.Fires[i]->ImageNo++;
			}
			else
			{
				delete Obj.Fires[i];
				Obj.Fires.erase(Obj.Fires.begin() + i);
			}
		cvPutText(scene, ScoreText, { 4, scene->height-8 }, &ScoreFont, CV_RGB(255, 255, 0));
		cvShowImage(WindowName, scene);
		input = cvWaitKey(20);
		switch (input)
		{
		case 27:return 0;
		case 't':return 1;
		case 'r':return 2;
		case 'p':GamePause(); break;
		case 'o':return 3;//调试用
		default:break;
		}
		cvCopy(background, scene);
		if (Obj.Move(scene, input))
			system->playSound(FireSE, 0, false, &channel[1]);
		Time++;
	}
}

void CGame::GameRelease()
{
	for (int i = 0; i < ET.size(); i++)
	{
		for (int j = 0; j < ET[i]->Fires.size(); j++)
			delete ET[i]->Fires[j];
		ET[i]->Fires.clear();
		delete ET[i];
	}
	ET.clear();
}

void CGame::ProgramEnd()
{
	for (int i = 0; i < 24; i++)//释放图片
		cvReleaseImage(&tank[i]);
	for (int i = 0; i < 14; i++)
		cvReleaseImage(&fire[i]);
	for (int i = 0; i < 16; i++)
		cvReleaseImage(&TankExplode[i]);
	for (int i = 0; i < 4; i++)
		cvReleaseImage(&MyTank[i]);
	cvReleaseImage(&background);
	cvReleaseImage(&scene); //释放图像
	cvReleaseImage(&GameOverImage);
	cvDestroyWindow(WindowName);//销毁窗口
	TitleBGM->release();
	GameBGM->release();
	OverBGM->release();
	PauseSE->release();
	HitSE->release();
	StartSE->release();
	MoveSE->release();
	FireSE->release();
	system->close();
	system->release();
}

// 加载图片
void CGame::InitImage()
{
	//载入图像
	background = cvLoadImage("pic\\BOARD.BMP", 1);
	char filename[200] = "";
	for (int i = 0; i < 24; i++)
	{
		sprintf_s(filename, 200, "pic\\TANK4-%03d.BMP", 15 * i);
		tank[i] = cvLoadImage(filename, 1);
	}
	for (int i = 0; i < 14; i++)
	{
		sprintf_s(filename, 200, "pic\\SHELL_EXPLODE_%02d.BMP", i);
		fire[i] = cvLoadImage(filename);
	}
	for (int i = 0; i < 16; i++)
	{
		sprintf_s(filename, 200, "pic\\TANKEXPLODE%02d.BMP", i + 1);
		TankExplode[i] = cvLoadImage(filename);
	}
	for (int i = 0; i < 4; i++)
	{
		sprintf_s(filename, 200, "pic\\TANK2-%03d.BMP", i * 90);
		MyTank[i] = cvLoadImage(filename);
	}
	scene=cvCloneImage(background);
	GameOverImage = cvLoadImage("pic\\GameOver.png");
	TitleImage = cvLoadImage("pic\\Title.png");
}


// 初始化坦克
void CGame::InitTank()
{
	Obj.Init(MyTank, fire, (scene->width - tank[Obj.ImageNo]->width) / 2, scene->height - tank[Obj.ImageNo]->height - 40, 1, 10);
	for (int i = 0; i < TankNum; i++)
	{
		CEnemyTank *Etank = new CEnemyTank();
		ET.push_back(Etank);
		ET[i]->Init(tank, fire, rand() % (scene->width - tank[0]->width), rand() % (scene->height - tank[0]->height), 0, 3);
		ET[i]->to_x = rand() % (scene->width - tank[ET[i]->ImageNo]->width);
		ET[i]->to_y = rand() % (scene->height - tank[ET[i]->ImageNo]->height);
	}
}


// 绘制
void CGame::Draw()
{
}



// 游戏标题
int CGame::Title(float apparency)
{
	channel[0]->stop();
	system->playSound(TitleBGM, 0, false, &channel[0]);
	IplImage *TitleTemp = cvCloneImage(background);
	char LevelText1[22] = "Number of tanks: ", LevelText2[22] = "Fire Frequency: ", InfoText[20] = "Press \'Z\' To Start!",
		InvText[20] = "Invincible: ", ShowPointText[22] = "Show Destination: ";
	int pos_x = (TitleTemp->width - TitleImage->width) / 2, pos_y = (TitleTemp->height - TitleImage->height) / 2 - 60;
	const int text_x = 60, text_y = scene->height / 2 + 40, text_vspacing = 30;
	CvFont CreditFont;
	cvInitFont(&CreditFont, CV_FONT_HERSHEY_PLAIN, 1, 1);
	cvPutText(TitleTemp, "Made by lxfly2000 (Test Version)", { 0, TitleTemp->height - 13 }, &CreditFont, CV_RGB(255, 216, 257));
	cvPutText(TitleTemp, "BGM from I_Wanna_Love_The_Yukari_Ver0.300", { 0, TitleTemp->height-1 }, &CreditFont, CV_RGB(255, 216, 257));
	CvPoint src_p = { 0, 0 };
	for (src_p.y = 0; src_p.y < TitleImage->height; src_p.y++)
	{
		for (src_p.x = 0; src_p.x < TitleImage->width; src_p.x++)
			if ((pos_x + src_p.x) >= 0 && (pos_x + src_p.x) < TitleTemp->width && (pos_y + src_p.y) >= 0 && (pos_y + src_p.y) < TitleTemp->height&&
				!((&CV_IMAGE_ELEM(TitleImage, uchar, src_p.y, src_p.x*TitleImage->nChannels))[0] == 0 &&
				(&CV_IMAGE_ELEM(TitleImage, uchar, src_p.y, src_p.x*TitleImage->nChannels))[1] == 255 &&
				(&CV_IMAGE_ELEM(TitleImage, uchar, src_p.y, src_p.x*TitleImage->nChannels))[2] == 0))
			{
				(&CV_IMAGE_ELEM(TitleTemp, uchar, pos_y + src_p.y, (pos_x + src_p.x)*TitleTemp->nChannels))[0] =
					(&CV_IMAGE_ELEM(TitleTemp, uchar, pos_y + src_p.y, (pos_x + src_p.x)*TitleTemp->nChannels))[0] * apparency +
					(&CV_IMAGE_ELEM(TitleImage, uchar, src_p.y, src_p.x*TitleImage->nChannels))[0] * (1 - apparency);
				(&CV_IMAGE_ELEM(TitleTemp, uchar, pos_y + src_p.y, (pos_x + src_p.x)*TitleTemp->nChannels))[1] =
					(&CV_IMAGE_ELEM(TitleTemp, uchar, pos_y + src_p.y, (pos_x + src_p.x)*TitleTemp->nChannels))[1] * apparency +
					(&CV_IMAGE_ELEM(TitleImage, uchar, src_p.y, src_p.x*TitleImage->nChannels))[1] * (1 - apparency);
				(&CV_IMAGE_ELEM(TitleTemp, uchar, pos_y + src_p.y, (pos_x + src_p.x)*TitleTemp->nChannels))[2] =
					(&CV_IMAGE_ELEM(TitleTemp, uchar, pos_y + src_p.y, (pos_x + src_p.x)*TitleTemp->nChannels))[2] * apparency +
					(&CV_IMAGE_ELEM(TitleImage, uchar, src_p.y, src_p.x*TitleImage->nChannels))[2] * (1 - apparency);
			}
	}
	cvPutText(TitleTemp, InfoText, { 95, TitleTemp->height - 40 }, &LevelFont, CV_RGB(255, 255, 0));
	int ConfigEntry = 0;//0为坦克数，1为炮弹频率，2为无敌，3为显示目的地。
	while (true)
	{
		cvCopy(TitleTemp, scene);
		sprintf_s(LevelText1, 22, ConfigEntry == 0 ? "Number of Tanks:%c%2d%c" : "Number of Tanks:%c%2d%c", ConfigEntry == 0 && TankNum != 0 ? '<' : ' ', TankNum, ConfigEntry == 0 && TankNum != 99 ? '>' : 0);
		cvPutText(scene, LevelText1, { text_x, text_y }, &LevelFont, CV_RGB(255, 255, 0));
		sprintf_s(LevelText2, 22, ConfigEntry == 1 ? "Fire Frequency:%c%2d%c" : "Fire Frequency:%c%2d%c", ConfigEntry == 1 && FireFrequency != 100 ? '<' : ' ', 100 - FireFrequency, ConfigEntry == 1 && FireFrequency != 1 ? '>' : 0);
		cvPutText(scene, LevelText2, { text_x, text_y+text_vspacing }, &LevelFont, CV_RGB(255, 255, 0));
		sprintf_s(InvText, 20, invincible ? ConfigEntry == 2 ? "Invincible:<ON>" : "Invincible: ON" : "");
		cvPutText(scene, InvText, { text_x, text_y+text_vspacing*2 }, &LevelFont, CV_RGB(255, 255, 0));
		sprintf_s(ShowPointText, 22, ShowTargetPoint ? ConfigEntry == 3 ? "Show Destination:<ON>" : "Show Destination: ON" : "");
		cvPutText(scene, ShowPointText, { text_x, text_y+text_vspacing*3 }, &LevelFont, CV_RGB(255, 255, 0));
		cvShowImage(WindowName, scene);
		switch (cvWaitKey())
		{
		case 27:cvReleaseImage(&TitleTemp); return 0;
		case 't':cvReleaseImage(&TitleTemp); return 1;
		case 'u':
			if (ConfigEntry > 0)
			{
				system->playSound(MoveSE, 0, false, &channel[1]);
				ConfigEntry--;
			}
			break;
		case 'j':
			if (ConfigEntry < 3)
			{
				system->playSound(MoveSE, 0, false, &channel[1]);
				ConfigEntry++;
			}
			break;
		case 'k':
			if (ConfigEntry == 0 && TankNum < 99)
			{
				system->playSound(MoveSE, 0, false, &channel[1]);
				TankNum++;
			}
			if (ConfigEntry == 1 && FireFrequency > 1)
			{
				system->playSound(MoveSE, 0, false, &channel[1]);
				FireFrequency--;
			}
			if (ConfigEntry == 2)
			{
				system->playSound(MoveSE, 0, false, &channel[1]);
				invincible = !invincible;
			}
			if (ConfigEntry == 3)
			{
				system->playSound(MoveSE, 0, false, &channel[1]);
				ShowTargetPoint = !ShowTargetPoint;
			}
			break;
		case 'h':
			if (ConfigEntry == 0 && TankNum > 0)
			{
				system->playSound(MoveSE, 0, false, &channel[1]);
				TankNum--;
			}
			if (ConfigEntry == 1 && FireFrequency < 100)
			{
				system->playSound(MoveSE, 0, false, &channel[1]);
				FireFrequency++;
			}
			if (ConfigEntry == 2)
			{
				system->playSound(MoveSE, 0, false, &channel[1]);
				invincible = !invincible;
			}
			if (ConfigEntry == 3)
			{
				system->playSound(MoveSE, 0, false, &channel[1]);
				ShowTargetPoint = !ShowTargetPoint;
			}
			break;
		case 'z':
			cvReleaseImage(&TitleTemp);
			system->playSound(StartSE, 0, false, &channel[1]);
			return 2;
		default:break;
		}
	}
	cvReleaseImage(&TitleTemp);
	return -1;
}


// 游戏结束
int CGame::GameOver(float apparency)
{
	channel[0]->stop();
	system->playSound(OverBGM, 0, false, &channel[0]);
	IplImage *OverTemp = cvCloneImage(scene);
	CvPoint src_p = { 0, 0 };
	unsigned time = 0;
	while (true)
	{
		switch (cvWaitKey(20))
		{
		case 27:cvReleaseImage(&OverTemp); return 0;
		case 't':cvReleaseImage(&OverTemp); return 1;
		case 'r':cvReleaseImage(&OverTemp); return 2;
		default:
			if (time < 50)
			{
				if (time < 16)
				{
					for (src_p.y = 0; src_p.y < TankExplode[time]->height; src_p.y++)
					{
						for (src_p.x = 0; src_p.x < TankExplode[time]->width; src_p.x++)
							if ((Obj.pos_x + src_p.x) >= 0 && (Obj.pos_x + src_p.x) < scene->width && (Obj.pos_y + src_p.y) >= 0 && (Obj.pos_y + src_p.y) < scene->height&&
								!((&CV_IMAGE_ELEM(TankExplode[time], uchar, src_p.y, src_p.x*TankExplode[time]->nChannels))[0] == 0 &&
								(&CV_IMAGE_ELEM(TankExplode[time], uchar, src_p.y, src_p.x*TankExplode[time]->nChannels))[1] == 255 &&
								(&CV_IMAGE_ELEM(TankExplode[time], uchar, src_p.y, src_p.x*TankExplode[time]->nChannels))[2] == 0))
							{
								(&CV_IMAGE_ELEM(scene, uchar, Obj.pos_y + src_p.y, (Obj.pos_x + src_p.x)*scene->nChannels))[0] =
									(&CV_IMAGE_ELEM(scene, uchar, Obj.pos_y + src_p.y, (Obj.pos_x + src_p.x)*scene->nChannels))[0] * apparency +
									(&CV_IMAGE_ELEM(TankExplode[time], uchar, src_p.y, src_p.x*TankExplode[time]->nChannels))[0] * (1 - apparency);
								(&CV_IMAGE_ELEM(scene, uchar, Obj.pos_y + src_p.y, (Obj.pos_x + src_p.x)*scene->nChannels))[1] =
									(&CV_IMAGE_ELEM(scene, uchar, Obj.pos_y + src_p.y, (Obj.pos_x + src_p.x)*scene->nChannels))[1] * apparency +
									(&CV_IMAGE_ELEM(TankExplode[time], uchar, src_p.y, src_p.x*TankExplode[time]->nChannels))[1] * (1 - apparency);
								(&CV_IMAGE_ELEM(scene, uchar, Obj.pos_y + src_p.y, (Obj.pos_x + src_p.x)*scene->nChannels))[2] =
									(&CV_IMAGE_ELEM(scene, uchar, Obj.pos_y + src_p.y, (Obj.pos_x + src_p.x)*scene->nChannels))[2] * apparency +
									(&CV_IMAGE_ELEM(TankExplode[time], uchar, src_p.y, src_p.x*TankExplode[time]->nChannels))[2] * (1 - apparency);
							}
					}
					cvShowImage(WindowName, scene);
					cvCopy(OverTemp, scene);
				}
				else if (time == 16)
				{
					cvShowImage(WindowName, scene);
				}
				time++;
			}
			else if (time == 50)
			{
				int pos_x = (scene->width - GameOverImage->width) / 2, pos_y = (scene->height - GameOverImage->height) / 2 - 10;
				char ScoreText[20] = "Your score is: ";
				sprintf_s(ScoreText, 20, "Your score is: %3d", Score);
				cvPutText(scene, ScoreText, { 92, scene->height / 2 + 100 }, &ScoreFont, CV_RGB(255, 255, 0));
				for (src_p.y = 0; src_p.y < GameOverImage->height; src_p.y++)
				{
					for (src_p.x = 0; src_p.x < GameOverImage->width; src_p.x++)
						if ((pos_x + src_p.x) >= 0 && (pos_x + src_p.x) < scene->width && (pos_y + src_p.y) >= 0 && (pos_y + src_p.y) < scene->height&&
							!((&CV_IMAGE_ELEM(GameOverImage, uchar, src_p.y, src_p.x*GameOverImage->nChannels))[0] == 0 &&
							(&CV_IMAGE_ELEM(GameOverImage, uchar, src_p.y, src_p.x*GameOverImage->nChannels))[1] == 255 &&
							(&CV_IMAGE_ELEM(GameOverImage, uchar, src_p.y, src_p.x*GameOverImage->nChannels))[2] == 0))
						{
							(&CV_IMAGE_ELEM(scene, uchar, pos_y + src_p.y, (pos_x + src_p.x)*scene->nChannels))[0] =
								(&CV_IMAGE_ELEM(scene, uchar, pos_y + src_p.y, (pos_x + src_p.x)*scene->nChannels))[0] * apparency +
								(&CV_IMAGE_ELEM(GameOverImage, uchar, src_p.y, src_p.x*GameOverImage->nChannels))[0] * (1 - apparency);
							(&CV_IMAGE_ELEM(scene, uchar, pos_y + src_p.y, (pos_x + src_p.x)*scene->nChannels))[1] =
								(&CV_IMAGE_ELEM(scene, uchar, pos_y + src_p.y, (pos_x + src_p.x)*scene->nChannels))[1] * apparency +
								(&CV_IMAGE_ELEM(GameOverImage, uchar, src_p.y, src_p.x*GameOverImage->nChannels))[1] * (1 - apparency);
							(&CV_IMAGE_ELEM(scene, uchar, pos_y + src_p.y, (pos_x + src_p.x)*scene->nChannels))[2] =
								(&CV_IMAGE_ELEM(scene, uchar, pos_y + src_p.y, (pos_x + src_p.x)*scene->nChannels))[2] * apparency +
								(&CV_IMAGE_ELEM(GameOverImage, uchar, src_p.y, src_p.x*GameOverImage->nChannels))[2] * (1 - apparency);
						}
				}
				cvShowImage(WindowName, scene);
				time++;
			}
			break;
		}
	}
	cvReleaseImage(&OverTemp);
	return -1;
}


void CGame::GamePause()
{
	system->playSound(PauseSE, 0, false, &channel[1]);
	char PauseText[13] = "Game Paused.";
	cvPutText(scene, PauseText, { 128, scene->height / 2 + 10 }, &PauseFont, CV_RGB(255, 255, 0));
	cvShowImage(WindowName, scene);
	while (cvWaitKey() != ' ');
}
