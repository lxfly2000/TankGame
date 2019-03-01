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
		*MyTank[4],//我的坦克图片列表
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
	//参数为窗口标题
	void GameInit();
	short GameStatus;//0为游戏退出，1为返回标题，2为重新开始，3为游戏结束
	//返回值：0为游戏退出，1为返回标题，2为开始游戏或重新开始，3为游戏结束
	int Run();
	void GameRelease();
	void ProgramEnd();
	// 加载图片
	void InitImage();
	// 初始化坦克
	void InitTank();
	// 绘制
	void Draw();
	int Score;
	int Time;
	float TimeScore;
	CvFont ScoreFont;
	CvFont LevelFont;
	CvFont GameOverFont;
	CvFont PauseFont;
	// 游戏标题，0为游戏退出，1为返回标题，2为开始游戏
	int Title(float apparency = 0);
	// 游戏结束，0为退出，1为返回标题，2为重新开始，参数为透明度
	int GameOver(float apparency = 0);
	void GamePause();
};

