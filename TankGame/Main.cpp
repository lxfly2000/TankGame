// "C:\Documents and Settings\All Users\Documents\My Pictures\示例图片\Blue hills.jpg"
// "C:\Documents and Settings\All Users\Documents\My Pictures\示例图片\Sunset.jpg"
#include"Tank.h"
#include"Game.h"
#include<Windows.h>
int WINAPI WinMain(HINSTANCE hInst,HINSTANCE hPrevInst,LPSTR param,int iShow)
{
	srand(time(0));
	CGame game1;
	const char *WindowName1 = "TankGame";
	game1.ProgramInit(WindowName1);

	while (game1.GameStatus != 0)
	{
		while (game1.GameStatus != 0)
		{
			//播放标题音乐或者重新播放
			game1.GameStatus = game1.Title();
			if (game1.GameStatus == 0)
			{
				game1.ProgramEnd();
				return 0;
			}
			else if (game1.GameStatus == 2)
				break;
		}
		game1.GameInit();
		while (game1.GameStatus != 0)
		{
			//播放游戏音乐或者重新播放
			game1.GameStatus = game1.Run();
			if (game1.GameStatus == 3)
				game1.GameStatus = game1.GameOver();
			if (game1.GameStatus != 0)
			{
				game1.GameRelease();
				if (game1.GameStatus == 1)
					break;
				game1.GameInit();
			}
		}
	}
	game1.GameRelease();

	game1.ProgramEnd();
	return 0;
}