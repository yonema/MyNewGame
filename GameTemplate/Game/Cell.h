#pragma once

namespace nsMyGame
{
	/**
	 * @brief AI�֘A�̃l�[���X�y�[�X
	*/
	namespace nsAI
	{
		/**
		 * @brief �i�r���b�V���̃Z���N���X
		*/
		class CCell
		{
		public:		// �R���X�g���N�^�ƃf�X�g���N�^
			/**
			 * @brief �R���X�g���N�^
			*/
			CCell() = default;
			/**
			 * @brief �f�X�g���N�^
			*/
			~CCell() = default;

		public:		// �����o�֐�

			/**
			 * @brief ���_���W��ݒ�
			 * @param[in] vertNo ���_�ԍ��B0�`2�B
			 * @param[in] pos ���W
			*/
			void SetVertexPosition(const int vertNo, const Vector3& pos)
			{
				CheckInvalidNo(vertNo, "Invalid vertNo");

				m_vertexPosition[vertNo] = pos;
			}

			/**
			 * @brief �@����ݒ�
			 * @param[in] normal �@��
			*/
			void SetNormal(const Vector3& normal)
			{
				m_normal = normal;
			}

			/**
			 * @brief �אڃZ������ݒ�
			 * @param[in] linkNo �אڔԍ��B0�`2.
			 * @param[in] linkCell �אڃZ��
			*/
			void SetLinkCell(const int linkNo, const CCell* linkCell)
			{
				CheckInvalidNo(linkNo, "Invalid linkNo");

				m_linkCell[linkNo] = linkCell;
			}

			/**
			 * @brief �Z���̒��S���W��ݒ�
			 * @param[in] centerPos �Z���̒��S���W
			*/
			void SetCenterPosition(const Vector3& centerPos)
			{
				m_centerPos = centerPos;
			}

			/**
			 * @brief �Z���̒��S���W���擾
			 * @return �Z���̒��S���W
			*/
			const Vector3& GetCenterPosition() const
			{
				return m_centerPos;
			}

			/**
			 * @brief �Z���ԍ����擾
			 * @param[in] no �Z���ԍ�
			*/
			void SetCellNo(const int no)
			{
				m_no = no;
			}

			/**
			 * @brief �Z���ԍ����擾
			 * @return �Z���ԍ�
			*/
			int GetCellNo() const
			{
				return m_no;
			}

			/**
			 * @brief �אڃZ�����擾
			 * @param[in] linkCellNo �אڃZ���ԍ�
			 * @return �אڃZ��
			*/
			const CCell* GetLinkCell(int linkCellNo)const
			{
				return m_linkCell[linkCellNo];
			}

			/**
			 * @brief �Z���̒��_���W���擾����
			 * @param vertNo ���_�ԍ�
			 * @return ���_���W
			*/
			const Vector3& GetVertexPosition(int vertNo) const
			{
				return m_vertexPosition[vertNo];
			}

		private:	// private�ȃ����o�֐�

			/**
			 * @brief �ԍ����L�����`�F�b�N����
			 * @param[in] No �ԍ�
			 * @param[in] invalidTypeText �����̃^�C�v�̃e�L�X�g
			*/
			void CheckInvalidNo(const int No, const char* invalidTypeText)
			{
#ifdef MY_DEBUG
				if (No >= 3)
				{
					MessageBoxA(nullptr, invalidTypeText, "�A�T�[�g", MB_OK);
					std::abort();
				}
#endif
			}

		private:	// �f�[�^�����o
			Vector3 m_vertexPosition[3];				//!< �Z���̒��_���W
			Vector3 m_normal;							//!< �@��
			Vector3 m_centerPos;						//!< ���S���W
			const CCell* m_linkCell[3] = { nullptr };	//!< �אڃZ��
			int m_no = 0;								//!< �Z���ԍ�
		};

	}
}