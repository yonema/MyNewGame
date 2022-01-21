#pragma once
#include "Noncopyable.h"
#include "PlayerConstData.h"
#include "SavedPlayerInputDataConstData.h"

namespace nsNinjaAttract
{
	// �O���錾
	namespace nsPlayer{ class CPlayer; }						// �v���C���[�N���X
	namespace nsExternalData { class CSavedPlayerInputData; }	// �v���C���[�̓��͏��̏����o���N���X


	/**
	 * @brief �v���C���[�֘A�̃l�[���X�y�[�X
	*/
	namespace nsPlayer
	{
		/**
		 * @brief �v���C���[�̓��͏��\����
		*/
		struct SPlayerInputData
		{
			float axisMoveForward = 0.0f;			//!< �O�A��ړ��̎����͏��
			float axisMoveRight = 0.0f;				//!< �E�A���ړ��̎����͏��
			bool inputMoveAxis = false;				//!< �ړ��̎����͂����������H
			float axisCameraRotVertical = 0.0f;		//!< �J�����̏c��]�̎����͏��
			float axisCameraRotHorizontal = 0.0f;	//!< �J�����̉���]�̎����͏��
			bool inputCameraAxis = false;			//!< �J�����̎����͂����������H
			bool actionJump = false;				//!< �W�����v�̓��͏��
			bool actionDush = false;				//!< �_�b�V���̓��͏��
			bool actionSwing = false;				//!< �X�C���O�̓��͏��
			bool actionSearchEnemy = false;			//!< �G��T�m������͏��
			bool actionCatchEnemy = false;			//!< �G��߂܂�����͏��
			//!< �R�}���h���͏��
			nsPlayerConstData::nsCatchEnemyConstData::EnQTEButtonType inputCommand =
				nsPlayerConstData::nsCatchEnemyConstData::enQTE_None;
		};

		struct SPlayerInputDataAndDeltaTime
		{
			SPlayerInputData playerInputData;	//!< �v���C���[�̓��̓f�[�^
			float deltaTime = 0.0f;				//!< �f���^�^�C��
		};

		/**
		 * @brief �v���C���[���̓N���X
		*/
		class CPlayerInput : private nsUtil::Noncopyable
		{
		public:		// �R���X�g���N�^�ƃf�X�g���N�^
			/**
			 * @brief �R���X�g���N�^
			*/
			CPlayerInput();
			/**
			 * @brief �f�X�g���N�^
			*/
			~CPlayerInput();

		public:		// �����o�֐�

			/**
			 * @brief ������
			 * @param[in,out] player �v���C���[�̎Q��
			*/
			void Init(CPlayer* player);

			/**
			 * @brief ���͏��������s����
			*/
			void ExecuteUpdate();

			/**
			 * @brief �v���C���[�̓��͏��𓾂�
			 * @return �v���Cy�|�̓��͏��
			*/
			const SPlayerInputData& GetPlayerInputData() const
			{
				return m_playerInputDataAndDeltaTime.playerInputData;
			}

			/**
			 * @brief �v���C���[�S�̂Ŏg�p����f���^�^�C�����擾
			 * @return �v���C���[�S�̂Ŏg�p����f���^�^�C��
			*/
			float GetDeltaTime() const
			{
				return m_playerInputDataAndDeltaTime.deltaTime;
			}

			/**
			 * @brief �ۑ�������̓f�[�^�̎��W���J�n����
			*/
			void StartSaveData()
			{
				m_isStartSaveData = true;
			}

			/**
			 * @brief �ۑ�������̓f�[�^�̎�����I�����āA�ۑ�����B
			 * @param[in] saveFileType �ۑ��t�@�C���̎��
			*/
			void EndSaveDataAndSave(
				const nsExternalData::nsSavedPlayerInputDataConstData::EnSavedFileType saveFileType
			);

			/**
			 * @brief �ۑ������f�[�^�̎g�p���J�n����
			 * @param[in] saveFileType �ۑ��t�@�C���̎��
			*/
			void StartUsingSavedData(
				const nsExternalData::nsSavedPlayerInputDataConstData::EnSavedFileType saveFileType
			);

			/**
			 * @brief �ۑ������f�[�^�̎g�p���I������
			*/
			void EndUsingSavedData();

			/**
			 * @brief ���[�h�����f�[�^�̐i�����A�Ō�܂ōs���Ă��邩�H�𓾂�B
			 * @return ���[�h�����f�[�^�̐i�����A�Ō�܂ōs���Ă��邩�H
			*/
			bool IsEndLoadDataProgress() const;

		private:	// private�ȃ����o�֐�

			/**
			 * @brief ���͏����X�V����
			*/
			void UpdateInput();

			/**
			 * @brief �����͏����X�V����
			*/
			void UpdateInputAxis();

			/**
			 * @brief �A�N�V�������͏����X�V����
			*/
			void UpdateInputAction();

			/**
			 * @brief �ړ��̎����͏��̍X�V
			*/
			void OnUpdateInputAxisMove();

			/**
			 * @brief �J�����̉�]�̎����͏��̍X�V
			*/
			void OnUpdateInputAxisCameraRot();

			/**
			 * @brief �R�}���h���͏��̍X�V
			*/
			void UpdateCommandInputData();

		private:	// �f�[�^�����o
			const GamePad* m_pad = nullptr;		//!<  �Q�[���p�b�h
			CPlayer* m_playerRef = nullptr;		//!< �v���C���[�̎Q��
			//!< �v���C���[�̓��͏��ƃf���^�^�C��
			SPlayerInputDataAndDeltaTime m_playerInputDataAndDeltaTime;
			bool m_canInputCommandAxis = false;	//!< �R�}���h�̎����͂��\���H
			nsExternalData::CSavedPlayerInputData* m_savedPlayerInputData = nullptr;
			bool m_isStartSaveData = false;		//!< �ۑ��f�[�^�̎��W���J�n���Ă��邩�H
			bool m_isUsingSavedData = false;	//!< �ۑ��f�[�^���g�p�����H
		};
	}
}

