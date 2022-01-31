#include "stdafx.h"
#include "MyEngine.h"
#include "GameTime.h"
#include "RenderingEngine.h"
#include "SoundEngine.h"
#include "LightManager.h"
#include "StringActionTargetManager.h"
#include "system/system.h"
#include "DebugManager.h"

namespace nsNinjaAttract
{
	/**
	 * @brief ���̃Q�[���p�̃Q�[���G���W���BTkEngine�����b�v����
	 * @note �V���O���g���p�^�[�����g�p
	*/
	namespace nsMyEngine
	{
		CMyEngine* CMyEngine::m_instance = nullptr;	// �B��̃C���X�^���X

		/**
		 * @brief �R���X�g���N�^
		*/
		CMyEngine::CMyEngine()
		{
			// tkEngine�̐����Ə�����
			m_tkEngine.reset(new TkEngine);
			m_tkEngine->Init(g_hWnd, FRAME_BUFFER_W, FRAME_BUFFER_H);

			// �e��C���X�^���X�𐶐�
			CreateInstances();

			// �X�g�b�v�E�H�b�`�̐���
			m_stopWatch.reset(new nsTimer::CStopWatch);

			return;
		}

		/**
		 * @brief �f�X�g���N�^
		*/
		CMyEngine::~CMyEngine()
		{
			// �e��C���X�^���X��j��
			DeleteInstances();

			return;
		}


		/**
		 * @brief �e��C���X�^���X�𐶐�
		*/
		void CMyEngine::CreateInstances()
		{
			GameObjectManager::CreateInstance();		// �Q�[���I�u�W�F�N�g�}�l�[�W���[�𐶐�
			PhysicsWorld::CreateInstance();				// �������[���h�𐶐�
			nsLight::CLightManager::CreateInstance();	// ���C�g�}�l�[�W���[�𐶐�
			CRenderingEngine::CreateInstance();			// �����_�����O�G���W���̐���
			EffectEngine::CreateInstance();				// �G�t�F�N�g�G���W���𐶐�
			nsSound::CSoundEngine::CreateInstance();	// �T�E���h�G���W���𐶐�
			// �X�C���O�̃^�[�Q�b�g�̃}�l�[�W���[�𐶐�
			nsStringActionTarget::CStringActionTargetManager::CreateInstance();
			nsDebug::CDebugManager::CreateInstance();	// �f�o�b�N�}�l�[�W���[�𐶐�

			return;
		}

		/**
		 * @brief �e��C���X�^���X��j��
		*/
		void CMyEngine::DeleteInstances()
		{
			nsDebug::CDebugManager::DeleteInstance();	// �f�o�b�N�}�l�[�W���[��j��
			// �X�C���O�̃^�[�Q�b�g�̃}�l�[�W���[��j��
			nsStringActionTarget::CStringActionTargetManager::DeleteInstance();
			nsSound::CSoundEngine::DeleteInstance();	// �T�E���h�G���W����j��
			EffectEngine::DeleteInstance();				// �G�t�F�N�g�G���W����j��
			CRenderingEngine::DeleteInstance();			// �����_�����O�G���W���̔j��
			nsLight::CLightManager::DeleteInstance();	// ���C�g�}�l�[�W���[��j��
			PhysicsWorld::DeleteInstance();				// �������[���h��j��
			GameObjectManager::DeleteInstance();		// �Q�[���I�u�W�F�N�g�}�l�[�W���[��j��

			return;
		}

		/**
		 * @brief �Q�[�����[�v�����s����
		*/
		void CMyEngine::ExecuteGameLoop()
		{
			// tkEngine�̃t���[���̍ŏ��̏���
			m_tkEngine->BeginFrame();

			// �e��A�b�v�f�[�g�����s������
			ExecuteUpdate();

			// �����_�����O�G���W�������s
			CRenderingEngine::GetInstance()->Execute(*m_stopWatch);

			// �X�s�����b�N�ɂ��FPS�Œ菈��
			Spinlock();

			//�X�g�b�v�E�H�b�`�v���I��
			m_stopWatch->Stop();
			//�f���^�^�C�����X�g�b�v�E�H�b�`�̌v�����Ԃ���A�v�Z����
			nsTimer::GameTime().PushFrameDeltaTime(static_cast<float>(m_stopWatch->GetElapsed()));

			//�X�g�b�v�E�H�b�`�v���J�n
			m_stopWatch->Start();

			// tkEngine�̃t���[���̍Ō�̏���
			m_tkEngine->EndFrame();

			return;
		}

		/**
		 * @brief �e��A�b�v�f�[�g�����s������
		*/
		void CMyEngine::ExecuteUpdate()
		{
			// �I�u�W�F�N�g�}�l�[�W���[�ɂ��A�b�v�f�[�g�̎��s
			GameObjectManager::GetInstance()->ExecuteUpdate();
			// �G�t�F�N�g�̃A�b�v�f�[�g
			EffectEngine::GetInstance()->Update(nsTimer::GameTime().GetFrameDeltaTime());
			// ���C�g�}�l�[�W���[�̃A�b�v�f�[�g
			nsLight::CLightManager::GetInstance()->Update();

			return;
		}

		/**
		 * @brief �X�s�����b�N�ɂ��FPS�Œ菈��
		*/
		void CMyEngine::Spinlock()
		{
			// �X�s�����b�N���s��
			int restTime = 0;
			do {
				m_stopWatch->Stop();
				restTime = nsTimer::STOP_WATCH_CONST_DATA::MILLISECOND_FOR_LOCK_60FPS -
					static_cast<int>(m_stopWatch->GetElapsedMillisecond());
			} while (restTime > 0);

			return;
		}


		/**
		 * @brief ���O�̔j������
		*/
		void CMyEngine::PreDelete()
		{
			nsDebug::CDebugManager::GetInstance()->PreDelete();
			CRenderingEngine::GetInstance()->PreDelete();

			return;
		}
	}
}
