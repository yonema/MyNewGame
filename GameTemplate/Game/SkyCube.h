#pragma once
#include "NatureConstData.h"
#include "ModelRender.h"


namespace nsNinjaAttract
{
	// �O���錾
	namespace nsGraphic { namespace nsModel { class CModelRender; } }	// ���f�������_���[�N���X

	/**
	 * @brief ���R�֘A�̃l�[���X�y�[�X
	*/
	namespace nsNature
	{
		/**
		 * @brief �X�J�C�L���[�u�N���X
		*/
		class CSkyCube : public IGameObject
		{
		public:		// �R���X�g���N�^�ƃf�X�g���N�^
			/**
			 * @brief �R���X�g���N�^
			*/
			CSkyCube() = default;
			/**
			 * @brief �f�X�g���N�^
			*/
			~CSkyCube() = default;
		public:		// �I�[�o�[���C�h���������o�֐�

			/**
			 * @brief �X�^�[�g�֐�
			 * @return �A�b�v�f�[�g���s�����H
			*/
			bool Start() override final;

			/**
			 * @brief �j�����ꂽ�Ƃ��ɌĂ΂��֐�
			*/
			void OnDestroy() override final;

		public:		// �����o�֐�

			/**
			 * @brief �������֐�
			 * @attention �I�u�W�F�N�g�𐶐����Ă����ɌĂ΂Ȃ��ƌ��ʂ�����܂���B
			 * @param[in] skyCubeType �X�J�C�L���[�u�̃^�C�v
			 * @param[in] isIBL IBL���s�����H
			*/
			void Init(const nsSkyCubeConstData::EnSkyCubeType skyCubeType, const bool isIBL = true);

			/**
			 * @brief ���W��ݒ肷��
			 * @param[in] pos ���W
			*/
			void SetPosition(const Vector3& pos)
			{
				m_position = pos;
				if (m_skyModelRender)
					m_skyModelRender->SetPosition(pos);
			}

			/**
			 * @brief ���W�𓾂�
			 * @return ���W
			*/
			const Vector3& GetPosition() const
			{
				return m_position;
			}

			/**
			 * @brief �g�嗦��ݒ肷��iVector3�j
			 * @param[in] scale �g�嗦
			*/
			void SetScale(const Vector3& scale)
			{
				m_scale = scale;
				if (m_skyModelRender)
					m_skyModelRender->SetScale(m_scale);
			}

			/**
			 * @brief �g�嗦��ݒ肷��ifloat�j
			 * @param[in] scale �g�嗦
			*/
			void SetScale(const float scale)
			{
				m_scale.x = scale;
				m_scale.y = scale;
				m_scale.z = scale;
				if (m_skyModelRender)
					m_skyModelRender->SetScale(m_scale);
			}

		private:	// private�ȃ����o�֐�

			/**
			 * @brief �e�N�X�`���̏������֐�
			*/
			void InitTexture();

			/**
			 * @brief ���f�������_���[�̏������֐�
			*/
			void InitModelRender();


		private:	// �f�[�^�����o
			nsGraphic::nsModel::CModelRender* m_skyModelRender = nullptr;	//!< �X�J�C�̃��f�������_���[
			Texture m_skyTexture;											//!< �X�J�C�̃e�N�X�`��

			Vector3 m_position = Vector3::Zero;								//!< ���W
			Vector3 m_scale = nsSkyCubeConstData::kDefaultSkySclae;			//!< �g�嗦
			
		};

	}
}