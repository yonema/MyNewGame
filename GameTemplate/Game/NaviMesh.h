#pragma once
#include "Cell.h"
#include "BSP.h"

namespace nsMyGame
{
	/**
	 * @brief AI�֘A�̃l�[���X�y�[�X
	*/
	namespace nsAI
	{
		/**
		 * @brief �i�r�Q�[�V�������b�V���N���X
		*/
		class CNaviMesh
		{
		public:		// �R���X�g���N�^�ƃf�X�g���N�^
			/**
			 * @brief �R���X�g���N�^
			*/
			CNaviMesh() = default;
			/**
			 * @brief �f�X�g���N�^
			*/
			~CNaviMesh() = default;

		public:		// �����o�֐�

			/**
			 * @brief tkn�t�@�C������i�r�Q�[�V�������b�V�����\�z����
			 * @param tknFilePath tkn�t�@�C���̃t�@�C���p�X
			*/
			void Init(const char* tknFilePath);

			/**
			 * @brief �w�肵�����W�ɍł��߂��Z������������
			 * @param[in] pos �w����W
			 * @return �ł��߂��Z��
			*/
			const CCell& FindNearestCell(const Vector3& pos) const;

			/**
			 * @brief �Z���̐����擾
			 * @return �Z���̐�
			*/
			int GetNumCell() const
			{
				return static_cast<int>(m_cellArray.size());
			}

			/**
			 * @brief �Z�����擾
			 * @param[in] cellNo �Z���̔ԍ�
			 * @return �Z��
			*/
			const CCell& GetCell(const int cellNo) const
			{
				return m_cellArray.at(cellNo);
			}

		private:	// �f�[�^�����o

			std::vector<CCell> m_cellArray;			//!< �Z���̔z��
			nsGeometry::CBSP m_cellCenterPosBSP;	//!< �Z���̒��S���W�ō\�z���ꂽBSP

		};

	}
}
