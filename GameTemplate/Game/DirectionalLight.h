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
		 * @brief �f�B���N�V�������C�g�N���X
		*/
		class CDirectionalLight : public IGameObject
		{
		public:		// �R���X�g���N�^�ƃf�X�g���N�^
			/**
			 * @brief �R���X�g���N�^
			*/
			CDirectionalLight();
			/**
			 * @brief �f�X�g���N�^
			*/
			~CDirectionalLight();

		public:		// �����o�֐�

			/**
			 * @brief ���C�g�f�[�^�̃|�C���^�Ƀf�[�^��ݒ肷��
			 * @param[in] lightData ���C�g�f�[�^
			*/
			void SetLightDataPtr(nsLightData::SDirectionalLightData& lightData)
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
			 * @brief ���C�g�̕�����ݒ肷��
			 * @param[in] direction ���C�g�̕���
			*/
			void SetDirection(const Vector3& direction)
			{
				m_lightDataPtr->direction = direction;
			}

			/**
			 * @brief ���C�g���e�𐶐����邩�H��ݒ�B0:�������Ȃ��A1:��������
			 * @param[in] castShadow �e�𐶐����邩�H
			*/
			void SetCastShadow(const int castShadow)
			{
				m_lightDataPtr->castShadow = castShadow;
			}

			/**
			 * @brief ���C�g�̃J���[��ݒ�
			 * @param[in] color ���C�g�̃J���[
			*/
			void SetColor(const Vector4& color)
			{
				m_lightDataPtr->color = color;
			}

		private:	// �f�[�^�����o
			nsLightData::SDirectionalLightData* m_lightDataPtr = nullptr;	//!< ���C�g�f�[�^�̃|�C���^
			int m_controlNumber = -1;										//!< ����i���o�[
		};

	}
}