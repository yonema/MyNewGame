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

	/**
	 * @brief �}�b�v�i���x���j�p�l�[���X�y�[�X
	*/
	namespace nsMaps
	{
		/**
		 * @brief �e�X�g�}�b�v�i���x���j�p�̃l�[���X�y�[�X
		*/
		namespace nsTestMaps
		{
			/**
			 * @brief �v���C���[�̓����̃e�X�g�}�b�v
			*/
			class CTestMapForPlayerMove : public IGameObject
			{
			public:		// �R���X�g���N�^�ƃf�X�g���N�^
				/**
				 * @brief �R���X�g���N�^
				*/
				CTestMapForPlayerMove() = default;
				/**
				 * @brief �R���X�g���N�^
				*/
				~CTestMapForPlayerMove() = default;

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

			private:	// �萔
				static const char* const m_kBuildingName;	//!< �����̖��O
				static const char* const m_kLevelFilePaht;	//!< ���x���̃t�@�C���p�X

			private:	// �f�[�^�����o
				nsLevel3D::CLevel3D m_level3D;				//!< 3D���x���N���X
				nsNature::CSkyCube* m_skyCube = nullptr;	//!< �X�J�C�L���[�u
				nsPlayer::CPlayer* m_player = nullptr;		//!< �v���C���[�N���X

			};

		}
	}
}