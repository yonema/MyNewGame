#pragma once
#include "VectorRender.h"

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
			static CDebugManager* m_instance;

		public:		// �����o�֐�
			void AddVector(const CVectorRender::SVectorRenderData& vectorRenderData)
			{
				m_vectorRender->AddVector(vectorRenderData);
			}

			/**
			 * @brief 
			*/
			void PreDelete();

		private:	// �f�[�^�����o
			CVectorRender* m_vectorRender = nullptr;
		};



		static inline void DrawVector(const CVectorRender::SVectorRenderData& vectorRenderData)
		{
			CDebugManager::GetInstance()->AddVector(vectorRenderData);
		}

		static inline void DrawVector(const Vector3 origin, const Vector3 vector, const std::string name)
		{
			CVectorRender::SVectorRenderData vectorRenderData =
			{
				name,
				vector,
				origin,
			};

			CDebugManager::GetInstance()->AddVector(vectorRenderData);
		}
	}
}