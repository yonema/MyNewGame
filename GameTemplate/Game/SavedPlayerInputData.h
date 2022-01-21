#pragma once
#include "PlayerInput.h"
#include "SavedPlayerInputDataConstData.h"

namespace nsNinjaAttract
{
	/**
	 * @brief �O���f�[�^�p�l�[���X�y�[�X
	*/
	namespace nsExternalData
	{
		/**
		 * @brief �v���C���[�̓��͏��̕ۑ��f�[�^�N���X
		*/
		class CSavedPlayerInputData
		{
		public:		// �R���X�g���N�^�ƃf�X�g���N�^
			/**
			 * @brief �R���X�g���N�^
			*/
			CSavedPlayerInputData() = default;
			/**
			 * @brief �f�X�g���N�^
			*/
			~CSavedPlayerInputData() = default;

		public:		// �����o�֐�

			/**
			 * @brief �ۑ��������ǉ�����
			 * @param[in] saveData �ۑ�����f�[�^
			*/
			void PushBuckData(const nsPlayer::SPlayerInputDataAndDeltaTime& saveData);

			/**
			 * @brief ���W�����v���C���[�̓��͏���ۑ�����
			 * @param[in] saveFileType �ۑ��t�@�C���̎��
			*/
			void SaveData(const nsSavedPlayerInputDataConstData::EnSavedFileType savedFileType);

			/**
			 * @brief ���W�����v���C���[�̓��͏����N���A����
			*/
			void ClearSaveData();

			/**
			 * @brief �ۑ����Ă���v���C���[�̓��͏������[�h����
			 * @param[in] saveFileType �ۑ��t�@�C��
			*/
			void LoadData(const nsSavedPlayerInputDataConstData::EnSavedFileType savedFileType);

			/**
			 * @brief ���[�h�����f�[�^���N���A����
			*/
			void ClearLoadData();

			/**
			 * @brief ���[�h�����f�[�^��ǂݍ���ŁA�i�߂�B
			 * @return ���݂̐i���̃��[�h�����f�[�^
			*/
			nsPlayer::SPlayerInputDataAndDeltaTime* GetAndProgressLoadData();

			/**
			 * @brief ���[�h�����f�[�^�̐i�������Z�b�g
			*/
			void ResetLoadDataProgress()
			{
				m_loadDataProgress = 0;
			}

			/**
			 * @brief ���[�h�����f�[�^�̐i�����A�Ō�܂ōs���Ă��邩�H�𓾂�B
			 * @return ���[�h�����f�[�^�̐i�����A�Ō�܂ōs���Ă��邩�H
			*/
			bool IsEndLoadDataProgress() const
			{
				// �i�����A�T�C�Y�𒴂��Ă�����A�����Ō�܂Ői��ł���B
				return m_loadDataProgress >= m_loadData.size();
			}

		private:	// �f�[�^�����o
			std::list<nsPlayer::SPlayerInputDataAndDeltaTime> m_saveData;	//!< �ۑ��p�f�[�^
			std::vector<nsPlayer::SPlayerInputDataAndDeltaTime> m_loadData;	//!< ���[�h�����f�[�^
			int m_loadDataProgress = 0;										//!< ���[�h�����f�[�^�̐i��
		};

	}
}