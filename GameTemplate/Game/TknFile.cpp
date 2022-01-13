#include "stdafx.h"
#include "TknFile.h"

namespace nsNinjaAttract
{
	/**
	 * @brief AI�֘A�̃l�[���X�y�[�X
	*/
	namespace nsAI
	{
		/**
		 * @brief tkn�t�@�C�������[�h����
		 * @param[in] filePath tkn�t�@�C���̃t�@�C���p�X
		 * @return ���[�h�ł������Hfalse���߂��Ă����烍�[�h���s�B
		*/
		bool CTknFile::Load(const char* filePath)
		{
			auto fp = fopen(filePath, "rb");
			if (fp == nullptr) {
				// �t�@�C���̃I�[�v���Ɏ��s�����B
				return false;
			}
			// �w�b�_�����[�h�B
			fread(&m_header, sizeof(m_header), 1, fp);

			// �Z���̔z����m��
			m_cellArray.resize(m_header.numCell);

			// �Z���������[�h�B
			fread(&m_cellArray.front(), sizeof(SCell) * m_header.numCell, 1, fp);

			// �{�f�B�����[�h�B
			fclose(fp);

			return true;
		}
	}
}