#pragma once
#include "GaussianBlur.h"
#include "Noncopyable.h"

namespace nsNinjaAttract
{
	/**
	 * @brief �O���t�B�b�N�֘A�̃l�[���X�y�[�X
	*/
	namespace nsGraphic
	{
		/**
		 * @brief �V���h�E�֘A�̃l�[���X�y�[�X
		*/
		namespace nsShadow
		{
			/**
			 * @brief �v���C���[��p�̃V���h�E�}�b�v
			*/
			class CPlayerShadowMap : private nsUtil::Noncopyable
			{
			public:		// �R���X�g���N�^�ƃf�X�g���N�^
				/**
				 * @brief �R���X�g���N�^
				*/
				CPlayerShadowMap();
				/**
				 * @brief �f�X�g���N�^
				*/
				~CPlayerShadowMap() = default;

			public:		// �����o�֐�

				/**
				 * @brief �`��֐�
				 * @param[in] rc �����_�[�R���e�L�X�g
				 * @param[in] ligNo ���C�g�̔ԍ�
				 * @param[in] lightDirection ���C�g�̕���
				*/
				void Render(RenderContext& rc,const int ligNo, const Vector3& lightDirection);

				/**
				 * @brief �v���C���[�����_���[��ݒ�
				 * @param[in] playerRender �v���C���[�����_���[
				*/
				void SetPlayerRender(CRender* playerRender)
				{
					m_playerRender = playerRender;
				}

				/**
				 * @brief �V���h�E�}�b�v���擾����
				 * @return �V���h�E�}�b�v�̃e�N�X�`���̎Q��
				*/
				Texture& GetShadowMap()
				{
					return m_blur.GetBokeTexture();
				}

				/**
				 * @brief ���C�g�r���[�v���W�F�N�V�����s����擾
				 * @return ���C�g�r���[�v���W�F�N�V�����s��
				*/
				const Matrix& GetLVPMatrix() const
				{
					return m_lvpMatrix;
				}

			private:	// private�ȃ����o�֐�

				/**
				 * @brief ����������
				*/
				void Init();

				/**
				 * @brief ���C�g�r���[�v���W�F�N�V�����s����v�Z����
				 * @param[out] lvpMatrixOut ���C�g�r���[�v���W�F�N�V�����̏o��
				 * @param[in] lightDirection ���C�g�̕���
				*/
				void CalcLightViewProjectionMatrix(Matrix* lvpMatrixOut, const Vector3& lightDirection);

			private:	// �f�[�^�����o

				RenderTarget m_shadowMap;	//!< �v���C���[�̉e�𗎂Ƃ��V���h�E�}�b�v
				CGaussianBlur m_blur;		//!< �V���h�E�}�b�v�ɂ�����K�E�V�A���u���[�N���X
				CRender* m_playerRender = nullptr;	//!< �v���C���[�̃����_���[
				Matrix m_lvpMatrix = Matrix::Identity;	//!< ���C�g�r���[�v���W�F�N�V�����s��

			};

		}
	}
}