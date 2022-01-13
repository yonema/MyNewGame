#pragma once

#pragma once

#include <memory>
#include "Noncopyable.h"

namespace nsNinjaAttract 
{
	/**
	 * @brief Util�֘A�̃l�[���X�y�[�X
	*/
	namespace nsUtil
	{
		/**
		 * @brief ���\�[�X�o���N�N���X
		 * @tparam TResource ���\�[�X�ɂ���N���X
		*/
		template<class TResource>
		class TResourceBank : public Noncopyable 
		{
		private:	// �G�C���A�X�錾
			using TResourcePtr = std::unique_ptr<TResource>;	//!< ���\�[�X�̃��j�[�N�|�C���^

		public:		// �R���X�g���N�^�ƃf�X�g���N�^
			/**
			 * @brief �R���X�g���N�^
			*/
			TResourceBank() = default;
			/**
			 * @brief �f�X�g���N�^
			*/
			~TResourceBank() = default;

		public:		// �����o�֐�

			/**
			 * @brief �o���N�ɓo�^����Ă��郊�\�[�X�𓾂�B���\�[�X��������Ȃ�����nullptr��߂��B
			 * @param[in] filePath ���\�[�X�̃t�@�C���p�X
			 * @return ���\�[�X
			*/
			TResource* Get(const char* filePath)
			{
				// �t�@�C���p�X���烊�\�[�X��T��
				auto it = m_resourceMap.find(filePath);
				if (it != m_resourceMap.end()) 
				{
					// �o���N�ɓo�^����Ă���
					// ���\�[�X��߂�
					return it->second.get();
				}

				// ���o�^
				// nullptr��߂�
				return nullptr;
			}

			/**
			 * @brief ���\�[�X��o�^����
			 * @param[in] filePath ���\�[�X�̃t�@�C���p�X
			 * @param[in] resource ���\�[�X�̃|�C���^
			*/
			void Regist(const char* filePath, TResource* resource)
			{
				// �t�@�C���p�X���烊�\�[�X��T��
				auto it = m_resourceMap.find(filePath);
				if (it == m_resourceMap.end())
				{
					// ���o�^
					// �o�^����
					m_resourceMap.insert(
						std::pair< std::string, TResourcePtr>(filePath, resource)
					);
				}
				// �o�^�ς݂Ȃ牽�����Ȃ�
			}
		private:
			std::map<std::string, TResourcePtr> m_resourceMap;	//!< ���\�[�X�̃}�b�v
		};
	}
}