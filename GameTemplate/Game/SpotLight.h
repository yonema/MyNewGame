#pragma once
#include "LightManager.h"

namespace nsMyGame
{
	/**
	 * @brief ���C�g�֘A�̃l�[���X�y�[�X
	*/
	namespace nsLight
	{
		/**
		 * @brief �X�|�b�g���C�g�N���X
		*/
		class CSpotLight : public IGameObject
		{
		public:		// �R���X�g���N�^�ƃf�X�g���N�^
			/**
			 * @brief �R���X�g���N�^
			*/
			CSpotLight();
			/**
			 * @brief �f�X�g���N�^
			*/
			~CSpotLight();

		public:		//�����o�֐�

			/**
			 * @brief ���C�g�f�[�^�̃|�C���^�Ƀf�[�^��ݒ肷��
			 * @param[in] lightData ���C�g�f�[�^
			*/
			void SetLightDataPtr(nsLightData::SSpotLightData& lightData)
			{
				m_lightDataPtr = &lightData;
			}

			/**
			 * @brief ����i���o�[��ݒ肷��
			 * @param[in] controlNumber ����i���o�[
			*/
			void SetControlNumber(const int controlNumber)
			{
				m_controlNumber = controlNumber;
			}

			/**
			 * @brief ����i���o�[�𓾂�
			 * @return ����i���o�[
			*/
			int GetControlNumber() const
			{
				return m_controlNumber;
			}

			/**
			 * @brief ���C�g�̍��W��ݒ�
			 * @param[in] pos ���C�g�̍��W
			*/
			void SetPosition(const Vector3& pos)
			{
				m_lightDataPtr->pointLightData.position = pos;
			}

			/**
			 * @brief ���C�g�̍��W�𓾂�
			 * @return ���C�g�̍��W
			*/
			const Vector3& GetPosition() const
			{
				return m_lightDataPtr->pointLightData.position;
			}

			/**
			 * @brief ���C�g�̉e���͈͂�ݒ�
			 * @param[in] range ���C�g�̉e���͈�
			*/
			void SetRange(const float range)
			{
				m_lightDataPtr->pointLightData.range = range;
			}

			/**
			 * @brief ���C�g�̃J���[��ݒ�
			 * @param[in] color ���C�g�̃J���[
			*/
			void SetColor(const Vector4& color)
			{
				m_lightDataPtr->pointLightData.color = color;
			}

			/**
			 * @brief ���C�g�̕�����ݒ肷��
			 * @param[in] direction ���C�g�̕���
			*/
			void SetDirection(const Vector3& direction)
			{
				m_lightDataPtr->direction = direction;
			}

			/**
			 * @brief ���C�g�̎ˏo�p�x��ݒ肷��
			 * @param angle ���C�g�̎ˏo�p�x
			*/
			void SetAngle(const float angle)
			{
				m_lightDataPtr->angle = angle;
			}

		private:	//�f�[�^�����o
			nsLightData::SSpotLightData* m_lightDataPtr = nullptr;		//!< ���C�g�f�[�^�̃|�C���^
			int m_controlNumber = -1;
		};

	}
}

