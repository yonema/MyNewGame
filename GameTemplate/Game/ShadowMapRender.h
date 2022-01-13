#pragma once
#include "Noncopyable.h"
#include "ShadowConstDatah.h"
#include "CascadeShadowMapMatrix.h"
#include "GaussianBlur.h"
#include "PlayerShadowMap.h"


namespace nsNinjaAttract
{
	/**
	 * @brief �O���t�B�b�N�֘A�̃l�[���X�y�[�X
	*/
	namespace nsGraphic
	{
		// �O���錾
		class CRender;
		namespace nsShadow { class CPlayerShadowMap; }

		/**
		 * @brief �V���h�E�֘A�̃l�[���X�y�[�X
		*/
		namespace nsShadow
		{


			/**
			 * @brief �V���h�E�}�b�v�����_���[�N���X
			*/
			class CShadowMapRender : private nsUtil::Noncopyable
			{
			public:		// �R���X�g���N�^�ƃf�X�g���N�^
				/**
				 * @brief �R���X�g���N�^
				*/
				CShadowMapRender() = default;
				/**
				 * @brief �R���X�g���N�^
				*/
				~CShadowMapRender() = default;

			public:		// �����o�֐�

				/**
				 * @brief �������֐�
				*/
				void Init();

				/**
				 * @brief �`��֐�
				 * @param[in] rc �����_�����O�R���e�L�X�g
				 * @param[in] ligNo ���C�g�̔ԍ�
				 * @param[in] lightDirection ���C�g�̕���
				 * @param[in] renderObjects �`�悷��I�u�W�F�N�g
				 * @param[in] sceneMaxPosition �Q�[���V�[���̍ő���W
				 * @param[in] sceneMinPosition �Q�[���V�[���̍ŏ����W
				*/
				void Render(
					RenderContext& rc,
					const int ligNo, 
					const Vector3& lightDirection,
					std::vector<CRender*>& renderObjects,
					const Vector3& sceneMaxPosition,
					const Vector3& sceneMinPosition
				);

				/**
				 * @brief �V���h�E�}�b�v���擾
				 * @param[in] areaNo �G���A�ԍ�
				 * @return �V���h�E�}�b�v�̃e�N�X�`���̎Q��
				*/
				Texture& GetShadowMap(const int areaNo)
				{
					return m_blur[areaNo].GetBokeTexture();
				}

				/**
				 * @brief ���C�g�r���[�v���W�F�N�V�����s����擾
				 * @param[in] areaNo �G���A�ԍ�
				 * @return ���C�g�r���[�v���W�F�N�V�����s��
				*/
				const Matrix& GetLVPMatrix(const int areaNo) const
				{
					return m_cascadeShadowMapMatrix.GetLightViewProjectionCropMatrix(areaNo);
				}

				/**
				 * @brief �v���C���[��p�̃��C�g�r���[�v���W�F�N�V�����s����擾
				 * @return �v���C���[��p�̃��C�g�r���[�v���W�F�N�V�����s��
				*/
				const Matrix& GetPlayerLVPMatrix() const
				{
					return m_playerShadowMapPtr->GetLVPMatrix();
				}

				/**
				 * @brief �J�X�P�[�h�V���h�E�̃G���A����ݒ�B
				 * @details �Q�[���J�����̋ߕ��ʂ��牓���ʂ܂ł̃G���A�́A��%�͈̔͂��ߋ����p�̍��𑜓x
				 * �̃V���h�E�}�b�v�ɁA��%�𒆋����p�̃V���h�E�}�b�v�ɁA��%���������p�̃V���h�E�}�b�v��
				 * �`�悷�邩���w�肵�����́B�Ⴆ�΁AnearArea  = 0.1(10%)�AmiddleArea = 0.3(30%), 
				 * farArea = 0.8(80%)�Ǝw�肳��Ă�����A�J��������10%�̃G���A�͋ߋ����p�̃V���h�E�}�b�v
				 * �ɕ`��A�J��������10%�`30%�̃G���A�͒������p�̃V���h�E�}�b�v�ɕ`��A�J��������30%�`80%
				 * �̃G���A�͉������p�̃V���h�E�}�b�v�ɕ`��ƂȂ�B
				 * @param[in] nearArea �߉e�̃G���A��
				 * @param[in] middleArea ���e�̃G���A��
				 * @param[in] farArea ���e�̃G���A��
				*/
				void SetCascadeNearAreaRates(const float nearArea, const float middleArea, const float farArea)
				{
					// �������G���A�͈̔͂��ߋ����G���A�͈̔͂�菬�����Ȃ��Ă����
					// �v�Z��s�s�����N����̂ŁA�␳�B
					const float newMiddleArea = max(nearArea + 0.01f, middleArea);
					// ���������G���A�͈̔͂��������G���A�͈̔͂�菬�����Ȃ��Ă����
					// �v�Z��s�s�����N����̂ŁA�␳�B
					const float newFarArea = max(newMiddleArea + 0.01f, farArea);
					m_cascadeAreaRateArray[nsShadowConstData::enShadowMapArea_near] = nearArea;
					m_cascadeAreaRateArray[nsShadowConstData::enShadowMapArea_middle] = newMiddleArea;
					m_cascadeAreaRateArray[nsShadowConstData::enShadowMapArea_far] = newFarArea;
				}

				/**
				 * @brief �v���C���[�̃����_���[��ݒ�
				 * @param[in] playerRender �v���C���[�̃����_���[
				*/
				void SetPlayerRender(CRender* playerRender)
				{
					m_playerShadowMapPtr->SetPlayerRender(playerRender);
				}

				/**
				 * @brief �v���C���[��p�̃V���h�E�}�b�v���擾
				 * @return �V���h�E�}�b�v�̃e�N�X�`���̎Q��
				*/
				Texture& GetPlayerShadowMap()
				{
					return m_playerShadowMapPtr->GetShadowMap();
				}

			private:	// �f�[�^�����o

				//!< �J�X�P�[�h�V���h�E�}�b�v�Ŏg�p����}�g���N�X�N���X
				CCascadeShadowMapMatrix m_cascadeShadowMapMatrix;
				//!< ���ꂼ��̃G���A�̃V���h�E�}�b�v
				RenderTarget m_shadowMaps[nsShadowConstData::enShadowMapArea_num];

				//!< �J�X�P�[�h�V���h�E�̃G���A��
				float m_cascadeAreaRateArray[nsShadowConstData::enShadowMapArea_num] = {};
				//!< �V���h�E�}�b�v�ɂ�����K�E�V�A���u���[�N���X
				CGaussianBlur m_blur[nsShadowConstData::enShadowMapArea_num];

				//!< �v���C���[��p�̃V���h�E�}�b�v
				std::unique_ptr<CPlayerShadowMap> m_playerShadowMapPtr;

			};
		}
	}
}


