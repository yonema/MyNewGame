#pragma once

namespace nsMyGame
{
	// �O���錾
	namespace nsGraphic {
		namespace nsModel { class CModelRender; }	// ���f�������_���[�N���X
	}

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
			 * @brief �V���h�E�̃e�X�g�p�}�b�v
			*/
			class CTestMapForShadow : public IGameObject
			{
			public:  // �R���X�g���N�^�ƃf�X�g���N�^
				/**
				 * @brief �R���X�g���N�^
				*/
				CTestMapForShadow() = default;
				/**
				 * @brief �f�X�g���N�^
				*/
				~CTestMapForShadow() = default;

			public:		// �I�[�o�[���C�h���������o�֐�
				/**
				 * @brief �X�^�[�g�֐�
				 * @return �A�b�v�f�[�g���s�����H
				*/
				bool Start() override final;

				/**
				 * @brief �j�������Ƃ��ɌĂ΂��֐�
				*/
				void OnDestroy() override final;

				/**
				 * @brief �A�b�v�f�[�g�֐�
				*/
				void Update() override final;

			private:	//�萔�̃f�[�^�����o

				static const int m_kPlayerNum = 3;
				static const int m_kBlockNum = 3;

			private:	// �f�[�^�����o

				nsGraphic::nsModel::CModelRender* m_planeModel;
				nsGraphic::nsModel::CModelRender* m_unityChanModels[m_kPlayerNum] = {};
				nsGraphic::nsModel::CModelRender* m_blockModels[m_kBlockNum] = {};
				float m_cameraVecAngle = 0.0f;
				float m_cameraVecLength = 500.0f;

			};
		}
	}
}
