#pragma once
#include "GameMainStateConstData.h"
#include "StopWatch.h"
#include "UIConstData.h"

namespace nsNinjaAttract
{
	// �O���錾
	namespace nsUI { class CGameMainUI; }	//!< �Q�[�����C����UI�N���X
	namespace nsPlayer { class CPlayer; }	//!< �v���C���[�N���X
	namespace nsAICharacter { class CAICar; } //!< �ԃN���X

	/**
	 * @brief �Q�[���X�e�[�g�֘A�̃l�[���X�y�[�X
	*/
	namespace nsGameState
	{
		/**
		 * @brief �Q�[�����C���̃X�e�[�g�N���X
		*/
		class CGameMainState : public IGameObject
		{
		public:		// �R���X�g���N�^�ƃf�X�g���N�^
			/**
			 * @brief �R���X�g���N�^
			*/
			CGameMainState()
			{
				m_instance = this;
			}
			/**
			 * @brief �f�X�g���N�^
			*/
			~CGameMainState() = default;

		public:		// �I�[�o�[���C�h���������o�֐�

			/**
			 * @brief Update�̒��O�ŌĂ΂��J�n����
			 * @return �A�b�v�f�[�g���s�����H
			*/
			bool Start() override final;

			/**
			 * @brief �X�V����
			*/
			void OnDestroy() override final;

			/**
			 * @brief ��������鎞�ɌĂ΂�鏈��
			*/
			void Update() override final;
		public:		// �����o�֐�

			/**
			 * @brief ����������
			 * @param[in] player �v���C���[�̎Q��
			*/
			void Init(const nsPlayer::CPlayer& player)
			{
				m_playerRef = &player;
			}

			/**
			 * @brief �Q�[���̃^�C���̌v���J�n
			*/
			void StartTimingGame()
			{
				m_isTimeGame = true;	// �^�C�����v��
				m_stopWatch.Start();	// �X�g�b�v�E�H�b�`�̌v���J�n
			}

			/**
			 * @brief �Q�[���̃^�C���̌v���I��
			*/
			void StopTimingGame()
			{
				m_isTimeGame = false;	// �^�C�����v��Ȃ�
				m_stopWatch.Stop();		// �X�g�b�v�E�H�b�`�̌v���I��
			}

			/**
			 * @brief �Q�[���̃^�C���𓾂�
			 * @return 
			*/
			float GetGameTime() const
			{
				return m_gameTimer;
			}

			/**
			 * @brief �v���C���[�̎Q�Ƃ𓾂�
			 * @return �v���C���[�̎Q��
			*/
			const nsPlayer::CPlayer& GetPlayer() const
			{
				return *m_playerRef;
			}

			/**
			 * @brief �Ԃ�ǉ�����
			 * @param[in,out] aiCar �ǉ������
			*/
			void AddAICar(nsAICharacter::CAICar* aiCar);

			/**
			 * @brief �ԒB�̎Q�Ƃ𓾂�
			 * @return �Ԃ����̎Q��
			*/
			std::vector<nsAICharacter::CAICar*>* GetAICar()
			{
				return &m_aiCarsRef;
			}

			/**
			 * @brief �~�b�V��������N���A����
			 * @param[in] missionType �N���A����~�b�V�����̃^�C�v
			*/
			void ClearOneMission(const nsUI::nsMissionUIConstData::EnMissionType missionType);

			/**
			 * @brief �N���A�t���O�𓾂�B�v�f�̐���nsMissionUIConstData::enMissionTypeNum�B
			 * @return �N���A�t���O
			*/
			const bool* GetClearFlag() const
			{
				return m_missionClearFlag;
			}

			/**
			 * @brief �X�e�[�g��J��
			 * @param[in] newState �V�����X�e�[�g
			*/
			void ChangeState(const nsGameMainStateConstData::EnGameMainStateState newState);

			/**
			 * @brief �Q�[�����C���X�e�[�g�̃X�e�[�g�𓾂�
			 * @return �Q�[�����C���X�e�[�g�̃X�e�[�g
			*/
			nsGameMainStateConstData::EnGameMainStateState GetGameMainStateState() const
			{
				return m_gameMainStateState;
			}

			/**
			 * @brief �R�}���h���͂̃~�X���J�E���g
			*/
			void CountMissCommand()
			{
				m_numOfCommandMiss++;
			}

			/**
			 * @brief �R�}���h�~�X�̉񐔂𓾂�
			 * @return �R�}���h�~�X�̉�
			*/
			int GetNumOfCommandMiss() const
			{
				return m_numOfCommandMiss;
			}

			/**
			 * @brief �~�b�V������\������
			*/
			void ShowMission();

		public:		// static�ȃ����o�֐��ƃf�[�^�����o

			static CGameMainState* m_instance;	//!< �C���X�^���X

			/**
			 * @brief �C���X�^���X�𓾂�
			 * @return �C���X�^���X
			*/
			static CGameMainState* GetInstance()
			{
				return m_instance;
			}

		private:	// private�ȃ����o�֐�

			/**
			 * @brief �Q�[���̃^�C�����v������
			*/
			void TimeGame();

		private:	// �f�[�^�����o

			//!< �Q�[�����C���X�e�[�g�̃X�e�[�g
			nsGameMainStateConstData::EnGameMainStateState m_gameMainStateState =
				nsGameMainStateConstData::enGS_startDirecting;
			float m_gameTimer = 0.0f;	//!< �Q�[���̃^�C�}�[
			bool m_isTimeGame = false;	//!< �Q�[���̃^�C�����v�邩�H
			nsTimer::CStopWatch m_stopWatch;	//!< �X�g�b�v�E�H�b�`

			nsUI::CGameMainUI* m_gameMainUI = nullptr;	//!< �Q�[�����C����UI�N���X

			const nsPlayer::CPlayer* m_playerRef = nullptr;	//!< �v���C���[��const�Q��
			std::vector<nsAICharacter::CAICar*> m_aiCarsRef;	//!< �ԒB�̎Q��

			//!< �~�b�V�����̃N���A�t���O
			bool m_missionClearFlag[nsUI::nsMissionUIConstData::enMissionTypeNum] = {};
			int m_clearCounter = 0;		//!< �N���A�J�E���^�[
			float m_directingTimer = 0.0f;	//!< ���o�p�̃^�C�}�[
			int m_numOfCommandMiss = 0;	//!< �R�}���h���͂��~�X������
			
		};

		/**
		 * @brief �Q�[�����C���X�e�[�g�̃C���X�^���X�𓾂�
		 * @return �Q�[�����C���X�e�[�g�̃C���X�^���X
		*/
		static CGameMainState* GameMainState()
		{
			return CGameMainState::GetInstance();
		}

	}
}