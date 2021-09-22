#include "stdafx.h"
#include "system/system.h"
#include "Game.h"
#include "GameTime.h"
#include "StopWatch.h"
#include "SoundEngine.h"
#include "LightManager.h"

using namespace nsMyGame;

///////////////////////////////////////////////////////////////////
// �E�B���h�E�v���O�����̃��C���֐��B
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//�Q�[���̏������B
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("Game"));

	//�Q�[���̐���
	NewGO<nsGame::CGame>(nsCommonData::enPriorityFirst);

	// ��������Q�[�����[�v�B
	while (DispatchWindowMessage())
	{
		// �G���W���̃Q�[�����[�v
		nsMyEngine::CMyEngine::GetInstance()->ExecuteGameLoop();

	}


	return 0;
}

