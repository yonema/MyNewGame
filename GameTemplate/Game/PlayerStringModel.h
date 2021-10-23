#pragma once
#include "ModelRender.h"

namespace nsMyGame
{
	// �O���錾
	namespace nsGraphic
	{
		namespace nsModel { class CModelRender; }	//���f�������_���[�N���X
	}

	/**
	 * @brief �v���C���[�֘A�̃l�[���X�y�[�X
	*/
	namespace nsPlayer
	{
		// �O���錾
		class CPlayer;		// �v���C���[�N���X

		/**
		 * @brief �v���C���[�̎��̃��f���N���X
		*/
		class CPlayerStringModel : public IGameObject
		{
		public:		// �R���X�g���N�^�ƃf�X�g���N�^
			/**
			 * @brief �R���X�g���N�^
			*/
			CPlayerStringModel() = default;
			/**
			 * @brief �f�X�g���N�^
			*/
			~CPlayerStringModel() = default;

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
			 * @param[in] player �v���C���[
			*/
			void Init(const CPlayer& player)
			{
				m_playerRef = &player;
			}

			/**
			 * @brief �w�肵�����W�Ɍ������ĐL�т鏈�����J�n����
			 * @param[in] pos �L�т��̍��W
			*/
			void StartStretchToPos(const Vector3& pos);

			/**
			 * @brief �w�肵�����W�Ɍ������ĐL�т鏈�����I������
			*/
			void EndStretchToPos();

			/**
			 * @brief �L�т����Ă��邩�H�𓾂�
			 * @return �L�т����Ă��邩�H
			*/
			bool IsStretched() const
			{
				return m_isStretched;
			}

			/**
			 * @brief ���̒����𓾂�
			 * @return ���̒���
			*/
			float GetStringLength() const
			{
				return m_modelRender->GetScale().z;
			}

		private:	// private�ȃ����o�֐�

			/**
			 * @brief ���f������]�����鏈��
			*/
			void ModelRotation();

		private:	// �f�[�^�����o
			const CPlayer* m_playerRef = nullptr;						//!< �v���C���[�̎Q��
			nsGraphic::nsModel::CModelRender* m_modelRender = nullptr;	//!< ���f�������_���[
			bool m_isStretched = true;									//!< �L�т����Ă��邩�H
			Vector3 m_toStretchPos = Vector3::Zero;						//!< �L�т��̍��W
			float m_stretchSpeed = 0.0f;								//!< �L�т鑬�x
		};

	}
}