#include "stdafx.h"
#include "SavedPlayerInputData.h"

namespace nsNinjaAttract
{
	/**
	 * @brief �O���f�[�^�p�l�[���X�y�[�X
	*/
	namespace nsExternalData
	{
		using namespace nsSavedPlayerInputDataConstData;

		/**
		 * @brief �ۑ��������ǉ�����
		 * @param[in] saveData �ۑ�����f�[�^
		*/
		void CSavedPlayerInputData::PushBuckData(const nsPlayer::SPlayerInputDataAndDeltaTime& saveData)
		{
			m_saveData.emplace_back(saveData);

			return;
		}

		/**
		 * @brief ���W�����v���C���[�̓��͏�����������
		 * @param[in] saveFileType �ۑ��t�@�C���̎��
		*/
		void CSavedPlayerInputData::SaveData(const EnSavedFileType savedFileType)
		{
			// �t�@�C�����o�C�i���������݃��[�h�ŊJ��
			auto fp = fopen(kOutputFilePath[savedFileType], "wb");

			// �ŏ��Ƀf�[�^�̐�����������
			const size_t dataNum = m_saveData.size();
			fwrite(&dataNum, sizeof(dataNum), 1, fp);

			for (const auto& saveData : m_saveData)
			{
				// �f�[�^�̒��g����������
				fwrite(&saveData, sizeof(saveData), 1, fp);
			}

			//�t�@�C�������
			fclose(fp);
			return;
		}

		/**
		 * @brief ���W�����v���C���[�̓��͏����N���A����
		*/
		void CSavedPlayerInputData::ClearSaveData()
		{
			m_saveData.clear();

			return;
		}

		/**
		 * @brief �ۑ����Ă���v���C���[�̓��͏������[�h����
		 * @param[in] saveFileType �ۑ��t�@�C��
		*/
		void CSavedPlayerInputData::LoadData(
			const nsSavedPlayerInputDataConstData::EnSavedFileType savedFileType
		)
		{
			if (m_loadData.empty() != true)
			{
				// ���[�h�f�[�^����ł͂Ȃ�������A�N���A����B
				ClearLoadData();
			}

			// �t�@�C�����o�C�i���ǂݍ��݃��[�h�ŊJ��
			auto fp = fopen(kOutputFilePath[savedFileType], "rb");

			if (fp == nullptr)
			{
				// �t�@�C���I�[�v���Ɏ��s�B
				return;
			}

			// �ŏ��Ƀf�[�^�̐���ǂݍ���
			size_t dataNum = 0;
			fread(&dataNum, sizeof(dataNum), 1, fp);

			// �f�[�^�̐��������T�[�u
			m_loadData.reserve(dataNum);

			for (int i = 0; i < dataNum; i++)
			{
				// �f�[�^�̒��g��ǂݍ���
				nsPlayer::SPlayerInputDataAndDeltaTime loadData;
				fread(&loadData, sizeof(loadData), 1, fp);
				m_loadData.emplace_back(loadData);
			}

			// �t�@�C�����N���[�Y
			fclose(fp);

			return;
		}

		/**
		 * @brief ���[�h�����f�[�^���N���A����
		*/
		void CSavedPlayerInputData::ClearLoadData()
		{
			m_loadData.clear();

			// ���[�h�����f�[�^�̐i�������Z�b�g
			ResetLoadDataProgress();

			return;
		}

		/**
		 * @brief ���[�h�����f�[�^��ǂݍ���ŁA�i�߂�B
		 * @return ���݂̐i���̃��[�h�����f�[�^
		*/
		nsPlayer::SPlayerInputDataAndDeltaTime* CSavedPlayerInputData::GetAndProgressLoadData()
		{
			// ���݂̐i���̃��[�h�����f�[�^
			nsPlayer::SPlayerInputDataAndDeltaTime* loadData = nullptr;

			if (IsEndLoadDataProgress())
			{
				// �Ō�܂Ői��ł���
				return loadData;
			}

			// ���݂̐i���̃f�[�^���i�[����
			loadData = &m_loadData[m_loadDataProgress];

			// �i����i�߂�
			m_loadDataProgress++;

			return loadData;
		}
	}
}