#include "DxLib.h"
// #include "Image.h"
// #include "Array2D.h"
// #include "File.h"
// #include <fstream>
#include "State.h"

int gCounter = 1;
bool gClear = false;

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ChangeWindowMode(TRUE);

	if (DxLib_Init() == -1)
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	// Image image("source/image/nimotsukun.png", 5, 5, 1, 32, 32);
	// for (int i = 0; i < 5; i++) {
	// 	image.draw(i, 0, i);
	// }
	// 
	// File file("source/stage/stageData.txt");
	// file.drawString();

	State* state = new State();

	while (!(ScreenFlip()) && !(ProcessMessage()) && !(ClearDrawScreen())) {
		if (!state->checkIsClear()) {
			state->draw();
			// state->drawDebug();
			state->update(0, 0);
		}
		else {
			if (!gClear) {
				state->draw();
				// state->drawDebug();
				state->update(0, 0);
				gCounter++;
			}
			if (gCounter % 60 == 0) {
				gClear = true;
				DrawString(200, 200, "Clear !! Exit to push escape key", GetColor(255, 255, 0));
				if (CheckHitKey(KEY_INPUT_ESCAPE))break;
			}
		}
	}
	// DrawPixel(320, 240, GetColor(255, 255, 255));	// 点を打つ

	// WaitKey();	// キー入力待ち

	DxLib_End();				// ＤＸライブラリ使用の終了処理
		
	
	return 0;				// ソフトの終了 
}
