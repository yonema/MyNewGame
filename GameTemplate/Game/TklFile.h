#pragma once

namespace nsNinjaAttract
{
	/**
	 * @brief 3D�̃��x���f�[�^�֘A�̃l�[���X�y�[�X
	*/
	namespace nsLevel3D
	{
		/**
		 * @brief tkl�t�@�C����ǂݍ��ރN���X
		*/
		class CTklFile
		{
		public:		// �f�[�^
			
			/**
			 * @brief �I�u�W�F�N�g�f�[�^
			*/
			struct SObject
			{
				std::unique_ptr<char[]> name;	//!< ���̖��O�B
				int parentNo;					//!< �e�̔ԍ��B
				float bindPose[4][3];			//!< �o�C���h�|�[�Y�B
				float invBindPose[4][3];		//!< �o�C���h�|�[�Y�̋t���B
				int no;							//!< �����̍��ԍ�

				//�X�N���v�g�̕����܂��������B
				bool shadowcasterflag = false;
				bool shadowreceiverflag = false;
				std::vector<int> intData;
				std::vector<float> floatData;
				std::vector<char*> charData;
				std::vector<Vector3> Vector3Data;
			};

		public:		// �R���X�g���N�^�ƃf�X�g���N�^
			/**
			 * @brief �R���X�g���N�^
			*/
			CTklFile() = default;

			/**
			 * @brief �f�X�g���N�^
			*/
			~CTklFile() = default;

		public:		// �����o�֐�

			/**
			 * @brief tkl�t�@�C�������[�h����
			 * @param[in] filepath tkl�t�@�C���p�X
			*/
			void Load(const char* filepath);

			/**
			 * @brief �N�G���֐�
			 * @param[in] query ���s����֐��I�u�W�F�N�g
			*/
			void QuaryObject(std::function<void(SObject& obj)> query)
			{
				for (auto& obj : m_objectlist) {
					query(obj);
				}
			}

		private:	// �f�[�^�����o
			int m_version = 0;					//!< tkl�t�@�C���̃o�[�W����
			int m_objectCout = 0;				//!< �I�u�W�F�N�g�̐�
			std::vector<SObject> m_objectlist;	//!< �I�u�W�F�N�g�̃��X�g
		};

	}
}


