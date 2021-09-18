#include "stdafx.h"
#include "system/system.h"
#include "Game.h"
#include "GameTime.h"
#include "StopWatch.h"
#include "SoundEngine.h"

using namespace nsMyGame;

///////////////////////////////////////////////////////////////////
// �E�B���h�E�v���O�����̃��C���֐��B
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//�Q�[���̏������B
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("Game"));

	//////////////////////////////////////
	// �������珉�������s���R�[�h���L�q����B
	//////////////////////////////////////

	//�Q�[���I�u�W�F�N�g�}�l�[�W���[�̃C���X�^���X���쐬����B
	GameObjectManager::CreateInstance();
	PhysicsWorld::CreateInstance();
	EffectEngine::CreateInstance();
	nsSound::CSoundEngine::CreateInstance();

	//�X�g�b�v�E�H�b�`�𐶐�����
	nsTimer::CStopWatch stopWatch;
	
	//////////////////////////////////////
	// ���������s���R�[�h�������̂͂����܂ŁI�I�I
	//////////////////////////////////////
	auto& renderContext = g_graphicsEngine->GetRenderContext();

	//�Q�[���̐���
	NewGO<nsGame::CGame>(nsCommonData::enPriorityFirst);

	// ��������Q�[�����[�v�B
	while (DispatchWindowMessage())
	{
		//�X�g�b�v�E�H�b�`�v���J�n
		stopWatch.Start();

		//�����_�����O�J�n�B
		g_engine->BeginFrame();
		

		//////////////////////////////////////
		//��������G��`���R�[�h���L�q����B
		//////////////////////////////////////
		
		GameObjectManager::GetInstance()->ExecuteUpdate();
		EffectEngine::GetInstance()->Update(nsTimer::GameTime().GetFrameDeltaTime());
		GameObjectManager::GetInstance()->ExecuteRender(renderContext);
		EffectEngine::GetInstance()->Draw();

#ifdef MY_DEBUG
		//FPS��`�悷��
		nsTimer::GameTime().DrawFPS(renderContext, (float)stopWatch.GetElapsed());
#endif
		
		//////////////////////////////////////
		//�G��`���R�[�h�������̂͂����܂ŁI�I�I
		//////////////////////////////////////
		g_engine->EndFrame();

		//FPS�Œ�̏���
		//�X�s�����b�N���s���B
		int restTime = 0;
		do {
			stopWatch.Stop();
			restTime = nsTimer::STOP_WATCH_CONST_DATA::MILLISECOND_FOR_LOCK_60FPS -
				static_cast<int>(stopWatch.GetElapsedMillisecond());
		} while (restTime > 0);

		//�X�g�b�v�E�H�b�`�v���I��
		stopWatch.Stop();

		//�f���^�^�C�����X�g�b�v�E�H�b�`�̌v�����Ԃ���A�v�Z����
		nsTimer::GameTime().PushFrameDeltaTime((float)stopWatch.GetElapsed());
	}

	nsSound::CSoundEngine::DeleteInstance();
	EffectEngine::DeleteInstance();
	PhysicsWorld::DeleteInstance();
	//�Q�[���I�u�W�F�N�g�}�l�[�W���[���폜�B
	GameObjectManager::DeleteInstance();
	return 0;
}

