#pragma once

namespace nsNinjaAttract
{

	// �O���錾
	namespace nsGraphic {
		namespace nsModel { class CModelRender; }	// ���f�������_���[�N���X
	}
	namespace nsLight { class CSpotLight; }		// �|�C���g���C�g�N���X

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
			 * @brief �X�|�b�g���C�g�̃e�X�g�p�}�b�v
			*/
			class CTestMapForSpotLight : public IGameObject
			{
			public:		// �R���X�g���N�^�ƃf�X�g���N�^
				/**
				 * @brief �R���X�g���N�^
				*/
				CTestMapForSpotLight() = default;

				/**
				 * @brief �f�X�g���N�^
				*/
				~CTestMapForSpotLight() = default;

			public:		// �I�[�o�[���C�h���������o�֐�
				/**
				 * @brief �X�^�[�g�֐�
				 * @return �A�b�v�f�[�g���s�����H
				*/
				bool Start() override final;

				/**
				 * @brief �j���������ɌĂ΂��
				*/
				void OnDestroy() override final;

				/**
				 * @brief �A�b�v�f�[�g�֐�
				*/
				void Update() override final;

			private:	// �f�[�^�����o
				nsGraphic::nsModel::CModelRender* m_playerModel = nullptr;	//!< �v���C���[���f��
				nsGraphic::nsModel::CModelRender* m_bgModel = nullptr;		//!< �o�b�N�O���E���h���f��
				nsGraphic::nsModel::CModelRender* m_spotLightModel = nullptr;//!< �X�|�b�g���C�g���f��
				nsLight::CSpotLight* m_spotLight = nullptr;					//!< �X�|�b�g���C�g
			};
		}
	}
}

