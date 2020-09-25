#include "DxLib.h"
#include "Image.h"

// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ChangeWindowMode(TRUE);

	if (DxLib_Init() == -1)
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}

	Image image("source/image/nimotsukun.png", 5, 5, 1, 32, 32);
	for (int i = 0; i < 5; i++) {
		image.draw(i, 0, i);
	}
	// DrawPixel(320, 240, GetColor(255, 255, 255));	// �_��ł�

	WaitKey();				// �L�[���͑҂�

	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I�� 
}
