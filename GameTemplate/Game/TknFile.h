#pragma once

namespace nsMyGame
{
	/**
	 * @brief AI�֘A�̃l�[���X�y�[�X
	*/
	namespace nsAI
	{
		/**
		 * @brief tkn�t�@�C���i�i�r�Q�[�V�������b�V���j�N���X
		*/
		class CTknFile
		{
		public:		// �\����
			/**
			 * @brief tkn�t�@�C���̃w�b�_�[
			*/
			struct SHeader
			{
				std::uint32_t numCell;	//!< �Z���̐�
			};

			/**
			 * @brief �Z���̃f�[�^
			*/
			struct SCell
			{
				Vector3 vertexPosition[3];	//!< �Z���̒��_���W
				Vector3 normal;				//!< �Z���̖@��
				std::int32_t linkCellNo[3];	//!< �אڃZ���̔ԍ�
			};

		public:		// �R���X�g���N�^�ƃf�X�g���N�^
			/**
			 * @brief �R���X�g���N�^
			*/
			CTknFile() = default;
			/**
			 * @brief �f�X�g���N�^
			*/
			~CTknFile() = default;

		public:		// �����o�֐�

			/**
			 * @brief tkn�t�@�C�������[�h����
			 * @param[in] filePath tkn�t�@�C���̃t�@�C���p�X
			 * @return ���[�h�ł������Hfalse���߂��Ă����烍�[�h���s�B
			*/
			bool Load(const char* filePath);

			/**
			 * @brief �Z���̐����擾
			 * @return �Z���̐�
			*/
			int GetNumCell() const
			{
				return static_cast<int>(m_header.numCell);
			}

			/**
			 * @brief �Z�����擾
			 * @param[in] cellNo �Z���̔ԍ�
			 * @return �Z��
			*/
			const SCell& GetCell(const int cellNo) const
			{
				return m_cellArray.at(cellNo);
			}

		private:	// �f�[�^�����o

			SHeader m_header;					//!< �w�b�_
			std::vector<SCell> m_cellArray;		//!< �Z���̔z��

		};

	}
}