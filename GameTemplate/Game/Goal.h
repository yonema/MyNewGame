#pragma once

namespace nsNinjaAttract
{
	// �O���錾
	namespace nsGraphic { namespace nsModel { class CModelRender; } }	// ���f�������_���[
	namespace nsPlayer { class CPlayer; }	// �v���C���[

	/**
	 * @brief �S�[���֘A�̃l�[���X�y�[�X
	*/
	namespace nsGoal
	{
		/**
		 * @brief �S�[���N���X
		*/
		class CGoal : public IGameObject
		{
		public:		// �R���X�g���N�^�ƃf�X�g���N�^
			/**
			 * @brief �R���X�g���N�^
			*/
			CGoal() = default;
			/**
			 * @brief �f�X�g���N�^
			*/
			~CGoal() = default;

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
			 * @brief ������
			 * @param[in] pos ���W
			 * @param[in] rot ��]
			 * @param[in] scale �g�嗦
			 * @param[in] player �v���C���[
			*/
			void Init(
				const Vector3& pos,
				const Quaternion& rot,
				const Vector3 scale,
				nsPlayer::CPlayer& player
				);

		private:

			/**
			 * @brief �N���N����]��������
			*/
			void Rotationg();

			/**
			 * @brief �S�[�����Ă��邩�𒲂ׂ�
			*/
			void CheckIsGoal();

		private:	// �f�[�^�����o
			nsGraphic::nsModel::CModelRender* m_goalMR = nullptr;	//!< �S�[���̃��f�������_���[
			const nsPlayer::CPlayer* m_player = nullptr;			//!< �v���C���[
			float m_rotateAngle = 0.0f;
		};

	}
}