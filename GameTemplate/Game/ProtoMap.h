#pragma once
#include "Level3D.h"

namespace nsNinjaAttract
{
	// �O���錾
	namespace nsGraphic {
		namespace nsModel { class CModelRender; }	// ���f�������_���[�N���X
	}
	namespace nsNature { class CSkyCube; }	// �X�J�C�N���X
	namespace nsPlayer { class CPlayer; }	// �v���C���[�N���X
	namespace nsGameState { class CGameMainState; }	// �Q�[�����C���X�e�[�g�N���X
	namespace nsGoal { class CGoal; }	// �S�[���N���X

	/**
	 * @brief �}�b�v�i���x���j�p�l�[���X�y�[�X
	*/
	namespace nsMaps
	{
		/**
		 * @brief �v���g�}�b�v�i���x���j�p�̃l�[���X�y�[�X
		*/
		namespace nsProtoMaps
		{
			/**
			 * @brief �v���g�p�}�b�v�N���X
			*/
			class CProtoMap : public IGameObject
			{
			public:		// �R���X�g���N�^�ƃf�X�g���N�^
				/**
				 * @brief �R���X�g���N�^
				*/
				CProtoMap() = default;
				/**
				 * @brief �R���X�g���N�^
				*/
				~CProtoMap() = default;

			public:		// �I�[�o�[���C�h���������o�֐�

				/**
				 * @brief �X�^�[�g�֐�
				 * @return �A�b�v�f�[�g���s�����H
				*/
				bool Start() override final;

				/**
				 * @brief �j���������ɌĂ΂��֐�
				*/
				void OnDestroy() override final;

				/**
				 * @brief �A�b�v�f�[�g�֐�
				*/
				void Update() override final;

			private:	// �f�[�^�����o
				nsLevel3D::CLevel3D m_level3D;				//!< 3D���x���N���X
				nsNature::CSkyCube* m_skyCube = nullptr;	//!< �X�J�C�L���[�u
				nsPlayer::CPlayer* m_player = nullptr;		//!< �v���C���[�N���X
				nsGoal::CGoal* m_goal = nullptr;			//!< �S�[���N���X
				nsGameState::CGameMainState* m_gameState = nullptr;	//!< �Q�[���X�e�[�g�N���X
			};

		}
	}
}