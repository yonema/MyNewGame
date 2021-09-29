#include "stdafx.h"
#include "system/system.h"
#include "Game.h"

using namespace nsMyGame;

///////////////////////////////////////////////////////////////////
// �E�B���h�E�v���O�����̃��C���֐��B
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//�Q�[���̏������B
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("Game"));

	//�Q�[���̐���
	nsGame::CGame* game = NewGO<nsGame::CGame>(nsCommonData::enPriorityFirst);

	// ��������Q�[�����[�v�B
	while (DispatchWindowMessage())
	{
		// �G���W���̃Q�[�����[�v
		nsMyEngine::CMyEngine::GetInstance()->ExecuteGameLoop();
	}

	// �Q�[���̔j��
	DeleteGO(game);

	// �Q�[����j�����邽�߂ɁA�I�u�W�F�N�g�}�l�[�W���[�ɂ��A�b�v�f�[�g�̎��s
	GameObjectManager::GetInstance()->ExecuteUpdate();

	// �G���W���̃C���X�^���X��j������
	nsMyGame::nsMyEngine::CMyEngine::DeleteInstance();


	return 0;
}

