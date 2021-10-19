#pragma once
#include "VectorRender.h"
#include "DebugConstData.h"
#include "Noncopyable.h"

namespace nsMyGame
{
	/**
	 * @brief �f�o�b�N�֘A�̃l�[���X�y�[�X
	*/
	namespace nsDebug
	{

		/**
		 * @brief �f�o�b�N�̃}�l�[�W���[
		 * @note �V���O���g���p�^�[�����g�p
		*/
		class CDebugManager : private nsUtil::Noncopyable
		{
		public:		// �R���X�g���N�^�ƃf�X�g���N�^
			/**
			 * @brief �R���X�g���N�^
			*/
			CDebugManager();
			/**
			 * @brief �f�X�g���N�^
			*/
			~CDebugManager();

		public:		// static�ȃ����o�֐�

			/**
			 * @brief �C���X�^���X�𐶐�
			*/
			static void CreateInstance()
			{
				m_instance = new CDebugManager;
			}

			/**
			 * @brief �C���X�^���X��j��
			*/
			static void DeleteInstance()
			{
				if (m_instance)
					delete m_instance;
			}

			/**
			 * @brief �C���X�^���X���擾
			 * @return �C���X�^���X
			*/
			static CDebugManager* GetInstance()
			{
				return m_instance;
			}

		private:	// static�ȃf�[�^�����o
			static CDebugManager* m_instance;		//!< �C���X�^���X

		public:		// �����o�֐�

			/**
			 * @brief �`�悷��x�N�g����ǉ�����
			 * @param[in] vectorRenderData �x�N�g���̕`��f�[�^
			*/
			void AddVector(const CVectorRender::SVectorRenderData& vectorRenderData)
			{
#ifdef MY_DEBUG
				m_vectorRender->AddVector(vectorRenderData);
#endif
			}

			/**
			 * @brief �x�N�g���̃t�H���g�̊g�嗦��ݒ�
			 * @param[in] scale �g�嗦
			*/
			void SetVectorFontScale(const float scale)
			{
#ifdef MY_DEBUG
				m_vectorRender->SetFontScale(scale);
#endif
			}

			/**
			 * @brief ���O�̔j������
			*/
			void PreDelete();

		private:	// �f�[�^�����o
			CVectorRender* m_vectorRender = nullptr;

		};


		/**
		 * @brief �x�N�g����`�悷��i�x�N�g���`��f�[�^����j
		 * @param[in] vectorRenderData �x�N�g���`��f�[�^
		*/
		static inline void DrawVector(const CVectorRender::SVectorRenderData& vectorRenderData)
		{
			CDebugManager::GetInstance()->AddVector(vectorRenderData);
		}

		/**
		 * @brief �x�N�g����`�悷��i��_�A�x�N�g���A���O����j
		 * @param[in] origin ��_
		 * @param[in] vector �x�N�g��
		 * @param[in] name �x�N�g�������O
		*/
		static inline void DrawVector(
			const Vector3 origin,
			const Vector3 vector,
			const std::string name = nsDebugConstData::kDefaultVectorName
		)
		{
			CVectorRender::SVectorRenderData vectorRenderData =
			{ name,vector,origin, };

			CDebugManager::GetInstance()->AddVector(vectorRenderData);
		}


	}
}