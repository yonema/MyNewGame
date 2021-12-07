#pragma once
#include "ShadowConstDatah.h"
#include "Noncopyable.h"

namespace nsMyGame
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
			 * @brief �J�X�P�[�h�V���h�E�}�b�v�Ŏg�p����}�g���N�X�N���X
			*/
			class CCascadeShadowMapMatrix : private nsUtil::Noncopyable
			{
			public:		// �R���X�g���N�^�ƃf�X�g���N�^

			public:		// �����o�֐�

				/**
				 * @brief ���C�g�r���[�v���W�F�N�V�����N���b�v�s����v�Z����
				 * @param[in] lightDirection ���C�g�̕���
				 * @param[in] cascadeAreaRateTbl �J�X�P�[�h�V���h�E�̃G���A���e�[�u��
				*/
				void CalcLightViewProjectionCropMatrix(
					const Vector3& lightDirection,
					const float cascadeAreaRateTbl[nsShadowConstData::enShadowMapArea_num]
				);

				/**
				 * @brief �v�Z���ꂽ���C�g�r���[�v���W�F�N�V�����N���b�v�s����擾
				 * @param[in] areaNum �V���h�E�}�b�v�̃G���A�ԍ�
				 * @return �N���b�v�s��
				*/
				const Matrix& GetLightViewProjectionCropMatrix(const int areaNum) const 
				{
					return m_lvpcMatrix[areaNum];
				};

			private:	// private�ȃ����o�֐�

				/**
				 * @brief ���C�g�r���[�v���W�F�N�V�����s����v�Z���Ă��瓾��
				 * @param[out] lvpMatrixOut ���C�g�r���[�v���W�F�N�V�����s��
				 * @param[in] lightDirection ���C�g�̕���
				*/
				void CalcAndGetLightViewProjectionMatrix(
					Matrix* lvpMatrixOut,
					const Vector3& lightDirection
				);

				/**
				 * @brief �G���A��8���_�̍��W���v�Z���Ă��瓾��
				 * @param[out] vertexOut ���_�̍��W������z��
				 * @param[in] nearDepth �G���A�̍ŏ��[�x�l
				 * @param[in] farDepth �G���A�̍ő�[�x�l
				 * @param[in] cameraForward �J�����̑O����
				 * @param[in] cameraRight �J�����̉E����
				 * @param[in] cameraUp �J�����̏����
				*/
				void CalcAndGetAreaVertex(
					Vector3 vertexOut[8],
					const float nearDepth,
					const float farDepth,
					const Vector3& cameraForward,
					const Vector3& cameraRight,
					const Vector3& cameraUp
					);

				/**
				 * @brief �N���b�v�s����v�Z���Ă��瓾��
				 * @param[out] clopMatrixOut �N���b�v�s��
				 * @param[in] vMax �ő�l
				 * @param[in] vMin �ŏ��l
				*/
				void CalcAndGetClopMatrix(Matrix* clopMatrixOut, const Vector3& vMax, const Vector3& vMin);

			private:	// �f�[�^�����o
				//!< ���C�g�r���[�v���W�F�N�V�����N���b�v�s��
				Matrix m_lvpcMatrix[nsShadowConstData::enShadowMapArea_num];
			};
		}
	}
}

