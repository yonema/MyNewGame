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
		 * @brief �|�C���g���C�g�N���X
		*/
		class CPointLight : public IGameObject
		{
		public:		// �R���X�g���N�^�ƃf�X�g���N�^
			/**
			 * @brief �R���X�g���N�^
			*/
			CPointLight();
			/**
			 * @brief �f�X�g���N�^
			*/
			~CPointLight();

		public:		//�����o�֐�

			/**
			 * @brief ���C�g�f�[�^�̃|�C���^�Ƀf�[�^��ݒ肷��
			 * @param[in] lightData ���C�g�f�[�^
			*/
			void SetLightDataPtr(nsLightData::SPointLightData& lightData)
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
				m_lightDataPtr->position = pos;
			}

			/**
			 * @brief ���C�g�̍��W�𓾂�
			 * @return ���C�g�̍��W
			*/
			const Vector3& GetPosition() const
			{
				return m_lightDataPtr->position;
			}

			/**
			 * @brief ���C�g�̉e���͈͂�ݒ�
			 * @param[in] range ���C�g�̉e���͈�
			*/
			void SetRange(const float range) 
			{
				m_lightDataPtr->range = range;
			}

			/**
			 * @brief ���C�g�̃J���[��ݒ�
			 * @param[in] color ���C�g�̃J���[
			*/
			void SetColor(const Vector4& color)
			{
				m_lightDataPtr->color = color;
			}

		private:	//�f�[�^�����o
			nsLightData::SPointLightData* m_lightDataPtr = nullptr;		//!< ���C�g�f�[�^�̃|�C���^
			int m_controlNumber = -1;									//!< ����i���o�[
		};

	}
}