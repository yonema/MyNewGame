#pragma once

namespace nsNinjaAttract
{
	// �O���錾
	namespace nsPlayer { class CPlayer; }			// �v���C���[�N���X
	namespace nsGameState { class CGameMainState; }	// �Q�[�����C���X�e�[�g�N���X
	namespace nsBGM { class CBGM; }					// BGM�N���X
	namespace nsBackGround { class CBackGround; }	// �o�b�N�O���E���h�N���X
	namespace nsAI { class CAIField; }				// AI�t�B�[���h�N���X
	namespace nsLevel3D { class CLevel3D; }			// 3D���x���N���X


	/**
	 * @brief �}�b�v�i���x���j�p�l�[���X�y�[�X
	*/
	namespace nsMaps
	{
		/**
		 * @brief ���C���}�b�v�N���X
		*/
		class CMainMap : public IGameObject
		{
		public:		// �R���X�g���N�^�ƃf�X�g���N�^
			/**
			 * @brief �R���X�g���N�^
			*/
			CMainMap() = default;
			/**
			 * @brief �f�X�g���N�^
			*/
			~CMainMap() = default;

		public:		// �I�[�o�[���C�h���������o�֐�

			/**
			 * @brief Update�̒��O�ŌĂ΂��J�n����
			 * @return �A�b�v�f�[�g���s�����H
			*/
			bool Start() override final;

			/**
			 * @brief ��������鎞�ɌĂ΂�鏈��
			*/
			void OnDestroy() override final;

			/**
			 * @brief �X�V����
			*/
			void Update() override final;

		public:		// �����o�֐�

		private:	// private�ȃ����o�֐�

			/**
			 * @brief �v���C���[�̏�����
			*/
			void InitPlayer();

			/**
			 * @brief �Ԃ̏�����
			*/
			void InitCar();

		private:	// �f�[�^�����o
			std::unique_ptr<nsLevel3D::CLevel3D> m_playerLevel;	//!< �v���C���[�p���x��
			std::unique_ptr<nsLevel3D::CLevel3D> m_carLevel;	//!< �v���C���[�p���x��
			nsPlayer::CPlayer* m_player = nullptr;		//!< �v���C���[�N���X
			nsGameState::CGameMainState* m_gameState = nullptr;	//!< �Q�[���X�e�[�g�N���X
			nsBGM::CBGM* m_bgm = nullptr;				//!< BGM�N���X
			nsBackGround::CBackGround* m_backGround = nullptr;	//!< �o�b�N�O���E���h�N���X
			std::unique_ptr<nsAI::CAIField> m_aiField;	//!< AI�̃t�B�[���h�N���X
		};

	}
}