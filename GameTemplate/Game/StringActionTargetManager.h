#pragma once

namespace nsMyGame
{

	/**
	 * @brief �����g�����A�N�V�����̃^�[�Q�b�g�֘A�̃l�[���X�y�[�X
	*/
	namespace nsStringActionTarget
	{
		// �O���錾
		namespace nsSwingTarget 
		{ 
			class CSwingTarget;		// �X�C���O�̃^�[�Q�b�g�N���X
		}

		/**
		 * @brief ��ԋ߂��X�C���O�^�[�Q�b�g�̃|�C���g�̍��W��T�������̃f�[�^
		*/
		struct SFindNearestSwingTargetPointData
		{
			Vector3 findSwingTargetOrigin = Vector3::Zero;			//!< �X�C���O�^�[�Q�b�g��T�����_
			float findSwingTargetScopeRadius = 0.0f;				//!< �X�C���O�^�[�Q�b�g��T���L���͈�
			Vector3 findSwingTargetPointOrigin = Vector3::Zero;		//!< �X�C���O�^�[�Q�b�g�̃|�C���g��T�����_
			float findSwingTargetPointScopeRadius = 0.0f;			//!< �X�C���O�^�[�Q�b�g�̃|�C���g��T���L���͈�
			float heightLowerLimit = 0.0f;							//!< �����̉���
			Vector3 forwardLimitDir = Vector3::Zero;				//!< �O�����̐����̕����x�N�g��
			Vector3 forwardLimitOrigin = Vector3::Zero;				//!< �O�����̐����̌��_
		};

		/**
		 * @brief �����g�����A�N�V�����̃^�[�Q�b�g�֘A�̃}�l�[�W���[
		 * @note �V���O���g���p�^�[�����g�p
		*/
		class CStringActionTargetManager
		{
		private:	// �R���X�g���N�^�ƃf�X�g���N�^�B
					// �V���O���g���p�^�[���̂���private�ɉB��
			/**
			 * @brief �R���X�g���N�^
			*/
			CStringActionTargetManager();
			/**
			 * @brief �f�X�g���N�^
			*/
			~CStringActionTargetManager() = default;

		private:	// static�ȃf�[�^�����o
			static CStringActionTargetManager* m_instance;		//!< �B��̃C���X�^���X		
		
		public:		// static�ȃ����o�֐�

			/**
			 * @brief �B��̃C���X�^���X�𐶐�����
			 * @note �V���O���g���p�^�[��
			*/
			static void CreateInstance()
			{
				m_instance = new CStringActionTargetManager;
			}

			/**
			 * @brief �B��̃C���X�^���X��j������
			 * @note �V���O���g���p�^�[��
			*/
			static void DeleteInstance()
			{
				if (m_instance)
					delete m_instance;
			}

			/**
			 * @brief �B��̃C���X�^���X�̎Q�Ƃ𓾂�
			 * @return �B��̃C���X�^���X�̎Q��
			 * @note �V���O���g���p�^�[��
			*/
			static CStringActionTargetManager* GetInstance()
			{
				return m_instance;
			}

		public:		// �����o�֐�

			/**
			 * @brief �X�C���O�^�[�Q�b�g��o�^����
			 * @param[in,out] swingTarget �o�^����X�C���O�^�[�Q�b�g
			*/
			void AddSwingTarget(nsSwingTarget::CSwingTarget* swingTarget)
			{
#ifdef MY_DEBUG
				if (m_swingTargets.size() >= m_kSwingTargetNum)
					MessageBoxA(nullptr, "�X�C���O�̃^�[�Q�b�g���\�z��葽������Ă���", "�x��", MB_OK);
#endif
				m_swingTargets.emplace_back(swingTarget);
			}

			/**
			 * @brief �X�C���O�^�[�Q�b�g�̓o�^����������
			 * @param[in] swingTarget ��������X�C���O�^�[�Q�b�g
			*/
			void RemoveSwingTarget(const nsSwingTarget::CSwingTarget* swingTarget);

			/**
			 * @brief �S�ẴX�C���O�^�[�Q�b�g�ɃN�G�������s
			 * @param[in] func ���s����֐�
			*/
			void QuerySwingTarget(const std::function<void(nsSwingTarget::CSwingTarget* swingTarget)>func)
			{
				for (const auto& st : m_swingTargets)
				{
					func(st);
				}

				return;
			}



		private:	// �萔

			static const int m_kSwingTargetNum = 256;	// �X�C���O�̃^�[�Q�b�g

		private:	// �f�[�^�����o
			std::vector<nsSwingTarget::CSwingTarget*> m_swingTargets;	// �X�C���O�̃^�[�Q�b�g�̃R���e�i

		};

		/**
		 * @brief �w�肳�ꂽ���W����L���͈͓��ɂ���X�C���O�^�[�Q�b�g�ɃN�G�������s
		 * @param[in] fromPos �w����W
		 * @param[in] scopeRadius �L���͈�
		 * @param[in] func ���s����֐�
		*/
		void QuerySwingTargetWithinScope(
			const Vector3& fromPos,
			const float scopeRadius,
			const std::function<void(nsSwingTarget::CSwingTarget* swingTarget)>func
		);

		/**
		 * @brief �w�肳�ꂽ���W����L���͈͓��ɂ���X�C���O�^�[�Q�b�g�̍��W�̒��ň�ԋ߂����W��T���Ă���
		 * @param[in] data ��ԋ߂��X�C���O�^�[�Q�b�g�̃|�C���g�̍��W��T�������̃f�[�^
		 * @return �X�C���O�^�[�Q�b�g�̍��W�B�L���͈͓��ɖ������nullptr��߂��B
		*/
		const Vector3* const FindNearestSwingTargetPoint(
			const SFindNearestSwingTargetPointData& data
		);
	}
}
