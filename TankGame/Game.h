#pragma once
#include"Fire.h"
#include"Tank.h"
#include "fmod/fmod.hpp"
#pragma comment(lib,"fmod64_vc.lib")
class CGame
{
public:
	CGame();
	~CGame();
	FMOD::System *system = NULL;
	FMOD::Sound *TitleBGM, *GameBGM, *OverBGM, *PauseSE, *HitSE, *StartSE, *MoveSE, *FireSE;
	FMOD::Channel *channel[2];
	const char *WindowName;
	IplImage *background,
		*scene,
		*tank[24],
		*MyTank[4],//�ҵ�̹��ͼƬ�б�
		*TankExplode[16],
		*fire[14],
		*TitleImage,
		*GameOverImage;
	CMyTank Obj;
	std::vector<CEnemyTank*> ET;
	int TankNum;
	int FireFrequency;
	bool invincible;
	bool ShowTargetPoint;
	void ProgramInit(const char*);
	//����Ϊ���ڱ���
	void GameInit();
	short GameStatus;//0Ϊ��Ϸ�˳���1Ϊ���ر��⣬2Ϊ���¿�ʼ��3Ϊ��Ϸ����
	//����ֵ��0Ϊ��Ϸ�˳���1Ϊ���ر��⣬2Ϊ��ʼ��Ϸ�����¿�ʼ��3Ϊ��Ϸ����
	int Run();
	void GameRelease();
	void ProgramEnd();
	// ����ͼƬ
	void InitImage();
	// ��ʼ��̹��
	void InitTank();
	// ����
	void Draw();
	int Score;
	int Time;
	float TimeScore;
	CvFont ScoreFont;
	CvFont LevelFont;
	CvFont GameOverFont;
	CvFont PauseFont;
	// ��Ϸ���⣬0Ϊ��Ϸ�˳���1Ϊ���ر��⣬2Ϊ��ʼ��Ϸ
	int Title(float apparency = 0);
	// ��Ϸ������0Ϊ�˳���1Ϊ���ر��⣬2Ϊ���¿�ʼ������Ϊ͸����
	int GameOver(float apparency = 0);
	void GamePause();
};

