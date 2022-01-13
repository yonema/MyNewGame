#pragma once
#include "Noncopyable.h"
#include "PlayerConstData.h"

namespace nsNinjaAttract
{
	// �O���錾
	namespace nsPlayer { class CPlayer; }
	namespace nsGraphic { namespace nsSprite { class CSpriteRender; } }

	/**
	 * @brief �v���C���[�֘A�̃l�[���X�y�[�X
	*/
	namespace nsPlayer
	{
		/**
		 * @brief �v���C���[�̃R�}���h���̓N���X
		*/
		class CPlayerCommandInput : private nsUtil::Noncopyable
		{
		private:	// �G�C���A�X�錾
			// QTE�Ɏg�p����{�^���̎��
			using EnQTEButtonType = nsPlayerConstData::nsCatchEnemyConstData::EnQTEButtonType;

		public:		// �R���X�g���N�^�ƃf�X�g���N�^
			/**
			 * @brief �R���X�g���N�^
			*/
			CPlayerCommandInput();
			/**
			 * @brief �f�X�g���N�^
			*/
			~CPlayerCommandInput() = default;

		public:		// �����o�֐�

			/**
			 * @brief ������
			 * @param[in] playerRef �v���C���[�̎Q��
			*/
			void Init(const CPlayer& playerRef)
			{
				m_playerRef = &playerRef;
			}

			/**
			 * @brief �R�}���h������������
			 * @param[in] comNum �R�}���h�̐�
			 * @param[in] comType �R�}���h�^�C�v
			*/
			void InitCommand(
				const int comNum,
				const nsPlayerConstData::nsCommandInputConstData::EnCommandType comType
			);

			/**
			 * @brief �R�}���h���͂����s����
			*/
			void Execute();

			/**
			 * @brief �R�}���h���͂��I��������H�𓾂�
			 * @return �R�}���h���͂��I��������Htrue�ŏI���Bfalse�ŏI�����Ă��Ȃ��B
			*/
			bool IsEndCommandInput() const
			{
				return m_isEndCommandInput;
			}
			
			/**
			 * @brief �R�}���h���͂��I��������
			*/
			void EndCommandInput()
			{
				m_isEndCommandInput = true;
			}

			/**
			 * @brief �R�}���h�̔z��𓾂�
			 * @return �R�}���h�̔z��
			*/
			const std::vector<EnQTEButtonType>& GetCommandButtonTypeArray() const
			{
				return m_commandButtonTypeArray;
			}

			/**
			 * @brief �R�}���h�̐i�s�x�𓾂�
			 * @return �R�}���h�̐i�s�x
			*/
			int GetCommandProgress() const
			{
				return m_commandProgress;
			}

			/**
			 * @brief �R�}���h�̌��ʂ𓾂�
			 * @return �R�}���h�̌���
			*/
			nsPlayerConstData::nsCommandInputConstData::EnCommandResult GetCommandResult() const
			{
				return m_commondResult;
			}

		private:	// private�ȃ����o�֐�

			/**
			 * @brief ������������
			*/
			void InitRand();

		private:	// �f�[�^�����o
			std::vector<EnQTEButtonType> m_commandButtonTypeArray;			//!< �R�}���h�̔z��

			std::unique_ptr<std::mt19937> m_mt;							//!< �����Z���k�c�C�X�^�[��32�r�b�g��
			//!< �͈͕t���̈�l�����A�R�}���h�̎�ޕʂ̗���
			std::unique_ptr<std::uniform_int_distribution<>>
				m_randByType[nsPlayerConstData::nsCommandInputConstData::enCT_typeNum];
			//!< �͈͕t���̈�l�����A�R�}���h�̎�ނ�I�Ԃ̗���
			std::unique_ptr<std::uniform_int_distribution<>> m_randSelectType;

			int m_commandProgress = 0;	//!< �R�}���h�̐i�s�x
			bool m_isEndCommandInput = true;	//!< �R�}���h���͂��I���������H
			//!< �R�}���h�̌���
			nsPlayerConstData::nsCommandInputConstData::EnCommandResult m_commondResult =
				nsPlayerConstData::nsCommandInputConstData::enCR_None;
			const CPlayer* m_playerRef = nullptr;
		};

	}
}